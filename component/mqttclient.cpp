#include "mqttclient.h"
#include <QTimer>
#include <QDebug>

mqttClient::mqttClient(QObject *parent)
    : QObject{parent}
{}

void mqttClient::init(crProtocol& cp, robotMessage& robotMsg) {
    // 连接状态改变时输出状态信息
    QObject::connect(&mClient, &QMqttClient::stateChanged, &mClient, [this](QMqttClient::ClientState state) {
        LOG << "Client state changed:" << state;
        if (state == QMqttClient::ClientState::Connected) {
            mConnectionStatus = true;
            emit connectionStatusChanged(1); // 发送信号
        } else {
            mConnectionStatus = false;
            emit connectionStatusChanged(0); // 发送信号
        }
    });

    // 接收到消息时输出消息内容和主题
    QObject::connect(&mClient, &QMqttClient::messageReceived, &mClient, [&cp, &robotMsg](const QByteArray &message, const QMqttTopicName &topic) {
        //qDebug() << "Message received:" << message << "from topic:" << topic.name(); // 以字符串格式输出
        //qDebug() << "Message rec(hex):" << message.toHex() << "from topic " << topic.name(); // 以16进制格式输出
        if (topic.name() == "CHG_STAT") {
            qDebug() << "Message received:" << message << "from topic:" << topic.name(); // 以字符串格式输出
            qDebug() << "Message rec(string):" << message << "from topic " << topic.name(); // 以字符串格式输出
        } else {
            // 遍历message消息
            for (auto byte : message) {
                if(true == cp.parse_Byte(static_cast<uint8_t>(byte))) {
                    // 解数据包成功，数据包符合协议
                    // 执行响应的代码
                    uint8_t set,id;
                    cp.get_Cmd_Set_And_ID(set,id); // 获取set、id
                    uint8_t data[kDataLength] = {0,};
                    uint32_t dataLen = 0;
                    cp.get_Data_And_DataLen(data, dataLen);
                    robotMsg.msgAnalysis(set, id, data, dataLen); // 解析数据包
                }
            }
        }
    });

    // 成功连接到MQTT代理时进行订阅和定时发布消息
    QObject::connect(&mClient, &QMqttClient::connected, this, [this](){
        LOG << "Connected to broker";
        // 订阅主题
        QMqttSubscription *subVCU_STAT = mClient.subscribe(QMqttTopicFilter("VCU_STAT"), 0);
        if (!subVCU_STAT) {
            LOG << "Could not subscribe. Is there a valid connect";
        } else {
            LOG << "Subscribe to the topic VCU_STAT  successful";
        }

        QMqttSubscription *subMOV_STAT = mClient.subscribe(QMqttTopicFilter("MOV_STAT"), 0);
        if (!subMOV_STAT) {
            LOG << "Could not subscribe. Is there a valid connect";
        } else {
            LOG << "Subscribe to the topic MOV_STAT  successful";
        }

        QMqttSubscription *subIMU_DATA = mClient.subscribe(QMqttTopicFilter("IMU_DATA/LOW_FREQ_ATTITUDE"), 0);
        if (!subIMU_DATA) {
            LOG << "Could not subscribe. Is there a valid connect";
        } else {
            LOG << "Subscribe to the topic IMU_DATA/LOW_FREQ_ATTITUDE successful";
        }

        QMqttSubscription *subMOV_CTRL = mClient.subscribe(QMqttTopicFilter("MOV_CTRL"), 0);
        if (!subMOV_CTRL) {
            LOG << "Could not subscribe. Is there a valid connect";
        } else {
            LOG << "Subscirbe to the topic MOV_CTRL successful";
        }

        QMqttSubscription *subCHG_STAT = mClient.subscribe(QMqttTopicFilter("CHG_STAT"), 0);
        if (!subCHG_STAT) {
            LOG << "Could not subscribe. Is there a valid connect";
        } else {
            LOG << "Subscirbe to the topic CHG_STAT successful";
        }

        // 创建QTimer对象并设定定时发布消息
        QTimer *timer = new QTimer(this);
        QObject::connect(timer, &QTimer::timeout, this, [this](){
            mClient.publish(QMqttTopicName("test/Hello"), "Hello, World");
        });
        timer->start(2000); // 每隔2秒发布一次消息
    });
}

/**
 * @brief 连接MQTT服务器
 *
 * @param hostIp : MQTT服务器Ip
 * @param port   : MQTT服务器Port
 * @param clientID : 客户端ID
 * @return void
 */
void mqttClient::connectToHost(const QString& brokerIp, quint16 brokerPort) {
    mClient.setHostname(brokerIp);
    mClient.setPort(brokerPort);
    mClient.connectToHost();
}

/**
 * @brief 设置MQTT客户端ID
 *
 * @param clientID : 客户端ID
 */
void mqttClient::setClientID(const QString& clientID) {
    mClientID = clientID;
    mClient.setClientId(clientID);
}

/**
 * @brief 断开MQTT服务器的连接
 */
void mqttClient::disconnectFromHost() {
    mClient.disconnectFromHost();
}


