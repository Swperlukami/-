import QtQuick 2.0
import QtQuick.Controls 2.2

Dialog{
    id:selectDevicesDialog
    modal: true
    width: 800
    height: 500+5
    closePolicy: Popup.NoAutoClose

    property alias sysSelectDevicesModelControl: sysSelcectDevicesModel

    DevicesSelectPageModel{
        id:sysSelcectDevicesModel
    }

    x:(rootwindow.width-selectDevicesDialog.width)/2
    y:(rootwindow.height-selectDevicesDialog.height)/2-100

    background: Rectangle{
        id: rectangle
        anchors.fill: parent
        color: Qt.rgba(1,1,1,1.6)
        radius: 5
        clip: true
    }

    RoundButton {
        id: roundButtonSeleectAll
        width: 100
        height: 30
        text: roundButtonSeleectAll.checked ? "取消全部" : "全选"
        checkable: true
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: -5
        onClicked: {
            sysSelcectDevicesModel.btnIsSelectAllAP(roundButtonSeleectAll.checked)
        }
    }

        GridView {
            id: gridView
            width: parent.width
            height: 400
            clip: true
            anchors.top: parent.top
            anchors.topMargin: 28
            anchors.horizontalCenter: parent.horizontalCenter
            cellHeight: 65
            cellWidth: parent.width/3+7

            flow: GridView.FlowTopToBottom
            model: sysSelcectDevicesModel.apSelectModel

            delegate: SelectedAPItemDelegate{

            }
        }



    header: Rectangle {
        id: rectangleheaderDel
        width: 600
        height: 30
        radius: 5
        color: "grey"
        Rectangle{
            width: parent.width
            height: parent.height/2
            anchors.bottom: parent.bottom
            color: parent.color
        }
        Text{
            color: "#ffffff"
            text: "请选择需要使用的功放设备"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 15
            anchors.fill: parent
        }
    }

    footer: Rectangle {
                id: rectangleFooterDel
                width: 600
                height: 40
                radius: 5
                color: "grey"
                Rectangle{
                    width: parent.width
                    height: parent.height/2
                    anchors.top: parent.top
                    color: parent.color
                }

                Text{
                    color: "#ffffff"
                    text: "退出"
                    font.pointSize: 22
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent

                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        selectDevicesDialog.visible = false
                    }
                }
    }


    Component.onCompleted: {
//        selectDevicesDialog.visible = true
//        sysSelcectDevicesModel.apSelectModel = sysDevicesStatePage.devicesStatePageModelAlias.apInfoModel.apInfoListModel

//        console.log("Select Page Component" ,sysSelcectDevicesModel.apSelectModel.get(0)["apUsedSate"] )

    }

//    Rectangle {
//        id: rectangle1
//        color: "transparent"
//        anchors.fill: parent
//        radius: 10
//        clip: true
//    }


    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 ; duration: 250 }
    }
    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 ; duration: 250 }
    }


}

