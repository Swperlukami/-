import QtQuick 2.0
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 800
    height: 400
    visible: false
    Rectangle{
        anchors.fill: parent
        color: "grey"
        visible: true
        radius: 10
        opacity: 0.95
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            item1.visible = false
        }
    }

    Image {
        id: image
        width: 260*0.6
        height: 238*0.6
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/images/HYS(1).png"
    }

    Text {
        id: sysName
        x: 189
        y: 37
        color: "#ffffff"
        text: qsTr("应急公共广播系统")
        font.weight: Font.DemiBold
        style: Text.Normal
        font.strikeout: false
        font.bold: true
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 26
    }

    GridLayout {
        x: 246
        y: 270
        width: 400
        height: 113
        anchors.horizontalCenterOffset: 30
        anchors.horizontalCenter: parent.horizontalCenter
        columnSpacing: 3
        rowSpacing: 5
        rows: 4
        columns: 2


        Text {
            id: text1
            color: "#ffffff"
            text: qsTr("软件版本:")
            font.pixelSize: 12
        }

        Text {
            id: version
            color: "#ffffff"
            text: qsTr("1.0.0_201711_Ubuntu_16.04_x86_gcc demo")
            Layout.preferredHeight: 15
            Layout.preferredWidth: 252
            font.pixelSize: 12
        }

        Text {
            id: text2
            color: "#ffffff"
            text: qsTr("版权所有:")
            font.pixelSize: 12
        }


        Text {
            id: copyright
            color: "#ffffff"
            text: qsTr("恒业世纪安全技术有限公司 2017-")
            Layout.preferredHeight: 15
            Layout.preferredWidth: 227
            font.pixelSize: 12
        }

        Text {
            id: text4
            color: "#ffffff"
            text: qsTr("执行标准:")
            font.pixelSize: 12
        }



        Text {
            id: describe1
            color: "#ffffff"
            text: qsTr("GB 16806-2006")
            Layout.preferredHeight: 15
            Layout.preferredWidth: 188
            font.pixelSize: 12
        }

        Text {
            id: text3
            color: "#ffffff"
            text: qsTr("系统描述:")
            font.pixelSize: 12
        }


        Text {
            id: describe
            color: "#ffffff"
            text: qsTr("线路、设备检测，多分区应急广播系统")
            Layout.preferredHeight: 15
            Layout.preferredWidth: 188
            font.pixelSize: 12
        }

    }

}
