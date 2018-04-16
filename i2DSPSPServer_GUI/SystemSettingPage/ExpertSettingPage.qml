import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id:expertSettingPage
    width: 1280
    height: 760

    Text {
        id: text1
        x: 46
        y: 43
        width: 221
        height: 40
        text: qsTr("应急话筒录音保存天数")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 22
    }

    SpinBox {
        id: spinBox
        x: 309
        y: 43
        to: 365
        from: 5
        value: sysSystemSettingModel.emgAudioRecordSaveDays
    }

    Text {
        id: text2
        x: 46
        y: 100
        width: 221
        height: 40
        text: qsTr("系统日志保存天数")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 22
    }

    SpinBox {
        id: spinBox1
        x: 309
        y: 100
        from: 5
        value: sysSystemSettingModel.sysRecordSaveDays
        to: 365
    }

}
