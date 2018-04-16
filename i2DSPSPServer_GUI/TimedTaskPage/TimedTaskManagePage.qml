import QtQuick 2.0
import QtQuick.Controls 2.2
import Qt.labs.calendar 1.0

import "../SysSingletonModel"
import "../UICompent"

Item {
    id:timedTaskManagePage
    width: 1280
    height: 710

    property alias timedEditPageAlias: timedTaskEditPage

    ListView {
        id: listView
        x: 0
        y: 0
        width: 1280
        height: 460
        clip: true
        anchors.horizontalCenter: parent.horizontalCenter
        delegate: TimedTaskManageItemDelegate{
        }
        header: TimedTaskManageHeader{

        }

        model: sysTimedTaskPageModel.manageTimedTaskInfoModel
    }

    TimedTaskEditPage {
        id: timedTaskEditPage
        x: 0
        anchors.top: listView.bottom
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
    }





}
