import QtQuick 2.0

Item {
    width: 600
    height: 30

    Rectangle {
        id: rectangle
        color: "#d2d2d2"
        anchors.fill: parent
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            if(sysAudioFilesModel.selectAudioMode === "single"){
                sysAudioFilesModel.singleCurSelectedFileName = audioName
                sysTimedTaskPage.timedTaskManagePageAlias.timedEditPageAlias.userSelectAudioFile(audioName)
            }
            else{
                listView.model.setProperty(index , "isSelect" , !isSelect)
                sysMusicPage.muiscPageModeAlias.userAddOrDelChannelSongListItem(sysAudioFilesModel.selectChannelIndex , isSelect ,
                                                                                audioName , audioSinger , audioAlbum ,  audioDuration)
            }
        }

    }

    Row {
        id: row
        anchors.fill: parent

        Text {
            id: textIndex
            width: 20
            height: 30
            text: (index+1)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
        }

        Text {
            id: textAudioName
            width: 460
            height: 30
            text: audioName
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
        Text {
            id: textAudioDuration
            width: 60
            height: 30
            text: audioDuration
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }

        Text {
            id: textAudioSelectedState
            width: 60
            height: 30
            color: "#12ff55"
            text: {
                if(sysAudioFilesModel.selectAudioMode === "single")
                    sysAudioFilesModel.singleCurSelectedFileName === audioName ? "已选" :""
                else{
                    isSelect ? "已选" :""
                }
            }
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
        }
    }


}
