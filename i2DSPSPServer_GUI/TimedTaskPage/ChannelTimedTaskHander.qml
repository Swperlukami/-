import QtQuick 2.0
import "../SysSingletonModel"


Item {
    id:timedtaskHander

    property int channelID: 0
    property string curFileName: ""
    property int curPlayVol: 0
    property int curFileDuration: 0
    property var curTaskDevices: new Object
    property bool preTaskIsOver: false
    property var curTaskSatrtIntTime : 0


    property var timedTaskMode: new Array


    //Init Current Channel Today TimedTask
    function initCurrentChannelTimedTask(timedTaskID , startDateTime , fileName , vol ,duration , devicesIDs){
        timedTaskMode.push({"timedTaskID":timedTaskID ,
                               "startDateTime":startDateTime ,
                               "fileName":fileName ,
                               "duration":duration ,
                               "devicesID": devicesIDs ,
                               "playVol":vol
                           })
    }

    //User Chaned Current Channel TimedTask Devices
    function userChangedChannelTaskDevices(timedTaskID , devicesIDs){
        console.log("User Save TimedTask Edit Device Changed")
        for(var taskIndex in timedTaskMode){
            if(timedTaskID === timedTaskMode[taskIndex]["timedTaskID"]){
                timedTaskMode[taskIndex]["devicesID"] = devicesIDs
            }
        }
    }

    Timer{
        id:checkHavaTimedTask
        repeat: true
        running: true
        interval: 1*1000
        onTriggered: {
            for(var taskIndex in timedTaskMode){
                //                console.log(timedTaskMode[taskIndex]["startDateTime"])
                //                console.log(Date.parse(new Date()))
                //                console.log(UISysModel.sysIntDateTime)

                if(UISysModel.sysIntDateTime === convertDateTimeToInt(timedTaskMode[taskIndex]["startDateTime"])){
                    //TimedTask Occured
                    console.log("TimedTask ID:",timedTaskMode[taskIndex]["timedTaskID"] , "Occured")
                    curTaskIsDone = false
                    curTaskSatrtIntTime = convertDateTimeToInt(timedTaskMode[taskIndex]["startDateTime"])

                    curFileDuration = timedTaskMode[taskIndex]["duration"]
                    curFileName = timedTaskMode[taskIndex]["fileName"]
                    curPlayVol = timedTaskMode[taskIndex]["playVol"]
                    curTaskDevices = timedTaskMode[taskIndex]["devicesID"]

                    console.log("CurDuration:" , curFileDuration , "StartTime:",curTaskSatrtIntTime)
                }
            }

            if(curFileDuration !== 0){
                var curChannelItem = sysChannelPlayers.getChannePlayerSate(channelID-1)
                if(!curTaskIsDone){   //First done Current TimedTask
                    if(curChannelItem["curSate"] === UISysModel.sysChannelPlayerIdel){
                        //To Do  New TimedTask
                        console.log("Task Done New")
                        setChannelPlayFileWithVol(channelID , curFileName , curPlayVol , UISysModel.sysChannelPlayerTimedTask)
                        curTaskIsDone = true
                    }
                    else if(curChannelItem["curSate"] === UISysModel.sysChannelPlayerTimedTask){
                        //To Do TimedTask Over Previous One
                        console.log("Task Done Over Previous One")

                        setChannelPlayFileWithVol(channelID , curFileName , curPlayVol , UISysModel.sysChannelPlayerTimedTask)
                        curTaskIsDone = true
                    }
                    else if(curChannelItem["curSate"] === UISysModel.sysChannelPlayerMusic){
                        if(UISysModel.enumAPMusic > UISysModel.enumAPTimedTask){
                            //Do Nothing
                        }
                        else{
                            //To Do TimedTask Instead Music
                            console.log("Task Done Instead Music")
                            //To Do Channel Stop Music
                            setChannelPlayFileWithVol(channelID , curFileName , curPlayVol , UISysModel.sysChannelPlayerTimedTask)
                            curTaskIsDone = true
                        }
                    }
                }
                else{  //Current Task Already Done
                    setCurrentTaskDevciesState(curTaskDevices , UISysModel.enumAPTimedTask , channelID)
                    console.log("Task Contuniu Done" , "sysTime:" , UISysModel.sysIntDateTime , "StartTime+Duration:",(curTaskSatrtIntTime+curFileDuration))
                    if(UISysModel.sysIntDateTime >= (curTaskSatrtIntTime+curFileDuration)){
                        //Current Task Over
                        console.log("Task Over")
                        setChannelPlayFileWithVol(channelID , "" , curPlayVol , UISysModel.sysChannelPlayerIdel)     // Channel TimedTask Play Over
                        setCurrentTaskDevciesState(curTaskDevices , UISysModel.enumAPOnline , UISysModel.sysEmgChannel)            //Task Over Set AP Idel
                        curTaskSatrtIntTime = 0;
                        curTaskIsDone = false
                        curFileDuration = 0
                    }
                }
            }
            else{
//                console.log("No Task Done")
//                var test = {"192.168.50.1":"Tets Prop 1" , "192.168.50.2":"Tets Prop 2"}
//////                var test = new Object
//////                test.Prop1 = "Tets Prop 1"
//////                test.Prop2 = "Tets Prop 1"

//                delete test["192.168.50.1"]
//                for(var prop in test)
//                    console.log(prop , test[prop])
//                var testTime = UISysModel.sysIntDateTime
//                console.log(testTime)
            }
        }
    }

    Component.onCompleted: {
        console.log("TimedsTaskDataModeTest")
        initCurrentChannelTimedTask(1,"2018-03-28 15:40:00" , "TimedTaskTetsFileName.mp3" ,50 , 30 , {"AP001":"Addr-1" , "AP002":"Addr-2" })

        console.log(timedTaskMode[0]["devicesID"])
        for(var pop in timedTaskMode[0]["devicesID"]){
            console.log(pop ,  timedTaskMode[0]["devicesID"][pop])
        }

        console.log(timedTaskMode[0]["devicesID"]["AP002"])


    }
    //Set Channel Player  State-TimedTask/Idel
    function setChannelPlayFileWithVol(channelIndex , fileName , vol , state){
        sysChannelPlayers.setChannelPlayerSate(channelID-1 , fileName , vol , state)
    }

    //Set AP Devices State-TimedTask/Idel
    function setCurrentTaskDevciesState(devicesIDs , apState , channel){
        for(var prop in devicesIDs){
            console.log("Timed Task Set AP :" , prop , "TimedTask State Channel:" , channelID)
            setAPTimedTaskSateAndChannel(prop  , apState , channel)
        }
    }

    //Set One AP Device  State-TimedTask/Idel
    function setAPTimedTaskSateAndChannel(apid , state , channel){
        var apItem = sysDevicesStatePage.devicesStatePageModelAlias.getAPItem(apid)
        if(apItem["apState"] === UISysModel.enumAPOnline){
            sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , state)
            sysDevicesStatePage.devicesStatePageModelAlias.setAPUseableState(apid , 2)
            if(apItem["apChannel"] !== channel)
                sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid , channel)
        }
        else if(apItem["apState"] === UISysModel.enumAPTimedTask && state === UISysModel.enumAPOnline && channel === 0){
            //TimedTask Over
            sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , state)
            sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid , channel)
            sysDevicesStatePage.devicesStatePageModelAlias.setAPUseableState(apid , 1)

        }
        else if(apItem["apState"] === UISysModel.enumAPMusic){
            if(UISysModel.enumAPMusic > UISysModel.enumAPTimedTask){
                //Do Nothing
            }
            else{
                //Chang Do TimedTask Instead Music
                sysDevicesStatePage.devicesStatePageModelAlias.setAPState(apid , state)
                if(apItem["apChannel"] !== channel)
                    sysDevicesStatePage.devicesStatePageModelAlias.setAPChannel(apid ,channel)
            }
        }
        else{
            console.log("TimedTask Change AP State But Lv Not Enough")
        }

    }


    function convertDateTimeToInt(strDateTime){
        var initDateTime = Date.parse(new Date(strDateTime))/1000
        return initDateTime
    }

}
