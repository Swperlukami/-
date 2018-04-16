import QtQuick 2.0
import "../SysSingletonModel"
Item {
    id:logview
    width: 1280
    height: 1024
    opacity: 1


    Rectangle {
        id: rectangle
        color: "#d3d1d1"
        radius: 0
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent


        Rectangle{
            id:logBK
            width: parent.width
            height: 230
            color: Qt.rgba(0,0,0,0.5)
        }
        Image {
            id: imgHYSLogo
            width: 150
            height: 150
            anchors.top: parent.top
            anchors.topMargin: 40
            anchors.left: parent.left
            anchors.leftMargin: 40
            fillMode: Image.Stretch
            source: "qrc:/images/HYS(1).png"
        }

        Text {
            id: hysName
            y: 86
            color: "#f06b00"
            text: qsTr("恒业世纪安全技术有限公司")
            anchors.left: imgHYSLogo.right
            anchors.leftMargin: 50
            anchors.verticalCenter: imgHYSLogo.verticalCenter
            styleColor: "#f06b00"
            font.pixelSize: 45
        }

        Image {
            id: imgUser
            x: 396
            y: 366
            width: 40
            height: 40
            anchors.horizontalCenterOffset: -160
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: imgPW.top
            anchors.bottomMargin: 15
            source: "qrc:/images/User.png"
        }



        Image {
            id: imgPW
            x: 396
            y: 512
            width: 40
            height: 40
            anchors.horizontalCenterOffset: -160
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: btnLogin.top
            anchors.bottomMargin: 20
            source: "qrc:/images/PassWord.png"
        }










        TextInput {
            id: textEditUser
            y: 407
            width: 300
            height: 40
            text: qsTr("Admin")
            anchors.left: imgUser.right
            anchors.leftMargin: 20
            clip: true
            anchors.bottom: imgUser.bottom
            anchors.bottomMargin: 0
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 22

            Rectangle {
                id: rectangle1
                x: 0
                y: 48
                width: parent.width
                height: 1
                color: "#000000"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
            }
        }






        TextInput {
            id: textEditPW
            y: 669
            width: 300
            height: 40
            text: qsTr("Admin")
            anchors.left: imgPW.right
            anchors.leftMargin: 20
            clip: true
            anchors.bottom: imgPW.bottom
            anchors.bottomMargin: 0
            echoMode: TextInput.Password
            font.pixelSize: 22
            horizontalAlignment: Text.AlignHCenter

            Rectangle {
                id: rectangle2
                x: 1
                y: 56
                width: parent.width
                height: 1
                color: "#000000"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
            }
        }

        Rectangle {
            id: btnLogin
            x: 396
            width: 360
            height: 40
            color: "#00ff1c"
            radius: 5
            anchors.top: parent.top
            anchors.topMargin: 700
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: txtlogin
                x: 220
                y: 10
                text: qsTr("登录")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 22
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    logview.visible = false
                    isSysReady = true
                }
            }
        }

        Text {
            id: txtLoginError
            x: 628
            width: 360
            height: 30
            color: "#ff0000"
            text: qsTr("用户名密码错误")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.top: btnLogin.bottom
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 22
            visible: false
        }
    }

}
