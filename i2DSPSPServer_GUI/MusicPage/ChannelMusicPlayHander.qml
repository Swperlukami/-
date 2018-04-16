import QtQuick 2.0
import "../SysSingletonModel"

Item {
    id:channelMusicPlayer

    property int channelID: 0
    property ListModel channelPlayListInfo: null
    property bool curChannelPlaying: false
    property int currentSongIndex: 0
    property int currentListSongCnt: 0
    property int currentSongDuration: 0
    property var currentSongStartIntTime: 0
    property var currentChannelDevices: new Object
    property int currentPlayMode: 1
    property int currentPlayVol: 0

    Component.onCompleted: {
//        currentListSongCnt = sysMusicPageModel.channelPlayListModels[channelID-1].count

        console.log(" CHannel Music Player  Component")

//        channelPlayListInfo = sysMusicPageModel.channelPlayListModels[channelID]

    }

    function userAddOrDelMusicPlayDevice(isAdd , DeviceID){
        if(isAdd)
            currentChannelDevices[DeviceID]=isAdd
        else{
            if(currentChannelDevices[DeviceID] === true){
                delete currentChannelDevices[DeviceID]
                var apItem = sysDevicesStatePage.devicesStatePageModelAlias.getAPItem(DeviceID)
                if(apItem["apState"] === UISysModel.enumAPMusic){
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPState(DeviceID , UISysModel.enumAPOnline)
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(DeviceID , UISysModel.sysEmgChannel)
                }
            }
        }


    }

    function userPlayTheSong(songIndex){
        if(sysChannelPlayers.channelPlayers[channelID-1]["curSate"] === UISysModel.sysChannelPlayerUnusable)
            return

        var songItem = channelPlayListInfo.get(songIndex)
        //To Do Set Channel Play The Song
        console.log("User Paly Song Index:",songIndex)
        sysChannelPlayers.setChannelPlayerSate(channelID-1 , songItem["songName"] , currentPlayVol , UISysModel.sysChannelPlayerMusic)
//        sysChannelPlayers.setChannelPlayerSate(3 , "6666.mp3" , 30 , UISysModel.sysChannelPlayerMusic)

        currentSongStartIntTime = UISysModel.sysIntDateTime
        currentSongDuration = songItem["duration"]

        curChannelPlaying = true

        channelPlayListInfo.setProperty(currentSongIndex , "isPlay" , false)
        currentSongIndex = songIndex
        channelPlayListInfo.setProperty(currentSongIndex , "isPlay" , true)

    }

    function userPauseChannelPlayer(){
        curChannelPlaying = false
        channelPlayListInfo.setProperty(currentSongIndex , "isPlay" , false)
        sysChannelPlayers.setChannelPlayerSate(channelID-1 , "", currentPlayVol , UISysModel.sysChannelPlayerIdel)

        sysMusicPage.synMusicPlayerUI(channelID-1)

        for(var DeviceID in currentChannelDevices){
            var apItem = sysDevicesStatePage.devicesStatePageModelAlias.getAPItem(DeviceID)
            if(apItem["apState"] === UISysModel.enumAPMusic){
                sysDevicesStatePage.devicesStatePageModelAlias.setAPState(DeviceID , UISysModel.enumAPOnline)
                sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(DeviceID , UISysModel.sysEmgChannel)
            }
        }


        currentChannelDevices = new Object

        sysMusicPage.muiscPageModeAlias.refreashUsedDevicesCnt(channelID-1)

    }

    function userContrllChannelPlayer(cmd , attachData){
        switch(cmd){
        case "Play":   //songIndex
            if(attachData < 0 || attachData > channelPlayListInfo.count)
                return
            userPlayTheSong(attachData)
            break
        case "Pause":
            userPauseChannelPlayer()
            break
        case "Next":
            userPlayTheSong(playListLoopMode(sysMusicPageModel.channelPlayListModels[channelID-1].count ,  currentSongIndex+1))
            break
        case "Previous":
            userPlayTheSong(playListLoopMode(sysMusicPageModel.channelPlayListModels[channelID-1].count ,  currentSongIndex-1))
            break
        case "Volum":   //Vol
            currentPlayVol = attachData
            break
        case "Mode":   //Mode
            console.log("User Set Play Mode" , attachData)
            currentPlayMode = attachData
            break

        }
    }

    onCurrentPlayVolChanged: {
        //Set Channel Player Vol
        console.log(channelID-1 , "Volum changed:" , currentPlayVol)
        sysChannelPlayers.setChannelVol(channelID-1 ,currentPlayVol)
    }

    Timer{
        id:checkMusicPlayTimer
        interval: 1*1000
        repeat: true
        running: curChannelPlaying
        onTriggered: {
//            console.log("The Song Is Playing" , currentSongDuration , currentSongStartIntTime , UISysModel.sysIntDateTime)
            if(UISysModel.sysIntDateTime === (currentSongStartIntTime+currentSongDuration)){
                //Current Song Play Over
                var newSongIndex = currentSongIndex
                if(currentPlayMode === 2){   //Random
                    newSongIndex = playListRandomMode(sysMusicPageModel.channelPlayListModels[channelID-1].count)
                }
                else{                  //List
                    newSongIndex = playListLoopMode(sysMusicPageModel.channelPlayListModels[channelID-1].count , currentSongIndex+1)
                }
                userPlayTheSong(newSongIndex)
            }
            setAPDevicesMusicAndChannel(currentChannelDevices , UISysModel.enumAPMusic , channelID)
        }
    }

    //Set  AP  Devices  Stae-Music/Idel
    function setAPDevicesMusicAndChannel(devicesIDs , state , channel){
        for(var apid in devicesIDs){
            var apItem = sysDevicesStatePage.devicesStatePageModelAlias.getAPItem(apid)
            if(currentChannelDevices[apid] === true){
                //This Device Choosed
//                console.log("Channel Player Timer Set Dev:" , apid , "Channel :" , channel )
                if(apItem["apState"] === UISysModel.enumAPOnline){
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , state)
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPUseableState(apid , 2)
                    if(apItem["apChannel"] !== channel)
                        sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid , channel)
                }
                else if(apItem["apState"] === UISysModel.enumAPMusic && state === UISysModel.enumAPOnline && channel ===0){
                    //Music Paush
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , state)
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid , channel)
                }
                else if(apItem["apState"] === UISysModel.enumAPTimedTask){
                    if(UISysModel.enumAPMusic > UISysModel.enumAPTimedTask){
                        //Chang Do Music Instead TimedTask
                        sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , state)
                        if(apItem["apChannel"] !== channel)
                            sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid ,channel)
                    }
                    else{
                        //Do Nothing

                    }
                }
                else if(apItem["apState"] === UISysModel.enumAPMusic && channel === channelID){
                    console.log("AP Already Play:" , apid)
                }

                else{
                    console.log("Music Change AP State But Lv Not Enough")
                }
            }

        }
    }


    //Mode : List Loop
    function playListLoopMode(songsCnt , currentIndex){
//        console.log("List Mode TotalCnt:" , songsCnt , "curIndex:", currentIndex)
        if(currentIndex < 0)
            return songsCnt-1
        else if(currentIndex === songsCnt)
            return 0
        else
            return currentIndex
    }
    //Mode : List Random
    function playListRandomMode(songsCnt){
        var randomNum = Math.round(Math.random()*(songsCnt-1))
        console.log("Random  Round Num:", randomNum)
        return randomNum
    }

    //Get Current Devices Cnt
    function getCurrentPlayingDevicesCnt(){
        var cnt = 0;
        for(var apid in currentChannelDevices){
            cnt = cnt +1
        }

        return cnt
    }
}
