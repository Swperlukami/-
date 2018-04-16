import QtQuick 2.0

Item {
    width: 1280
    height: 50

    Rectangle {
        id: rectangle
        color: Qt.rgba(0,0,0,0.8)
        anchors.fill: parent

    }

    Row {
        id: row
        width: 1280
        height: 50

        Text {
            id: textStartTime
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("开始时间")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskName
            width: 200
            height: 50
            color: "#ffffff"
            text: qsTr("任务名称")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskDuration
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("持续时长")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskVol
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("播放音量")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskSongName
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("歌曲名称")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskStartDay
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("开始日期")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskEndDay
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("结束日期")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskWeekDay
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("工作日")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskEnable
            width: 100
            height: 50
            color: "#ffffff"
            text: qsTr("启用状态")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskDevs
            width: 600
            height: 50
            color: "#ffffff"
            text: qsTr("播放设备")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 18
        }
    }






}
