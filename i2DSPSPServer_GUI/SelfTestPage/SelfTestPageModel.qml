import QtQuick 2.0
import "../SysSingletonModel"

Item {
    id:selftTestPageModel

    property int testindex: 0

    property ListModel sysTestModel: ListModel{
        ListElement{testname:"电源管理设备检测" ; testState:"待检测" ; testResult:""}
        ListElement{testname:"监听扬声器检测" ; testState:"待检测" ; testResult:""}
        ListElement{testname:"音频矩阵检测" ; testState:"待检测" ; testResult:""}
        ListElement{testname:"功率放大器检测" ; testState:"待检测" ; testResult:""}
        ListElement{testname:"屏幕检测" ; testState:"待检测" ; testResult:""}
    }


    function sysSelfTest(){
        selfTestPage.visible = true
        testtimer.running = true
        btnexit.visible = false
        btnretest.visible = false

        isSysReady = false

        for(var index = 0 ; index < sysTestModel.count ; index++){
            sysTestModel.get(index).testState = "待检测"
            sysTestModel.get(index).testResult = ""
        }
    }

    function testTheItem(testindex){
        sysTestModel.get(testindex).testState = "检测中"
        replaySysSelfTest(testindex)
        switch(testindex)
        {
        case 0:
            CPPowerModelController.sysSetPowerModelSelfTest()
            break
        case 1:
            CPListenSPKController.userSetListenSPKEvent("SelfTest")
            break
        case 2: //No Self Test
            break;
        case 3:
            APController.userSetAPEvents("SelfTest")
            break
        case 4:
            screentest.visible = true
            break
        }
    }
    function outputSelfTestMsg(msg){
        textAreaTestOutput.append(msg)
    }


    function replaySysSelfTest(testIndex){
        switch(testindex)
        {
        case 0:
            var netinfo , homepowerInfo , standbyPowerInfo

            netinfo = "设备开始自检"
            outputSelfTestMsg(netinfo)

            if(sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["isOnline"] === true){
                netinfo = "电源管理设备网络连接正常"
            }
            else
                netinfo = "电源管理设备网络链接断开"

            homepowerInfo = "设备主电源"+
                    sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["mainPowerState"]
            standbyPowerInfo = "设备备用电源"+
                    sysDevicesStatePage.devicesStatePageModelAlias.powerDviceModel["standbyPowerState"]

            outputSelfTestMsg(netinfo+'\n'+homepowerInfo+'\n'+standbyPowerInfo)
            break
        case 1:
            netinfo = "电源管理设备测试完毕"
            outputSelfTestMsg(netinfo)

            if(sysDevicesStatePage.devicesStatePageModelAlias.listeningSPKModel["isOnline"] === true){
                netinfo = "监听扬声器网络连接正常"
            }
            else
                netinfo = "监听扬声器网络链接断开"

            outputSelfTestMsg(netinfo)
            break
        case 2: //No Self Test
            netinfo = "监听扬声器测试完毕"
            outputSelfTestMsg(netinfo)
            break;
        case 3:
            netinfo = "音频矩阵测试完毕"
            outputSelfTestMsg(netinfo)

            var apInfo
            for(var apindex = 0 ;apindex < sysDevicesStatePage.devicesStatePageModelAlias.apInfoModel.apInfoListModel.count ; apindex++ ){
                var apItem = sysDevicesStatePage.devicesStatePageModelAlias.apInfoModel.apInfoListModel.get(apindex)
                if(apItem["apState"] === UISysModel.enumAPOnline){
                    apInfo = "网络功放:"+apItem["apip"]+apItem["apName"]+" 正常工作"
                }
                else if(apItem["apState"] === UISysModel.enumAPOffline){
                    apInfo = "网络功放:"+apItem["apip"]+apItem["apName"]+" 网络断开"
                }
                else if(apItem["apState"] >= UISysModel.enumAPShort){
                    apInfo = "网络功放:"+apItem["apip"]+apItem["apName"]+" 处于故障"
                }
                else{
                    apInfo = "网络功放:"+apItem["apip"]+apItem["apName"]+" 正常工作"
                }

                outputSelfTestMsg(apInfo)
            }

            break
        case 4:
            netinfo = "网络功放测试完毕"
            outputSelfTestMsg(netinfo)
            break
        }
        if(testIndex >0)
            sysTestModel.get(testIndex-1).testState = "检测完毕"

    }

    property Timer testtimer: Timer{
        running: false
        repeat: true
        triggeredOnStart: true
        interval: 10*1000
        onTriggered: {
            if(testindex > 0){
                if(sysTestModel.get(testindex-1).testState === "检测中"){
                    sysTestModel.get(testindex-1).testState = "检测超时"
                }
            }

            if(testindex >= 5){
                testindex = 0
                testtimer.running = false

                btnexit.visible = true
                btnretest.visible = true
                return
            }
            testTheItem(testindex)
            testindex++
        }
    }
}
