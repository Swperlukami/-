import QtQuick 2.0
import "../SysSingletonModel"
import "../images"

Item {
    id:fcnBar
    width: UISysModel.sysScreenWidth
    height: 184
    Rectangle{
        anchors.fill: parent
        color: Qt.rgba(0,0,0,0.7)
    }
    clip: true

    signal funcChanged(string functionname)

    Component.onCompleted:{
        //        console.log("FuncBar Tets:",listView.currentItem.data[1])
        //        funcChanged("联动控制")
        //        listView.currentItem.data[1].clicked(Qt.LeftButton)
        sysUIPubicFunction.compareDateTime("","")

    }

    ListView {
        id: listView
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 22
        delegate: Item {
            id: item1
            anchors.verticalCenter: parent.verticalCenter

            width: parent.height-42
            height: item1.width
            Column {
                id: row1
                spacing: 3

                Image {
                    id:iocnimage
                    width: item1.width
                    height: item1.width
                    source: iconsource
                    //                    radius: 30
                    //                    color: colorCode
                }

                Text {
                    text: name
                    horizontalAlignment: Text.AlignHCenter
                    width: iocnimage.width
                    color: "#ffffff"
                    font.bold: true
                }
            }

            MouseArea{
                id:funcMouse
                anchors.fill: parent
                onClicked:{
                    //System Emg Lock Function Bar
                    if(UISysModel.sysState === UISysModel.sysEmgState || UISysModel.sysState === UISysModel.sysEmgTalkState){
                        return
                    }

                    if(index !== listView.currentIndex)
                        funcChanged(name)
                    listView.currentIndex = index


                }
            }
        }
        model: ListModel {
            ListElement {
                name: "应急广播"
                iconsource: "../images/紧急广播.png"
            }

            ListElement {
                name: "业务广播"
                iconsource: "../images/业务广播.png"
            }
            ListElement {
                name: "设备状态"
                iconsource: "../images/设备信息.png"
            }
            ListElement {
                name: "定时任务"
                iconsource: "../images/定时任务.png"
            }
            ListElement {
                name: "背景音乐"
                iconsource: "../images/背景音乐.png"
            }
            ListElement {
                name: "系统设置"
                iconsource: "../images/系统设置.png"
            }

            ListElement {
                name: "用户管理"
                iconsource: "../images/用户管理.png"
            }

            ListElement {
                name: "音频管理"
                iconsource: "qrc:/images/音频管理.png"
            }

            ListElement {
                name: "故障记录"
                iconsource: "qrc:/images/系统日志.png"
            }

            ListElement {
                name: "联动控制"
                iconsource: "qrc:/images/us4000.png"
            }
        }
    }


}
