import QtQuick 2.0
import "../SysSingletonModel"


Item {
    id:selectAPItemDel
    width: 250
    height: 60


    Component.onCompleted: {
        console.log("Select Item Over")
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {

            if(apUsedSate === 5)
                return
            var isAdd = true
            if(apUsedSate === 1){
                isAdd = true
                apUsedSate = 2
            }
            else if(apUsedSate === 2){
                isAdd = false
                apUsedSate = 1
            }
            else if(apUsedSate === 3){
                isAdd = true
                apUsedSate = 4
            }
            else if(apUsedSate === 4){
                isAdd = false
                apUsedSate = 3
            }
            sysSelcectDevicesModel.setApDoEvent(isAdd , apip , apName)
            console.log("Set apip:",apip ,  "apUsedSate :" , apUsedSate)

        }
    }

    //    function setApDoEvent(isAdd){
    //        if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPEmg)
    //            sysMusicPage.channelMusicPlayers[sysSelcectDevicesModel.curChannelIndex].userAddOrDelMusicPlayDevice(isAdd , apip)
    //        else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPTalk){
    //            sysBroadcastPage.braodcastPageModelAlias.userAddOrDelTalkDevice(isAdd , apip)
    //        }
    //        else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPTimedTask)
    //            sysMusicPage.channelMusicPlayers[sysSelcectDevicesModel.curChannelIndex].userAddOrDelMusicPlayDevice(isAdd , apip)
    //        else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPMusic){
    //            sysMusicPage.channelMusicPlayers[sysSelcectDevicesModel.curChannelIndex].userAddOrDelMusicPlayDevice(isAdd , apip)
    //            sysMusicPage.muiscPageModeAlias.refreashUsedDevicesCnt(sysSelcectDevicesModel.curChannelIndex)
    //        }

    //    }

    Rectangle {
        id: rectangleBK
        color: {
            switch(apUsedSate){
            case 1: return "green"
            case 2:
            case 3:
                if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPEmg)
                    return UISysModel.colorAPEmg
                else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPTalk)
                    return UISysModel.colorAPTalk
                else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPTimedTask)
                    return UISysModel.colorAPTimedTask
                else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPMusic)
                    return UISysModel.colorAPMusic
                else
                    return "lightgrey"
            case 4:
            case 5:
                return UISysModel.colorAPOffline

            }
        }

        anchors.fill: parent
        radius: 5
        opacity: {
            if(apUsedSate === 3 || (apUsedSate === 2 && (apState === UISysModel.enumAPOffline ||
                                                         apState >= UISysModel.enumAPShort)))
                return 0.6
            else
                return 1
        }


        Text {
            id: txtAPUsedState
            y: 0
            width: parent.width*0.3
            height: parent.height*0.6
            color: "#ffffff"
            text: {
                switch(apUsedSate){
                case 1:
                case 4:
                    return "可用"
                case 2:
                case 3:
                    return "已用"
                case 5:
                    return "不可用"
                }
            }

            font.pointSize: 15
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: txtAPName.right
            anchors.leftMargin: 1
        }


        Text {
            id: txtAPState
            width: txtAPUsedState.width
            height: parent.height*0.4
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.top: txtAPUsedState.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: txtAPUsedState.horizontalCenter
            font.pixelSize: 12
            text: {
                switch(apState){
                case UISysModel.enumAPOnline:
                    return "空闲"+apChannel
                case UISysModel.enumAPOffline:
                    return "掉线"+apChannel
                case UISysModel.enumAPMusic:
                    return "音乐"+apChannel
                case UISysModel.enumAPTimedTask:
                    return "定时任务"+apChannel
                case UISysModel.enumAPTalk:
                    return "业务广播"+apChannel
                case UISysModel.enumAPEmgTalk:
                    return "应急话筒"+apChannel
                case UISysModel.enumAPEmg:
                    return "应急广播"+apChannel
                case UISysModel.enumAPShort:
                    return "短路"+apChannel
                case UISysModel.enumAPOpen:
                    return "断路"+apChannel
                }
            }

        }

        Text {
            id: txtAPName
            y: 0
            width: parent.width*0.7
            height: parent.height
            color: "#ffffff"
            text: apAddr
            anchors.left: parent.left
            anchors.leftMargin: 0
            wrapMode: Text.WordWrap
            font.pointSize: 15
            elide: Text.ElideRight
            fontSizeMode: Text.HorizontalFit
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            id: rectangle
            x: 132
            y: 0
            height: parent.height-20
            color: "#ffffff"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: txtAPUsedState.left
            anchors.rightMargin: 0
            anchors.left: txtAPName.right
            anchors.leftMargin: 0
        }

    }
}
