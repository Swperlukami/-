import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

Item {
    id:audioFilesManagementPage
    width: 1280
    height: 750

    property alias audioFilesManagePageModelAlias: sysAudioFilesModel
    property alias audioFilesSelectPageAlias: sysAudioFilesSelectPage

    AudioFilesManageMentPageModel{
        id:sysAudioFilesModel
    }

    AudioFilesSelectPage{
        id:sysAudioFilesSelectPage
    }

    TabBar {
        id: tabBar
        x: 8
        width: 1280
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter

        TabButton {
            id: tabButtonEmgRecord
            x: 0
            y: 0
            text: qsTr("应急话筒录音文件")

        }

        TabButton {
            id: tabButtonEmgFiles
            x: 128
            y: 0
            text: qsTr("应急广播文件")
        }

        TabButton {
            id: tabButtonRecordFiles
            x: 515
            y: 0
            text: qsTr("业务广播录音文件")
        }

        TabButton {
            id: tabButtonMusic
            x: 767
            y: 0
            text: qsTr("音乐文件")
        }

        TabButton {
            id: tabButtonBells
            x: 1025
            y: 0
            text: qsTr("铃声文件")
        }
    }

    ListView {
        id: listView
        x: 0
        width: 1280
        height: 670
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        anchors.top: tabBar.bottom
        anchors.topMargin: 2
        spacing: 1
        delegate: AuidoFileItemDelegate{
            width: listView.width
        }

        model:{
            switch(tabBar.currentIndex){
            case 0:
                return sysAudioFilesModel.audioEmgReordFilesModel
            case 1:
                return sysAudioFilesModel.audioEmgFilesModel
            case 2:
                return sysAudioFilesModel.audioRecordFilesModel
            case 3:
                return sysAudioFilesModel.audioMusicFilesModel
            case 4:
                return sysAudioFilesModel.audioBellsFilesModel
            }
        }

    }

    RoundButton {
        id: roundButtonAddFile
        x: 108
        width: 1200
        height: 25
        text: "添加歌曲"
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: listView.bottom
        anchors.topMargin: 20
        onClicked: {
            fileDialog.visible = true
        }
        visible: tabBar.currentIndex != 0 && tabBar.currentIndex != 2
    }



    FileDialog {
          id: fileDialog
          title: "Please choose a file"
          folder: shortcuts.music
          selectMultiple:true
          nameFilters: [ "audio files (*.mp3 *wav)"]
          onAccepted: {
              console.log("You chose: " + fileDialog.fileUrls)
              for(var filePath in fileUrls){
                  console.log(fileUrls[filePath])
                  AduioFileManageController.addAudioFileToSysFloder(tabBar.currentIndex+1 ,fileUrls[filePath].substr(7))
              }
              if(tabBar.currentIndex === 3)
                  CPMatrixController.userSetMatrixEvent("Mount" , 0 , "" , 0)
          }
          onRejected: {
              console.log("Canceled")
          }
      }
}
