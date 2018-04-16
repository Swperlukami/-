import QtQuick 2.0
import "../SysSingletonModel"
import "../UICompent"
import QtQuick.Controls 2.2



Item {
    id:emgbroadcasrpage
    width: 1280
    height: 750

    property alias emgBroadcastPageModelAlias: emgPageModel

    EmgbroadcastPageModel{
        id:emgPageModel
    }

    //Button Emg File
    Rectangle {
        id: btnemg
        width: 100
        height: 100
        radius: 50
        anchors.top: parent.top
        anchors.topMargin: 150
        anchors.left: parent.left
        anchors.leftMargin: 150
        color: UISysModel.sysState === UISysModel.sysEmgState ? "#fd8282" : "green"
        Loader {
            sourceComponent: RGlowEffect{}
            visible: UISysModel.sysState === UISysModel.sysEmgState
        }

        Text {
            id: text2
            color: "#ffffff"
            anchors.centerIn: parent
            text: UISysModel.sysState === UISysModel.sysEmgState ? "停止广播" : qsTr("应急广播")
            font.bold: true
            font.pixelSize: 18
        }
        MouseArea {
            id: mousbtnemg
            anchors.fill: parent
            onClicked: {
                if(UISysModel.sysState !== UISysModel.sysEmgState){
                    //Emg File
                    sysEmgPageView.emgBroadcastPageModelAlias.userEmg()
                }
                else if(UISysModel.sysState === UISysModel.sysEmgState){
                    //Stop Emg File
                    sysEmgPageView.emgBroadcastPageModelAlias.userCancelEmg()

                }
            }
        }
    }


    ListView{
        id:emgfilelistview
        clip: true
        width: 400
        height: 250
        spacing: 0
        anchors.right: parent.right
        anchors.rightMargin: 150
        anchors.top: parent.top
        anchors.topMargin: 100
        model: emgPageModel.emgFileModel
        delegate: emglistviewDelegate
        highlight: Rectangle{
            width: parent.width
            height: 40
            color: Qt.rgba(1,0,0,0.8)
            radius: 5
        }

        Rectangle{
            anchors.fill: parent
            radius: 5
            color: Qt.rgba(1,1,1,0)
            border.color: "black"
        }

    }

    RoundButton {
        id: refreashEmgFile
        x: 1030
        width: /*110*/0
        height: 30
        text: qsTr("刷新应急文件")
        anchors.right: emgfilelistview.right
        anchors.rightMargin: 0
        anchors.top: emgfilelistview.bottom
        anchors.topMargin: 10
        onClicked: {
            emgPageModel.emgFileModel = sysAudioFilesManagementPage.audioFilesManagePageModelAlias.audioEmgFilesModel
        }
        visible: false
    }

    //Preset Emg File Btn
    RoundButton {
        id: presetEmgFile
        x: 1039
        y: -2
        width: 100
        height: 30
        text: qsTr("设为预设")
        anchors.right: refreashEmgFile.left
        anchors.rightMargin: 10
        anchors.topMargin: 10
        anchors.top: emgfilelistview.bottom
        onClicked: {
            sysEmgPageView.emgBroadcastPageModelAlias.userPresetEmgFile()
        }
    }

    //Button Emg Talk
    Rectangle {
        id: btnemgtalk
        width: 100
        height: 100
        radius: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 150
        anchors.left: parent.left
        anchors.leftMargin: 150
        color: UISysModel.sysState === UISysModel.sysEmgTalkState ? "#fd8282" : "green"
        opacity: sysDevicesStatePage.devicesStatePageModelAlias.netMicModel["isOnline"] === true ? 1: 0.5

        Loader {
            sourceComponent: RGlowEffect{}
            visible: UISysModel.sysState === UISysModel.sysEmgTalkState
        }

        Text {
            id: txtemgtalk
            color: "#ffffff"
            anchors.centerIn: parent
            text: UISysModel.sysState === UISysModel.sysEmgTalkState ? "停止话筒" : qsTr("应急话筒")
            font.bold: true
            font.pixelSize: 18
        }
        MouseArea {
            id: mousbtnemgtalk
            anchors.fill: parent
            enabled: btnemgtalk.opacity === 1
            onClicked: {
                if(UISysModel.sysState !== UISysModel.sysEmgTalkState){
                    //Start Emg Talk
                    sysEmgPageView.emgBroadcastPageModelAlias.userEmgTalk()
                }
                else if(UISysModel.sysState === UISysModel.sysEmgTalkState){
                    //Stop Emg Talk
                    sysEmgPageView.emgBroadcastPageModelAlias.userStopEmgTalk()

                }
            }
        }
    }



    RoundButton {
        id: roundButton
        x: 966
        y: 718
        width: 150
        height: 30
        text: "播放设备:全部"
        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 150
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 42
        onClicked: {
            sysUIPubicFunction.showSelectDevicesDialog()
        }
    }



    Component{
        id:emglistviewDelegate
        Row{
            id:delegateRow
            width: emgfilelistview.width
            height: 40
            Rectangle{
                width: parent.width
                height: parent.height
                color: Qt.rgba(0,0,0,0.5)
                radius: 5
                Text {
                    id: filename
                    width: parent.width
                    height: parent.height
                    text: audioName
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: audioName === sysEmgPageView.emgBroadcastPageModelAlias.curPlayingEmgFileName ? "green" :" "
                }
                MouseArea{
                    id:emgItemMouse
                    anchors.fill: parent
                    drag.target: delegateRow
                    drag.axis: Drag.XAxis
                    drag.minimumX: /*-100*/0
                    drag.maximumX: 0
                    onMouseXChanged:  {
                        if(delegateRow.x < -99){
                            console.log("dele")
                        }
                    }
                    onReleased: {
                        if(delegateRow.x > -100){
                            console.log("Back")
                            delegateRow.x = 0
                        }
                    }
                    onClicked: {
                        emgfilelistview.currentIndex = index
                    }

                }

                Rectangle{
                    width: 10
                    height: 10
                    radius: 5
                    color: "green"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    visible: audioName === sysEmgPageView.emgBroadcastPageModelAlias.curPresetFileName
                }
            }

            Behavior on x {
                NumberAnimation{duration: 500 ; easing.type: Easing.OutQuad ; alwaysRunToEnd: false}
            }



            DelListViewDelegate{
                width: 100
                height: parent.height
                onDeleteButonClicked: {
                    console.log("List View Delete Item Index:" , index)
                    sysEmgPageView.emgBroadcastPageModelAlias.emgFileModel.remove(index)
                }
            }
        }
    }



}
