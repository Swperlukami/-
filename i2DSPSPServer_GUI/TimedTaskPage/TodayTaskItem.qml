import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    width: 1280
    height: 50

    Rectangle {
        id: rectangleBK
        color: (index%2) !== 0 ? Qt.rgba(0,0,0,0.4) : Qt.rgba(1,1,1,0.1)
        anchors.fill: parent
//        radius: 5
    }

    property color timedTaskIsPlaying: sysTimedTaskPageModel.curPlayingTaskIndex === index ? "green" : "white"

    Row{
        id: row
        anchors.fill: parent
        Text {
            id: textStartTime
            width: 100
            height: 50
            color: timedTaskIsPlaying
            text: startTime
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskName
            width: 200
            height: 50
            color: timedTaskIsPlaying
            text: taskName
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        RoundButton {
            id: roundButtonEnable
            width: 60
            height: 30
            text: roundButtonEnable.checked ? "关闭" : "开启"
            anchors.verticalCenter: parent.verticalCenter
            checkable: true
            onClicked: {
                sysTimedTaskPageModel.userTemSetTodayTimedTaskEnable(index , !roundButtonEnable.checked)
            }
        }

        Text {
            id: textTaskDuration
            width: 100
            height: 50
            color: timedTaskIsPlaying
            text: duration
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskVol
            width: 100
            height: 50
            color: timedTaskIsPlaying
            text: volume
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskSongName
            width: 100
            height: 50
            color: timedTaskIsPlaying
            text: audioName
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textTaskDevs
            width: 600
            height: 50
            color: timedTaskIsPlaying
            text: devicesName
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 12
        }


    }

}
