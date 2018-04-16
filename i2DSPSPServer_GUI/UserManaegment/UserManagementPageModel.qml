import QtQuick 2.0

QtObject {
    id:userManagementPageModel


    property ListModel userInfoModel: ListModel{
        ListElement{
            userID:1
            userName:"SystemAdmin"
            passwd:"hysj"
            userLv:3
        }
        ListElement{
            userID:2
            userName:"Admin"
            passwd:"hysj"
            userLv:2
        }
        ListElement{
            userID:3
            userName:"User"
            passwd:"hysj"
            userLv:1
        }
    }
}
