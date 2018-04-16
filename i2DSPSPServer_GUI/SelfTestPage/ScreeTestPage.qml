import QtQuick 2.0

Item {
    id:screenTestpage

    anchors.fill:parent
    property color screencolor: "black"
    property int  nTimeCnt: 0

    signal testover

    function  screencolorchanged()
    {
        switch(nTimeCnt)
        {
        case 0:
            screencolor = "black";
            break
        case 1:
            screencolor = "white";
            break
        case 2:
            screencolor = "red";
            break
        case 3:
            screencolor = "blue";
            break
        case 4:
            screencolor = "green";
            break
        default:
            screencolor = "black";
            break
        }
        nTimeCnt++;
        return screencolor;

    }
    Rectangle{
        anchors.fill: parent
        color:screencolor
    }
    Timer {
        interval: 2000; running: screenTestpage.visible; repeat: true
        triggeredOnStart: true
        onTriggered: {
            screencolorchanged();
            if(nTimeCnt > 4){
                nTimeCnt = 0
                screenTestpage.visible = false
                testover()
                running = false
            }
            print("screen test ",nTimeCnt)
        }
    }
}
