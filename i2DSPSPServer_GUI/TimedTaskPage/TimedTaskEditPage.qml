import QtQuick 2.7
import QtQuick.Controls 2.2
import "../SysSingletonModel"

Item {
    width: 1280
    height: 300

    property QtObject curEditTimedTaskItem: QtObject{
        property int taskNo:0
        property string taskName:""
        property int channelNum:0
        property bool enable:true
        property string  audioName:""
        property string  startDay: ""
        property string  endDay:""
        property string  weekDay:"0000000"                       //Sun=0
        property string  startTime:""
        property int duration:0
        property int volume:0
        property string devicesName:""
        property var  devicesID: new Object
    }

    property QtObject initEditTimedTaskItem: QtObject{
        property int taskNo:0
        property string taskName:""
        property int channelNum:0
        property bool enable:true
        property string  audioName:""
        property string  startDay: ""
        property string  endDay:""
        property string  weekDay:"0000000"                       //Sun=0
        property string  startTime:""
        property int duration:0
        property int volume:0
        property string devicesName:""
        property var  devicesID: new Object
    }

    property QtObject listTimedTaskItem : null


    property int currentEditIndex: -1
    property int editMode : 0             //0 overView  1 modify 2 new

    function setCurrentTimedTaskItem(taskIndex ,timedTaskItem){
        console.log("Set Edit TimedTask Item" , timedTaskItem["audioName"])
        currentEditIndex = taskIndex
        listTimedTaskItem = timedTaskItem
        curEditTimedTaskItem = timedTaskItem
        clearBtnsState()
        editMode = 0
        converDevIDsStringToMap(timedTaskItem)
    }

    function userisAddorDelTimedTaskDevice(isAdd , DevcieID , deviceName){
        console.log("Timed Task Edit Page Select Devs:" , isAdd , DevcieID , deviceName)
        if(isAdd){
            curEditTimedTaskItem["devicesID"][DevcieID] = deviceName
        }
        else{
            delete curEditTimedTaskItem["devicesID"][DevcieID]
        }
    }

    function userSelectAudioFile(audioName){
        curEditTimedTaskItem["audioName"] = audioName
    }

    function getEditViewInfo(){
        curEditTimedTaskItem["taskName"]  = textInputTaskName.text
        curEditTimedTaskItem["enable"]  = !btnEnable.checked
        curEditTimedTaskItem["audioName"]  = textInputSongName.text
        curEditTimedTaskItem["startDay"]  = textInputstartDay.text
        curEditTimedTaskItem["endDay"]  = textInputendDay.text
        curEditTimedTaskItem["weekDay"]  = getWeekDayInfo()
        curEditTimedTaskItem["startTime"]  = textInputstarttime.text
        curEditTimedTaskItem["duration"]  = parseInt(textInputduration.text)
        curEditTimedTaskItem["volume"]  = parseInt(textInputVol.text)

        converDevIDsMapToString()
        //Modify The TimedTask
        if(currentEditIndex !== -1)  //Modify Time Task
            sysTimedTaskPageModel.userModifyTimedTask(currentEditIndex , curEditTimedTaskItem)
        else
            sysTimedTaskPageModel.manageTimedTaskInfoModel.append(curEditTimedTaskItem)



        //If User Edit TimedTask Already Done Next Time It Will Be Take Effect  , Not Devices
        if(currentEditIndex !== -1  && curEditTimedTaskItem["devicesID"] !== listTimedTaskItem["devicesID"]){
            converDevIDsStringToMap(curEditTimedTaskItem)
            channelTimedTaskhander.userChangedChannelTaskDevices(curEditTimedTaskItem["taskNo"] , curEditTimedTaskItem["devicesID"])
        }
        else
            converDevIDsStringToMap(curEditTimedTaskItem)


    }


    function converDevIDsMapToString(){
        var  apIDS = new Array
        var apNames = new Array
        for(var apid in curEditTimedTaskItem["devicesID"]){
            apIDS.push(apid)
            apNames.push(curEditTimedTaskItem["devicesID"][apid])
        }
        console.log(apIDS , apNames)

        curEditTimedTaskItem["devicesID"] = apIDS.join(' | ')
        curEditTimedTaskItem["devicesName"] = apNames.join(' | ')
        console.log("Conver Object To String" ,curEditTimedTaskItem["devicesID"] )
    }

    function converDevIDsStringToMap(timedTaskItem){
        var devicesID = timedTaskItem["devicesID"].split(' | ')
        var devicesName = timedTaskItem["devicesName"].split(' | ')

        curEditTimedTaskItem["devicesID"] = new Object

        for(var i = 0 ; i < devicesID.length ; i++){
            console.log(devicesID[i].trim())
            curEditTimedTaskItem["devicesID"][devicesID[i].trim()] = devicesName[i].trim()
        }

        for(var prop in curEditTimedTaskItem["devicesID"]){
            console.log("Conver DeviceIDs String To Map" , prop)
        }
    }

    function getWeekDayInfo(){
        var weekDayStr = new Array('0','0','0','0','0','0','0')

        for(var i = 0 ; i < 7 ; i++){
            if(repeater.itemAt(i).checked === true){
                weekDayStr[i] = '1'
            }
        }
        return weekDayStr.join('')
    }
    function clearBtnsState(){
        btnModeifSave.checked = false
        btnNew.checked = false
        btnEnable.checked = false

    }
    function clearEditViewInfo(){
        initEditTimedTaskItem["taskName"]  = ""
        initEditTimedTaskItem["enable"]  = true
        initEditTimedTaskItem["audioName"]  = ""
        initEditTimedTaskItem["startDay"]  = ""
        initEditTimedTaskItem["endDay"]  = ""
        initEditTimedTaskItem["weekDay"]  = "0000000"
        initEditTimedTaskItem["startTime"]  = ""
        initEditTimedTaskItem["duration"]  = 0
        initEditTimedTaskItem["volume"]  = 0
        initEditTimedTaskItem["devicesID"] = new Object
        curEditTimedTaskItem = initEditTimedTaskItem
    }




    Rectangle {
        id: rectangle
        color: Qt.rgba(1,1,1,0.5)
        radius: 10
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.fill: parent
    }

    Text {
        id: textstartTime
        x: 26
        y: 28
        width: 91
        height: 34
        text: qsTr("开始时间")
        font.bold: true
        renderType: Text.NativeRendering
        font.pixelSize: 22
    }

    TextInput {
        id: textInputstarttime
        x: 145
        y: 28
        width: 130
        height: 34
        text: curEditTimedTaskItem["startTime"]
        font.pixelSize: 22
        validator: RegExpValidator{
            regExp: new RegExp("([0-2][0-4])[:]([0-6]{2})[:]([0-6]{2})")
        }
        //        inputMask: "00:00:00"

        enabled: editMode !== 0
        Rectangle{ visible: editMode !== 0
            anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}
    }

    Text {
        id: textduration
        x: 337
        y: 28
        width: 91
        height: 34
        text: qsTr("播放时长")
        renderType: Text.NativeRendering
        font.pixelSize: 22
        font.bold: true
    }

    TextInput {
        id: textInputduration
        x: 439
        y: 28
        width: 130
        height: 34
        text: curEditTimedTaskItem["duration"]
        font.pixelSize: 22
        enabled: editMode !== 0
        validator: IntValidator{ top:360 ; bottom: 1 }
        Rectangle{ visible: editMode !== 0 ; anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}
        onTextChanged: {
            if((parseInt(textInputduration.text) <= 0 ||
                textInputduration.text === "" )&&
                    editMode !== 0 ){
                textInputduration.color = "red"
                btnModeifSave.enabled = false
            }
            else{
                textInputduration.color = "black"
                btnModeifSave.enabled = true

            }

        }
    }




    Text {
        id: texttaskName
        x: 26
        y: 81
        width: 91
        height: 34
        text: qsTr("任务名称")
        renderType: Text.NativeRendering
        font.pixelSize: 22
        font.bold: true
    }

    TextInput {
        id: textInputTaskName
        x: 145
        y: 82
        width: 424
        height: 34
        text: curEditTimedTaskItem["taskName"]
        font.pixelSize: 22
        enabled: editMode !== 0
        Rectangle{ visible: editMode !== 0 ;  anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}

    }

    Text {
        id: textstartDay
        x: 26
        y: 133
        width: 91
        height: 34
        text: qsTr("开始日期")
        renderType: Text.NativeRendering
        font.pixelSize: 22
        font.bold: true
    }

    TextInput {
        id: textInputstartDay
        x: 145
        y: 133
        width: 130
        height: 34
        text: curEditTimedTaskItem["startDay"]
        font.pixelSize: 22
        enabled: editMode !== 0
        validator: RegExpValidator{
            regExp: new RegExp("([2-3][0-9]{3})[-]([0-1][0-9])[-]([0-2][0-9]|3[0-1])")
        }
        Rectangle{ visible: editMode !== 0 ; anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}

    }

    Text {
        id: textendDay
        x: 337
        y: 133
        width: 91
        height: 34
        text: qsTr("结束日期")
        renderType: Text.NativeRendering
        font.pixelSize: 22
        font.bold: true

    }

    TextInput {
        id: textInputendDay
        x: 439
        y: 133
        width: 130
        height: 34
        text: curEditTimedTaskItem["endDay"]
        font.pixelSize: 22
        enabled: editMode !== 0
        validator: RegExpValidator{
            regExp: new RegExp("([2-3][0-9]{3})[-]([0-1][0-9])[-]([0-2][0-9]|3[0-1])")
        }
        Rectangle{ visible: editMode !== 0 ; anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}

    }

    Text {
        id: textPlayDevs
        x: 26
        y: 182
        width: 91
        height: 34
        text: qsTr("播放设备")
        renderType: Text.NativeRendering
        font.pixelSize: 22
        font.bold: true
    }

    Text {
        id: broadcastDvicesName
        x: 145
        y: 182
        width: 500
        height: 34
        text: curEditTimedTaskItem["devicesName"]
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 20
        enabled: editMode !== 0
        clip: true
        Rectangle{ visible: editMode !== 0 ;  anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}
        MouseArea{
            anchors.fill: parent
            enabled: editMode !== 0
            onClicked: {
                sysUIPubicFunction.showSelectDevicesDialog(UISysModel.enumAPTimedTask , UISysModel.sysTimedTaskChannel , curEditTimedTaskItem["devicesID"])
            }
        }
    }

    Text {
        id: startDay2
        x: 26
        y: 237
        width: 91
        height: 34
        text: qsTr("工 作 日")
        horizontalAlignment: Text.AlignHCenter
        renderType: Text.NativeRendering
        font.pixelSize: 22
        font.bold: true
    }

    Row {
        id: row
        x: 145
        y: 237
        width: 424
        height: 34
        spacing: 10
        Repeater{
            id: repeater
            model: 7
            CheckBox {
                id: checkBox
                text: qsTr("周"+sysTimedTaskPageModel.getStrWeekDayName(index))
                anchors.verticalCenter: parent.verticalCenter
                enabled: editMode !== 0
                checked: {
                    if(curEditTimedTaskItem["weekDay"][index] === '1')
                        return true
                    else
                        return false
                }
            }


        }
    }

    Button {
        id: btnModeifSave
        x: 1032
        y: 28
        width: 80
        height: 34
        text: btnModeifSave.checked ?  "保存" : qsTr("修改")
        checkable: true
        onClicked: {
            if(btnModeifSave.checked)
                editMode = 1
            else{
                editMode = 0
                getEditViewInfo()
            }
        }
        visible: editMode !== 2 && currentEditIndex !== -1
    }

    Button {
        id: btnEnable
        x: 1137
        y: 28
        width: 80
        height: 34
        checked: !curEditTimedTaskItem["enable"]
        text: !curEditTimedTaskItem["enable"] ? qsTr("启用") : "停用"
        checkable: true
        onClicked: {
            curEditTimedTaskItem["enable"] = !btnEnable.checked
            sysTimedTaskPageModel.manageTimedTaskInfoModel.setProperty(currentEditIndex , "enable" ,!btnEnable.checked )
        }
    }

    Button {
        id: btnNew
        x: 1032
        y: 82
        width: 80
        height: 33
        checkable: true
        text: btnNew.checked ?  "保存"  : qsTr("新增")
        onClicked: {
            if(btnNew.checked){
                editMode = 2
                clearEditViewInfo()
                currentEditIndex = -1
            }
            else{
                editMode = 0
                getEditViewInfo()
                listView.currentIndex = sysTimedTaskPageModel.manageTimedTaskInfoModel.count-1
                setCurrentTimedTaskItem(sysTimedTaskPageModel.manageTimedTaskInfoModel.count-1 , curEditTimedTaskItem)
            }
        }
        visible: editMode !== 1
    }

    Button {
        id: btnDelete
        x: 1137
        y: 82
        width: 80
        height: 33
        text: qsTr("删除")
        onClicked: {
            curEditTimedTaskItem = initEditTimedTaskItem
            sysTimedTaskPageModel.manageTimedTaskInfoModel.remove(currentEditIndex)
            currentEditIndex = -1


        }
        visible: currentEditIndex !== -1 && editMode != 2
    }

    Button {
        id: btnUnsave
        x: 1032
        y: 134
        width: 80
        height: 33
        text: qsTr("取消保存")
        visible: editMode !== 0
        onClicked: {
            editMode = 0
            clearBtnsState()

            curEditTimedTaskItem = initEditTimedTaskItem
            curEditTimedTaskItem = listTimedTaskItem
        }
    }

    Button {
        id: btnDeleteAll
        x: 1137
        y: 134
        width: 80
        height: 33
        text: qsTr("删除全部")
        onClicked: {
            curEditTimedTaskItem = initEditTimedTaskItem
            sysTimedTaskPageModel.manageTimedTaskInfoModel.clear()
            currentEditIndex = -1

        }
        visible: currentEditIndex !== -1 && editMode != 2
    }

    Text {
        id: textSongNameLabel
        x: 595
        y: 28
        width: 91
        height: 34
        text: qsTr("歌曲名称")
        font.pixelSize: 22
        renderType: Text.NativeRendering
        font.bold: true

    }

    Text {
        id: textInputSongName
        x: 697
        y: 28
        width: 315
        height: 34
        text: curEditTimedTaskItem["audioName"]
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 22
        enabled: editMode !== 0
        Rectangle{ visible: editMode !== 0 ;  anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}

        MouseArea{
            anchors.fill: parent
            onClicked: {
                sysUIPubicFunction.showSelectAudioFilesDialog("single" , curEditTimedTaskItem["audioName"] , -1)
            }
        }
    }

    Text {
        id: textPlayVol
        x: 595
        y: 82
        width: 91
        height: 34
        text: qsTr("播放音量")
        font.pixelSize: 22
        renderType: Text.NativeRendering
        font.bold: true
    }

    TextInput {
        id: textInputVol
        x: 697
        y: 82
        width: 50
        height: 34
        text: curEditTimedTaskItem["volume"]
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 22
        enabled: editMode !== 0
        validator: IntValidator{ top:100 ; bottom: 0 }
        Rectangle{ visible: editMode !== 0 ;  anchors.bottom: parent.bottom; anchors.bottomMargin: 0 ;width:parent.width ;height: 1; color: "black"}

        onTextChanged: {
            if((parseInt(textInputVol.text) <= 0 ||
                textInputVol.text === "" )&&
                    editMode !== 0 ){
                textInputVol.color = "red"
                btnModeifSave.enabled = false
            }
            else{
                textInputVol.color = "black"
                btnModeifSave.enabled = true

            }

        }
    }



}
