#include "robot_message.h"
#include "QDebug"
#include <QDateTime>
#include "component/common.h"

robotMessage::robotMessage(QObject *parent)
    : QObject{parent}
{}

void robotMessage::msgAnalysis(uint8_t cmdSetVal, uint8_t cmdIdVal, const uint8_t* const data, uint32_t dataLen) {
    cmdSet set = static_cast<cmdSet>(cmdSetVal);
    switch(set) {
        case cmdSet::VCU:
            {
                VCU_cmdID cmdId = static_cast<VCU_cmdID>(cmdIdVal);
                switch(cmdId) {
                    /* 传感器设备状态 */
                    case VCU_cmdID::SENSOR_STATUS:
                        sensor_Status(data, dataLen);
                        break;
                    /* 超声波传感器的实时距离值 */
                    case VCU_cmdID::ULTRASONIC_DISTANCE:
                        ult_Distance(data, dataLen);
                        break;
                    /* 轮毂电机温度、电流信息 */
                    case VCU_cmdID::WHEEL_TEMP_CUR_STATUS:
                        wheel_Temp_Current_Status(data, dataLen);
                        break;
                    /* 托刷电机状态 */
                    case VCU_cmdID::MOP_STATUS:
                        mop_Status(data, dataLen);
                        break;
                    /* 滚刷电机状态 */
                    case VCU_cmdID::ROLLING_BRUSH_STATUS:
                        roll_Brush_Status(data, dataLen);
                        break;
                    /* 地毯传感器状态 */
                    case VCU_cmdID::CARPET_SENSOR:
                        carpet_Sensor_Status(data, dataLen);
                        break;
                    default: break;
                }
            }
            break;
        case cmdSet::MOVE:
            {
                MOVE_cmdID cmdId = static_cast<MOVE_cmdID>(cmdIdVal);
                switch(cmdId) {
                    /* 电机运动状态（无时间戳）*/
                    case MOVE_cmdID::WHEEL_STATUS:
                        wheel_Status(data, dataLen);
                        break;
                    /* SLAM IMU信息 */
                    case MOVE_cmdID::LOW_FREQ_IMU_STATUS:
                        low_Frequence_SLAM_IMU(data, dataLen);
                        break;
                    /* 底盘直接控制 */
                    case MOVE_cmdID::CONTROL_WHEEL:
                        direct_Move_Ctrl(data, dataLen);
                        break;

                    default: break;

                }
            }
            break;
        defualt: break;
    }
}

/**
 * @brief 解析开关传感器状态
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::sensor_Status(const uint8_t* const data, uint32_t dataLen) {
    static const uint32_t kExpectedDataLen = 11U;

    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }
    if (kExpectedDataLen != dataLen) {
        LOG << "Invalid data length";
        return;
    }

    uint32_t offset = 0;
    this->stopButton               = data[offset++];
    this->bumperStrip              = data[offset++];
    this->handleSensor             = data[offset++];
    this->coverSensor              = data[offset++];
    offset += 2; // I/O的防跌落传感器已经去掉
    this->netWaterBox              = data[offset++];
    this->dirtyWaterBox            = data[offset++];
    this->netWaterLevel            = data[offset++];
    this->dirtyWaterLevel          = data[offset++];
    this->stepperMotorOriginSensor = data[offset++];
    // if (this->logSwitch) {
    //     qDebug().nospace() << "SysTime:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
    //                        << " stopButton:"    << this->stopButton
    //                        << " bumperStrip:"   << this->bumperStrip
    //                        << " handle:"        << this->handleSensor
    //                        << " cover:"         << this->coverSensor
    //                        << " netBox:"        << this->netWaterBox
    //                        << " dirtyBox:"      << this->dirtyWaterBox
    //                        << " netLevel:"      << this->netWaterLevel
    //                        << " dirtyLevel:"    << this->dirtyWaterLevel
    //                        << " stepperSensor:" << this->stepperMotorOriginSensor;
    // }
}

/**
 * @brief 解析超声波距离信息
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::ult_Distance(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }
    this->ultFrontLeft  = toUint16BE(&data[2]);
    this->ultFrontRight = toUint16BE(&data[5]);
    this->ultLeft       = toUint16BE(&data[8]);
    this->ultRight      = toUint16BE(&data[11]);
    this->ultBack       = toUint16BE(&data[14]);
    this->ultRearLeft   = toUint16BE(&data[17]);
    this->ultRearRight  = toUint16BE(&data[20]);
    // if (this->logSwitch) {
    //     qDebug().nospace() << "SysTime:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
    //                        << " 前左超声波:"  << this->ultFrontLeft
    //                        << " 前右超声波:"  << this->ultFrontRight
    //                        << " 左侧超声波:"   << this->ultLeft
    //                        << " 右侧超声波:"   << this->ultRight
    //                        << " 后超声波:"   << this->ultBack
    //                        << " 后左超声波:"  << this->ultRearLeft
    //                        << " 后右超声波:"  << this->ultRearRight;
    // }
}

/**
 * @brief 解析轮毂电机状态信息（无时间戳）
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::wheel_Status(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }
    this->wheelEn    = data[0];
    this->leftSpeed  =  static_cast<int16_t>(data[1] << 8) | static_cast<int16_t>(data[2]);
    this->rightSpeed =  static_cast<int16_t>(data[3] << 8) | static_cast<int16_t>(data[4]);
    this->leftOdo    =  static_cast<uint32_t>(data[5] << 24) |
                        static_cast<uint32_t>(data[6] << 16) |
                        static_cast<uint32_t>(data[7] << 8)  |
                        static_cast<uint32_t>(data[8]);
    this->rightOdo   =  static_cast<uint32_t>(data[9] << 24)  |
                        static_cast<uint32_t>(data[10] << 16) |
                        static_cast<uint32_t>(data[11] << 8)  |
                        static_cast<uint32_t>(data[12]);
    this->wheelTrack  = static_cast<uint16_t>(data[13] << 8) | static_cast<uint16_t>(data[14]);
    this->wheelRadius = static_cast<uint16_t>(data[15] << 8) | static_cast<uint16_t>(data[16]);
    if (this->logSwitch) {
        qDebug().nospace() << "SysTime:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
                           << " 电机使能:"     << this->wheelEn
                           << " Left_Spd:" << this->leftSpeed
                           << " Right_Spd:" << this->rightSpeed
                           << " Left_Odo:"   << this->leftOdo
                           << " Right_Odo:"   << this->rightOdo
                           << " Left_Temp:"  << this->leftTemp
                           << " Right_Temp:"  << this->rightTemp
                           << " Left_Current:"   << this->leftCurrent
                           << " Right_Current:"   << this->rightCurrent;
    }
}

/**
 * @brief 解析轮毂电机的温度、电流信息
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::wheel_Temp_Current_Status(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }
    int16_t leftTemp     = static_cast<int16_t>(data[0] << 8) | static_cast<int16_t>(data[1]);
    int16_t rightTemp    = static_cast<int16_t>(data[2] << 8) | static_cast<int16_t>(data[3]);
    this->leftCurrent    = static_cast<int16_t>(data[4] << 8) | static_cast<int16_t>(data[5]);
    this->rightCurrent   = static_cast<int16_t>(data[6] << 8) | static_cast<int16_t>(data[7]);
    this->leftTemp       = static_cast<float>(leftTemp) * 0.1f;
    this->rightTemp      = static_cast<float>(rightTemp) * 0.1f;
}

/**
 * @brief 解析SLAM IMU消息
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::low_Frequence_SLAM_IMU(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }
    int16_t roll  = static_cast<int16_t>(data[0] << 8) | static_cast<int16_t>(data[1]);
    int16_t pitch = static_cast<int16_t>(data[2] << 8) | static_cast<int16_t>(data[3]);
    int16_t yaw   = static_cast<int16_t>(data[4] << 8) | static_cast<int16_t>(data[5]);
    int16_t temp  = static_cast<int16_t>(data[6] << 8) | static_cast<int16_t>(data[7]);
    this->roll    = static_cast<float>(roll)  * 0.1f;
    this->pitch   = static_cast<float>(pitch) * 0.1f;
    this->yaw     = static_cast<float>(yaw)   * 0.1f;
    this->temp    = static_cast<float>(temp)  * 0.1f;
    // if (this->logSwitch) {
    //     qDebug().nospace() << "SysTime:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
    //                        << " 翻滚角:"  << this->wheelEn
    //                        << " 俯仰角:"  << this->leftSpeed
    //                        << " 偏航角:"  << this->rightSpeed
    //                        << " imu温度:" << this->leftOdo;
    // }
}

/**
 * @brief 解析SLAM IMU消息
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::direct_Move_Ctrl(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }
    this->navWheelEn = data[0];
    this->navLinearSpeed = static_cast<int16_t>(data[1] << 8) | static_cast<int16_t>(data[2]);
    this->navAngleSpeed = static_cast<int16_t>(data[3] << 8) | static_cast<int16_t>(data[4]);

    if (this->logSwitch) {
        qDebug().nospace() << "SysTime:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
                           << " NavWheelEn:"   << this->wheelEn
                           << " NavLinearSpd:" << this->leftSpeed
                           << " NavAngleSpd:"  << this->rightSpeed;
    }
}


/**
 * @brief 解析托刷电机状态信息
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::mop_Status(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }

    int16_t temp   = static_cast<int16_t>(data[1] << 8) | static_cast<int16_t>(data[2]);
    this->mopSpeed = static_cast<uint16_t>(data[3] << 8) | static_cast<uint16_t>(data[4]);
    this->mopTemp  = static_cast<float>(temp) * 0.1f;
    // if (this->logSwitch) {
    //     qDebug().nospace() << "SysTime:"  << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
    //                        << " 托刷温度:" << this->mopTemp
    //                        << " 托刷速度:" << this->mopSpeed
    //                        << " 滚刷温度:" << this->rollBrushTemp
    //                        << " 滚刷速度:" << this->rollBrushSpeed;
    //     qDebug().nospace() << "Data (Hex): "
    //                        << QString("0x%1").arg(static_cast<uint8_t>(data[0]), 2, 16, QChar('0')).toUpper() << " "
    //                        << QString("0x%1").arg(static_cast<uint8_t>(data[1]), 2, 16, QChar('0')).toUpper() << " "
    //                        << QString("0x%1").arg(static_cast<uint8_t>(data[2]), 2, 16, QChar('0')).toUpper() << " "
    //                        << QString("0x%1").arg(static_cast<uint8_t>(data[3]), 2, 16, QChar('0')).toUpper() << " "
    //                        << QString("0x%1").arg(static_cast<uint8_t>(data[4]), 2, 16, QChar('0')).toUpper();
    // }
}

/**
 * @brief 解析滚刷电机状态信息
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::roll_Brush_Status(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }
    this->rollBrushSpeed = static_cast<uint16_t>(data[0] << 8) | static_cast<uint16_t>(data[1]);
    int16_t temp         = static_cast<int16_t>(data[2] << 8) | static_cast<int16_t>(data[3]);
    this->rollBrushTemp  = static_cast<float>(temp) * 0.1f;
    // 打印加在mop_Status()方法一起打印，方便观察
}

/**
 * @brief 解析地毯传感器的状态
 *
 * @param data : 数据段首地址
 * @param dataLen 数据长度
 */
void robotMessage::carpet_Sensor_Status(const uint8_t* const data, uint32_t dataLen) {
    if (nullptr == data) {
        LOG << "Data pointer is null";
        return;
    }

    this->carpetLeft = data[0] & 0x01;         // 提取bit0
    this->carpetRight = (data[0] >> 1) & 0x01; // 提取bit1
    //打印
    // if (this->logSwitch) {
    //     qDebug().nospace() << "SysTime:"  << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
    //                        << " 左轮侧的地毯传感器:" << this->carpetLeft
    //                        << " 右轮侧的地毯传感器:" << this->carpetRight;
    // }
}











