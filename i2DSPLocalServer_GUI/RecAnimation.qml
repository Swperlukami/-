import QtQuick 2.0
import QtGraphicalEffects 1.0
Item{
    width:100
    height:100
    property color flashColor: "#befa9c"
    property bool runEnable: true
    property alias devType: devtext.text
    Rectangle {
        width:parent.width
        height:parent.height
        color: "white"
        id:backgound
        RectangularGlow {
            id: effect
            glowRadius: 20
            anchors.fill: rectangle
            spread: 0.8
            color:  flashColor
            cornerRadius:rectangle.radius+glowRadius
            opacity: 0
        }
        Rectangle {
            id: rectangle
            Text {
                id:devtext
                text:"devType"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
            }
            width: Math.round(backgound.width/1.5)
            height: Math.round(backgound.height/1.5)
            color:flashColor
            radius:20
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
        SequentialAnimation {
            id:test
            running: runEnable
            loops: Animation.Infinite
            NumberAnimation {id:an; target: effect; property: "opacity";
                alwaysRunToEnd: true; from:0;to:0.6;
                duration: 2500;}
            NumberAnimation {id:an2; target: effect; property: "opacity";
                alwaysRunToEnd: true; from:0.6;to:0;
                duration: 2500;}
        }


    }

}
