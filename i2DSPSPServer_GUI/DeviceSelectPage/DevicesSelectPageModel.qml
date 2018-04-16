import QtQuick 2.0

import "../SysSingletonModel"

QtObject {
    id:devicesSelectPageModel


    Component.onCompleted: {
        console.log("Select Page Model Over");
    }

    property int apSelectType : 1
    property int curChannelIndex: 0

    function btnIsSelectAllAP(isSelectAllAP){
        for(var index = 0 ; index < apSelectModel.count ; index++ ){
            var selectAPItem = apSelectModel.get(index)
            if(isSelectAllAP){
                if(selectAPItem.apUsedSate === 1)
                    selectAPItem.apUsedSate = 2
                if(selectAPItem.apUsedSate === 4)
                    selectAPItem.apUsedSate = 3
            }
            else{
                if(selectAPItem.apUsedSate === 2)
                    selectAPItem.apUsedSate = 1
                if(selectAPItem.apUsedSate === 3)
                    selectAPItem.apUsedSate = 4
            }

            setApDoEvent(isSelectAllAP , selectAPItem["apip"] , selectAPItem["apName"])

        }
    }


    function setApDoEvent(isAdd , apip , apName){
        if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPEmg){
            //            sysMusicPage.channelMusicPlayers[sysSelcectDevicesModel.curChannelIndex].userAddOrDelMusicPlayDevice(isAdd , apip)
        }
        else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPTalk){
            sysBroadcastPage.braodcastPageModelAlias.userAddOrDelTalkDevice(isAdd , apip)
        }
        else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPTimedTask){
            sysTimedTaskPage.timedTaskManagePageAlias.timedEditPageAlias.userisAddorDelTimedTaskDevice(isAdd , apip , apName)
            //            sysTimedTaskPage.timedTaskManagePageAlias.timedEditPageAlias.userisAddorDelTimedTaskDevice(isAdd , apip , apName)
        }
        else if(sysSelcectDevicesModel.apSelectType === UISysModel.enumAPMusic){
            sysMusicPage.channelMusicPlayers[sysSelcectDevicesModel.curChannelIndex].userAddOrDelMusicPlayDevice(isAdd , apip)
            sysMusicPage.muiscPageModeAlias.refreashUsedDevicesCnt(sysSelcectDevicesModel.curChannelIndex)
        }
    }

    function setSelectPageUsedDevices(channelIndex ,UsedDevcies){
        curChannelIndex = channelIndex
        console.log("Select Page For Channel:" , channelIndex ,"Devcices:")
        for(var prop in UsedDevcies){console.log(prop)}

        var channelID = channelIndex+1
        initSelectApState()
        for(var apid in UsedDevcies){
            var apSelectIndex = getSelectPageApIndex(apid)
            var apStateItem = getSelectPageApItem(apid)
            if(apSelectType !== UISysModel.enumAPTimedTask){
                //User Select Talk Music Devices
                if(channelID === apStateItem["apChannel"]){
                    if(apSelectType === apStateItem["apState"]){
                        apSelectModel.setProperty(apSelectIndex , "apUsedSate" , 2)
                    }
                    else if(apStateItem["apState"] === UISysModel.enumAPOffline ||
                            apStateItem["apState"] === UISysModel.enumAPOpen ||
                            apStateItem["apState"] === UISysModel.enumAPShort)
                        apSelectModel.setProperty(apSelectIndex , "apUsedSate" , 3)

                }
                else if(channelID !== apStateItem["apChannel"]){
                    if(apSelectType > apStateItem["apState"] &&
                            apStateItem["apState"] < UISysModel.enumAPShort){
                        apSelectModel.setProperty(apSelectIndex , "apUsedSate" , 1)

                    }
                    else if(apStateItem["apState"] === UISysModel.enumAPOffline ||
                            apStateItem["apState"] === UISysModel.enumAPOpen ||
                            apStateItem["apState"] === UISysModel.enumAPShort){
                        apSelectModel.setProperty(apSelectIndex , "apUsedSate" , 4)

                    }
                    else if(apSelectType <= apStateItem["apState"]){
                        apSelectModel.setProperty(apSelectIndex , "apUsedSate" , 5)

                    }

                }
            }
            else{
                //User Select TimedTask Devices
                apSelectModel.setProperty(apSelectIndex , "apUsedSate" , 2)

            }
        }
    }

    property ListModel apSelectModel: sysDevicesStatePage.devicesStatePageModelAlias.apInfoModel.apInfoListModel
    // apUsedSate :1 CanBeUesed 2 Used 3 PrepareUsed 4 NoPrepareUsed 5 Unusable

    function getSelectPageApItem(apid){
        for(var apindex = 0 ; apindex < apSelectModel.count ; apindex++){
            var apItem = apSelectModel.get(apindex)
            if(apItem["apip"] === apid)
                return apItem
        }
    }

    function getSelectPageApIndex(apid){
        for(var apindex = 0 ; apindex < apSelectModel.count ; apindex++){
            var apItem = apSelectModel.get(apindex)
            if(apItem["apip"] === apid)
                return apindex
        }
    }

    function initSelectApState(){
        for(var apindex = 0 ; apindex < apSelectModel.count ; apindex++){
            var apStateItem = apSelectModel.get(apindex)

            console.log("Clear Select Page SelectType:" , apSelectType , "Cur Dev State:" ,apStateItem["apState"])
            if(apSelectType !== UISysModel.enumAPTimedTask){
                //Init Talk Music Devices State
                if(apStateItem["apState"] === UISysModel.enumAPOffline ||
                        apStateItem["apState"] === UISysModel.enumAPOpen ||
                        apStateItem["apState"] === UISysModel.enumAPShort ||
                        apSelectType < apStateItem["apState"])
                    apSelectModel.setProperty(apindex , "apUsedSate" , 5)
                else if(apSelectType === apStateItem["apState"]){
                    if(apSelectType === UISysModel.enumAPMusic)
                        apSelectModel.setProperty(apindex , "apUsedSate" , 5)
                    else if(apSelectType === UISysModel.enumAPTalk)
                        apSelectModel.setProperty(apindex , "apUsedSate" , 1)
                }
                else
                    apSelectModel.setProperty(apindex , "apUsedSate" , 1)

            }

            else{
                //Init Select TimeTask Devices
                apSelectModel.setProperty(apindex , "apUsedSate" , 1)
            }
        }
    }
    //    function setAllAPUsedSate(apUsedSate){
    //        for(var index = 0 ; index < apSelectModel.count ; index++ ){
    //            apSelectModel.get(index).apUsedSate = apUsedSate
    //        }
    //    }

}
