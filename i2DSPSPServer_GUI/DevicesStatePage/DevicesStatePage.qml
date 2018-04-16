import QtQuick 2.0

import "../SysSingletonModel"

Item {
    id:devicesStatePage
    width: 1280
    height: 750

    property alias devicesStatePageModelAlias: sysDevicesStatePageModel

    DevicesStatePageModel{
        id:sysDevicesStatePageModel
    }

    Column {
        id: column
        x: 0
        width: 1280
        height: 220
        anchors.top: parent.top
        anchors.topMargin: 20
        spacing: 20

        Rectangle {
            id: powerMode
            width: 1200
            height: 40
            color: sysDevicesStatePageModel.powerDviceModel["isOnline"] ? UISysModel.colorAPOnline : UISysModel.colorAPOffline
            radius: 3
            border.width: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: txtPowerDevName
                color: "#ffffff"
                text: qsTr("电源设备")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 50
                font.pixelSize: 30
            }

            Text {
                id: txtPowerDevIP
                color: "#ffffff"
                text: sysDevicesStatePageModel.powerDviceModel["ip"]
                font.pixelSize: 22
                anchors.left: txtPowerDevName.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtmainPower
//                color: "#ffffff"
                color: sysDevicesStatePageModel.powerDviceModel["mainPowerState"]==="正常" ? "white" : UISysModel.colorDevError
                text: qsTr("主电源:")
                anchors.left: txtPowerDevName.right
                anchors.leftMargin: 250
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 22
            }

            Text {
                id: txtmainPowerState
//                color: "#ffffff"
                text: sysDevicesStatePageModel.powerDviceModel["mainPowerState"]
                color: sysDevicesStatePageModel.powerDviceModel["mainPowerState"]==="正常" ? "white" : UISysModel.colorDevError
                font.pixelSize: 22
                anchors.left: txtmainPower.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtaminPowerUsed
                color: "#ffffff"
                text: sysDevicesStatePageModel.powerDviceModel["mainPowerState"]==="正常" ? qsTr("使用中") : ""
                font.pixelSize: 22
                anchors.left: txtmainPowerState.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtstanbyPower
//                color: "#ffffff"
                color: sysDevicesStatePageModel.powerDviceModel["standbyPowerState"] === "正常" ? "white" : UISysModel.colorDevError
                text: qsTr("备用电源:")
                font.pixelSize: 22
                anchors.left: txtaminPowerUsed.right
                anchors.leftMargin: 100
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtstandbyPowerState
//                color: "#ffffff"
                color: sysDevicesStatePageModel.powerDviceModel["standbyPowerState"] === "正常" ? "white" : UISysModel.colorDevError
                text: sysDevicesStatePageModel.powerDviceModel["standbyPowerState"]
                font.pixelSize: 22
                anchors.left: txtstanbyPower.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtstandbuPowerUsed
                width: 63
                color: "#ffffff"
                text: sysDevicesStatePageModel.powerDviceModel["mainPowerState"]!=="正常" ? qsTr("使用中") : ""
                font.pixelSize: 22
                anchors.left: txtstandbyPowerState.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtPowerNet
                width: 49
                color: "#ffffff"
                text: qsTr("网络:")
                font.pixelSize: 22
                anchors.left: txtstandbuPowerUsed.right
                anchors.leftMargin: 100
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtPowerNetState
                x: -8
                y: 0
                color: "#ffffff"
                text: sysDevicesStatePageModel.powerDviceModel["isOnline"] ?  qsTr("正常") : "断开"
                font.pixelSize: 22
                anchors.left: txtPowerNet.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

        }

        Rectangle {
            id: listeningSPKMode
            width: 1200
            height: 40
            color: sysDevicesStatePageModel.listeningSPKModel["isOnline"] ? UISysModel.colorAPOnline : UISysModel.colorAPOffline
            radius: 3
            border.width: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: txtListingSPKName
                color: "#ffffff"
                text: qsTr("监听喇叭")
                font.pixelSize: 30
                anchors.left: parent.left
                anchors.leftMargin: 50
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtListenSPKDevIP
                color: "#ffffff"
                text: sysDevicesStatePageModel.listeningSPKModel["ip"]
                font.pixelSize: 22
                anchors.left: txtListingSPKName.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtListenSPKNet
                color: "#ffffff"
                text: qsTr("网络:")
                font.pixelSize: 22
                anchors.left: parent.left
                anchors.leftMargin: 1045
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtListenSPKNetStat
                color: "#ffffff"
                text: sysDevicesStatePageModel.listeningSPKModel["isOnline"] ? qsTr("正常") : "断开"
                font.pixelSize: 22
                anchors.left: txtListenSPKNet.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }


        }

        Rectangle {
            id: netMicMode
            width: 1200
            height: 40
            color: sysDevicesStatePageModel.netMicModel["isOnline"] ? UISysModel.colorAPOnline : UISysModel.colorAPOffline
            radius: 3
            Text {
                id: txtMetMicName
                color: "#ffffff"
                text: qsTr("网络麦克")
                font.pixelSize: 30
                anchors.left: parent.left
                anchors.leftMargin: 50
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtNetMicDevIP
                color: "#ffffff"
                text: sysDevicesStatePageModel.netMicModel["ip"]
                font.pixelSize: 22
                anchors.left: txtMetMicName.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtNetMicNet
                color: "#ffffff"
                text: qsTr("网络:")
                font.pixelSize: 22
                anchors.left: parent.left
                anchors.leftMargin: 1045
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtNetMicState
                color: "#ffffff"
                text: sysDevicesStatePageModel.netMicModel["isOnline"] ? qsTr("正常") : "断开"
                font.pixelSize: 22
                anchors.left: txtNetMicNet.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: 0
        }

        Rectangle {
            id: martixMode
            width: 1200
            height: 40
            color: sysDevicesStatePageModel.netMartixModel["isOnline"] ? UISysModel.colorAPOnline : UISysModel.colorAPOffline
            radius: 3
            Text {
                id: txtMartixName
                color: "#ffffff"
                text: qsTr("音频矩阵")
                font.pixelSize: 30
                anchors.left: parent.left
                anchors.leftMargin: 50
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtMartixDevIP
                color: "#ffffff"
                text: sysDevicesStatePageModel.netMartixModel["ip"]
                font.pixelSize: 22
                anchors.left: txtMartixName.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtMartixNet
                color: "#ffffff"
                text: qsTr("网络:")
                font.pixelSize: 22
                anchors.left: parent.left
                anchors.leftMargin: 1045
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtMartixNetStat
                color: "#ffffff"
                text: sysDevicesStatePageModel.netMartixModel["isOnline"] ? qsTr("正常") : "断开"
                font.pixelSize: 22
                anchors.left: txtMartixNet.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: 0
        }
    }




    // AP & SP View
    ListView {
        id: listView
        x: 40
        width: 1200
        height: 530
        clip: true
        anchors.top: column.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        model: sysDevicesStatePageModel.apInfoModel.apInfoListModel
        delegate: APSPKDelegateViewItem{
            spModel: {
                if(sysDevicesStatePageModel.spkModels[index] === undefined)
                    return undefined
                else
                    return sysDevicesStatePageModel.spkModels[index]
            }
        }
    }



    Component.onCompleted: {
//        sysDevicesStatePageModel.createSPKModelItem(0)
//        sysDevicesStatePageModel.createSPKModelItem(1)
//        sysDevicesStatePageModel.createSPKModelItem(2)
//        sysDevicesStatePageModel.createSPKModelItem(3)
//        sysDevicesStatePageModel.createSPKModelItem(4)
//        sysDevicesStatePageModel.createSPKModelItem(5)


//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.52" , "SPK0000001" , 1 , 0 , "SPK Test Addr 1" , 1)
//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.52" , "SPK0000002" , 2 , 1 , "SPK Test Addr2" , 2)

//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.53" , "SPK0000003" , 1 , 0 , "SPK Test Addr 3" , 1)
//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.53" , "SPK0000004" , 2 , 1 , "SPK Test Addr 4" , 2)

//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.54" , "SPK0000005" , 1 , 0 , "SPK Test Addr 5" , 1)
//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.54" , "SPK0000006" , 2 , 1 , "SPK Test Addr 6" , 2)

//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.55" , "SPK0000007" , 1 , 0 , "SPK Test Addr 7" , 1)
//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.55" , "SPK0000008" , 2 , 1 , "SPK Test Addr 8" , 2)

//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.56" , "SPK0000007" , 1 , 0 , "SPK Test Addr 9" , 1)
//        sysDevicesStatePageModel.initSPKDevices("LTK000001" , "192.168.2.56" , "SPK0000008" , 2 , 1 , "SPK Test Addr 10" , 2)

//        for(var apindex = 0 ; apindex < sysDevicesStatePageModel.apInfoModel.apInfoListModel.count ; apindex++){
//            var apItem =  sysDevicesStatePageModel.apInfoModel.apInfoListModel.get(apindex)
//            var index = sysDevicesStatePageModel.getAPModelIndex(apItem["apip"])
//            listView.currentIndex = index

//            listView.currentItem.spModel = sysDevicesStatePageModel.spkModels[index]
//        }
////        listView.currentItem.spModel = sysDevicesStatePageModel.spkModels[0]
//        console.log("Dev Page ", listView.currentItem.spModel)
    }
}
