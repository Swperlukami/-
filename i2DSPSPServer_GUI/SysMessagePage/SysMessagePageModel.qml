import QtQuick 2.0

Item {
    id:messagePageModel

    function appendMessage(msg){
       messageModel.insert(0 , {"msg":msg})

        if(messageModel.count > 100){
            messageModel.remove(10 ,90 )
        }
    }

    property ListModel messageModel: ListModel{
//        ListElement{
//            msg: "show msg"
//        }
    }


}
