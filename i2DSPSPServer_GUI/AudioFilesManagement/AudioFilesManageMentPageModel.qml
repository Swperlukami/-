import QtQuick 2.0

Item {
    id:audioFilesManagementPageModel


    //Tyep 1
    property ListModel audioEmgReordFilesModel: ListModel{
//        ListElement{
//            audioName:"Tets Audio Emg Record File Name.wav"
//            audioDuration:100
//        }
    }
    //Tyep 2
    property ListModel audioEmgFilesModel: ListModel{
//        ListElement{
//            audioName:"Tets Audio Emg File Name.mp3"
//            audioDuration:80
//        }
    }
    //Tyep 3
    property ListModel audioRecordFilesModel: ListModel{
//        ListElement{
//            audioName:"Tets Audio Record Name.mp3"
//            audioDuration:70
//        }
    }
    //Tyep 4
    property ListModel audioMusicFilesModel: ListModel{
//        ListElement{
//            audioName:"1.mp3"
//            audioDuration:70
//            audioSinger:"Adel"
//            audioAlbum:"Adel Alnum"
//            isSelect:false
//        }
    }
    //Tyep 5
    property ListModel audioBellsFilesModel: ListModel{
//        ListElement{
//            audioName:"Get Up-1.mp3"
//            audioDuration:70
//            audioSinger:""
//            audioAlbum:""
//            isSelect:false
//        }
    }

    property string curReplayFileName: ""
    property string selectAudioMode : ""   //single  multiple
    property string singleCurSelectedFileName: ""
    property int selectChannelIndex: -1

    function setAudioFileSelectModeAndExistFiles(selectMode , existAudios){
        selectAudioMode = selectMode
        initAudioFilesSelectState()
        if(selectMode === "single"){
            console.log("User Select Audio Exist:" , existAudios)
            var curFileIndex = getFileIndex(existAudios , audioBellsFilesModel)
            if(curFileIndex !== -1){
                audioBellsFilesModel.setProperty(curFileIndex , "isSelect" , true)
                singleCurSelectedFileName = existAudios
                return
            }
            curFileIndex = getFileIndex(existAudios , audioMusicFilesModel)
            if(curFileIndex !== -1){
                audioMusicFilesModel.setProperty(curFileIndex , "isSelect" , true)
                singleCurSelectedFileName = existAudios
                return
            }
        }
        else{

            for(var fileIndex =0  ;fileIndex <  existAudios.count ; fileIndex++){
                console.log("User Select Audio Exist:" , existAudios.get(fileIndex)["songName"])

                for(var index = 0 ; index < audioMusicFilesModel.count ; index++){
                    if(existAudios.get(fileIndex)["songName"] === audioMusicFilesModel.get(index)["audioName"]){
                        audioMusicFilesModel.setProperty(index , "isSelect" , true)
                    }
                }

                for(var index2 = 0 ; index2 < audioBellsFilesModel.count ; index2++){
                    if(existAudios.get(fileIndex)["songName"] === audioBellsFilesModel.get(index2)["audioName"]){
                        audioBellsFilesModel.setProperty(index2 , "isSelect" , true)
                    }
                }

            }
        }
    }


    Connections{
        target: AduioFileManageController
        onInitSyaAudioFilesInfo:{
            console.log("init Aduio Files" , fileName)

            if(fileType === 1){  //Emg Record File
                audioEmgReordFilesModel.append({"audioName":fileName ,
                                                   "audioDuration":duration
                                               })
            }
            else if(fileType == 2){  //Emg File
                audioEmgFilesModel.append({"audioName":fileName ,
                                              "audioDuration":duration
                                          })
            }
            else if(fileType === 3){ //TalkRecord File
                audioRecordFilesModel.append({"audioName":fileName ,
                                                 "audioDuration":duration
                                             })
            }
            else if(fileType == 4){  //Music File
                audioMusicFilesModel.append({ "audioName":fileName ,
                                                "audioDuration":duration ,
                                                "audioSinger":singer ,
                                                "audioAlbum":albnum ,
                                                "isSelect":false
                                            })
            }
            else if(fileType === 5){ //Bells Files
                audioBellsFilesModel.append({"audioName":fileName ,
                                                "audioDuration":duration ,
                                                "audioSinger":singer ,
                                                "audioAlbum":albnum ,
                                                "isSelect":false
                                            })
            }
        }
    }


    function setSelectChannelIndex(channelIndex){
        selectChannelIndex = channelIndex
    }

    function userDeleteAudioFile(fileName , fileType){
        AduioFileManageController.deleteTheAudioFile(fileName , fileType)
    }

    function userTryRePlayAudioFile(isPlay ,  fileName , fileType){
        var cmd
        if(isPlay)
            cmd = "ReplayAudiodFile"
        else
            cmd = "StopReplayAudiodFile"
        AduioFileManageController.userSetAudioManageEvents(cmd , fileName , fileType)
    }


    function getFileIndex(fileName ,  fileModel){
        for(var fileIndex = 0 ; fileIndex < fileModel.count ; fileIndex++){
            //            console.log("Find Audio Name:" , fileIndex , fileName , fileModel.get(fileIndex)["audioName"])
            if(fileName === fileModel.get(fileIndex)["audioName"])
                return fileIndex
        }
        return -1
    }

    function initAudioFilesSelectState(){
        for(var fileIndex=0 ; fileIndex < audioMusicFilesModel.count ; fileIndex++){
            audioMusicFilesModel.setProperty(fileIndex , "isSelect" , false)
        }
        for(var fileIndex2=0 ; fileIndex2 < audioBellsFilesModel.count ; fileIndex2++){
            audioBellsFilesModel.setProperty(fileIndex2 , "isSelect" , false)
        }
    }
}
