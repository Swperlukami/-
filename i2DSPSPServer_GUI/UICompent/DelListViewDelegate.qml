import QtQuick 2.0

Item {
    id:dellistviewdelegate
    width: 100
    height: parent.height
//    property Component listviewDelegate: null
    signal deleteButonClicked

    Rectangle{
        id: rectangle
        anchors.fill: parent
//        anchors.right: parent.right
        color: "lightgrey"

        Text {
            id: text1
            width: parent.width

            text: qsTr("删除")
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        MouseArea{
            anchors.fill: parent
            onClicked: deleteButonClicked()
        }
    }

//    Loader{sourceComponent: listviewDelegate }
}
