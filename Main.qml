import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: qsTr("Robot Tool")
    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                spacing: 5
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                Button {
                    Layout.preferredWidth: 100
                    onClicked: stackLayout.currentIndex = 0
                    contentItem: Text {
                        text: qsTr("首页")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.margins: 10
                    }
                }
                Button {
                    Layout.preferredWidth: 100
                    onClicked: stackLayout.currentIndex = 1
                    contentItem: Text {
                        text: qsTr("传感器")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.margins: 10
                    }
                }
                Button {
                    Layout.preferredWidth: 100
                    onClicked: stackLayout.currentIndex = 2
                    contentItem: Text {
                        text: qsTr("清洁电机")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.margins: 10
                    }
                }
                Button {
                    Layout.preferredWidth: 100
                    onClicked: stackLayout.currentIndex = 3
                    contentItem: Text {
                        text: qsTr("轮毂电机")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.margins: 10
                    }
                }
                Button {
                    Layout.preferredWidth: 100
                    onClicked: stackLayout.currentIndex = 4
                    contentItem: Text {
                        text: qsTr("充电桩")
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 16
                        anchors.margins: 10
                    }
                }
            }
            StackLayout {
                id: stackLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: 0
                Rectangle {
                    color: "#00B000"
                    Text {
                        text: qsTr("首页")
                        anchors.centerIn: parent
                        font.pixelSize: 24
                    }
                }
                Rectangle {
                    color: "steelblue"
                    Text {
                        text: qsTr("传感器")
                        anchors.centerIn: parent
                        font.pixelSize: 24
                    }
                }
                Rectangle {
                    color: "lightgrey"
                    Text {
                        text: qsTr("清洁电机")
                        anchors.centerIn: parent
                        font.pixelSize: 24
                    }
                }
                Rectangle {
                    color: "lightgrey"
                    Text {
                        text: qsTr("轮毂电机")
                        anchors.centerIn: parent
                        font.pixelSize: 24
                    }
                }
                Rectangle {
                    color: "lightgrey"
                    Text {
                        text: qsTr("充电桩")
                        anchors.centerIn: parent
                        font.pixelSize: 24
                    }
                }
            }
        }
        RowLayout {
            Layout.fillHeight:  true
            Layout.preferredHeight: 20
            spacing: 20
            Item {
                width: 1
                height: 1
                Layout.alignment: Qt.AlignLeft
            }
            Rectangle{
                id: ledIndicator
                width: 15
                height: 15
                radius: 7.5
                color: "red"
                Layout.alignment: Qt.AlignLeft
            }
            Text {
                text: qsTr("IP:")
                Layout.alignment: Qt.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: brokerIp
                text: "127.0.0.1"
                placeholderText: qsTr("broker IP")
                Layout.preferredWidth: 130
                Layout.alignment: Qt.AlignLeft
            }
            Text {
                text: qsTr("Port:")
                Layout.alignment: Qt.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: brokerPort
                text: "1883"
                placeholderText: qsTr("port")
                Layout.preferredWidth: 80
                Layout.alignment: Qt.AlignLeft
            }
            Button {
                id: connectButton
                text: qsTr("连接")
                Layout.alignment: Qt.AlignLeft
                onClicked: {
                    if (mqttClient.getConnectionStatus() === false) {
                        var ip = brokerIp.text;
                        var port = parseInt(brokerPort.text);
                        mqttClient.connectToHost(ip, port);
                        console.log("connectToHost:",mqttClient.getConnectionStatus());
                    } else {
                        mqttClient.disconnectFromHost();
                        console.log("disconnectfromHost:",mqttClient.getConnectionStatus());
                    }
                }
            }
        }
    }

    Connections {
        target: mqttClient
        function onConnectionStatusChanged(state) {
            if (state === 1) {
                ledIndicator.color = "green";
                connectButton.text = qsTr("断开连接");
            } else if (state === 0) {
                ledIndicator.color = "red";
                connectButton.text = qsTr("连接");
            }
        }
    }
}
