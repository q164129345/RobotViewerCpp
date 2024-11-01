#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QObject>
#include <QMqttClient>
#include <component/cr_protocol.h>
#include <component/robot_message.h>
#include <component/common.h>

class mqttClient : public QObject
{
    Q_OBJECT
public:
    explicit mqttClient(QObject *parent = nullptr);
    void init(crProtocol& cp, robotMessage& robotMsg);
    Q_INVOKABLE void connectToHost(const QString& brokerIp, quint16 brokerPort);
    Q_INVOKABLE void disconnectFromHost();
    // 设置器
    void setClientID(const QString& clientID);
    // 获取器
    Q_INVOKABLE bool getConnectionStatus() const {return mConnectionStatus;}

private:
    QMqttClient mClient;
    QString     mClientID;
    bool        mConnectionStatus = false;

signals:
    void connectionStatusChanged(quint8 value);
};

#endif // MQTTCLIENT_H
