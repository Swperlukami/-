import QtQuick 2.0
import QtQuick.Controls 2.2

import "../SysSingletonModel"


Item {
    id:userManagementPage
    width: 1280
    height: 750


    UserManagementPageModel{
        id:sysUserManagementPageModel
    }

    ListView {
        id: listView
        x: 114
        clip: true
        width: 1200
        height: 600
        spacing: 20
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        model:sysUserManagementPageModel.userInfoModel
        delegate: UserItemDelegate{

        }
    }

    RoundButton {
        id: roundButtonAddUser
        x: 72
        width: 500
        height: 25
        text: "添加新用户"
        anchors.horizontalCenterOffset: -300
        anchors.top: listView.bottom
        anchors.topMargin: 29
        anchors.horizontalCenter: parent.horizontalCenter
        visible: UISysModel.usrlv >=2 ? true : false
    }

    RoundButton {
        id: roundButtonChgUser
        x: 72
        y: -6
        width: 500
        height: 25
        text: "更换用户"
        anchors.top: listView.bottom
        visible: UISysModel.usrlv >=2 ? true : false
        anchors.horizontalCenterOffset: 300
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 29
    }


}
