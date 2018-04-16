import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id:selfTestPage
    width: 1280
    height: 1024
    visible: false

    property alias selfTestPageModelAlias: sysSelftTestModel

    SelfTestPageModel{
        id:sysSelftTestModel
    }

    Rectangle{
        id: rectangle1
        anchors.fill: parent
        color: "grey"

        RoundButton {
            id: btnexit
            x: 1076
            y: 643
            width: 170
            height: 40
            text: qsTr("退出")
            visible: true
            font.pointSize: 20
            onClicked: {
                selfTestPage.visible = false
                isSysReady = true
            }
        }

        RoundButton {
            id: btnretest
            x: 890
            y: 643
            width: 170
            height: 40
            text: qsTr("重新检测")
            visible: true
            font.pointSize: 20
            onClicked: {
                sysSelftTestModel.sysSelfTest()
            }
        }


        ScrollView{
            x: 0
            y: 739
            width: 1280
            height: 250
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            TextArea {
                id: textAreaTestOutput
                anchors.fill: parent
                readOnly: true
                selectByMouse: true
                selectedTextColor: "green"
//                text: qsTr("Text Area \n\n\n\nn\n\n\n\n\n\nn\n\n\nn\n\n\n\n\n\n\n\n\n\n")
                background: Rectangle{
                    anchors.fill: parent
                    color: "White"
                }
            }
        }
    }


    Column{
        anchors.verticalCenterOffset: -185
        anchors.horizontalCenterOffset: -15
        anchors.centerIn: parent
        Repeater{
            model: sysSelftTestModel.sysTestModel
            Item{
                id: item2
                width: 1000
                height: 40
                Rectangle {
                    id: rectangle
                    width: parent.width*0.6
                    height: parent.height
                    radius: 10
                    color: testState === "检测完毕" ? "green" : Qt.rgba(1,1,1,0.8)
                    Text {
                        id: text1
                        text: testname
                        font.pixelSize: 22
                        anchors.centerIn: parent
                    }
                }

                Text {
                    id: tetstate
                    width: 120
                    text: testState
                    font.pointSize: 20
                    anchors.left: rectangle.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: rectangle.verticalCenter
                }

                Text {
                    id: tetresult
                    width: 370
                    height: 100
                    text: testResult
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAnywhere
                    font.pointSize: 15
                    anchors.left: tetstate.right
                    anchors.leftMargin: 0
                    anchors.verticalCenter: rectangle.verticalCenter
                }
            }
        }
        spacing: 10

    }

    ScreeTestPage{
        id:screentest
        anchors.fill: parent
        visible: false
        onTestover: {
            sysSelftTestModel.sysTestModel.get(4).testState = "检测完毕"
            sysSelftTestModel.outputSelfTestMsg("屏幕检测完毕")
        }
    }


}
