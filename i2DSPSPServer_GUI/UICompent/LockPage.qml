import QtQuick 2.0
import QtQuick.Controls 2.2

import "../images"

Item {
    id:lockpage
    width: 1280
    height: 1024
    opacity: 0
    MouseArea{
        anchors.fill: parent
        enabled: lockpage.opacity === 1 ? true : false
    }

    Rectangle {
        id: rectangle
        color: Qt.rgba(0,0,0,0.8)
        anchors.fill: parent

        Text {
            id: text1
            x: 343
            y: 328
            width: 594
            height: 144
            color: "#ffffff"
            text: qsTr("长按解锁")
            anchors.horizontalCenterOffset: 0
            anchors.verticalCenterOffset: 33
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 80

            SequentialAnimation on opacity {
                loops: Animation.Infinite
                alwaysRunToEnd: true
                running: true
                NumberAnimation {
                    to: 0.1
                    duration: 1500
                }
                NumberAnimation {
                    to: 1
                    duration: 3000
                }
            }
        }

        DelayButton{
            id:btnunlock
            x: 590
            y: 354
            width: 100
            height: 100
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            delay: 2000

            background: Rectangle{
                width: parent.width
                height: parent.height*btnunlock.progress
                radius: 10
                color: "green"
            }

            onActivated: {
                btnunlock.progress = 0
                lockpage.opacity = 0
            }

            Image {
                id: image
                width: 100
                height: 100
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
//                source: "images/Finger.png"
                source: "qrc:/images/Finger.png"
            }
        }

    }

    onOpacityChanged:  {
        if(lockpage.opacity === 1)
            return
        animation.start()
    }
    NumberAnimation {
        id: animation
        target: lockpage
        property: "opacity"
        from: 1
        to: 0
        duration: 500
        //        loops: 1
        //        onRunningChanged: {
        //            if (!running) {
        //                loop.stop()
        //                deviceIcon.destroy()
        //            }
        //        }
    }



}
