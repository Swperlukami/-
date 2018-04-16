import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    width: 1280
    height: 40

    Rectangle {
        id: rectangle
        color: Qt.rgba(1,1,1,0.5)
        anchors.fill: parent
        radius: 5

        Row {
            id: row
            anchors.fill: parent
        }

        Text {
            id: txtAudioName
            y: 13
            width: 526
            height: 26
            text: audioName
            anchors.left: txtIndex.right
            anchors.leftMargin: 20
            anchors.verticalCenterOffset: 0
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 22
        }

        Text {
            id: txtIndex
            y: 13
            text: (index+1)
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }

        RoundButton {
            id: roundButtonTry
            x: 1093
            y: 8
            width: 80
            height: 25
            checkable: true
            text: roundButtonTry.checked && sysAudioFilesModel.curReplayFileName === txtAudioName.text? "停止" : "试听"
            anchors.right: parent.right
            anchors.rightMargin: 20
            checked: sysAudioFilesModel.curReplayFileName === txtAudioName.text  ? true : false
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                if(roundButtonTry.checked)
                    sysAudioFilesModel.curReplayFileName = txtAudioName.text
                else
                    sysAudioFilesModel.curReplayFileName = ""

                sysAudioFilesModel.userTryRePlayAudioFile(roundButtonTry.checked , txtAudioName.text , tabBar.currentIndex)
            }
        }

        Text {
            id: txtAudioDuration
            x: -7
            y: 11
            width: 213
            height: 26
            text: audioDuration
            anchors.left: txtAudioName.right
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 150
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 22
            anchors.verticalCenterOffset: 0
            horizontalAlignment: Text.AlignHCenter
            visible: tabBar.currentIndex !==0 && tabBar.currentIndex !==2
        }

        RoundButton {
            id: roundButtonDel
            x: 1096
            y: 2
            width: 80
            height: 25
            text: "删除"
            anchors.right: roundButtonTry.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 30
            onClicked: {
                listView.model.remove(index)
                sysAudioFilesModel.userDeleteAudioFile(txtAudioName.text , tabBar.currentIndex)
            }
        }
    }
}
