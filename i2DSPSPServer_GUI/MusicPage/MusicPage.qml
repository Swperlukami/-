import QtQuick 2.0
import QtQuick.Controls 2.2

import "../SysSingletonModel"

Item {
    id:musicPage
    width: 1280
    height: 750

    function userControlPlayers(channelIndex , cmd , songIndex){
//        console.log(channelMusicPlayers[channelIndex])
        channelMusicPlayers[channelIndex].userContrllChannelPlayer(cmd , songIndex)

    }

    function synMusicPlayerUI(channelIndex){
        listView.currentIndex = channelIndex
        var curMusicItem = listView.currentItem
        curMusicItem.channelState = sysChannelPlayers.channelPlayers[channelIndex]["curSate"]
    }

    property alias muiscPageModeAlias: sysMusicPageModel

    property var channelMusicPlayers: new Array
    property Component comPlayer:  null

    function initCreateChannelPlayers(){
        comPlayer = Qt.createComponent("ChannelMusicPlayHander.qml")
        for(var channelPlayerIndex = 0 ; channelPlayerIndex< 5 ; channelPlayerIndex++){
            channelMusicPlayers[channelPlayerIndex] = comPlayer.createObject(musicPage ,{"channelID":(channelPlayerIndex+1)})
            console.log(channelMusicPlayers[channelPlayerIndex].channelID)
        }
    }



    ListView {
        id: listView
        width: 1220
        height: 740
        spacing: 28
        clip: true
        //        header: Rectangle{width: parent.width  ; height: 30
        //            radius: 5
        //            Text{
        //                anchors.centerIn: parent
        //                text:"Song "
        //            }
        //        }
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenterOffset: 0
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        model:sysMusicPageModel.channelAlbumInfoModel
        delegate: AlbumItemDelegate{
            albumSongListModel: sysMusicPageModel.channelPlayListModels[index]
            onChannlVolChanged: {
                if( channelMusicPlayers[index] === undefined)
                    return
                channelMusicPlayers[index].currentPlayVol = vol
            }
        }
    }

    MusicPageModel{
        id:sysMusicPageModel
    }

    Component.onCompleted: {
        console.log("Music Page Component")

        initCreateChannelPlayers()

        sysMusicPageModel.initCreateChannelSongListModels()

        for(var channelIndex = 0 ; channelIndex < sysMusicPageModel.channelAlbumInfoModel.count ; channelIndex++){
            listView.currentIndex = channelIndex
            listView.currentItem.albumSongListModel =  sysMusicPageModel.channelPlayListModels[channelIndex]

            channelMusicPlayers[channelIndex].channelPlayListInfo =  sysMusicPageModel.channelPlayListModels[channelIndex]


            console.log("Init channel List model")
        }
    }

    //    RoundButton {
    //        id: roundButton
    //        x: 68
    //        width: 294
    //        height: 40
    //        text: "新建歌单"
    //        checkable: false
    //        font.pointSize: 11
    //        anchors.top: listView.bottom
    //        anchors.topMargin: 40

    //        onClicked: {
    //            sysMusicPageModel.iniAlbumInfoModel(1,"Album Test Name",2,60,10)
    //        }
    //    }

    //    RoundButton {
    //        id: roundButton1
    //        x: 400
    //        y: 0
    //        width: 294
    //        height: 40
    //        text: "删除歌单"
    //        anchors.top: listView.bottom
    //        anchors.topMargin: 40
    //    }

    //    RoundButton {
    //        id: roundButton2
    //        x: 729
    //        y: -4
    //        width: 294
    //        height: 40
    //        text: "编辑歌单名"
    //        checkable: true
    //        anchors.top: listView.bottom
    //        anchors.topMargin: 40
    //    }


}
