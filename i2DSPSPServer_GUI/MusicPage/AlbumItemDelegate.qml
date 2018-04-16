import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4

import "../SysSingletonModel"

Item {
    id:albumItemDel
    width: 1220
    height: 100

    property int channelIndex: index
    property int channelState: sysChannelPlayers.channelPlayers[channelIndex]["curSate"]



    function fllowCurrentListIndex(){
        ListView.view.currentIndex = index
    }

    property bool isshowsongs: false

    property alias albumSongListModel: songslistView.model

    signal channlVolChanged(int vol)

    Component.onCompleted: {
//        console.log("song list model :" , albumSongListModel)
    }

    //    ListView.onAdd:SequentialAnimation{
    //        PropertyAction { target: albumItemDel; property: "height"; value: 0 }
    //        NumberAnimation { target: albumItemDel; property: "height"; to: 100; duration: 100; easing.type: Easing.InOutQuad }

    //    }

    Rectangle{
        id: rootrectangle
        anchors.fill: parent
        radius: 5
        color: Qt.rgba(1,1,1,0.6)
        clip: true

        MouseArea{
            id: mouseArea
            width: 1220
            height: 100
            //            hoverEnabled: true
            onClicked: {
                fllowCurrentListIndex()

            }
        }
        Text {
            id: txtchannel
            text: qsTr("频道-")+(index+1)
            //            text: qsTr("频道-1")
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 50
            font.pixelSize: 30
        }

        RoundButton {
            id: btnSongs
            x: 367
            y: 30
            width: 113
            height: 40
            text: qsTr("歌曲数量:")+albumSongListModel.count
            onClicked: {
                if(listView.currentIndex !== index)
                    isshowsongs = false
                fllowCurrentListIndex()
//                if(isshowsongs){
//                    listView.positionViewAtIndex(index , ListView.Beginning)
//                }
                isshowsongs = !isshowsongs
                listView.interactive = !isshowsongs

//                albumSongListModel = sysMusicPageModel.channelPlayListModels[index]
//                sysMusicPageModel.channelAlbumInfoModel.setProperty(index , "albumSongCnt" , albumSongListModel.count )
            }
        }


        RoundButton {
            id: roundButtonPlay
            x: 1061
            y: 20
            width: 60
            height: 60
            checked: channelState === UISysModel.sysChannelPlayerMusic
            text: {
                if(channelState === UISysModel.sysChannelPlayerIdel)
                    return "开始"
                else if(channelState === UISysModel.sysChannelPlayerMusic)
                    return "停止"
                else
                    return "开始"

            }
            onClicked: {
                fllowCurrentListIndex()
                var cmd = ""
                if(channelState === UISysModel.sysChannelPlayerIdel)
                    cmd = "Play"
                else if(channelState === UISysModel.sysChannelPlayerMusic){
                    cmd = "Pause"
                }
                console.log("cdm:" , cmd , "channnelState:",channelState)

                sysMusicPage.userControlPlayers(channelIndex , cmd , 0)
                channelState = sysChannelPlayers.channelPlayers[channelIndex]["curSate"]
            }
        }

        RoundButton {
            id: roundButtonNext
            x: 1134
            y: 30
            text: "下一首"
            enabled: roundButtonPlay.checked
            onClicked: {
                fllowCurrentListIndex()
                sysMusicPage.userControlPlayers(channelIndex , "Next" , 0)
            }
        }

        RoundButton {
            id: roundButtonPrevious
            x: 991
            y: 30
            text: "上一首"
            enabled: roundButtonPlay.checked

            onClicked: {
                fllowCurrentListIndex()
                sysMusicPage.userControlPlayers(channelIndex , "Previous" , 0)
            }
        }

        RoundButton {
            id: roundButtonPlayMode
            x: 786
            y: 62
            width: 164
            height: 24
            text:{
                switch(albumPlayMode){
                case 1:
                    return "列表循环"
                case 2:
                    return "随机播放"
                }
            }
            onClicked: {
                fllowCurrentListIndex()
                var mode = 0
                if(albumPlayMode === 1)
                    mode = 2
                else
                    mode = 1
                sysMusicPageModel.channelAlbumInfoModel.setProperty(channelIndex , "albumPlayMode" , mode)
                sysMusicPage.userControlPlayers(channelIndex , "Mode" , mode)
            }
        }

        Slider {
            id: sliderVol
            x: 822
            y: 13
            width: 128
            height: 34
            minimumValue: 0
            maximumValue:100
            value: albumPlayVol
            stepSize: 5
            onPressedChanged: {
                fllowCurrentListIndex()
            }
            onValueChanged: {
                channlVolChanged(sliderVol.value)
            }
        }

        Text {
            id: txtVol
            x: 786
            y: 22
            text: qsTr("音量")
            font.pixelSize: 12
        }

        //        ComboBox {
        //            id: comboBox
        //            x: 535
        //            y: 30
        //            width: 98
        //            height: 40
        //            model: ["频道-1" ,"频道-2" ,"频道-3" ,"频道-4" ,"频道-5"]
        //            onPressedChanged:  {
        //                fllowCurrentListIndex()
        //                if(!down)
        //                    down = true
        //            }

        //            onCurrentIndexChanged: {
        //                console.log("Channel Index",comboBox.currentIndex)
        //            }

        //        }

        Rectangle {
            id: rectangleMark
            width: 10
            height: 100
            radius: 5
            anchors.left: parent.left
            anchors.leftMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 0
            color: "#2bfe65"
            visible: listView.currentIndex === index
        }

        ListView {
            id: songslistView
            x: 0
            width: 1220
            height: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 110
            model: albumSongListModel
            opacity: 0
            clip: true
            header: Rectangle{
                width:  songslistView.width
                height: 40
                color: Qt.rgba(0,0,0,0.8)
                Row{
                    anchors.fill: parent
                    Text{
                        width: parent.width*0.4
                        text:"歌名"

                        height: parent.height
                        font.pixelSize: 22
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "white"
                    }
                    Text{
                        width: parent.width*0.2
                        text:"歌手"
                        height: parent.height
                        font.pixelSize: 22
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "white"
                    }
                    Text{
                        width: parent.width*0.2
                        text:"专辑"
                        height: parent.height
                        font.pixelSize: 22
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "white"
                    }
                    Text{
                        width: parent.width*0.1
                        text:"时长"
                        height: parent.height
                        font.pixelSize: 22
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "white"
                    }
                }
            }

            delegate: SongItemDelegate {
                channelIndex: albumItemDel.channelIndex
            }
            highlight: Rectangle{
                width: parent.width
                height: 40
                color: Qt.rgba(1,0,0,0.4)
            }

//            backgroundVisible: false
//            frameVisible: false
//            headerDelegate:  Rectangle{
//                implicitHeight: 40
//                color: Qt.rgba(0,0,0,0.8)
//                clip: true

//                Text{
//                    anchors.fill: parent
//                    //                anchors.centerIn: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: styleData.value === "播放设备" ? Text.AlignLeft : Text.AlignHCenter
//                    text:styleData.value
//                    font.pixelSize: 20
//                    color: "white"

//                }
//            }


//            rowDelegate: Rectangle{
//                height: 30
//                color: styleData.alternate ? Qt.rgba(0,0,0,0.4) : Qt.rgba(1,0.2,1,0.1)
//            }
//            TableViewColumn {
//                role: "songName"
//                title: "歌名"
//                width: parent.width*0.4
//                horizontalAlignment: Text.AlignHCenter

//            }
//            TableViewColumn {
//                role: "singer"
//                title: "歌手"
//                width: parent.width*0.2
//                horizontalAlignment: Text.AlignHCenter

//            }
//            TableViewColumn {
//                role: "album"
//                title: "专辑"
//                width: parent.width*0.2
//                horizontalAlignment: Text.AlignHCenter

//            }
//            TableViewColumn {
//                role: "duration"
//                title: "时长"
//                width: parent.width*0.1
//                horizontalAlignment: Text.AlignHCenter

//            }

        }


        RoundButton {
            id: roundButtonPlayDevs
            x: 513
            y: 30
            width: 113
            height: 40
            enabled: roundButtonPlay.checked
            text: {
                if(channelPlayDevCnt === 0)
                    return "播放设备"
                else
                    return "播放设备:"+ channelPlayDevCnt

            }
            onClicked: {
                sysUIPubicFunction.showSelectDevicesDialog(UISysModel.enumAPMusic , channelIndex ,
                                                           channelMusicPlayers[channelIndex].currentChannelDevices)
            }
        }



        RoundButton {
            id: roundButtonAddSong
            x: 125
            width: 295
            height: 30
            text: "新增歌曲"
            anchors.horizontalCenterOffset: -200
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: songslistView.bottom
            anchors.topMargin: 10
            opacity: 0
            onClicked: {
                sysUIPubicFunction.showSelectAudioFilesDialog("multiple" , albumSongListModel , channelIndex)
            }
        }

//        RoundButton {
//            id: roundButtonDelSong
//            x: 126
//            width: 295
//            height: 30
//            text: "删除歌曲"
//            anchors.horizontalCenterOffset: 200
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.top: songslistView.bottom
//            opacity: 0
//            anchors.topMargin: 10
//        }
    }


    states: [
        State {
            name: "showsongs"
            when: listView.currentIndex === index && isshowsongs

            PropertyChanges {
                target: songslistView
                x: 8
                y: 112
                width: 1220
                height: 350
                anchors.topMargin: 100
                opacity: 1
            }

            PropertyChanges {
                target: albumItemDel
                height: 500
            }

            PropertyChanges {
                target: roundButtonAddSong
                x: 258
                y: 452
                width: 110
                height: 30
                text: "编辑歌曲"
                anchors.horizontalCenterOffset: 500
                anchors.bottomMargin: 8
                anchors.topMargin: 10
                opacity: 1
            }

//            PropertyChanges {
//                target: roundButtonDelSong
//                opacity: 1
//            }
            //            PropertyChanges {
            //                target: listView
            //                interactive:false
            //            }

        }
    ]

    transitions: Transition{
//        PropertyAnimation{target: albumItemDel ; property: "height" ; to:500 ; duration: 250}
//        PropertyAnimation{target: songslistView ; property: "height" ; to:400 ; duration: 250}
//        PropertyAnimation{target: albumItemDel ; property: "height" ; to:100 ; duration: 250}
//        PropertyAnimation{target: songslistView ; property: "height" ; to:0 ; duration: 250}
        PropertyAnimation{ property: "height" ; duration: 250}
        PropertyAnimation{ property: "opacity" ; duration: 250}

    }


}
