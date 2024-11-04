#ifndef ROBOT_MESSAGE_H
#define ROBOT_MESSAGE_H

#include <QObject>

class robotMessage : public QObject
{
    Q_OBJECT
public:
    explicit robotMessage(QObject *parent = nullptr);
    void msgAnalysis(uint8_t cmdSetVal, uint8_t cmdIdVal, const uint8_t* const data, uint32_t dataLen);

private:
    enum class cmdSet : uint8_t {
        VCU  = 0x20, // 控制VCU集合
        MOVE = 0x30, // 运动控制于反馈集合
    };

    enum class VCU_cmdID : uint8_t {
        SENSOR_STATUS          = 0x01,  // 传感器设备状态
        ULTRASONIC_DISTANCE    = 0x03,  // 超声波传感器的实时距离值
        INFRARED_DISTANCE      = 0x04,  // 红外测距传感器的实时距离值
        FAN_MOTOR_STATUS       = 0x05,  // 吸尘电机状态
        STEPPER_STATUS         = 0x07,  // 升降电机状态信息
        THREE_WAY_VALVE_INFO   = 0x0A,  // 反馈三通电磁阀状态信息
        SUCTION_AND_LIFT_STATUS= 0x0C,  // 吸水机升降状态
        MOP_STATUS             = 0xA6,  // 托刷电机状态
        ROLLING_BRUSH_STATUS   = 0xA8,  // 滚刷电机状态
        WHEEL_TEMP_CUR_STATUS  = 0x24,  // 轮毂电机温度、电流信息
        CARPET_SENSOR          = 0x2C,  // 地毯传感器的状态
    };

    enum class MOVE_cmdID : uint8_t {
        WHEEL_STATUS           = 0x01,  // 电机运动状态（无时间戳）
        LOW_FREQ_IMU_STATUS    = 0x03,  // 低频IMU状态数据
        CONTROL_WHOLE_VEHICLE  = 0x0A,  // 上层控制车体运动
        CONTROL_WHEEL          = 0x11,  // 直接控制底盘运动
        WHEEL_MOTOR_ENCODER    = 0x20,  // 轮毂电机和编码器状态码
    };

    /* 开关传感器状态 */
    bool stopButton = false;               // 急停按钮
    bool bumperStrip = false;              // 触边
    bool handleSensor = false;             // 把手传感器
    bool coverSensor = false;              // 上盖传感器
    bool netWaterBox = false;              // 清水箱
    bool dirtyWaterBox = false;            // 污水箱
    bool netWaterLevel = false;            // 清水箱液位
    bool dirtyWaterLevel = false;          // 污水箱液位
    bool stepperMotorOriginSensor = false; // 步进电机原点
    bool carpetLeft = false;              // 左轮侧地毯传感器
    bool carpetRight = false;             // 右轮侧地毯传感器
    /* 超声波 */
    uint16_t ultFrontLeft = 0;             // 前左超声波
    uint16_t ultFrontRight = 0;            // 前右超声波
    uint16_t ultLeft = 0;                  // 左侧超声波
    uint16_t ultRight = 0;                 // 右侧超声波
    uint16_t ultRearLeft = 0;              // 后左超声波
    uint16_t ultRearRight = 0;             // 后右超声波
    uint16_t ultBack = 0;                  // 后超声波
    /* 轮毂电机 */
    bool     wheelEn = 0;                  // 使能(true,false)
    int16_t  leftSpeed = 0;                // 左轮轮速（单位0.1rpm)
    int16_t  rightSpeed = 0;               // 右轮轮速（单位0.1rpm)
    uint32_t leftOdo = 0;                  // 左轮里程计（16384/圈）
    uint32_t rightOdo = 0;                 // 右轮里程计（16384/圈）
    uint16_t wheelTrack = 0;               // 轮距（单位0.1mm)
    uint16_t wheelRadius = 0;              // 轮子半径（单位0.1mm）
    float    leftTemp = 0;                 // 左轮温度（单位0.1摄氏度）
    float    rightTemp = 0;                // 右轮温度（单位0.1摄氏度）
    int16_t  leftCurrent = 0;              // 左轮电流（单位1ma)
    int16_t  rightCurrent = 0;             // 右轮电流（单位1ma)
    /* 导航直接控制底盘 */
    bool    navWheelEn = 0;                // 导航下发的使能控制
    int16_t navLinearSpeed = 0;            // 导航下发的线速度
    int16_t navAngleSpeed = 0;             // 导航下发的角速度

    /* SLAM-IMU */
    float    roll = 0;                     // 横滚角
    float    pitch = 0;                    // 俯仰角
    float    yaw = 0;                      // 偏航角
    float    temp = 0;                     // 温度
    /* 清洁电机 */
    float    mopTemp = 0;                  // 托刷电机温度（单位0.1摄氏度）
    float    mopSpeed = 0;                 // 托刷电机实时转速（脉冲/ms）
    float    rollBrushTemp = 0;            // 滚刷电机温度（单位0.1摄氏度）
    float    rollBrushSpeed = 0;           // 滚刷电机实时转速（ms/脉冲）
    /* 充电桩 */
    bool clips_stat = false;               // 弹片是否被压下（false:未压下，true:压下）
    bool chg_stat   = false;               // 电极片是否检测到机器人电压（false:没有电压，true:有电压）
    bool pwr_stat   = false;               // 是否闭合继电器（false:没闭合，true:闭合）
    bool ir_en      = false;               // 是否打开发射器（false:未打开，true:打开）
    QString Charge_Pile_SN = "";           // 充电桩的SN号
    uint8_t Charge_Ver[4] = {0,};          // 充电桩版本号

    bool logSwitch = true;                 // Log打印开关

    void sensor_Status(const uint8_t* const data, uint32_t dataLen);
    void ult_Distance(const uint8_t* const data, uint32_t dataLen);
    void infrared_Distance(const uint8_t* const data, uint32_t dataLen);
    void fan_Motor_Status(const uint8_t* const data, uint32_t dataLen);
    void stepper_Status(const uint8_t* const data, uint32_t dataLen);
    void mop_Status(const uint8_t* const data, uint32_t dataLen);
    void roll_Brush_Status(const uint8_t* const data, uint32_t dataLen);
    void carpet_Sensor_Status(const uint8_t* const data, uint32_t dataLen);

    void wheel_Status(const uint8_t* const data, uint32_t dataLen);
    void wheel_Temp_Current_Status(const uint8_t* const data, uint32_t dataLen);
    void low_Frequence_SLAM_IMU(const uint8_t* const data, uint32_t dataLen);
    void direct_Move_Ctrl(const uint8_t* const data, uint32_t datalen);

signals:



};

#endif // ROBOT_MESSAGE_H
