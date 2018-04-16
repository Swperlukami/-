import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id:linkageControlPage
    width: 1280
    height: 750

    property alias uscPageModel: sysFireinfoModel

    LinekageControlPageModel{
        id:sysFireinfoModel
    }

    ListView{
        id:fireinfolistview
        height: 700
        width: parent.width-40
        anchors.horizontalCenter: parent.horizontalCenter
        model: sysFireinfoModel.usc4000FireModel
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 20
        clip: true
        header:Rectangle{
            width: parent.width
            height: 40
            color: Qt.rgba(1,1,1,0.6)
            radius: 5
            Text{
                anchors.fill: parent
                text:"USC4000联动控制器报警信息"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
            }
        }
        delegate: LinkageItemDelegate{

        }
    }

    RoundButton {
        id: roundButton
        x: 1032
        width: 229
        height: 30
        text: "清空记录"
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: fireinfolistview.bottom
        anchors.topMargin: 5
    }


}
