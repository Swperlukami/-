pragma Singleton
import QtQuick 2.0

QtObject {
    id:sysUIModel

    readonly property int sysScreenWidth : 1280
    readonly property int sysScreenHeigth : 1024

    //system state :
    //Idel SelfTest Emg EmgTalk Talk
    property string sysState: "Idel"
    property string sysPreviousState: "Idel"
    readonly property string sysIdelState: "Idel"
    readonly property string sysSelfTestState: "SelfTest"
    readonly property string sysEmgState: "Emg"
    readonly property string sysEmgTalkState: "EmgTalk"
    readonly property string sysTalkState: "Talk"

    property string sysMode: "Auto"    //Manual

    //Sys Channel Delfine
    readonly property int sysTalkChannel: 6
    readonly property int sysEmgTalkChannel: 6
    readonly property int sysEmgChannel: 6
    readonly property int sysTimedTaskChannel: 6




    //Music And TimedTask Lv
//    property int sysMusicLv: 2
//    property int sysTimedTaskLv: 3


    //ChannelPlayer Sate
    property int sysChannelPlayerUnusable: 0     //When Update Songs
    property int sysChannelPlayerIdel: 1
    property int sysChannelPlayerTimedTask: 2
    property int sysChannelPlayerMusic: 3



    //User Lv
    property int usrID : 1
    property string usrname : ""
    property string passwd: ""
    property int usrlv: 3

    readonly property int enumSysAdmin: 3
    readonly property int enumAdmin: 2
    readonly property int enumGeneralUser: 1


    //Mic State
    property bool sysIsMicOnline: false

    //AP State
    readonly property int enumAPOffline: 0
    readonly property int enumAPOnline: 1
    property int enumAPMusic: 3
    property int enumAPTimedTask: 2
    readonly property int enumAPTalk: 4
    readonly property int enumAPEmgTalk: 5
    readonly property int enumAPEmg: 6
    readonly property int enumAPShort: 7
    readonly property int enumAPOpen: 8

    //AP Select Lv
    readonly property int apEmgSelectMode : 4
    readonly property int apTalkSelectMode : 3
    readonly property int apTimedTaskSelectMode : 2
    readonly property int apMusicSelectMode : 1

    //AP Color
    readonly property color colorAPOnline: "green"
    readonly property color colorAPOffline: "grey"
    readonly property color colorAPEmg: "red"
    readonly property color colorAPTalk: "blue"
    readonly property color colorAPTimedTask: "pink"
    readonly property color colorAPMusic: Qt.rgba(0.2,0.4,1,1)

    readonly property color colorDevError: '#808000'

    //Speaker State
    readonly property int enumSPKOffline: 0
    readonly property int enumSPKOnline: 1
//    readonly property int enumSPKShort: 2
    readonly property int enumSPKOpen: 2


    //Sys Devices Names
    readonly property string devTypePowerModel: "电源管理设备"
    readonly property string devTypeListenSPK: "监听扬声器"
    readonly property string devTypeNetMic: "网络麦克"
    readonly property string devTypeMatrix: "音频矩阵"
    readonly property string devTypeNetAP: "网络功放"

    readonly property string errorTypeNetOpen: "网络链接断开"



    property string sysDateTime : "DateTime"
    property var sysIntDateTime: 0

    property Timer sysDateTimer: Timer{
        id:datatimer
        interval: 1000
        repeat: true
        running: true
        onTriggered: {
            sysDateTime = new Date().toLocaleString(Qt.locale() , "yyyy-MM-dd ddd hh:mm:ss")
            sysIntDateTime = Date.parse(new Date())/1000
//            console.log(Date.parse(new Date()))

        }
    }

}
