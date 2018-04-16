import QtQuick 2.0

Item {
    width: 1280
    height: 140

    Rectangle {
        id: rectangle
        color: Qt.rgba(1,1,1,0.5)
        anchors.fill: parent
        radius: 5

        Text {
            id: txtErrorTime
            x: 16
            y: 19
            width: 203
            height: 25
            text: new Date().toLocaleString(Qt.locale() , "yyyy-MM-dd hh:mm:ss")
            font.pixelSize: 22
        }

        Text {
            id: txtErrorDevType
            x: 364
            y: 19
            width: 184
            height: 25
            text: "设备类型:" + errorDevType
            font.pixelSize: 22
        }

        Text {
            id: text1
            x: 0
            y: 58
            width: 1272
            height: 74
            text: errorAddr+"   "+errorStr
            verticalAlignment: Text.AlignVCenter
            clip: true
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 30
        }
    }
}
