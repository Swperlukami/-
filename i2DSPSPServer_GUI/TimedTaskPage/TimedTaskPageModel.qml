import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls.Styles 1.4

QtObject {
    id:timedTaskPageModel



    function getStrWeekDayName(index){
        switch(index){
        case 0:
            return "日"
        case 1:
            return "一"
        case 2:
            return "二"
        case 3:
            return "三"
        case 4:
            return "四"
        case 5:
            return "五"
        case 6:
            return "六"
        }
    }

    function userTemSetTodayTimedTaskEnable(index , iseEnabled){
        manageTimedTaskInfoModel.setProperty(index , "enable" , iseEnabled)
    }

    function userModifyTimedTask(index , timedTaskItem){
        manageTimedTaskInfoModel.set(index , timedTaskItem)
       console.log("User Modify TimedTask", timedTaskItem["weekDay"])
    }

    property int curPlayingTaskIndex: -1


    property ListModel manageTimedTaskInfoModel: ListModel{
        id:timedTaskManageModel
        ListElement{
            taskNo:1
            taskName:"Task Name Test"
            channelNum:1
            enable:true
            audioName:"Get Up-1.mp3"
            startDay: "2018-03-19"
            endDay:"2018-03-30"
            weekDay:"0111110"                       //Sun=0
            startTime:"06:30:00"
            duration:30
            volume:40
            devicesName:"Device-1 | Device-2 | Device-3"
            devicesID: "192.168.2.53 | 192.168.2.54 | 192.168.2.55"
        }
        ListElement{
            taskNo:2
            taskName:"Task Name Test"
            channelNum:1
            enable:true
            audioName:"Get Up-2.mp3"
            startDay: "2018-03-19"
            endDay:"2018-03-30"
            weekDay:"0111110"                       //Sun=0
            startTime:"06:30:00"
            duration:30
            volume:40
            devicesName:"Device-1 | Device-2 | Device-3"
            devicesID: "192.168.2.53 | 192.168.2.54 | 192.168.2.55"
        }
    }

    property ListModel todayTimedTaskInfoModel: ListModel{
        id:timedTaskInfo
        ListElement{
            taskNo:1
            taskName:"Task Name Test"
            channelNum:1
            audioName:"Get Up-1.mp3"
            startDay: "2018-03-19"
            endDay:"2018-03-30"
//            weekDay:"0111110"                       //Sun=0
            startTime:"06:30:00"
            duration:30
            volume:40
            devicesName:"Device-1 | Device-2 | Device-3"
            devicesID: ""
            enable:true
        }
        ListElement{
            taskNo:2
            taskName:"Task Name Test"
            channelNum:1
            audioName:"Get Up-2.mp3"
            startDay: "2018-03-19"
            endDay:"2018-03-30"
//            weekDay:"0111110"                       //Sun=0
            startTime:"06:30:00"
            duration:30
            volume:40
            devicesName:"Device-1 | Device-2 | Device-3"
            devicesID: ""
            enable:true
        }
    }


    property Component timedTaskTableHeaderDel: Component{
        Rectangle{
            implicitHeight: 50
            color: Qt.rgba(0,0,0,0.8)
            clip: true

            Text{
                anchors.fill: parent
//                anchors.centerIn: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: styleData.value === "播放设备" ? Text.AlignLeft : Text.AlignHCenter
                text:styleData.value
                font.pixelSize: 20
                color: "white"

            }
        }
    }

    property Component timedTaskTableRowDel: Component{
        Rectangle{
            height: 50
            color: styleData.alternate ? Qt.rgba(0,0,0,0.4) : Qt.rgba(1,1,1,0.1)
        }
    }

    property Component tableViewScrollStyle: Component{
        TableViewStyle{
            scrollBarBackground: Rectangle{
                height: 20
//                color: "transparent"
                color: Qt.rgba(0,0,0,0.5)
            }
            handle: Rectangle{
                height: 16
                width: 300
                radius: 5
                color: "red"
//                color: Qt.rgba(1,1,1,1)
            }
        }
    }




}
