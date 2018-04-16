import QtQuick 2.0

Item {
    id: item1
    width: 1280
    height: 50

    Rectangle {
        id: rectangle
        color: Qt.rgba(0,0,0,0.8)
        anchors.fill: parent
    }


    Row{
        id: row
        anchors.fill: parent
        Text {
            id: textStartTime
            width: 100
            height: 50
            color: "#ffffff"
            text: "开始时间"
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskName
            width: 200
            height: 50
            color: "#ffffff"
            text: "任务名称"
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textEnable
            width: 60
            height: 50
            color: "#ffffff"
            text: "临时启停"
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: textTaskDuration
            width: 100
            height: 50
            color: "#ffffff"
            text: "持续时长"
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskVol
            width: 100
            height: 50
            color: "#ffffff"
            text: "播放音量"
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskSongName
            width: 100
            height: 50
            color: "#ffffff"
            text: "歌曲名称"
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }

        Text {
            id: textTaskDevs
            width: 600
            height: 50
            color: "#ffffff"
            text: "播放设备"
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 18
        }


    }
}
