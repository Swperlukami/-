import QtQuick 2.0

Item {
    anchors.fill:parent
    property color screencolor: "black"
    property int  nTimeCnt: 0
    function  screencolorchanged()
    {
        nTimeCnt ++;
        switch(nTimeCnt)
        {
        case 0:
            screencolor = "black";
            return screencolor;
        case 1:
            screencolor = "white";
            return screencolor;
        case 2:
            screencolor = "red";
            return screencolor;
        case 3:
            screencolor = "blue";
            return screencolor;
        case 4:
            screencolor = "green";
            nTimeCnt =-1;
            return screencolor;
        default:screencolor = "black";
            return screencolor;

        }
    }
    Rectangle{
        anchors.fill: parent
        color:screencolor
    }
    Timer {
        interval: 3000; running: true; repeat: true
        onTriggered: {
            screencolorchanged();
            print(nTimeCnt)
        }
    }
}
