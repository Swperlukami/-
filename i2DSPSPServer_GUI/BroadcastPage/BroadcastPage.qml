import QtQuick 2.0
import "../SysSingletonModel"
import "../UICompent"
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
//import Qt.labs.platform 1.0


Item {
    id:broadcasrpage
    width: 1280
    height: 750

    property alias braodcastPageModelAlias: sysBroadcastPageModel

    BroadcastPageModel{
        id:sysBroadcastPageModel
    }


    //Button Talk
    Rectangle {
        id: btntalk
        width: 100
        height: 100
        radius: 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 150
        color: UISysModel.sysState === UISysModel.sysTalkState ? "#fd8282" : "green"
        opacity: sysDevicesStatePage.devicesStatePageModelAlias.netMicModel["isOnline"] === true ? 1: 0.5
        Loader {
            sourceComponent: RGlowEffect{}
            visible: UISysModel.sysState === UISysModel.sysTalkState
        }

        Text {
            id: text2
            color: "#ffffff"
            anchors.centerIn: parent
            text: UISysModel.sysState === UISysModel.sysTalkState ? "停止广播" : qsTr("业务广播")
            font.bold: true
            font.pixelSize: 18
        }
        MouseArea {
            id: mousbtnemg
            anchors.fill: parent
            enabled: btntalk.opacity === 1
            onClicked: {
                if(UISysModel.sysState !== UISysModel.sysIdelState && UISysModel.sysState !== UISysModel.sysTalkState ){
                    messageDialog.visible = true
                }
                else if(UISysModel.sysState === UISysModel.sysTalkState){
                    UISysModel.sysPreviousState = UISysModel.sysState
                    UISysModel.sysState = UISysModel.sysIdelState
                    //Stop Braodacast
                    sysBroadcastPageModel.userStopTalk()
                }
                else {
                    UISysModel.sysPreviousState = UISysModel.sysState
                    UISysModel.sysState = UISysModel.sysTalkState
                    //Start Broadcast
                    sysBroadcastPageModel.userStartTalk()
                }
            }
        }
    }

    Slider {
        id: volslider
        width: 300
        height: 40
        anchors.top: btntalk.bottom
        anchors.topMargin: 60
        anchors.left: parent.left
        anchors.leftMargin: 140
        from: 0
        to:100
        stepSize: 5
        value: 50
        enabled: UISysModel.sysState === UISysModel.sysTalkState
        opacity: volslider.enabled ? 1 : 0.5
        onValueChanged: {
            sysBroadcastPageModel.curBroadcastVol = volslider.value
        }

    }

    Switch {
        id: recordswitch
        x: 924
        y: 321
        enabled: UISysModel.sysState !== UISysModel.sysTalkState
        text: recordswitch.checked ? qsTr("停用录音") : "启用录音"
        anchors.right: parent.right
        anchors.rightMargin: 300
        anchors.verticalCenter: volslider.verticalCenter
        opacity: recordswitch.enabled ? 1 : 0.5
    }


    MessageDialog {
        id: messageDialog
        title: "注意"
        text: "当前状态无法使用业务广播"
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok


        //        onAccepted: {
        //            console.log("And of course you could only agree.")
        //            Qt.quit()
        //        }
        //        Component.onCompleted: visible = true
    }

    RoundButton {
        id: roundButton
        x: 966
        y: 718
        width: 150
        height: 30
        enabled: UISysModel.sysState === UISysModel.sysTalkState
        text: {
            if(sysBroadcastPageModel.curBroadcastCnt === 0)
                return  "播放设备"
            else
                return "播放设备:"+sysBroadcastPageModel.curBroadcastCnt

        }

        anchors.verticalCenter: volslider.verticalCenter
        anchors.horizontalCenter: btntalk.horizontalCenter
        onClicked: {
            sysUIPubicFunction.showSelectDevicesDialog(UISysModel.enumAPTalk , UISysModel.sysTalkChannel-1 , sysBroadcastPageModel.currentBroadcastDevices)

        }
    }


}
