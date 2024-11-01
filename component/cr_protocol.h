#ifndef CR_PROTOCOL_H
#define CR_PROTOCOL_H

#include <QObject>

constexpr uint16_t kDataLength = 1024; // 定义固定数据长度

// 消息结构
struct protocolFrame {
    uint8_t sof;
    uint8_t version;
    uint8_t cmd_set;
    uint8_t cmd_id;
    uint8_t flags;
    uint8_t from_id;
    uint8_t reserved[2];
    uint8_t data_len[4];
    uint8_t data[kDataLength]; // 固定长度的数组
    uint8_t crc[2];
};

class crProtocol : public QObject
{
    Q_OBJECT
public:
    explicit crProtocol(QObject *parent = nullptr);
    bool parse_Byte(uint8_t byte);
    // 设置器
    void set_Log_Enabled(bool enabled) { this->logEnabled = enabled; }

    // 获取器
    void get_Cmd_Set_And_ID(uint8_t& cmdSet, uint8_t& cmdID) const;
    uint8_t get_From_ID() const { return msgFrame.from_id; }
    void get_Data_And_DataLen(uint8_t* data, uint32_t& len) const;
    bool get_Log_Enabled() const { return this->logEnabled; }

private:
    // FSM解包状态机
    enum State {
        SOF,
        VERSION,
        CMD_SET,
        CMD_ID,
        FLAGS,
        FROM_ID,
        RESERVED,
        DATA_LEN,
        DATA,
        CRC16,
        FRAME_ERROR
    };
    /* 成员 */
    State msgState = SOF;   // 初始化状态为SOF
    protocolFrame msgFrame; // 定义帧结构体（运行状态机，组包）
    uint32_t dataLen;       // 消息的长度(运行状态机，组包）
    protocolFrame msgCopy;  // 存放最新一组数据包
    uint32_t dataLenCopy;   // 最新一组数据包的长度
    uint16_t crc16;         // CRC值
    uint8_t reservedBytesRead = 0; // 已读取的保留字节计数
    uint8_t dataLenBytesRead = 0;  // 已读取的数据长度字节计数
    uint8_t cycBytesRead = 0;      // 已读取的CRC字节计数
    uint16_t dataBytesRead = 0;    // 已读取的数据字节计数
    bool logEnabled = false;       // 日志打印开关，默认关闭

    /* 方法 */
    void reset(); // 重置FSM状态机

signals:

};

#endif // CR_PROTOCOL_H
