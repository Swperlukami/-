import QtQuick 2.0
import "../SysSingletonModel"

Item {
    id:sysUIPublicFunction



    function showSelectDevicesDialog(apSelectType , channelIndex , devices){
        sysDeviceSelectPage.sysSelectDevicesModelControl.apSelectType = apSelectType
        sysDeviceSelectPage.sysSelectDevicesModelControl.setSelectPageUsedDevices(channelIndex ,devices)
        sysDeviceSelectPage.visible = true
    }

    function showSelectAudioFilesDialog(selctMode , existAudios , channelIndex){
        if(channelIndex !== -1)
            sysAudioFilesManagementPage.audioFilesManagePageModelAlias.setSelectChannelIndex(channelIndex)

        sysAudioFilesManagementPage.audioFilesManagePageModelAlias.setAudioFileSelectModeAndExistFiles(selctMode , existAudios)
        sysAudioFilesManagementPage.audioFilesSelectPageAlias.visible = true
    }

    Component.onCompleted: {
//        sysMessagePage.messagePageModelAlias.appendMessage("devType"+" " + "addr"+ " " + "errorinfo")
    }

    function sysDeviceError(devType , errorinfo , addr){
        sysLogPage.logPageModelAlias.appendNewLogInfo(devType , errorinfo , addr)
        if(isSysReady)
            sysMessagePage.messagePageModelAlias.appendMessage(devType + addr + errorinfo)
    }

    function compareDateTime(DateTimeOne , DateTimeTwo){
         var datetime1 = new Date("2018-03-27 05:20:36")
        var datetime2 = new Date("2018-03-27 05:20:35")

        console.log(Date.parse(datetime1))
        console.log(Date.parse(datetime2))

    }

    Connections{
        target: CPPowerModelController
        onPowerModelEvents:{
            switch(eventType){
            case "Online":
                sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["isOnline"] = true
                sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["ip"] = params
                break
            case "Offline":
                sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["isOnline"] = false
                sysUIPubicFunction.sysDeviceError(UISysModel.devTypePowerModel , UISysModel.errorTypeNetOpen , "")
                //Set Light Voice
                if(isSysReady)
                    CPPowerModelController.sysSetPowerModelEVent(true , true)
                break
            case "HomePower":
                sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["mainPowerState"] = params
                if(params !== "正常"){
                    //Set Light Voice
                    if(isSysReady)
                        CPPowerModelController.sysSetPowerModelEVent(true , true)
                    sysUIPubicFunction.sysDeviceError(UISysModel.devTypePowerModel , "主电源"+params , "")
                }
                break
            case "StandbyPower":
                sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["standbyPowerState"] = params
                if(params !== "正常"){
                    //Set Light Voice
                    if(isSysReady)
                        CPPowerModelController.sysSetPowerModelEVent(true , true)

                    sysUIPubicFunction.sysDeviceError(UISysModel.devTypePowerModel , "备用电源"+params , "")
                }
                break
            case "Emg":
                sysEmgPageView.emgBroadcastPageModelAlias.userEmg()
                break
            case "CancelEmg":
                sysEmgPageView.emgBroadcastPageModelAlias.userCancelEmg()
                break
            case "SelfTest":
                sysSelfTestPage.selfTestPageModelAlias.sysSelfTest()
                break
            }
        }
    }

    Connections{
        target: CPListenSPKController
        onListenSPKEvents:{
            switch(cmdType){
            case "Online":
                sysDevicesStatePage.devicesStatePageModelAlias.listeningSPKModel["isOnline"] = true
                sysDevicesStatePage.devicesStatePageModelAlias.listeningSPKModel["ip"] = params
                break
            case "Offline":
                sysDevicesStatePage.devicesStatePageModelAlias.listeningSPKModel["isOnline"] = false
                //Set Light Voice
                if(isSysReady)
                    CPPowerModelController.sysSetPowerModelEVent(true , true)

                sysUIPubicFunction.sysDeviceError(UISysModel.devTypeListenSPK , UISysModel.errorTypeNetOpen , "")
                break
            }
        }
    }

    Connections{
        target: CPNetMicController
        onNetMicEvents:{
            switch(cmdType){
            case "Online":
                sysDevicesStatePage.devicesStatePageModelAlias.netMicModel["isOnline"] = true
                sysDevicesStatePage.devicesStatePageModelAlias.netMicModel["ip"] = params
                break
            case "Offline":
                sysDevicesStatePage.devicesStatePageModelAlias.netMicModel["isOnline"] = false
                sysUIPubicFunction.sysDeviceError(UISysModel.devTypeNetMic , UISysModel.errorTypeNetOpen , "")

                if(UISysModel.sysState === UISysModel.sysEmgTalkState){
                    sysEmgPageView.emgBroadcastPageModelAlias.userStopEmgTalk()
                }
                else if(UISysModel.sysState === UISysModel.sysTalkState){
                    sysBroadcastPage.braodcastPageModelAlias.userStopTalk()
                }

                break
            case "Broadcast":
                sysBroadcastPage.braodcastPageModelAlias.userStartTalk()
                break
            case "StopBroadcast":
                sysBroadcastPage.braodcastPageModelAlias.userStopTalk()
                break
            }
        }
    }

    Connections{
        target: CPMatrixController
        onMatrixEvents:{
            switch(cmdType){
            case "Online":
                sysDevicesStatePage.devicesStatePageModelAlias.netMartixModel["isOnline"] = true
                sysDevicesStatePage.devicesStatePageModelAlias.netMartixModel["ip"] = params

                for(var channelPlayerIndex = 0  ; channelPlayerIndex < 5 ; channelPlayerIndex++){
                    sysChannelPlayers.channelPlayers[channelPlayerIndex]["curSate"] = UISysModel.sysChannelPlayerIdel
                }

                break
            case "Offline":
                sysDevicesStatePage.devicesStatePageModelAlias.netMartixModel["isOnline"] = false
                //Set Light Voice
                if(isSysReady)
                    CPPowerModelController.sysSetPowerModelEVent(true , true)

                sysUIPubicFunction.sysDeviceError(UISysModel.devTypeMatrix , UISysModel.errorTypeNetOpen , "")


                for(channelPlayerIndex = 0  ; channelPlayerIndex < 5 ; channelPlayerIndex++){
                    sysMusicPage.channelMusicPlayers[channelPlayerIndex].userPauseChannelPlayer()
                    sysChannelPlayers.channelPlayers[channelPlayerIndex]["curSate"] = UISysModel.sysChannelPlayerUnusable
                }
                break
            }
        }
    }

    Connections{
        target: USCController
        onRs485Event:{
            if(UISysModel.sysMode !== "Auto")   //Manual Do No Thing
                return

            if(event === "Reset"){
                console.log("USC4000 Reset")
                sysUSC4000FirePage.uscPageModel.hostReset()
            }
            else if(event === "SelfTest"){
                console.log("USC4000 SelfTest")

            }
            else{
                var fileInfo = event.split("*")
                 sysUSC4000FirePage.uscPageModel.appendFireInfo(fileInfo[0] , fileInfo[1] , fileInfo[2] , fileInfo[3])
            }
        }

    }


}
