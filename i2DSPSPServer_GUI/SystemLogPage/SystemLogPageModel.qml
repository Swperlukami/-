import QtQuick 2.0

QtObject {
    id:logPageModel

    function appendNewLogInfo(devType , error , addr){
        devErrorLogModel.insert( 0 , {"errorDevType":devType ,
                                  "errorStr":error ,
                                  "errorAddr":addr
                                })
        listview.currentIndex = 0

        if(devErrorLogModel.count > 200){
            devErrorLogModel.remove(50 ,150)
        }
    }

    function useClearRecord(){
        devErrorLogModel.clear()
    }

    property ListModel devErrorLogModel: ListModel{
//        ListElement{
//            errorDevType:7
//            errorStr:"Open"
//            errorAddr:"Test Addr "
//        }
//        ListElement{
//            errorDevType:1
//            errorStr:"Open"
//            errorAddr:"Test Addr "
//        }
    }

}
