#include "cr_protocol.h"
#include <QDebug>
#include <cstring>

crProtocol::crProtocol(QObject *parent)
    : QObject{parent}, dataLen(0), crc16(0)
{}

/**
 * @brief 复位FSM状态机
 */
void crProtocol::reset() {
    msgState = SOF; // 状态机重置为初始状态
    reservedBytesRead = 0; // 重置已读取的保留字节计数
    dataLenBytesRead = 0; // 重置已读取的数据长度字节计数
    dataBytesRead = 0; // 重置已读取的数据字节计数
    cycBytesRead = 0; // 重置已读取的CRC字节计数
    dataLen = 0; // 重置数据长度
    crc16 = 0; // 重置CRC值
    std::memset(&msgFrame, 0, sizeof(msgFrame)); // 重置帧数据
}

/**
 * @brief 放入一个字节，解析报文
 *
 * @param byte 需要解析的字节
 * @return bool true:解包成功,false:解包没成功
 */
bool crProtocol::parse_Byte(uint8_t byte) {
    switch (msgState) {
        case SOF: // 解析SOF字段
            if (byte == 0xAA) {
                msgFrame.sof = byte;
                msgState = VERSION; // 跳转到解析版本号
            } else {
                msgState = FRAME_ERROR; // 若SOF字段错误，则跳转到错误状态
            }
            break;
        case VERSION: // 解析版本号
            msgFrame.version = byte;
            msgState = CMD_SET; // 跳转到解析命令集
            break;
        case CMD_SET: // 解析命令集
            msgFrame.cmd_set = byte;
            msgState = CMD_ID; // 跳转到解析命令ID
            break;
        case CMD_ID: // 解析命令ID
            msgFrame.cmd_id = byte;
            msgState = FLAGS; // 跳转到解析标志位
            break;
        case FLAGS: // 解析标志位
            msgFrame.flags = byte;
            msgState = FROM_ID; // 跳转到解析发送者ID
            break;
        case FROM_ID: // 解析发送者ID
            msgFrame.from_id = byte;
            msgState = RESERVED; // 跳转到解析保留字段
            break;
        case RESERVED: // 解析保留字段
            msgFrame.reserved[reservedBytesRead++] = byte;
            if (reservedBytesRead == 2) {
                msgState = DATA_LEN; // 保留字段读取完成，跳转到解析数据长度
                reservedBytesRead = 0; // 重置已读取的保留字节计数
            }
            break;
        case DATA_LEN: // 解析数据长度
            msgFrame.data_len[dataLenBytesRead++] = byte;
            if (dataLenBytesRead == 4) {
                // 将4个字节的data_len数组转换为一个uint32_t
                dataLen = (static_cast<uint32_t>(msgFrame.data_len[0]) << 24) |
                          (static_cast<uint32_t>(msgFrame.data_len[1]) << 16) |
                          (static_cast<uint32_t>(msgFrame.data_len[2]) << 8)  |
                          static_cast<uint32_t>(msgFrame.data_len[3]);
                msgState = DATA; // 数据长度读取完成，跳转到解析数据
                dataLenBytesRead = 0; // 重置已读取的数据长度字节计数
            }
            break;
        case DATA: // 解析数据
            if (dataBytesRead < kDataLength) {
                msgFrame.data[dataBytesRead++] = byte;
            }
            if (dataBytesRead == dataLen || dataBytesRead == kDataLength) { // 限制数据长度不超过kDataLength
                msgState = CRC16; // 数据读取完成，跳转到解析CRC16
            }
            break;
        case CRC16: // 解析CRC16
            msgFrame.crc[cycBytesRead++] = byte;
            if (cycBytesRead == 2) {
                crc16 = (msgFrame.crc[0] << 8) | msgFrame.crc[1];
                // 这里可以进行CRC16校验和进一步处理
                // 打印data段的数据
                if (this->logEnabled) {
                    QString dataString;
                    for (uint32_t i = 0; i < dataLen && i < kDataLength; ++i) {
                        dataString += QString::number(msgFrame.data[i], 16).rightJustified(2, '0') + " ";
                    }
                    qDebug().nospace() << "Frame complete,and Data:" << dataString.trimmed();
                }
                // reset()之前，保存数据包给其他模块解析
                std::memcpy(&this->msgCopy, &this->msgFrame, sizeof(msgFrame));
                this->dataLenCopy = this->dataLen;
                //
                reset();
                return true;
            }
            break;
        case FRAME_ERROR: // 错误状态
            if (this->msgState >= CMD_ID) {
                qDebug().nospace() << "Frame error:"
                                   << "CMD_SET: " << QString::number(this->msgFrame.cmd_set,16).rightJustified(2,'0')
                                   << ",CMD_ID" << QString::number(this->msgFrame.cmd_id,16).rightJustified(2,'0');
            } else {
                qDebug().nospace() << "Frame error:"
                                   << "Incomplete CMD_SET and CMD_ID information";
            }
            reset(); // 复位状态机以准备解析下一个帧
            break;
        default: break;
    }

    return false;
}

/**
 * @brief 获取cmd_Set、cmd_ID
 *
 * @param *cmdSet
 * @param *cmdID
 * @return void
 */
void crProtocol::get_Cmd_Set_And_ID(uint8_t& cmdSet, uint8_t& cmdID) const {
    // 这里不能使用成员函数msgFrame，它们在组包成功之后，马上会reset()
    cmdSet = msgCopy.cmd_set;
    cmdID = msgCopy.cmd_id;
}

/**
 * @brief 获取data段地址与data段长度
 *
 * @param *data
 * @param *len
 * @return void
 */
void crProtocol::get_Data_And_DataLen(uint8_t* data, uint32_t& len) const {
    if (data) {
        // 这里不能使用成员函数msgFrame与dataLen，它们在组包成功之后，马上会reset()
        len = this->dataLenCopy;
        std::memcpy(data, msgCopy.data, this->dataLenCopy);
    }
}





