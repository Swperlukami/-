import QtQuick 2.0
//import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4
import "../SysSingletonModel"
import QtQuick.Controls 2.2


Item {
    id:settingPage
    width: 1280
    height: 800

    SystemSettingPageModel{
        id:sysSystemSettingModel
    }

    TabBar {
        id: tabBar
        y: 0
        width: 1280
        height: 40
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
        }

        TabButton {
            id: tabButton
            width: 640
            height: 40
            text: qsTr("基础设置")
            font.pixelSize: 22
            background: Rectangle{
                anchors.fill: parent
                color: tabButton.checked ? "transparent" : Qt.rgba(0,0,0,0.4)
            }
            contentItem: Text {
                anchors.fill: parent
                text: tabButton.text
                font: tabButton.font
                color: tabButton.checked ? "white" : "black"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

        TabButton {
            id: tabButton1
            width: 640
            height: 40
            text: qsTr("高级设置")
            font.pixelSize: 22
            enabled:  UISysModel.usrlv >= UISysModel.enumAdmin ? true : false
            background: Rectangle{
                anchors.fill: parent
                color: tabButton1.checked ? "transparent" : Qt.rgba(0,0,0,0.4)
            }
            contentItem: Text {
                anchors.fill: parent
                text: tabButton1.text
                font: tabButton1.font
                color: tabButton1.checked ? "white" : "black"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    GeneralSettingPage{
        id:sysGeneralSetingPage
        anchors.top: tabBar.bottom
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        visible: tabButton.checked
    }
    ExpertSettingPage{
        id:sysExpertlSetingPage
        anchors.top: tabBar.bottom
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        visible: tabButton1.checked
    }



//    TabView {
//        id: tabView
//        anchors.fill: parent
//        Tab{
//            id:todayTimedTask
//            title: qsTr("基础设置")

//        }
//        Tab{
//            id:mangeTimedTask
//            title: qsTr("高级设置")
////            visible: UISysModel.usrlv >= UISysModel.enumAdmin ? true : false
//            active:  false

//        }

//        style: TabViewStyle {
//                 frameOverlap: 1
//                 tab: Rectangle {
//                     color: styleData.selected ? "transparent" : Qt.rgba(0,0,0,0.4)
////                     border.color:  "steelblue"
////                     implicitWidth: Math.max(text.width + 4, 80)
//                     implicitWidth: tabView.width/2

//                     implicitHeight: 40
//                     radius: 2
//                     Text {
//                         id: txtTabTitle
//                         anchors.centerIn: parent
//                         text: styleData.title
//                         font.pixelSize: 22
//                         color: styleData.selected ? "white" : "black"
//                     }
//                 }
//                 frame: Rectangle { color: "transparent" }
//             }
//    }
}
