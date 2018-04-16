import QtQuick 2.0
import "../SysSingletonModel"
import "../EmgBroadcastPage"
import "../BroadcastPage"
import "../DevicesStatePage"
import "../TimedTaskPage"
import "../MusicPage"
import "../SystemSettingPage"
import "../UserManaegment"
import "../AudioFilesManagement"
import "../SystemLogPage"
import "../LinkageControlPage"
import "../LoginPage"
import "../DeviceSelectPage"
import "../SelfTestPage"
import "../UIController"
import "../SysMessagePage"
import "../UICompent"


Item {
    id:launchapage
    width: UISysModel.sysScreenWidth
    height: UISysModel.sysScreenHeigth

    property bool  isSysReady: false

    UIPubilcFunc{
        id:sysUIPubicFunction
    }
    MatrixController{
        id:sysMatrixController
    }

    ChannelPalyerModel{
        id:sysChannelPlayers
    }

    TitleBar{
        id:systitlebar
    }

    SysVersionInfoPage{
        id:sysVersionPage
        z:7
        x:(parent.width-sysVersionPage.width)/2
        y: 200

    }

    LockPage{
        id:sysLockPage
        z:8
    }

    SelfTestPage{
        id:sysSelfTestPage
        z:10
    }

    LoginPage{
        id:sysLoginPage
        anchors.fill: parent
        visible: true
        z:9
    }

    Rectangle{
        id:midBKRect
        y : systitlebar.height
        width: UISysModel.sysScreenWidth
        clip : true
        height: UISysModel.sysScreenHeigth - systitlebar.height - functionBar.height -sysMessagePage.height
        color: Qt.rgba(0,0,0,0.2)
        z:1

        DevicesSelectPage{
            id:sysDeviceSelectPage
        }

        EmgBroadcastPage{
            id:sysEmgPageView
            anchors.fill: parent
//            visible: false

        }

        BroadcastPage{
            id:sysBroadcastPage
            anchors.fill: parent
            visible: false
        }

        DevicesStatePage{
            id:sysDevicesStatePage
            anchors.fill: parent
            visible: false
        }

        TimedTaskPage{
            id:sysTimedTaskPage
            anchors.fill: parent
            visible: false
        }


        MusicPage{
            id:sysMusicPage
            anchors.fill: parent
            visible: false
        }

        SystemSettingPage{
            id:sysSettingPage
            anchors.fill: parent
            visible: false
        }

        UserManagementPage{
            id:sysUserManagementPage
            anchors.fill: parent
            visible: false
        }


        AudioFilesMangementPage{
            id:sysAudioFilesManagementPage
            anchors.fill: parent
            visible: false
        }
        SystemLogPage{
            id:sysLogPage
            anchors.fill: parent
            visible: false
        }
        LinkageContrlPage{
            id:sysUSC4000FirePage
            anchors.fill: parent
            visible: false
        }

    }

    SysMessagePage{
        id:sysMessagePage
        anchors.top: midBKRect.bottom
    }


    FunctionBar {
        id: functionBar
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        onFuncChanged: {
            console.log("Usr Changed Func:" , functionname )
            hideAllViews()
            switch(functionname){
            case "应急广播":
                sysEmgPageView.visible = true
                break
            case "业务广播":
                sysBroadcastPage.visible = true
                break
            case "设备状态":
                sysDevicesStatePage.visible = true
                break
            case "定时任务":
               sysTimedTaskPage.visible = true
                break
            case "背景音乐":
               sysMusicPage.visible = true
                break
            case "系统设置":
               sysSettingPage.visible = true
                break
            case "用户管理":
               sysUserManagementPage.visible = true
                break
            case "音频管理":
               sysAudioFilesManagementPage.visible = true
                break
            case "故障记录":
               sysLogPage.visible = true
                break
            case "联动控制":
               sysUSC4000FirePage.visible = true
                break
            }

        }
    }

    function hideAllViews(){
        sysEmgPageView.visible = false
        sysBroadcastPage.visible = false
        sysDevicesStatePage.visible = false
        sysTimedTaskPage.visible = false
        sysMusicPage.visible = false
        sysSettingPage.visible = false
        sysUserManagementPage.visible = false
        sysAudioFilesManagementPage.visible = false
        sysLogPage.visible = false
        sysUSC4000FirePage.visible = false

    }


}
