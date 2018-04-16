import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4

import "../SysSingletonModel"
import "../UICompent"

Item {
    id:todayTimedTaskPage
    width: 1280
    height: 710

    ListView {
        id: listView
        width: 1280
        height: 760
//        spacing: 1
        anchors.horizontalCenter: parent.horizontalCenter
        delegate: TodayTaskItem{

        }
        header: TodayTimedTaskHeader{
            height: 50
        }

        model: sysTimedTaskPageModel.todayTimedTaskInfoModel
    }






//    TableView{
//        id:sysTodayTimedTaskTableView
//        height: 720
//        width: 1280


//        anchors.bottom: tabBar.top
//        anchors.bottomMargin: 0
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.left: parent.left
//        anchors.leftMargin: 0
//        model: sysTimedTaskPageModel.todayTimedTaskInfoModel
//        headerDelegate: sysTimedTaskPageModel.timedTaskTableHeaderDel
//        frameVisible: false
//        backgroundVisible: false
//        rowDelegate: sysTimedTaskPageModel.timedTaskTableRowDel

//        TableViewColumn {
//            role: "startTime"
//            title: "开始时间"
//            width: 100
//            horizontalAlignment: Text.AlignHCenter

//        }
//        TableViewColumn {
//            role: "taskName"
//            title: "任务名"
//            width: 200
//            horizontalAlignment: Text.AlignHCenter

//        }
//        TableViewColumn {
//            role: "duration"
//            title: "播放时长"
//            width: 100
//            horizontalAlignment: Text.AlignHCenter

//        }
//        TableViewColumn {
//            role: "volume"
//            title: "音量"
//            width: 100
//            horizontalAlignment: Text.AlignHCenter

//        }
//        TableViewColumn {
//            role: "audioName"
//            title: "歌曲名"
//            width: 100
//            horizontalAlignment: Text.AlignHCenter

//        }
//        TableViewColumn {
//            role: "devicesName"
//            title: "播放设备"
//            width: 680
////            horizontalAlignment: Text.AlignHCenter
//        }
//    }







//    function channelClicked(index){
//        console.log("User Selcet Today Timed Task Cnannel Index:" , index)
//    }

//    TabBar {
//        id: tabBar
//        width: 1280
//        height: 40
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 0
//        background: Item {
//            anchors.fill: parent
//        }

//        Repeater{
//            model: 5
//            TabButton {
//                id: tabButton
//                text: qsTr("频道-"+(index+1))
//                height: 40
//                onClicked: channelClicked(index)
//                background: Item {
//                    anchors.fill: parent
//                }
//                contentItem: Rectangle {
//                    anchors.fill: parent
//                    color: tabButton.checked ? "transparent" : Qt.rgba(1,1,1,0.4)

//                    Text {
//                        anchors.fill: parent
//                        text: qsTr(tabButton.text)
//                        font: tabButton.font
//                        color: tabButton.checked ? "white" : "black"
//                        verticalAlignment: Text.AlignVCenter
//                        horizontalAlignment: Text.AlignHCenter
//                    }
//                }
//            }
//        }


//    }

}
