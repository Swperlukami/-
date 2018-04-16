import QtQuick 2.0

Item {
    width: 1280
    height: 50

    signal timedTaskManageItemClicked(int index)
    Rectangle {
        id: rectangle
        color: (index%2) !== 0 ? Qt.rgba(0,0,0,0.4) : Qt.rgba(1,1,1,0.1)
        anchors.fill: parent

        Rectangle {
            id: rectangle1
            width: 5
            height: 50
            color: "green"
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            visible: listView.currentIndex === index
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            listView.currentIndex = index
//            timedTaskManageItemClicked(index)
            timedTaskEditPage.setCurrentTimedTaskItem(index ,sysTimedTaskPageModel.manageTimedTaskInfoModel.get(index))
        }
    }

    Row {
        id: row
        width: 1280
        height: 50

        Text {
            id: textStartTime
            width: 100
            height: 50
            color: "#000000"
            text: startTime
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskName
            width: 200
            height: 50
            color: "#000000"
            text: taskName
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskDuration
            width: 100
            height: 50
            color: "#000000"
            text: duration
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskVol
            width: 100
            height: 50
            color: "#000000"
            text: volume
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskSongName
            width: 100
            height: 50
            color: "#000000"
            text: audioName
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskStartDay
            width: 100
            height: 50
            color: "#000000"
            text: startDay
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskEndDay
            width: 100
            height: 50
            color: "#000000"
            text: endDay
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskWeekDay
            width: 100
            height: 50
            color: "#000000"
            text: {
                var weekdayStr = ""
                for(var index = 0 ; index < 7 ; index++){
                    if(weekDay[index] === '1'){
                        weekdayStr =weekdayStr+" " +sysTimedTaskPageModel.getStrWeekDayName(index)
                    }
                }

                return weekdayStr
            }

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskEnable
            width: 100
            height: 50
            color: "#000000"
            text: enable ? "已启用" : "已停用"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskDevs
            width: 600
            height: 50
            color: "#000000"
            text: devicesName
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 12
        }
    }






}
