pragma Singleton
import QtQuick 2.0


QtObject {
    id:sysUIModel

    //system state :
    //Idel SelfTest Emg EmgTalk Talk
    property string sysState: "Idel"
    property string sysMode: "Auto"    //Manual

    property string usrname : null
    property string passwd: null
    property int usrlv: null

}
