 import QtQuick 2.0

Item {

    signal usrBuildClicked(int btype , string bstrIndex)

    TreeListView{
        id:buildtreeview
        objectName: "objbuildtreeview"
        onUsrClicked: usrBuildClicked(type , strIndex)
    }
    TwoDMapView{
        id:twomapview
        objectName: "objtwomapview"
        width: parent.width
        height: parent.height-buildtreeview.height
//        contentWidth: twomapview.width
//        contentHeight: twomapview.height
        anchors.top: buildtreeview.bottom
    }


}
