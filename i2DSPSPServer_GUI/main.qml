import QtQuick 2.8
import QtQuick.Window 2.2

import "LaunchPage"
import "SysSingletonModel"
import "MusicPage"

Window {
    id:rootwindow
    objectName: "objrootwindow"
    width: UISysModel.sysScreenWidth
    height: UISysModel.sysScreenHeigth
    title: qsTr("恒业世纪")
    visible: true
//    visibility: Window.FullScreen

    Image {
        id: bkrect
        source: "images/BKMountain.jpg"
    }

    LaunchPage{
        id:syslaunchpagepage
    }

}
