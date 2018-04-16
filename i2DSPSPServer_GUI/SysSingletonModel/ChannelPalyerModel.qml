import QtQuick 2.0
import "../SysSingletonModel"

QtObject {
    id:channnelPlayModel


    function getChannePlayerSate(channelIndex){
        if(channelIndex < 0 || channelIndex > 4)
            return null
        return channelPlayers[channelIndex]
    }

    function setChannelPlayerSate(channelIndex , curFileName , curVol , curSate){
        if(channelIndex < 0 || channelIndex > 4)
            return null
        channelPlayers[channelIndex]["curPlayFile"] = curFileName
        channelPlayers[channelIndex]["curVol"] = curVol
        channelPlayers[channelIndex]["curSate"] = curSate
        var cmdType = ""
        if(curSate === UISysModel.sysChannelPlayerMusic)
            cmdType = "Play"
        else if(curSate === UISysModel.sysChannelPlayerIdel)
            cmdType = "Pause"
        sysMatrixController.userSetMatrixEvents(cmdType , channelIndex , curFileName , curVol)
    }

    function setChannelVol(channelIndex , curVol){
        if(channelIndex < 0 || channelIndex > 4)
            return null
        channelPlayers[channelIndex]["curVol"] = curVol
        sysMatrixController.userSetMatrixEvents("Vol" , channelIndex , "" , curVol)
    }

    property var channelPlayers : new Array({ "curPlayFile": "" , "curVol": 0 , "curSate": 0 } ,
                            { "curPlayFile": "" , "curVol": 0 , "curSate": 0 } ,
                            { "curPlayFile": "" , "curVol": 0 , "curSate": 0 } ,
                            { "curPlayFile": "" , "curVol": 0 , "curSate": 0 } ,
                            { "curPlayFile": "" , "curVol": 0 , "curSate": 0 }
                            )




//    QtObject{
//        id:channelFour
//        property string curPlayFile: ""
//        property int curVol: 0
//        property int curSate: 0
//    }
//    QtObject{
//        id:channelFive
//        property string curPlayFile: ""
//        property int curVol: 0
//        property int curSate: 0
//    }

}
