import QtQuick 2.0
import "../SysSingletonModel"

Item {
    id:broadcastPageModel


    property var currentBroadcastDevices: new Object
    property int curBroadcastCnt: 0
    property int curBroadcastVol: 0
    onCurBroadcastVolChanged: {
        userChangTalkVol(curBroadcastVol)
    }


    function userAddOrDelTalkDevice(isAdd , DeviceID){
        var apItem = sysDevicesStatePage.devicesStatePageModelAlias.getAPItem(DeviceID)

        if(isAdd){
            currentBroadcastDevices[DeviceID] = isAdd
            if(apItem["apState"] >= UISysModel.enumAPOnline &&  apItem["apState"] < UISysModel.enumAPTalk){
                sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(DeviceID , UISysModel.sysTalkChannel)
                sysDevicesStatePage.devicesStatePageModelAlias.setAPState(DeviceID , UISysModel.enumAPTalk)
            }
        }
        else{
            if(currentBroadcastDevices[DeviceID] === true){
                delete currentBroadcastDevices[DeviceID]
                if(apItem["apState"] === UISysModel.enumAPTalk){
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(DeviceID , UISysModel.sysEmgChannel)
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPState(DeviceID , UISysModel.enumAPOnline)
                }

            }
        }

        refreashBroadcastCnt()

    }


    function userStartTalk(){
        UISysModel.sysPreviousState = UISysModel.sysState
        UISysModel.sysState = UISysModel.sysTalkState

        CPNetMicController.userSetNetMicEvent("Broadcast" , curBroadcastVol)
        SysBroadcastController.userSetPCEvents("Talk")
    }

    function userStopTalk(){
        UISysModel.sysPreviousState = UISysModel.sysState
        UISysModel.sysState = UISysModel.sysIdelState

        CPNetMicController.userSetNetMicEvent("StopBroadcast")
        SysBroadcastController.userSetPCEvents("StopTalk")


        for(var apid in currentBroadcastDevices){
            var apItem = sysDevicesStatePage.devicesStatePageModelAlias.getAPItem(apid)
            if(currentBroadcastDevices[apid] === true){
                if(apItem["apState"] === UISysModel.enumAPTalk){
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid , UISysModel.sysEmgChannel)
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , UISysModel.enumAPOnline)
                }

            }
        }

        currentBroadcastDevices = new Object
        refreashBroadcastCnt()
    }

    function userChangTalkVol(Vol){
        CPNetMicController.userSetNetMicEvent("Volume" , curBroadcastVol)
    }
    function userSetIsRecord(isRecord){

    }

    function refreashBroadcastCnt(){
        var cnt = 0
        for(var apid in currentBroadcastDevices){
            cnt = cnt +1
        }

        curBroadcastCnt = cnt
    }

    Timer{
        id:checkTalkDevicesTimer
        interval: 1*1000
        repeat: true
        running: UISysModel.sysState === UISysModel.sysTalkState
        onTriggered: {
            for(var apid in currentBroadcastDevices){
                var apItem = sysDevicesStatePage.devicesStatePageModelAlias.getAPItem(apid)
                    if(apItem["apState"] === UISysModel.enumAPOnline){
                        sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid , UISysModel.sysTalkChannel)
                        sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , UISysModel.enumAPTalk)
                        sysDevicesStatePage.devicesStatePageModelAlias.setAPUseableState(apid , 2)
                    }
            }
        }
    }
}
