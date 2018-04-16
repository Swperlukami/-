import QtQuick 2.0
import QtQuick.Controls 2.2
import "../SysSingletonModel"

Item {
    id: item1
    width: 1200
    height: 60
    visible: true

    property bool  showUserDetail: false
    Rectangle{
        id: rectangle
        radius: 5
        anchors.fill: parent
        color: UISysModel.usrID === userID ? Qt.rgba(0.4,1,1,0.8) :Qt.rgba(1,1,1,0.6)

        Text {
            id: text1
            x: 83
            text: qsTr("用户:")
            anchors.top: parent.top
            anchors.topMargin: 13
            font.pixelSize: 22
        }

        Text {
            id: txtUserName
            width: 299
            height: 26
            text: userName
            anchors.top: text1.top
            anchors.topMargin: 0
            anchors.left: text1.right
            anchors.leftMargin: 40
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 22
        }

        RoundButton {
            id: roundButtonDetail
            x: 1106
            width: 74
            height: 30
            text: showUserDetail ? "收起" : "详情"
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.right: parent.right
            anchors.rightMargin: 20
            visible: UISysModel.usrlv >= userLv
            onClicked: {
                showUserDetail = !showUserDetail
            }
        }

        Text {
            id: txtUserLv
            x: 696
            width: 299
            height: 26
            text: {
                switch(userLv){
                case 1:
                    return "普通用户"
                case 2:
                    return "管理员"
                case 3:
                    return "系统管理员"
                }
            }
            anchors.top: text1.top
            anchors.topMargin: 0

            anchors.left: text1.right
            anchors.leftMargin: 550
            font.pixelSize: 22
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: text2
            x: 74
            y: 9
            text: qsTr("用户:")
            visible: false
            anchors.top: parent.top
            font.pixelSize: 22
            anchors.topMargin: 13
        }

        TextInput {
            id: txtUserNameDetail
            x: 6
            y: 9
            width: 299
            height: 26
            text: userName
            visible: false
            anchors.left: text1.right
            anchors.leftMargin: 40
            anchors.top: text1.top
            font.pixelSize: 22
            anchors.topMargin: 0
            horizontalAlignment: Text.AlignHCenter
            enabled: roundButtonModifyPSWD.checked ? true : false
            Rectangle{
                width: parent.width
                height: 1
                color: "black"
                anchors.bottom: parent.bottom
            }

        }

        Text {
            id: text3
            x: 77
            y: 2
            text: qsTr("用户:")
            anchors.top: parent.top
            font.pixelSize: 22
            visible: false
            anchors.topMargin: 13

        }

        TextInput {
            id: usrpasswd
            x: 814
            y: 96
            text: qsTr("Text")
            opacity: 0
            font.pixelSize: 12
            clip:true
            enabled: roundButtonModifyPSWD.checked ? true : false
            Rectangle{
                width: parent.width
                height: 1
                color: "black"
                anchors.bottom: parent.bottom
            }
        }

        RoundButton {
            id: roundButtonModifyPSWD
            x: 274
            y: 246
            text: "+"
            opacity: 0
        }

        RoundButton {
            id: roundButtonDelUser
            x: 476
            y: 246
            text: "+"
            opacity: 0
        }

//        TextInput {
//            id: textInputPswd
//            x: 781
//            y: 133
//            width: 80
//            height: 20
//            text: qsTr("Text Input")
//            opacity: 0
//            font.pixelSize: 12
//            clip:true
//            Rectangle{
//                width: parent.width
//                height: 1
//                color: "black"
//                anchors.bottom: parent.bottom
//            }

//        }

//        TextInput {
//            id: textInputNewPSwd
//            x: 771
//            y: 125
//            width: 80
//            height: 20
//            text: qsTr("Text Input")
//            opacity: 0
//            font.pixelSize: 12
//            clip:true
//            Rectangle{
//                width: parent.width
//                height: 1
//                color: "black"
//                anchors.bottom: parent.bottom
//            }

//        }

//        TextInput {
//            id: textInput
//            x: 781
//            y: 213
//            width: 80
//            height: 20
//            text: qsTr("Text Input")
//            opacity: 0
//            font.pixelSize: 12
//            clip:true
//            Rectangle{
//                width: parent.width
//                height: 1
//                color: "black"
//                anchors.bottom: parent.bottom
//            }
//        }

        Text {
            id: txtUserLvDetail
            x: 71
            y: 18
            text: qsTr("用户:")
            anchors.top: parent.top
            font.pixelSize: 22
            visible: false
            anchors.topMargin: 13
        }

        ComboBox {
            id: comboBoxUsrLv
            x: 188
            y: 152
            model: ["管理员" , "普通用户"]
            opacity: 0
            enabled: roundButtonModifyPSWD.checked ? true : false
        }
    }
    states: [
        State {
            name: "userDetail"
            when: showUserDetail

            PropertyChanges {
                target: rectangle
                height: 600
                radius: 2
                anchors.rightMargin: 0
                anchors.leftMargin: 0
            }

            PropertyChanges {
                target: item1
                height: 300
            }

            PropertyChanges {
                target: text2
                x: 83
                text: qsTr("用户名:")
                visible: true
                anchors.topMargin: 89
            }

            PropertyChanges {
                target: txtUserNameDetail
                x: 172
                y: 91
                visible: true
                anchors.leftMargin: 40
                anchors.topMargin: 79
            }

            PropertyChanges {
                target: text3
                x: 682
                text: qsTr("用户密码:")
                visible: true
                anchors.topMargin: 89
            }

            PropertyChanges {
                target: usrpasswd
                x: 781
                y: 92
                width: 200
                height: 26
                text: qsTr("hysj")
                horizontalAlignment: Text.AlignHCenter
                echoMode: TextInput.Password
                font.pixelSize: 22
                opacity: 1
            }

            PropertyChanges {
                target: roundButtonModifyPSWD
                x: 67
                y: 252
                width: 150
                height: 29
                text: roundButtonModifyPSWD.checked ? "保存" : "修改用户"
                checkable: true
                opacity: 1
                visible: UISysModel.usrID === userID

            }

            PropertyChanges {
                target: roundButtonDelUser
                x: 1030
                y: 252
                width: 150
                height: 29
                text: "删除用户"
                opacity: 1
                visible: UISysModel.usrID !== 1

            }

//            PropertyChanges {
//                target: textInputPswd
//                width: 200
//                height: 26
//                echoMode: TextInput.Password
//                font.pixelSize: 22
//                opacity: 1
//            }

//            PropertyChanges {
//                target: textInputNewPSwd
//                x: 781
//                y: 174
//                width: 200
//                height: 27
//                echoMode: TextInput.Password
//                font.pixelSize: 22
//                opacity: 1
//            }

//            PropertyChanges {
//                target: textInput
//                width: 200
//                height: 26
//                font.pixelSize: 22
//                echoMode: TextInput.Password
//                opacity: 1
//            }

            PropertyChanges {
                target: txtUserLvDetail
                x: 74
                width: 90
                height: 29
                text: qsTr("用户等级")
                visible: true
                anchors.topMargin: 163
            }

            PropertyChanges {
                target: comboBoxUsrLv
                x: 262
                y: 163
                width: 120
                height: 29
                opacity: 1
            }
        }
    ]
}
