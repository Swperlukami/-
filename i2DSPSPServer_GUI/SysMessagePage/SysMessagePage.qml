import QtQuick 2.0

Item {
    id:messagePage
    width: 1280
    height: 50

    property alias messagePageModelAlias: sysMessagepageModel

    SysMessagePageModel{
        id:sysMessagepageModel
    }

    ListView {
        id: listView
        orientation: ListView.Horizontal
        anchors.fill: parent
        model: sysMessagepageModel.messageModel
        delegate: MessageItemDelegate{

        }
    }


}
