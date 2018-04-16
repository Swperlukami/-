import QtQuick 2.0

Item {
    width: 1280
    height: 50


    Rectangle{
        color: "#fdf8b9"
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 3
        anchors.topMargin: 3
        anchors.fill: parent
        opacity: 0.3
        radius: 10
    }

    Row {
        id: row
        anchors.fill: parent

        Text {
            id: textTime
            width: 250
            text: new Date().toLocaleString(Qt.locale() , "yyyy-MM-dd hh:mm:ss")
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
        }

        Text {
            id: textMsg
            width: 1030
            text: msg
            elide: Text.ElideLeft
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 25
        }

    }
}
