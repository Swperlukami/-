import QtQuick 2.0
import QtQuick.Controls 2.2
//import QtQuick.Controls.Styles 1.4

import "../SysSingletonModel"

Item {
    id:timedTaskPage
    width: 1280
    height: 750

    TimedTaskPageModel{
        id:sysTimedTaskPageModel
    }

    ChannelTimedTaskHander{
        id:channelTimedTaskhander
        channelID: UISysModel.sysTimedTaskChannel
    }
    property alias toadyTImedTaskHanderAlias: channelTimedTaskhander
    property alias toadyTImedTaskPageAlias: sysTodayTaskPage
    property alias timedTaskManagePageAlias: sysTimdTaskManagePage

    TabBar {
        id: tabBar
        x: 0
        y: 0
        width: 1280
        height: 40
        anchors.horizontalCenter: parent.horizontalCenter
        TabButton {
            id: tabButtonTodayTImedTask
            x: 144
            y: 0
            text: qsTr("今日任务")
            font.pixelSize: 22
        }
        TabButton {
            id: tabButtonTimedTaskManage
            x: 570
            y: 0
            text: qsTr("任务管理")
            font.pixelSize: 22
        }
    }

    TodayTimedTaskPage{
        id:sysTodayTaskPage
        anchors.top: tabBar.bottom
        anchors.topMargin: 0
        visible: tabButtonTodayTImedTask.checked
    }


    TimedTaskManagePage{
        id:sysTimdTaskManagePage
        anchors.top: tabBar.bottom
        anchors.topMargin: 0
        visible: tabButtonTimedTaskManage.checked

    }
//    TabView {
//        id: tabView
//        anchors.fill: parent

//        Tab{
//            id:todayTimedTask
//            title: qsTr("今日任务")

//        }
//        Tab{
//            id:mangeTimedTask
//            title: qsTr("任务管理")

//        }

//        style: TabViewStyle {
//            frameOverlap: 1
//            tab: Rectangle {
//                color: styleData.selected ? "transparent" : Qt.rgba(0,0,0,0.4)
//                //                     border.color:  "steelblue"
//                //                     implicitWidth: Math.max(text.width + 4, 80)
//                implicitWidth: tabView.width/2

//                implicitHeight: 40
//                radius: 2
//                Text {
//                    id: txtTabTitle
//                    anchors.centerIn: parent
//                    text: styleData.title
//                    font.pixelSize: 22
//                    color: styleData.selected ? "white" : "black"
//                }
//            }
//            frame: Rectangle { color: "transparent" }
//        }
//    }



    Component.onCompleted:{
    }

}
