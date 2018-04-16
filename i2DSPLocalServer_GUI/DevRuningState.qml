import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id: item1
    width: parent.width
    height: parent.height
    property alias failInfo: textFailCause.text
    function setTypeState(type,color,enable,casue)
    {
        type.flashColor = color
        type.runEnable = enable
        failInfo= casue
    }
    function receiveDevInfo(type,state,casue)
    {
        switch(type){
        case 0 :
            if(state)
                setTypeState(sPK,"yellow",true,casue)
            else
                setTypeState(sPK,"lightgreen",false,casue)
            break
        case 1:
            if(state)
                setTypeState(homePower,"yellow",true,casue)
            else
                setTypeState(homePower,"lightgreen",false,casue)
            break
        case 2:if(state)
                setTypeState(reservedPower,"yellow",true,casue)
            else
                setTypeState(reservedPower,"lightgreen",false,casue)
            break
        default: break
        }
    }
    function sendCloseMusicCmd()
    {

    }
    Row {
        id: row
        width:parent.width
        height: parent.height
        anchors.fill:parent
        spacing: 5
        RecAnimation {
            id: sPK
            width:row.width/4
            runEnable:false
            flashColor: "lightgreen"
            devType: qsTr("SPK")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
        }
        RecAnimation {
            id: homePower
            width:row.width/4
            devType:qsTr("主电源")
            runEnable:false
            flashColor: "lightgreen"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
        }

        RecAnimation {
            id: reservedPower
            width:row.width/4
            devType:qsTr("备用电源")
            runEnable:false
            flashColor: "lightgreen"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
        }
        Rectangle{
            id: rectangle
            width: row.width/4
            height: row.height
            color: "white"
            Button{
                id:button
                width: rectangle.width
                height: row.height/3
                text:qsTr("关闭声音")
                anchors.top: parent.top
                anchors.topMargin: row.height/8
            }
            Rectangle {
                id: displayRect
                width: rectangle.width
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: button.bottom
                anchors.topMargin: 2
                Label {
                    id: label
                    width: displayRect.width
                    height: displayRect.height/8
                    text: qsTr("故障原因：")
                }
                Text {
                    id: textFailCause
                    width: displayRect.width
                    height: displayRect.height/8
                    text: " "
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.top: label.bottom
                    anchors.topMargin: 3
                    font.pixelSize: 12
                }
            }
        }
    }

    Connections {
        target: button
        onClicked: {
            print("aaaaaaaaaa")
            sendCloseMusicCmd()
            receiveDevInfo(0,1,"HHHH");
        }
    }
}
