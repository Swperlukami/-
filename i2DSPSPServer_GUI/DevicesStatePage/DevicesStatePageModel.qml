import QtQuick 2.0
import "../SysSingletonModel"

Item {
    id:devicesStateModel


    Component.onCompleted: {
        console.log("Devices State Page Model Over")
    }

    property QtObject powerDviceModel: QtObject{
        property string devName : "设备电源"
        property string mainPowerState: "正常"
        property string standbyPowerState: "正常"
        property bool isOnline: false
        property string ip: "192.168.2.56"
    }

    property QtObject listeningSPKModel: QtObject{
        property string devName : "监听喇叭"
        property bool isOnline: false
        property string ip: "192.168.2.57"
    }

    property QtObject netMicModel: QtObject{
        property string devName : "网络麦克"
        property bool isOnline: false
        property string ip: "192.168.2.58"
    }

    property QtObject netMartixModel: QtObject{
        property string devName : "音频矩阵"
        property bool isOnline: false
        property string ip: "192.168.2.59"
    }

    property QtObject apInfoModel: QtObject{
        property int apCnt: 3
        property int apErrorCnt: 0
        property int apOfflineCnt: 0

        property int spkCnt: 9
        property int spkEmgCnt: 0
        property int spkOfflineCnt: 0

        property ListModel apInfoListModel: ListModel{
            //           ListElement{
            //               apip:"192.168.2.52"
            //               apChannel:0
            //               isOnline:false
            //               apVol:100
            //               apAddr:"AP Test Addr"
            //               apName:"AP Test Name"
            //               apState:0
            //               apUsedSate:1
            //           }
        }
    }
    //   Component.onCompleted: {
    //       createSPKModelItem(0)
    //       createSPKModelItem(1)
    //       createSPKModelItem(2)

    //       initSPKDevices("LTK000001" , "192.168.2.52" , "SPK0000001" , 1 , 1 , "SPK Test Addr" , 1)
    //   }

    Connections{
        target: APController
        onInitAPInfo:{
            apInfoModel.apInfoListModel.append({   "apip":apip ,
                                                   "apChannel":0 ,
                                                   "isOnline":false ,
                                                   "apVol":100 ,
                                                   "apAddr":addr ,
                                                   "apName":apname ,
                                                   "apState":0 ,
                                                   "apUsedSate": 5
                                               })
            createSPKModelItem(apInfoModel.apInfoListModel.count-1)
            listView.currentIndex = (apInfoModel.apInfoListModel.count-1)

            listView.currentItem.spModel = sysDevicesStatePageModel.spkModels[(apInfoModel.apInfoListModel.count-1)]
        }
        onApEvents:{
            var apstate = 0 , apstatestr = ""
            switch(cmdType){
            case "OK":
                apstate = UISysModel.enumAPOnline
                apstatestr = "正常"
                break;
            case "Short":
                apstate = UISysModel.enumAPShort
                apstatestr = "保护"
                break;
            case "Open":
                apstate = UISysModel.enumAPOpen
                apstatestr = "开路"
                break;
            case "OverDrive":
                apstate = UISysModel.enumAPShort
                apstatestr = "保护"
                break;
            case "Protect":
                apstate = UISysModel.enumAPShort
                apstatestr = "保护"
                break;
            case "Online":
                apstate = UISysModel.enumAPOnline
                apstatestr = "上线"
                break;
            case "Offline":
                apstate = UISysModel.enumAPOffline
                apstatestr = "掉线"
                break;
            }

            if(apstate === UISysModel.enumAPOnline){
                setAPChannel(params , UISysModel.sysEmgChannel)
            }
            else{
                sysUIPubicFunction.sysDeviceError(UISysModel.devTypeNetAP , params+" "+apstatestr  , getAPItem(params)["apAddr"])
            }

            setAPState(params , apstate)
        }
    }

    Connections{
        target: SPController
        onSpInit: {
            initSPKDevices(ltisn, apid, spisn, port , state, addr , apspno)
        }
        onSpEvent: {
            spStateChanged(ltisn, apid, spisn,apspno, state)
        }

    }

    property var spkModels: new Array
    function createSPKModelItem(apindex){
        spkModels[apindex] = Qt.createQmlObject('import QtQml.Models 2.2 ; ListModel{}' , devicesStateModel , "")
    }

    //Init SPK Devices
    function initSPKDevices(ltisn , apid  , spisn , subltno , spstate , spadrr , subapno){
        if(apid === "")
            return
        var apindex = getAPModelIndex(apid)
        if(apindex === -1)
            return

        console.log("Init SPK" , apid , spadrr, subltno , subapno)
        var apspmodel = spkModels[apindex]
        apspmodel.append({"ltisn":ltisn , "apid":apid , "spisn":spisn ,
                             "subltno":subltno , "spstate":spstate , "spadrr":spadrr ,
                             "subapno":subapno})

        console.log("Init SPK OK" , apspmodel , apid , subapno)

    }
    //SPK Sate Changed
    function spStateChanged(ltisn , apid , spisn , apspno , spstate){
        var errorstr = ""
        if(apid === "" && apspno === 0 && spstate === "grey"){
            //LT Offline
            for(var apcnt = 0 ; apcnt < apInfoModel.apInfoListModel.count ; apcnt++){
                for(var spindex = 0 ; spindex < spkModels[apcnt].count ; spindex++){
                    if(spkModels[apcnt].get(spindex).ltisn === ltisn){
                        spkModels[apcnt].get(spindex).spstate = spstate
                    }
                }
            }
            return;
        }

        var apindex = getAPModelIndex(apid)
        if(apindex === -1)
            return
        var apspmodel = spkModels[apindex]
        var offlinesp = 0;

        console.log("SPK $$$$$$$$$" , ltisn , apid , apspno , spstate)

        for(var apspindex = 0 ; apspindex < apspmodel.count ; apspindex++){
            console.log(apspmodel.get(apspindex).apid , apspmodel.get(apspindex).subapno)
            if(apspmodel.get(apspindex).subapno === apspno){
                //First Time SPK Emg
                if((spstate === "yellow" || spstate === "grey")  && apspmodel.get(apspindex).spstate !== "yellow"){
                    errorstr = apInfoModel.apInfoListModel.get(apindex).addr +" 功放 "+apspmodel.get(apspindex).apid +" "+ apspno+" 号扬声器故障"
//                    errorUI.addmsgInfo(errorstr)
                }
                //Set SPK State
                console.log("SPK &&&&&&&& , ltisn , apid , apspno , spstate")

                apspmodel.get(apspindex).spstate = spstate
            }
            if(apspmodel.get(apspindex).spstate === "grey")
                offlinesp++
        }
        //Check AP Is Open
        if(offlinesp >= apspmodel.count){
            console.log("Check AP Is Open :" , apid , apindex)
            if(apInfoModel.apInfoListModel.get(apindex).apstate === UISysModel.enumAPOnline){
                apInfoModel.apInfoListModel.get(apindex).apstate = UISysModel.enumAPOpen
                errorstr = apInfoModel.apInfoListModel.get(apindex)["apAddr"] +" 功放 "+apDevicesMode.get(apindex)["apip"] +" 开路"
//                errorUI.addmsgInfo(errorstr)
            }
        }

        //SPK Online Set This AP Open To Online
        if(spstate === "green" && apInfoModel.apInfoListModel.get(apindex).apState === UISysModel.colorAPOnline){
            apInfoModel.apInfoListModel.get(apindex)["apState"] = UISysModel.enumAPOnline
        }

    }

    Timer{
        id:checkDevicesErrorTimer
        repeat: true
        interval: 5*1000
        running: true
        onTriggered: checkDevicesErrorInfo()
    }

    function checkDevicesErrorInfo(){
        var apcnt = 0 , apofflinecnt = 0 , apshortcnt = 0 , apopencnt = 0
        var spkcnt = 0 , spkofflinecnt = 0 , spkemgcnt = 0

        for(var apindex = 0 ; apindex < apInfoModel.apInfoListModel.count ; apindex++){
            //AP State Info
            apcnt += 1
            var apItem = apInfoModel.apInfoListModel.get(apindex)
            if(apItem["apState"] === UISysModel.enumAPOffline){
                apofflinecnt += 1
            }
            else if(apItem["apState"] === UISysModel.enumAPShort){
                apshortcnt +=1
            }
            else if(apItem["apState"] === UISysModel.enumAPOpen){
                apopencnt+=1
            }

            //SPK State Info
            for(var spkindex = 0 ; spkindex < spkModels[apindex].count ; spkindex++){
                spkcnt +=1
                var spkItem = spkModels[apindex].get(spkindex)
                if(spkItem["spstate"] === "grey"){
                    spkofflinecnt +=1
                }
                else if(spkItem["spstate"] === "yellow"){
                    spkemgcnt +=1
                }
            }

        }

        var aperrorcnt = apofflinecnt + apshortcnt + apopencnt
        var spkerrorcnt = spkofflinecnt + spkemgcnt
        if(aperrorcnt + spkerrorcnt > (apInfoModel.apErrorCnt + apInfoModel.apOfflineCnt) + (apInfoModel.spkOfflineCnt + apInfoModel.spkEmgCnt)){
            //New AP Error Or New SPK Error
            if(isSysReady)
                CPPowerModelController.sysSetPowerModelEVent(true , true)
            console.log("New AP Or SPK Error")
        }
        //       else
        //           CPPowerModelController.sysSetPowerModelEVent(true , false)


        if(powerDviceModel["mainPowerState"]==="正常" && powerDviceModel["standbyPowerState"]==="正常" && powerDviceModel["isOnline"]=== true &&
                listeningSPKModel["isOline"] === true &&
                netMartixModel["isOnline"] === true &&
                (aperrorcnt+spkerrorcnt) === 0){
            //All Device OK
            if(isSysReady)
                CPPowerModelController.sysSetPowerModelEVent(false , false)
        }
        //       else
        //           CPPowerModelController.sysSetPowerModelEVent(true , false)

        apInfoModel.apCnt = apcnt
        apInfoModel.apErrorCnt = aperrorcnt
        apInfoModel.apOfflineCnt = apofflinecnt
        apInfoModel.spkCnt = spkcnt
        apInfoModel.spkEmgCnt = spkemgcnt
        apInfoModel.spkOfflineCnt = spkofflinecnt

    }

    function getAPModelIndex(apid){
        for(var apindex = 0 ; apindex < apInfoModel.apInfoListModel.count ;apindex++){
            if(apInfoModel.apInfoListModel.get(apindex).apip === apid)
                return apindex
        }
        return -1
    }


    function getAPItem(apid){
        if(apid === "")
            return null
        var apindex = getAPModelIndex(apid)
        if(apindex === -1)
            return null

        return apInfoModel.apInfoListModel.get(apindex)
    }

    function setAPState(apid , state){
        console.log("User Set AP:" , apid , "State:" , state)
        apInfoModel.apInfoListModel.setProperty(getAPModelIndex(apid) , "apState" , state)
    }
    function setAPUseableState(apid , useableState){
        console.log("User Set AP:" , apid , "UseableState:" , state)
        apInfoModel.apInfoListModel.setProperty(getAPModelIndex(apid) , "apUsedSate" , useableState)
    }
    function setAPChannel(apid , channel){
        console.log("User Set AP:" , apid , "Channel:" , channel)
        apInfoModel.apInfoListModel.setProperty(getAPModelIndex(apid) , "apChannel" , channel)
        APController.userSetAPEvents("SynInput" , apid+"#"+channel)
    }
}
