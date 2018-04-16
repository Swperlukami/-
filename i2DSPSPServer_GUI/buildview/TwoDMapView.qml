import QtQuick 2.2
import QtMultimedia 5.3
import QtQuick.Controls 1.2
import "JSHash.js" as JSHash


Flickable {
    contentWidth: mapsocureimage.width*myscale.xScale
    contentHeight: mapsocureimage.height*myscale.yScale
    clip: true

    property string imagename: ""
    property string currentDis: ""
    property string currentBuild: ""
    property string currentFLoor: ""

    property var hash: new JSHash.HashTable()
    property var emgdevicehash: new JSHash.HashTable()
    property bool emgMode: false
    property int count: 0
    property int emgVoiceCount: 0
    property bool editMode: false



    function createObject(type,id,ltid,port,state,x,y,power,emgno,emgtime) {
        var component = Qt.createComponent("DeviceIcon.qml")
        if(component.status === Component.Ready){
            var item = component.createObject(mapsocureimage)
            if (item === null) {
                console.log(component.errorString())
            }
            else {
                item.deviceState = state
                item.deviceType = type
                item.deviceId = type==="LT" ? ltid :id
                item.ltid = ltid
                item.editMode = false
                item.x = x
                item.y = y
                item.emgNumber = emgno
                console.log("init device id:",type,id,ltid)
                hash.add(item.deviceId, item)
            }
        }
    }

    function deviceEvent(id , event ,islt){
        console.log("two map view event:",id,event)
        switch(event){
        case "Online":if (hash.containsKey(id)) {
                hash.getValue(id).deviceState = "green"
            }
            break
        case "Offline":
            if (hash.containsKey(id)) {
                hash.getValue(id).deviceState = "grey"
            }
            break
        case "Emg":
            if (hash.containsKey(id)) {
                hash.getValue(id).deviceState = "red"
            }
            break
        case "CancelEmg":
            if (hash.containsKey(id)) {
                hash.getValue(id).deviceState = "green"
            }
            break
        case "Lowpower":
            if (hash.containsKey(id)) {
                hash.getValue(id).lowpower = true
            }
            break
        case "CancelLowpower":
            if (hash.containsKey(id)) {
                hash.getValue(id).lowpower = false
            }
            break
        }
    }

    function clearMapIcon() {
        if (hash.getSize() !== 0) {
            for (var i=0; i!==hash.getSize(); ++i) {
                hash.getValues()[i].destroyIcon = true
            }
            hash.clear()
        }
    }

    function showDeviceId(id, x, y) {
        deviceIdRect.idText = id
        deviceIdRect.x = x + 40
        deviceIdRect.y = y
        deviceIdRect.visible = true
    }

    function playAlarmVoice() {
        //don't use for CC alarm
        if (!confirmAlarm.playing) {
            confirmAlarm.play()
        }
        else {
            ++emgVoiceCount
        }
    }
    function playCancelAlarmVoice(){
        if(!cancelAlarm.playing){
            cancelAlarm.play()
        }
    }

    Rectangle {
        id: deviceIdRect
        parent: mapsocureimage
        width: 160
        height: 50
        x: 0
        y: 0
        visible: false
        color: "#99393939"
        z:10
        property alias idText: deviceIdText.text
        Text {
            id: deviceIdText
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20
            color: "white"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: deviceIdRect.visible = false
        }
    }


    Scale{
        id:myscale
        xScale: 1
        yScale: 1
    }

    Image {
        id: mapsocureimage
        anchors.centerIn: parent.Center
        fillMode: Image.PreserveAspectFit
//        source: "file:///home/wangxiaobo/WangXiaobo/GUI_Build/BuildingMap/2DMap/A1B1F1.png"
        source: imagename


        transform:myscale
        MouseArea{
            anchors.fill: parent
            onWheel: {
                if (1/*wheel.modifiers*/ /*& Qt.ControlModifier*/) {
                    if (wheel.angleDelta.y > 0)
                    {
//                        if(myscale.xScale < 3){
//                            myscale.xScale +=0.03
//                            myscale.yScale +=0.03
//                        }
//                        console.log("ZoomOut")
                    }
                    else
                    {
//                        if(myscale.xScale > 0.366){
//                            myscale.xScale -=0.03
//                            myscale.yScale -=0.03
//                        }
//                        console.log("ZoomIn")
                    }
                }
            }
        }
    }

    SoundEffect {
        id: confirmAlarm
        volume: 1.0
//        source: "qrc:/Music/Confirm.wav"
        onLoopsRemainingChanged: {
            if (!loopsRemaining && emgVoiceCount) {
                play()
                --emgVoiceCount
            }
        }
    }

    SoundEffect {
        id: cancelAlarm
        volume: 1.0
//        source: "qrc:/Music/Cancel.wav"
//        onLoopsRemainingChanged: {
//            if (!loopsRemaining && emgVoiceCount) {
//                play()
//                --emgVoiceCount
//            }
//        }
    }

//    Component.onCompleted: {
//        createObject("SD" , "B22F2LT02S03" , "green" , 200, 400 , 10)
//    }
}
