import QtQuick 2.0

Rectangle {
    id:songItemdelegate
    width: 1220
    height: 35
    color: index%2 === 0 ? Qt.rgba(0,0,0,0.4) : Qt.rgba(1,0.2,1,0.1)

    property int channelIndex : 0

    MouseArea{
        anchors.fill: parent
        onClicked: {
            songslistView.currentIndex = index
        }
        onDoubleClicked: {
            console.log("SongItem ChannelIndex:" ,channelIndex , "songIndex:" ,index)
            sysMusicPage.userControlPlayers(channelIndex ,"Play" , index)
            albumItemDel.channelState = sysChannelPlayers.channelPlayers[channelIndex]["curSate"]
        }
    }

    Text {
        id: txtsongIndex
        y: 12
        width: 20
        height: 35
        text: (index+1)
        anchors.left: parent.left
        anchors.leftMargin: 0
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 12
        color: isPlay ? "green" : ""

    }

    Row {
        id: row
        y: 8
        width: 1200
        height: 35
        anchors.left: txtsongIndex.right
        anchors.leftMargin: 0
        anchors.verticalCenter: parent.verticalCenter

        Text {
            id: txtSongName
            x: 159
            y: 10
            width: songItemdelegate.width*0.4-20
            height: songItemdelegate.height
            text: qsTr(songName)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
            color: isPlay ? "green" : ""

        }

        Text {
            id: txtSongSinger
            x: 159
            y: 10
            width: songItemdelegate.width*0.2
            height: songItemdelegate.height
            text: qsTr(singer)
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
            color: isPlay ? "green" : ""

        }

        Text {
            id: txtSongAlbum
            x: 159
            y: 10
            width: songItemdelegate.width*0.2
            height: songItemdelegate.height
            text: qsTr(album)
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
            color: isPlay ? "green" : ""

        }

        Text {
            id: txtSongDuration
            x: 159
            y: 10
            width: songItemdelegate.width*0.1
            height: songItemdelegate.height
            text: duration
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
            color: isPlay ? "green" : ""
        }
    }
}
