import QtQuick 2.0
import QtMultimedia 5.8

Item {
    id:musicPageModel


    function iniAlbumInfoModel(albumID ,albumName ,albumPlayMode ,albumPlayVol ,albumSongCnt ,playDevCnt ){
        musicPageModel.channelAlbumInfoModel.append({"albumID":albumID ,
                                                 "albumName":albumName ,
                                                 "albumPlayMode":albumPlayMode ,
                                                 "albumPlayVol":albumPlayVol ,
                                                 "albumSongCnt":albumSongCnt ,
                                                 "channelPlayDevCnt":playDevCnt

                                             })
    }

    property ListModel channelAlbumInfoModel: ListModel{
        ListElement{
//            albumID:1
//            albumName:"My Favorite Songs"
            albumPlayMode: 1
            albumPlayVol: 50
            albumSongCnt:1
            channelPlayDevCnt:0
        }

        ListElement{
//            albumID:1
//            albumName:"My Favorite Songs"
            albumPlayMode: 2
            albumPlayVol: 50
            albumSongCnt:1
            channelPlayDevCnt:0
        }
        ListElement{
//            albumID:1
//            albumName:"My Favorite Songs"
            albumPlayMode: 1
            albumPlayVol: 50
            albumSongCnt:1
            channelPlayDevCnt:0
        }
        ListElement{
//            albumID:1
//            albumName:"My Favorite Songs"
            albumPlayMode: 1
            albumPlayVol: 50
            albumSongCnt:1
            channelPlayDevCnt:0
        }
        ListElement{
//            albumID:1
//            albumName:"My Favorite Songs"
            albumPlayMode: 1
            albumPlayVol: 50
            albumSongCnt:10
            channelPlayDevCnt:0
        }
    }


    property var channelPlayListModels: new Array

    Connections{
        target: ChannelMusicController
        onInitChannelMusicList:{
            channelPlayListModels[channelIndex].append({"songName":songName ,
                                                               "singer":singer ,
                                                               "album":album ,
                                                               "duration":duration ,
                                                               "isPlay": false
                                                           })
        }
    }

    function userAddOrDelChannelSongListItem(channelIndex ,isAdd ,songName , singer , album , duration){
        if(isAdd){
            channelPlayListModels[channelIndex].append({"songName":songName ,
                                                            "singer":singer ,
                                                            "album":album ,
                                                            "duration":duration ,
                                                            "isPlay": false
                                                        })
        }
        else{
            var index = getChanneSongListItemIndex(channelIndex , songName)
            console.log("User Edit Channel Song List" , index , songName)
            if(index !== -1){
                channelPlayListModels[channelIndex].remove(index)
//                channelMusicPlayers[channelIndex].channelPlayListInfo.remove(index)
            }
        }
        ChannelMusicController.userAddOrDelSong(isAdd , channelIndex+1, songName)

    }

    function getChanneSongListItemIndex(channnelIndex , songName){
        for(var songIndex = 0 ; songIndex < channelPlayListModels[channnelIndex].count ; songIndex++){
            if(channelPlayListModels[channnelIndex].get(songIndex)["songName"] === songName)
                return songIndex
        }
        return -1
    }

    function refreashUsedDevicesCnt(channelIndex){
        channelAlbumInfoModel.setProperty(channelIndex , "channelPlayDevCnt" , channelMusicPlayers[channelIndex].getCurrentPlayingDevicesCnt())
    }

    function initCreateChannelSongListModels(){
        for(var channelListIndex = 0 ; channelListIndex< channelAlbumInfoModel.count ; channelListIndex++){
            channelPlayListModels[channelListIndex] = Qt.createQmlObject('import QtQml.Models 2.2 ; ListModel{}' , musicPageModel , "")
        }
        console.log("Music Page Model Create Channel Song List Models OK")
    }

    Component.onCompleted: {
        console.log("Music Page Model Component")


    }

//    property ListModel albumSongsModel: ListModel{
//        ListElement{
//            songName:"Song Name.mp3"
//            singer:"Adele"
//            album:"Adele Album"
//            duration:100
//            isPlay:false

//        }
//        ListElement{
//            songName:"Song Name.mp3"
//            singer:"Adele"
//            album:"Adele Album"
//            duration:100
//            isPlay:false

//        }
//        ListElement{
//            songName:"Song Name.mp3"
//            singer:"Adele"
//            album:"Adele Album"
//            duration:100
//            isPlay:true
//        }
//    }

}
