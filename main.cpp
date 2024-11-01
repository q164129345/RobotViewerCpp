#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QMqttClient>
#include <QDebug>
#include <QTimer>
#include "component/cr_protocol.h"
#include "component/robot_message.h"
#include "component/mqttclient.h"
#include "QQmlContext"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    crProtocol protocol;    // 创建crProtocol对象
    robotMessage robotMsg;  // 创建robotMessage对象
    mqttClient mqttHandler;

    engine.rootContext()->setContextProperty("mqttClient",&mqttHandler); // 让QML认识实例化的mqttHandler

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("RobotViewerCpp", "Main");

    mqttHandler.init(protocol, robotMsg);
    mqttHandler.setClientID("Robot_Tool");

    return app.exec();
}
