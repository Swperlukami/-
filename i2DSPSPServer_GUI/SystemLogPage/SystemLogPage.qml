import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id:logPage
    width: 1280
    height: 750

    property alias logPageModelAlias: sysLogPageModel
    SystemLogPageModel{
        id:sysLogPageModel
    }

    ListView{
        id:listview
        width: 1280-40
        height: 700
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        model: sysLogPageModel.devErrorLogModel
        spacing: 20
        delegate:LogPageItemDelegate{
            width: listview.width
        }
    }

    RoundButton {
        id: roundButton
        x: 1028
        width: 150
        height: 30
        text: "清空列表"
        anchors.top: listview.bottom
        anchors.topMargin: 5
    }

}
