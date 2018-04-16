import QtQuick 2.0

QtObject {
    id:linkeControlPageModel

    property ListModel usc4000FireModel:  ListModel{
        id:fireInfpmodel
//        ListElement{
//            hostno:1
//            loopno:2
//            addrno:3
//            devtype:4
//        }
    }

    function appendFireInfo(host , loop , addr , devtype){
        fireInfpmodel.append({"hostno":host , "loopno":loop , "addrno":addr , "devtype":devtype})
    }

    function hostReset(){
        fireInfpmodel.clear()
    }

}
