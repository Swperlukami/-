import QtQuick 2.0
import QtQuick.Controls 2.2

Dialog {
    id:audioFilesSelectPage
    width: 600
    height: 400
    topPadding: 0
    rightPadding:0
    leftPadding:0
    background: Rectangle{
        id: rectangle
        anchors.fill: parent
        color: Qt.rgba(1,1,1,1.6)
        radius: 5
        clip: true
    }

    x:(rootwindow.width-audioFilesSelectPage.width)/2
    y:(rootwindow.height-audioFilesSelectPage.height)/2-100

    TabBar {
        id: tabBar
        x: 0
        y: -5
        width: parent.width
        height: 30

        TabButton {
            id: tabButtonMusicFIles
            height: parent.height
            x: 36
            y: 0
            text: qsTr("音乐文件")
        }

        TabButton {
            id: tabButtonBells
            height: parent.height
            x: 246
            y: 0
            text: qsTr("铃声文件")
        }
    }

    ListView {
        id: listView
        x: 51
        width: parent.width
        height: 360
        spacing: 1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: tabBar.bottom
        anchors.topMargin: 0
        delegate: AudioFileSelectItemDelegate{
            width: listView.width
        }
        model: {
            if(tabBar.currentIndex === 0)
                sysAudioFilesModel.audioMusicFilesModel
            else
                sysAudioFilesModel.audioBellsFilesModel
        }
    }



    header: Rectangle {
        id: rectangleheaderDel
        width: 600
        height: 30
        radius: 5
        color: "grey"
        Rectangle{
            width: parent.width
            height: parent.height/2
            anchors.bottom: parent.bottom
            color: parent.color
        }
        Text{
            color: "#ffffff"
            text: "请选择需要使用的音频文件"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 15
            anchors.fill: parent
        }
    }

    footer: Rectangle {
                id: rectangleFooterDel
                width: 600
                height: 40
                radius: 5
                color: "grey"
                Rectangle{
                    width: parent.width
                    height: parent.height/2
                    anchors.top: parent.top
                    color: parent.color
                }
                Text{
                    color: "#ffffff"
                    text: "退出"
                    font.pointSize: 22
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent

                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        audioFilesSelectPage.visible = false
                    }
                }
    }


    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 ; duration: 250 }
    }
    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 ; duration: 250 }
    }

//    Component.onCompleted: audioFilesSelectPage.visible = true

}
