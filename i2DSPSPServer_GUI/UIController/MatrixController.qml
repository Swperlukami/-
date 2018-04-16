import QtQuick 2.0

Item {
    id:qmlMatrixController

    function userSetMatrixEvents(cmdType , channelNum , songName , vol){
        CPMatrixController.userSetMatrixEvent(cmdType , channelNum , songName , vol)
    }
}
