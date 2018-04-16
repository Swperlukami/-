import QtQuick 2.0
import QtGraphicalEffects 1.0

Component{
    id:effectcom
    RectangularGlow {
        width: 100
        height: 100
        glowRadius: 0
        spread: 0.04
        //            color: "#DB5C5C"
        color: "red"

        cornerRadius: 50
        visible: true

        SequentialAnimation on glowRadius {
            id: loop
            loops: Animation.Infinite
            alwaysRunToEnd: true
            running: true
            PropertyAnimation{
                to: 50
                duration: 1500
            }
            PropertyAnimation{
                to: 0
                duration: 1500
            }
        }
    }
}

