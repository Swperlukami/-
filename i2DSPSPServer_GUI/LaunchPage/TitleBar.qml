import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "../SysSingletonModel"

Rectangle {
    id: rectangle
    width: UISysModel.sysScreenWidth
    height: 40
    color: "#4b4b4b"

    signal visibleSysInfo
    Text {
        id: i2dsp
        x: 17
        y: 13
        color: "#ffffff"
        text: qsTr("恒业世纪")
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
        MouseArea {
            id: mouVisSysInfo
            anchors.fill: parent
            onClicked: {
                sysVersionPage.visible = true
            }
        }
    }

    Connections {
        target: mouVisSysInfo
        onClicked: {
            visibleSysInfo()
        }
    }

    Text {
        id: txtdatetime
        x: 840
        y: 16
        color: "#ffffff"
        text: qsTr(UISysModel.sysDateTime)
        property string property0: "none.none"
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
    }

    Text {
        id: sysState
        x: 197
        y: 11
        width: 111
        height: 18
        color: "white"
        text: {
            switch(UISysModel.sysState){
            case UISysModel.sysIdelState:
                return "空闲"
            case UISysModel.sysEmgState:
                return "紧急"
            case UISysModel.sysEmgTalkState:
                return "紧急"
            case UISysModel.sysTalkState:
                return "广播"
            case UISysModel.sysSelfTestState:
                return "自检"
            }

            qsTr(UISysModel.sysState)
        }
        anchors.horizontalCenterOffset: -100
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20

    }

    Rectangle {
        id: rectangle1
        x: 615
        y: 6
        width: 95
        height: 30
        color: UISysModel.sysMode === "Auto" ? "green" : "red"
        radius: 5
        anchors.right: txtdatetime.left
        anchors.rightMargin: 20

        Text {
            id: sysMode
            y: 5
            width: 75
            height: 18
            color: "#ffffff"
            text: UISysModel.sysMode === "Auto" ? qsTr("自动模式") : "手动模式"
            anchors.horizontalCenter: parent.horizontalCenter
            font.capitalization: Font.AllUppercase
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 20

            MouseArea {
                id: mousSysMode
                anchors.fill: parent
            }
        }
    }

    Connections {
        target: mousSysMode
        onClicked: {
            if (UISysModel.sysMode === "Auto") {
                UISysModel.sysMode = "Manual"
            } else {
                UISysModel.sysMode = "Auto"
            }
        }
    }

    Text {
        id: screenlock
        y: 8
        width: 83
        height: 24
        color: "#ffffff"
        text: qsTr("屏幕锁定")
        anchors.left: i2dsp.right
        anchors.leftMargin: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        MouseArea {
            id: lockcreenmous
            anchors.fill: parent
            onClicked: {
                sysLockPage.opacity = 1
            }
        }
    }

    Connections {
        target: lockcreenmous
        onClicked: {
            lockview.opacity = 1
        }
    }
}
