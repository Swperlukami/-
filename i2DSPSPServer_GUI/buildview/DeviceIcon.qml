import QtQuick 2.3
import QtGraphicalEffects 1.0
Item {
    id: deviceIcon
    width: 30
    height: 70

    property int emgNumber: 0
    property bool editMode: false
    property bool destroyIcon: false
    property bool click: false
    property bool textVisible: true
    property bool lowpower: false
    property string deviceType: "SD"
    property string deviceState: "green"
    property string deviceId: ""
    property string ltid: ""
    readonly property string font: setFont(deviceType)
    readonly property int topLeftX: setTopLeftX(deviceType)
    readonly property int topLeftY: setTopLeftY(deviceType)

    function setFont(type) {
        if (type === "MC") {
            return "14px Sawasdee"
        }
        else if (type === "CC" || type === "BP") {
            return "15px Sawasdee"
        }
        else {
            return "16px Sawasdee"
        }
    }

    function setTopLeftY(type) {
        if (type === "MC") {
            return 20
        }
        else if (type === "CC" || type === "BP" || type === "SD") {
            return 21
        }
        else {
            return 22
        }
    }

    function setTopLeftX(type) {
        switch (type) {
        case "CC":
            return 3
        case "MC":
            return 4
        case "LT":
            return 7
        case "BP":
            return 6
        case "SP":
            return 6
        case "SD":
            return 5
        default:
            return 6
        }
    }

    onDeviceStateChanged: {
        canvas.requestPaint()
    }
    onLowpowerChanged: {
        canvas.requestPaint()
    }

    onDestroyIconChanged: {
        if (destroyIcon) {
            animation.start()
        }
        else {
            return
        }
    }

    RectangularGlow {
        anchors.fill: parent
        glowRadius: 0
        spread: 0.02
        color: "#DB5C5C"
        cornerRadius: deviceIcon.width / 2
        visible: deviceState == "red"

        SequentialAnimation on glowRadius {
            id: loop
            loops: Animation.Infinite
            alwaysRunToEnd: true
            running: deviceState == "red"
            PropertyAnimation{
                to: 0
                duration: 800
            }
            PropertyAnimation{
                to: 40
                duration: 800
            }
        }
    }

    Text{
        id:emgnumber
        anchors.bottom: canvas.top
        width: parent.width
        height: 20
        color:"red"
        font{bold:true ;pixelSize:20}
        text:emgNumber
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: emgNumber ? true : false
    }

    Canvas {
        id: canvas
        width: parent.width
        height: parent.height
        onPaint: {
            var ctx = getContext("2d")
            //绘制外圆
            ctx.fillStyle = deviceState
            ctx.beginPath()
            ctx.moveTo(15, 15)
            ctx.arc(15, 15, 15, 0, Math.PI * 2, true)
            ctx.closePath()
            ctx.fill()
            //绘制底部
            ctx.beginPath()
            ctx.moveTo(29, 15)
            ctx.arc(15, 15, 14, 0, -Math.PI, false)
            ctx.lineTo(15, 70)
            ctx.closePath()
            ctx.fill()
            //绘制内圆
            ctx.beginPath()
            ctx.fillStyle = "lightgrey"
            ctx.arc(15, 15, 12, 0, Math.PI * 2, true)
            ctx.closePath()
            ctx.fill()
            //绘制文字
            if (textVisible) {
                ctx.beginPath()
                ctx.fillStyle = "black"
                ctx.font = font
                ctx.fillText(deviceType, topLeftX, topLeftY)
                ctx.closePath()
            }
            if(lowpower){
                ctx.beginPath()
                ctx.fillStyle = "yellow"
                ctx.rect(10,35,10,5)
                ctx.fill()
            }
        }

        MouseArea {
            id: area
            anchors.fill: parent

            property point pos: "1,1"

            drag.target: editMode ? deviceIcon : null

            onPressed: {
                if (editMode) {
                    pos = Qt.point(mouse.x, mouse.y)
                }
                else {
                    click = !click
                }
//                ManageCenter.show_device_id(deviceId, deviceIcon.x, deviceIcon.y)
            }
            onPositionChanged: {
                if (editMode) {
//                    var delta = Qt.point(mouse.x-pos.x, mouse.y-pos.y)
//                    deviceIcon.x += delta.x
//                    deviceIcon.y += delta.y
                }
            }
            onReleased: {
                if (editMode) {
//                    ManageCenter.update_position(deviceId, deviceType, deviceIcon.x, deviceIcon.y)
                }
            }
        }
    }

    NumberAnimation {
        id: animation
        target: deviceIcon
        property: "opacity"
        from: 1
        to: 0
        duration: 200
        onRunningChanged: {
            if (!running) {
                loop.stop()
                deviceIcon.destroy()
            }
        }
    }
}
