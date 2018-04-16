import QtQuick 2.0
import QtQuick.Controls 2.2

import "../SysSingletonModel"

Item {
    width: 1200
    height: 150

    //AP info
//    property alias itemapip: txtapip.text
    property alias spModel: listView.model
    onSpModelChanged: {
        console.log(index , "SPK Model Changed")
    }

    Rectangle{
        id: rectangle
        radius: 5
        anchors.fill: parent
        color: Qt.rgba(0,0,0,0.5)

        Rectangle {
            id: rectangle1
            width: 1200
            height: 42
            radius: 5
            color: {
                if(apState === UISysModel.enumAPOffline)
                    return UISysModel.colorAPOffline
                else if(apState === UISysModel.enumAPShort ||
                        apState === UISysModel.enumAPOpen)
                    return  UISysModel.colorDevError
                else
                    return UISysModel.colorAPOnline
            }
        }

        Text {
            id: text1
            y: 8
            color: "#ffffff"
            text: qsTr("功放IP:")
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.pixelSize: 22
        }

        Text {
            id: txtapip
            y: 8
            width: 154
            height: 26
            color: "#ffffff"
            text: apip
            anchors.left: text1.right
            anchors.leftMargin: 5
            font.pixelSize: 22
        }

        Text {
            id: text2
            y: 8
            color: "#ffffff"
            text: qsTr("状态:")
            anchors.left: txtapip.right
            anchors.leftMargin: 100
            font.pixelSize: 22
        }

        Text {
            id: txtapstate
            y: 8
            width: 96
            height: 26
            color: "#ffffff"
            text: {
                switch(apState){
                case UISysModel.enumAPOnline:
                    return "空闲"
                case UISysModel.enumAPOffline:
                    return "掉线"
                case UISysModel.enumAPMusic:
                    return "音乐"
                case UISysModel.enumAPTimedTask:
                    return "定时任务"
                case UISysModel.enumAPTalk:
                    return "业务广播"
                case UISysModel.enumAPEmgTalk:
                    return "应急话筒"
                case UISysModel.enumAPEmg:
                    return "应急广播"
                case UISysModel.enumAPShort:
                    return "短路"
                case UISysModel.enumAPOpen:
                    return "断路"
                }
            }

            anchors.left: text2.right
            anchors.leftMargin: 5
            font.pixelSize: 22
        }

        Text {
            id: text3
            y: 8
            color: "#ffffff"
            text: qsTr("位置:")
            anchors.left: txtapstate.right
            anchors.leftMargin: 100
            font.pixelSize: 22
        }

        Text {
            id: txtapAddr
            y: 8
            width: 345
            height: 26
            color: "#ffffff"
            text: qsTr(apAddr)
            anchors.left: text3.right
            anchors.leftMargin: 5
            font.pixelSize: 22
        }

        Text {
            id: text4
            x: 1045
            y: 8
            color: "#ffffff"
            text: qsTr("网络:")
            font.pixelSize: 22
        }

        Text {
            id: apNetSate
            y: 8
            width: 90
            height: 26
            color: "#ffffff"
            text: {
                if(apState === UISysModel.enumAPOffline)
                    return qsTr("断开")
                else
                    return qsTr("正常")
            }
            anchors.left: text4.right
            anchors.leftMargin: 5
            font.pixelSize: 22
        }

        ListView {
            id: listView
            x: 10
            y: 48
            width: 1180
            height: 68
            orientation: ListView.Horizontal
            model:spModel
            highlight: Rectangle{width: 52 ; height: 52 ; radius: 5 ;
                border.width: 1 ; border.color: "white" ; color: "transparent"}
            delegate: Item {
                id: item1
                x: 5
                width: 68
                height: 68
                    Rectangle {
                        width: 50
                        height: 50
                        radius: 5
                        anchors.centerIn: parent
                        color: {
                            switch(spstate){
                            case "grey":
                                return "grey"
                            case "green":
                                return "darkgreen"
                            case "yellow":
                                return "yellow"
                            }
                        }

                        Text {
                            text: subapno
                            font.bold: true
                            anchors.centerIn: parent
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: listView.currentIndex = index
                    }


            }
        }

        Text {
            id: text5
            x: -2
            y: 116
            color: "#ffffff"
            text: qsTr("扬声器位置:")
            font.pixelSize: 22
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        Text {
            id: txtSPKAdrr
            x: -5
            y: 116
            width: 438
            height: 29
            color: "#ffffff"
            text: {
                if(spModel.count === 0)
                    return ""
                else
                    return spModel.get(listView.currentIndex).spadrr
            }
            font.pixelSize: 22
            anchors.left: text5.right
            anchors.leftMargin: 5
        }

        Text {
            id: text6
            x: 2
            y: 116
            color: "#ffffff"
            text: qsTr("扬声器状态:")
            font.pixelSize: 22
            anchors.left: parent.left
            anchors.leftMargin: 553
        }

        Text {
            id: text7
            x: 1
            y: 116
            color: "#ffffff"
            text: {
                if(spModel.count === 0)
                    return ""
                switch(spModel.get(listView.currentIndex).spstate){
                case "grey":
                    return "掉线"
                case "green":
                    return "正常"
                case "yellow":
                    return "断开"

                }
            }

            font.pixelSize: 22
            anchors.left: text6.right
            anchors.leftMargin: 5
        }

        RoundButton {
            id: btnEditAdrr
            x: 1064
            y: 125
            width: 125
            height: 20
            checkable: true
            text: checked ? "保存" : qsTr("编辑设备位置")
        }

    }

    Component.onCompleted: {
        listView.currentIndex = 0
        console.log("APSPK Item Over" , spModel)
    }
}
