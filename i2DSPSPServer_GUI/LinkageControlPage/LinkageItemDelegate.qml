import QtQuick 2.0

Item {
    width: 1280-40
    height: 90
    Rectangle{
        radius: 5
        anchors.fill: parent
        color: Qt.rgba(1,1,1,0.5)
        Row{
            width: parent.width
            anchors.verticalCenterOffset: 15
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            spacing: 0
            Text {
                id: hostNo
                text: qsTr("USC400主机号：")+1
                width: parent.width/4
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 22
            }
            Text {
                id: loopNo
                text: qsTr("回路号：")+2
                width: parent.width/4
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 22
            }
            Text {
                id: addrNo
                text: qsTr("地址号：")+3
                width: parent.width/4
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 22
            }
            Text {
                id: devType
                text: qsTr("设备类型：")+4
                width: parent.width/4
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 22
            }
        }

        Text {
            id: text1
            x: 8
            y: 14
            width: 201
            height: 15
            text: new Date().toLocaleString(Qt.locale() , "yyyy-MM-dd hh:mm:ss")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 22
        }
    }
}
