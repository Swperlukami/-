import QtQuick 2.0
import "../SysSingletonModel"

Item{
    id:emgbroadcastpagemodel

    property string curPresetFileName: ""
    property string curPlayingEmgFileName: ""

    function userEmg(){
        if(UISysModel.sysState === UISysModel.sysEmgTalkState)
            userStopEmgTalk()

        UISysModel.sysPreviousState = UISysModel.sysState
        UISysModel.sysState = UISysModel.sysEmgState

        curPlayingEmgFileName = emgFileModel.get(emgfilelistview.currentIndex)["audioName"]

        setAllAPState(UISysModel.enumAPEmg , UISysModel.sysEmgChannel)
        console.log("User Emg")
        SysBroadcastController.userSetPCEvents("Emg" ,curPlayingEmgFileName)

        CPPowerModelController.userSetPowerModelEvent(false , true)
    }
    function userCancelEmg(){
        UISysModel.sysPreviousState = UISysModel.sysState
        UISysModel.sysState = UISysModel.sysIdelState

        setAllAPState(UISysModel.enumAPOnline , UISysModel.sysEmgChannel)
        curPlayingEmgFileName = ""
        console.log("User Cancel  Emg")
        SysBroadcastController.userSetPCEvents("CancelEmg")
        if(isSysReady)
            CPPowerModelController.userSetPowerModelEvent(false , false)

    }
    function userEmgTalk(){
        if(UISysModel.sysState === UISysModel.sysEmgState){
            userCancelEmg()
            UISysModel.sysPreviousState = UISysModel.sysEmgState
        }
        else
            UISysModel.sysPreviousState = UISysModel.sysTalkState


        UISysModel.sysState = UISysModel.sysEmgTalkState

        setAllAPState(UISysModel.enumAPEmgTalk , UISysModel.sysEmgTalkChannel)

        console.log("User Emg Talk")
        CPNetMicController.userSetNetMicEvent("Broadcast" , 100)
        SysBroadcastController.userSetPCEvents("EmgTalk")
        if(isSysReady)
            CPPowerModelController.userSetPowerModelEvent(false , true)
    }
    function userStopEmgTalk(){
        if(UISysModel.sysState === UISysModel.sysEmgTalkState){
            setAllAPState(UISysModel.enumAPOnline , UISysModel.sysEmgChannel)
            UISysModel.sysState = UISysModel.sysIdelState
        }
        console.log("User Stop Emg Talk")
        sysAudioFilesManagementPage.audioFilesManagePageModelAlias.audioEmgReordFilesModel.clear()

        CPNetMicController.userSetNetMicEvent("StopBroadcast" , 100)
        SysBroadcastController.userSetPCEvents("StoplEmgTalk")

        CPPowerModelController.userSetPowerModelEvent(false , false)


        if(UISysModel.sysPreviousState === UISysModel.sysEmgState)
            userEmg()
    }


    function setAllAPState(state , channel){
        for(var apindex = 0 ;  apindex < sysDevicesStatePage.devicesStatePageModelAlias.apInfoModel.apInfoListModel.count ; apindex++){
            var apItem = sysDevicesStatePage.devicesStatePageModelAlias.apInfoModel.apInfoListModel.get(apindex)
            if(apItem["apState"] > UISysModel.enumAPOffline && apItem["apState"] !== state){
                sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apItem["apip"] , state);
                sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apItem["apip"] , channel);

                //Set AP Model
                if(state === UISysModel.enumAPEmg)
                    APController.userSetAPEvents("Emg" , apItem["apip"])
                else if(state === UISysModel.enumAPOnline)
                    APController.userSetAPEvents("CancelEmg" , apItem["apip"])

            }
        }
    }


    function userPresetEmgFile(){
        curPresetFileName = emgFileModel.get(emgfilelistview.currentIndex)["audioName"]
    }

    property ListModel emgFileModel: sysAudioFilesManagementPage.audioFilesManagePageModelAlias.audioEmgFilesModel

    Timer{
        id:checkEmgDevicesTimer
        interval: 2*1000
        repeat: true
        running: UISysModel.sysState === UISysModel.sysEmgState || UISysModel.sysState === UISysModel.sysEmgTalkState
        onTriggered: {
            if(UISysModel.sysState === UISysModel.sysEmgState)
                setAllAPState(UISysModel.enumAPEmg ,UISysModel.sysEmgChannel )
            else if(UISysModel.sysState === UISysModel.sysEmgTalkState)
                setAllAPState(UISysModel.enumAPEmgTalk , UISysModel.sysEmgTalkChannel)
        }
    }


}
