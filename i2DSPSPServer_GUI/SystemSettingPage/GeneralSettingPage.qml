import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0

Item {
    id:genralSettingPage
    width: 1280
    height: 760

    Text {
        id: text1
        x: 47
        y: 47
        width: 86
        height: 32
        text: qsTr("自动登录")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 22
    }

    Switch {
        id: switchAutoLogin
        x: 451
        y: 43
        width: 136
        height: 25
        checked: sysSystemSettingModel.isAutoLogin ? false : true
        text: sysSystemSettingModel.isAutoLogin ?  qsTr("开启") : qsTr("关闭")
        font.pixelSize: 20
    }

    Text {
        id: text2
        x: 47
        y: 101
        width: 223
        height: 32
        text: qsTr("业务广播录音保存天数")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 22
    }

    SpinBox {
        id: spinBox
        x: 447
        y: 97
        width: 140
        height: 25
        font.pointSize: 11
        from: 5
        value: sysSystemSettingModel.audioRecordSaveDays
        to: 60
        editable: true
    }

    Item {
        id: testItem
        x: 47
        y: 246
        width: 1200
        height: 400

        TextInput {
            id: textInputSongName
            x: 0
            y: 0
            width: 255
            height: 40
            text: qsTr("SongName")
            font.pixelSize: 12
        }

        Button {
            id: buttonVolAdd
            x: 514
            y: 0
            text: qsTr("Vol+")
            onClicked: {
                sysMatrixController.userSetMatrixEvents("Vol" , parseInt(textInputChannel.text) , "" , parseInt(textInputVol.text))
            }
        }

        Button {
            id: buttonPlay
            x: 273
            y: 0
            text: qsTr("Play")
            onClicked: {
                sysMatrixController.userSetMatrixEvents("Play" , parseInt(textInputChannel.text) , textInputSongName.text , parseInt(textInputVol.text))
            }
        }

        Button {
            id: buttonVolInc
            x: 641
            y: 0
            text: qsTr("Vol-")
        }

        Button {
            id: buttonStop
            x: 393
            y: 0
            text: qsTr("Stop")
            onClicked: {
                sysMatrixController.userSetMatrixEvents("Pause" , parseInt(textInputChannel.text) , "" , parseInt(textInputVol.text))
            }
        }

        TextInput {
            id: textInputChannel
            x: 0
            y: 59
            width: 255
            height: 40
            text: qsTr("Channel")
            font.pixelSize: 12
        }

        TextInput {
            id: textInputVol
            x: 514
            y: 90
            width: 255
            height: 40
            text: qsTr("Vol")
            font.pixelSize: 12
        }

        Button {
            id: button
            x: 0
            y: 181
            width: 255
            height: 40
            text: qsTr("SynAudioFiles")
            onClicked: {
                if(CPMatrixController.syncMatrixAudioFiles()){
                    console.log("Syn Audio Files OK")
                }
            }
        }

        Button {
            id: buttonMountDisk
            x: 308
            y: 181
            text: qsTr("MountDisk")
            onClicked: {
                sysMatrixController.userSetMatrixEvents("Mount" , 0 , "" , 0)
            }
        }

        Button {
            id: buttonMountUnDisk
            x: 434
            y: 181
            text: qsTr("UnMountDisk")
            onClicked: {
                sysMatrixController.userSetMatrixEvents("Unmount" , 0 , "" , 0)
            }
        }
    }












}
