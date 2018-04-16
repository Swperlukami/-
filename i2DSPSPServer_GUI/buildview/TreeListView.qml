import QtQuick 2.0

Rectangle {
    width: parent.width
    height: 150
    color:"#DD393939"

    signal usrClicked(int type , string strIndex)

    ListView{
        id:disttricklist
        objectName: "objdislistview"
        anchors.top: parent.top
        width: parent.width
        height: 50
        orientation: ListView.Horizontal
//        model: DistraickModel
        delegate: dislistviewdele
        clip: true
        highlight: dishgihlight
        highlightFollowsCurrentItem: false
        focus: true

    }

    ListView{
        id:buildlsitview
        objectName: "objbuildlistview"
        anchors.top: disttricklist.bottom
        width: parent.width
        height: 50
        orientation: ListView.Horizontal
//        model:BuildsModel
        delegate: buildlistviewdele
        clip: true
        highlight: buildhgihlight
        highlightFollowsCurrentItem: false
        focus: true

    }

    ListView{
        id:floorlistview
        objectName: "objfloorlistview"
        anchors.top: buildlsitview.bottom
        width: parent.width
        height: 50
        orientation: ListView.Horizontal
//        model:FloorModel
        delegate: floorlistviewdele
        clip: true
        highlight: floorhgihlight
        highlightFollowsCurrentItem: false
        focus: true

    }


    Component{
        id:dislistviewdele
        Rectangle{
            id:disdelerec
            width: 200 ; height: 50
            color: false ? Qt.rgba(1,0,0,0.8):"transparent"
            border.color: "gray"
            border.width: 1

            Text{
                id:name
                width: parent.width
                height: 30
                fontSizeMode:Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true
                wrapMode: Text.WordWrap
                text:buildname
                color: "white"
            }
            Text{
                id:number
                width: parent.width
                height: 20
                anchors.top: name.bottom
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:numemg+"/"+numdevices
                color: isemg ? "red" : "white"

            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    disdelerec.ListView.view.currentIndex = index
                    usrClicked(1 , buildnumber)
                }
            }
        }
    }


    Component{
        id:buildlistviewdele
        Rectangle{
            id:builddelerec
            width: 200 ; height: 50
            color: false ? Qt.rgba(1,0,0,0.5):"transparent"
            border.color: "gray"
            border.width: 1

            Text{
                id:name
                width: parent.width
                height: 30
                fontSizeMode:Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true
                wrapMode: Text.WordWrap
                text:buildname
                color: "white"
            }
            Text{
                id:number
                width: parent.width
                height: 20
                anchors.top: name.bottom
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:numemg+"/"+numdevices
                color: isemg ? "red" : "white"

            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    builddelerec.ListView.view.currentIndex = index
                    usrClicked(2 , buildnumber)
                }
            }
        }
    }

    Component{
        id:floorlistviewdele
        Rectangle{
            id:floordelerec
            width: 200 ; height: 50
            color: false ? Qt.rgba(1,0,0,0.5):"transparent"
            border.color: "gray"
            border.width: 1

            Text{
                id:name
                width: parent.width
                height: 30
                fontSizeMode:Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true
                wrapMode: Text.WordWrap
                text:buildname
                color: "white"
            }
            Text{
                id:number
                width: parent.width
                height: 20
                anchors.top: name.bottom
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:numemg+"/"+numdevices
                color: isemg ? "red" : "white"

            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    floordelerec.ListView.view.currentIndex = index
                    usrClicked(3 , buildnumber)
                }
            }
        }
    }
    Component{
        id:dishgihlight
        Rectangle{
            width: 200 ; height: 50
            color: "#DD3BAFDA"
            x:disttricklist.currentItem.x
            radius: 0
            Behavior on x { SpringAnimation { spring: 1; damping: 0.1 } }

        }
    }
    Component{
        id:buildhgihlight
        Rectangle{
            width: 200 ; height: 50
            color: "#DD3BAFDA"
            x:buildlsitview.currentItem.x
            radius: 0
            Behavior on x { SpringAnimation { spring: 1; damping: 0.1 } }

        }
    }
    Component{
        id:floorhgihlight
        Rectangle{
            width: 200 ; height: 50
            color: "#DD3BAFDA"
            x:floorlistview.currentItem.x
            radius: 0
            Behavior on x { SpringAnimation {spring: 1; damping: 0.1 } }

        }
    }


}
