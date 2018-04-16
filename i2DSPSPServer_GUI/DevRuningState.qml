import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 400
    height: 100


    property int devofflinecnt: 0
    property int devemgcnt: 0


    Timer{
        id:checkisdeverror
        repeat: true
        interval: 1000*5
        running: true
        triggeredOnStart: false
        onTriggered: {
            var isdeverror = false , isvoice = false ;
            var offlinecnt = 0 , emgcnt = 0  , apercnt = 0 ;
            if(aperror.flashColor === "yellow" || sPK.flashColor === "yellow"){
                isdeverror = true
            }
//            console.log(aperror.flashColor)
//            console.log(sPK.flashColor)

//            console.log("isdeverror ?:" , isdeverror)
            if(aperror.cause !== ""){
                var apcntlist = aperror.cause.split("/")
                emgcnt = parseInt(apcntlist[0])
                offlinecnt = parseInt(apcntlist[1])
            }

            if(sPK.cause !== ""){
                var cntlist = sPK.cause.split("/")
//                console.log(cntlist[0] , cntlist[1] , cntlist[2])
                emgcnt += parseInt(cntlist[0])
                offlinecnt += parseInt(cntlist[1])

            }
//            console.log("off",offlinecnt,"preoff",devofflinecnt,"emg",emgcnt,"preemg",devemgcnt)

            if(offlinecnt > devofflinecnt  || emgcnt > devemgcnt)
                isvoice = true


           devemgcnt = emgcnt
           devofflinecnt = offlinecnt
            if(!isemg)
                APController.usrSetLightOfAPSPState(isdeverror , isvoice)
        }
    }

    function setTypeState(type,color,enable,casue)
    {
        type.flashColor = color
        type.runEnable = enable
        type.cause= casue
    }
    function receiveDevInfo(type,state,casue)
    {

        var errorstr = ""
//        console.log("PowerType:", type , state , "cause:" , casue)
        switch(type){
        case 0 :
            if(state){
                setTypeState(homePower,"yellow",true,casue)
                if(standbyPower.flashColor === "green"){
                    errorstr= "系统主电源"+casue
                    errorUI.addmsgInfo(errorstr)
                    APController.usrSetLightOfAPSPState(true , true)
                    if(casue === "故障"){
                        ishomepowerused = false
                        errorstr= "系统主电源"+casue+",切换备用电源使用"
                        errorUI.addmsgInfo(errorstr)
                        APController.usrSetLightOfAPSPState(true , true)

                    }
                    else if(casue === "欠压")
                        ishomepowerused = true
                }
            }
            else{
                setTypeState(homePower,"green",false,casue)
                ishomepowerused = true
            }

            break
        case 1:
            if(state)
                setTypeState(usc4000,"red",true,casue)
            else
                setTypeState(usc4000,"green",false,casue)
            break
        case 2:
            if(state)
                setTypeState(aperror,"yellow",true,casue)
            else
                setTypeState(aperror,"green",false,casue)
            break
        case 3:
            if(state)
                setTypeState(sPK,"yellow",true,casue)
            else
                setTypeState(sPK,"green",false,casue)
            break
        case 4:
            if(state)
                setTypeState(other,"yellow",true,casue)
            else
                setTypeState(other,"green",false,casue)
            break
        case 5:
            if(state){
                setTypeState(standbyPower,"yellow",true,casue)
                ishomepowerused = true
                errorstr = "系统备用电源"+casue
                errorUI.addmsgInfo(errorstr)
                APController.usrSetLightOfAPSPState(false , true)

            }
            else{
                setTypeState(standbyPower,"green",false,casue)
                if(homePower.flashColor === "green"){
                    ishomepowerused = true
                }else
                    ishomepowerused = false
            }
            break
        default: break
        }
    }



    Connections {
        target: button
        onClicked: {
//            sendCloseMusicCmd()
//            receiveDevInfo(0,1,"HHHH");
        }
    }
    Rectangle{
        width: parent.width
        height: parent.height+10
        anchors.top: parent.top
        anchors.topMargin: -10
        radius: 5
//        border.color: "grey"
        color: "#2a2323"
        opacity: 0.5
    }

    property bool ishomepowerused: true

    Row {
        id: row
        width: parent.width*0.9
        height: parent.height*0.9
        anchors.horizontalCenter: parent.horizontalCenter
        RecAnimation {
            id: homePower
            width:row.width/5
            height: row.height

            devType:qsTr("主电源")
            runEnable:false
            flashColor: "green"
            Text {
                id: homepowerused
                text: ishomepowerused ? "使用中" : ""
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 35
            }
        }
        RecAnimation {
            id: standbyPower
            width:row.width/5
            height: row.height

            devType:qsTr("备用电源")
            runEnable:false
            flashColor: "green"

            Text {
                id: stanbypowerused
                text: !ishomepowerused ? "使用中" : ""
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 35
            }

        }
        RecAnimation {
            id: usc4000
            width:row.width/5
            height: row.height

            devType:qsTr("USC4000")
            runEnable:false
            flashColor: "green"

        }
        RecAnimation {
            id: aperror
            width:row.width/5
            height: row.height
            runEnable:false
            flashColor: "green"
            devType: qsTr("消防功放")
        }

        RecAnimation {
            id: sPK
            width:row.width/5
            height: row.height
            runEnable:false
            flashColor: "green"
            devType: qsTr("消防喇叭")
        }
        RecAnimation {
            id: other
            width: 0

//            width:row.width/6
            height: row.height

            devType:qsTr("其他故障")
            runEnable:false
            flashColor: "green"
            visible: false
        }

        Button{
            id:button
            width: 0
//            width: row.width/6
            height: row.height
            text:qsTr("关闭声音")
            visible: false
        }
    }



}
