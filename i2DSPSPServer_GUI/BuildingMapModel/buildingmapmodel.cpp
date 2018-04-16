#include "buildingmapmodel.h"
#include <QApplication>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomAttr>
#include <QDebug>

BuildingMapModel::BuildingMapModel(QObject *parent) :
    QObject(parent)
{
    setBuildingMapInfoFilePath("./BuildingMap/buildinginfo.xml");
    initQuickItem();
}
void BuildingMapModel::initQuickItem()
{
    const MapModelList *DisList = getDistrickModel();

    initTwoMapImage(m_CurrentDistrcik , m_CurrentBuild , m_CurrentFloor);

//    mp_RootContext->setContextProperty(m_DisModelName , const_cast<MapModelList*>(DisList));
//    mp_RootContext->setContextProperty(m_BuildModelName , const_cast<MapModelList*>(getBuildingModel(m_CurrentDistrcik)));
//    mp_RootContext->setContextProperty(m_FloorModelName , const_cast<MapModelList*>(getFloorModel(m_CurrentDistrcik,m_CurrentBuild)));

//    qDebug()<<QApplication::applicationDirPath();
//    qDebug()<<QApplication::applicationFilePath();

    
    pTwoMapView = parent()->findChild<QQuickItem*>("objtwomapview");
    if(pTwoMapView == NULL){
        qDebug()<<"Get Qml TwoMapView QQucikItem Error !";
    }

    pTwoMapView->setProperty("imagename" , "file://"+QApplication::applicationDirPath()+
                             "/BuildingMap/2DMap/"+m_CurrentDistrcik+m_CurrentBuild+m_CurrentFloor+".png");

    pDisListView = parent()->findChild<QQuickItem*>("objdislistview");
    if(pDisListView == NULL){
        qDebug()<<"Get Qml DistrickListView QQuickItem Error !!";
    }
    pDisListView->setProperty("model" ,QVariant::fromValue((MapModelList*)DisList));


    pBuildListView = parent()->findChild<QQuickItem*>("objbuildlistview");
    if(pBuildListView == NULL){
        qDebug()<<"Get Qml BuildListView QQuickItem Error !!";
    }
    pBuildListView->setProperty("model" ,QVariant::fromValue((MapModelList*)getBuildingModel(m_CurrentDistrcik)));

    pFloorListView = parent()->findChild<QQuickItem*>("objfloorlistview");
    if(pBuildListView == NULL){
        qDebug()<<"Get Qml BuildFloorView QQuickItem Error !!";
    }
    pFloorListView->setProperty("model" ,QVariant::fromValue((MapModelList*)getFloorModel(m_CurrentDistrcik,m_CurrentBuild)));

}

void BuildingMapModel::userClickedDistrickItem(int Type , QVariant Number)
{
   //1表示区域 ， 2表示建筑 ，3表示楼层。若层次结构有变化，此处可继续添加。

//    qDebug()<<Number.toString();

    if(Number.toString().isEmpty())
        return;

    if(Number == m_CurrentDistrcik || Number == m_CurrentBuild || Number == m_CurrentFloor)
        return;


    if(Type == 1){
        const MapModelList *BuildList = getBuildingModel(Number.toString());
        if(BuildList == NULL)
            return;
        m_CurrentDistrcik = Number.toString();
        const MapModelList *BuildInfoList = getBuildingModel(Number.toString());
        pBuildListView->setProperty("model" ,QVariant::fromValue((MapModelList*)BuildInfoList));

        QString BuildNumber = BuildInfoList->data(createIndex(0 , 0 , NULL) , MapModelList::RoleName_Number).toString();
        pFloorListView->setProperty("model" ,QVariant::fromValue((MapModelList*)getFloorModel("" , BuildNumber)));
        m_CurrentBuild = BuildNumber;
        m_CurrentFloor = getBuildInfoByBuildNumber(m_CurrentBuild)->StartNumber;
    }
    else if(Type == 2){
        const MapModelList *FloorList = getFloorModel("",Number.toString());
        if(FloorList == NULL)
            return;
        m_CurrentBuild = Number.toString();
        pFloorListView->setProperty("model" ,QVariant::fromValue((MapModelList*)getFloorModel("",Number.toString())));
        m_CurrentFloor = getBuildInfoByBuildNumber(m_CurrentBuild)->StartNumber;

    }
    else if(Type == 3){
        m_CurrentFloor = Number.toString();
        qDebug()<<"Set Dis:"<<m_CurrentDistrcik<<"Build:"<<m_CurrentBuild<<"Floor:"<<m_CurrentFloor;
    }

    //切换建筑地图
    pTwoMapView->setProperty("imagename" , "file://"+QApplication::applicationDirPath()+
                             "/BuildingMap/2DMap/"+m_CurrentDistrcik+m_CurrentBuild+m_CurrentFloor+".png");

}
void BuildingMapModel::setBuildingMapInfoFilePath(QString filePath)
{
    if(filePath.isEmpty())
        return;

    QFile xmlFile(filePath);
    if(!xmlFile.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(NULL,QString("title"),QString("xml file open error !"));
        return;
    }
    //    QString text = xmlFile.readAll();

    QDomDocument xmlDoc;
    QString errStr;
    int errLine=0;
    int errColumn=0;
    if(!xmlDoc.setContent(&xmlFile,&errStr , &errLine , &errColumn)){
        qDebug()<<"set xml content Error:"<<errStr<<"error line:"<<errLine<<"error column:"<<errColumn;
        return;
    }

    QDomElement rootElement = xmlDoc.documentElement();
    if(rootElement.tagName() != "mcbuildings"){
        qDebug()<<"Error :Not a mcbuildings xml file";
        return;
    }

    QDomNode distraickNode = rootElement.firstChild();
    qDebug()<<distraickNode.nodeName();
    QDomNodeList distraicksList = rootElement.childNodes();

    if(distraicksList.isEmpty())
        return;

    for(int disNum = 0 ; disNum < distraicksList.count() ; disNum++){
        QDomElement distraickElement = distraicksList.at(disNum).toElement();
        QString disName , disNumber , disBuildingNumber = "";
        bool isEmg = false;
        bool isSkipGrade = false;
        int numEmg = 0;
        int numDevices = 100 ;

        if(distraickElement.hasAttribute("dname"))
            disName = distraickElement.attributeNode("dname").value();
//        qDebug()<<distraickElement.attributeNode("dname").value();
        if(distraickElement.hasAttribute("dnumber"))
            disNumber = distraickElement.attributeNode("dnumber").value();
//        qDebug()<<distraickElement.attributeNode("dnumber").value();
        if(distraickElement.hasAttribute("dbnumber"))
            disBuildingNumber = distraickElement.attributeNode("dbnumber").value();
//        qDebug()<<distraickElement.attributeNode("dbnumber").value();

        if(disName.isEmpty() || disNumber.isEmpty() || disBuildingNumber.isEmpty()){
            QMessageBox::information(NULL,QString("title"),QString("xml file get Districl information error !"));
        }

        if(disBuildingNumber.toInt() <= 1)
            isSkipGrade = true ;

        MapListItem DistricktInfoItem(isEmg , isSkipGrade , disName , disNumber , numEmg , numDevices);
        m_DistrickModelList.addMapModelItem(DistricktInfoItem);
        m_DisIndex.insert(disNumber , disNum);

        QDomNodeList buildingList = distraickElement.childNodes();

        MapModelList *perDisBuildingList = new MapModelList();

        for(int buildNum = 0 ; buildNum < buildingList.count() ; buildNum++){
            QDomElement buildingElement = buildingList.at(buildNum).toElement();
            QDomNodeList buildInfoList = buildingElement.childNodes();

            struct BuildingInfo *buildInfo = new BuildingInfo[1];
            buildInfo->DisNumber = disNumber;
            buildInfo->ListIndex = buildNum;
            buildInfo->StartNumber = "F1";            //默认从一楼开始

            QString buildName , buildNumber , buildFloorNumber = "";
            isEmg = false ;
            isSkipGrade = false ;
            numEmg = 0 ;
            numDevices = 50;
            int startFloorNumber = 1;

            for(int perBuildInfoNum = 0 ; perBuildInfoNum < buildInfoList.count() ; perBuildInfoNum++){
                QDomElement buildInfoElement = buildInfoList.at(perBuildInfoNum).toElement();
                if(buildInfoList.at(perBuildInfoNum).nodeName() == "name"){
                    buildName = buildInfoElement.text();
                    buildInfo->Name = buildName;
                }
                if(buildInfoList.at(perBuildInfoNum).nodeName() == "bnumber"){
                    buildNumber = buildInfoElement.text();
                    buildInfo->Number = buildNumber;
                }
                if(buildInfoList.at(perBuildInfoNum).nodeName() == "snumber"){
                    startFloorNumber = buildInfoElement.text().toInt();
                    buildInfo->StartNumber = "F"+QString::number(startFloorNumber);
                }

                if(buildInfoList.at(perBuildInfoNum).nodeName() == "fnumber")
                {
                    buildFloorNumber = buildInfoElement.text();

                    MapModelList *perBuildFloorList = new MapModelList();

                    if(buildFloorNumber.toInt() > 0 ){
                        for(int floorNum=0 ; floorNum < buildFloorNumber.toInt() ; floorNum++){
                            QString floorName , floorNumber = "";
                            isEmg = false ;
                            isSkipGrade = false ;
                            numEmg = 0 ;
                            numDevices = 10;

                            floorName = QString("%1F").arg(startFloorNumber);
                            floorNumber = QString("F%1").arg(startFloorNumber);

                            startFloorNumber++;
                            //没有0层，当从负楼层开始的地面上为1层
                            if(startFloorNumber == 0)
                                startFloorNumber++;

                            MapListItem floorInfoItem(isEmg , false , floorName , floorNumber , numEmg , numDevices);
                            perBuildFloorList->addMapModelItem(floorInfoItem);
                        }
                    }
                    else{
                        QMessageBox::information(NULL,QString("title"),QString("xml file building:%1 Floor Number:%2  error !").arg(buildName).arg(buildFloorNumber));
                    }
                    m_FloorInfoList.insert(buildNumber , perBuildFloorList);
                }

//                qDebug()<<buildInfoList.at(perBuildInfoNum).nodeName()<<buildInfoElement.text();
            }

            m_PerBuilkdInfoList.insert(buildNumber , buildInfo);
            m_BuildIndex.insert(buildNumber , buildNum);     //记录楼号与列表索引关系
            m_DisBuildNumberList.insert(buildNumber , disNumber);  //记录楼号与区号的对应关系
            MapListItem buildInfoItem(isEmg , isSkipGrade , buildName , buildNumber , numEmg , numDevices);
            perDisBuildingList->addMapModelItem(buildInfoItem);
        }
        m_BuildingInfoList.insert(disNumber , perDisBuildingList);
    }


}
const MapModelList *BuildingMapModel::getDistrickModel()
{
    return &m_DistrickModelList;
}
const MapModelList* BuildingMapModel::getBuildingModel(QString DistrickNum)
{
    if(!m_BuildingInfoList.contains(DistrickNum))
        return NULL;

    return m_BuildingInfoList.value(DistrickNum);
}
const MapModelList* BuildingMapModel::getFloorModel(QString DistrickNum, QString BuildingNum)
{
    Q_UNUSED(DistrickNum);

    if(!m_FloorInfoList.contains(BuildingNum))
        return NULL;

    return m_FloorInfoList.value(BuildingNum);
}
void BuildingMapModel::setNumInfoDevices(QMap<QString, int> Districk, QMap<QString, int> Building, QMap<QString, int> Floor)
{

    for(int NumDis = 0 ; NumDis < m_DistrickModelList.rowCount() ; NumDis++){
        QModelIndex DisIndex = m_DistrickModelList.createIndex(NumDis , 0 , NULL);
        QString DisNumber = m_DistrickModelList.data(DisIndex , MapModelList::RoleName_Number).toString();
        m_DistrickModelList.setData(DisIndex , Districk.value(DisNumber) , MapModelList::RoleName_NumDevices);

        MapModelList *BuildList = m_BuildingInfoList.value(DisNumber);
        for(int NumBuild = 0 ; NumBuild < BuildList->rowCount() ; NumBuild++){
            QModelIndex NumIndex = m_DistrickModelList.createIndex(NumBuild , 0 , NULL);
            QString BuildNumber = BuildList->data(NumIndex , MapModelList::RoleName_Number).toString();
            BuildList->setData(NumIndex , Building.value(BuildNumber) , MapModelList::RoleName_NumDevices);

            MapModelList *FloorList = m_FloorInfoList.value(BuildNumber);
            for(int NumFloor = 0 ; NumFloor < FloorList->rowCount() ; NumFloor++){
                QModelIndex NumIndex = m_DistrickModelList.createIndex(NumFloor , 0 , NULL);
                QString FloorNumber = FloorList->data(NumIndex , MapModelList::RoleName_Number).toString();
                FloorList->setData(NumIndex , Floor.value(BuildNumber+"+"+FloorNumber) , MapModelList::RoleName_NumDevices);
            }
        }
    }

}
void BuildingMapModel::initTwoMapImage(QString &Dis, QString &Build, QString &Floor)
{
    QModelIndex index = createIndex(0 , 0 , NULL);
    Dis = m_DistrickModelList.data(index , MapModelList::RoleName_Number).toString();
    Build = m_BuildingInfoList.value(Dis)->data(index , MapModelList::RoleName_Number).toString();
    Floor = m_FloorInfoList.value(Build)->data(index , MapModelList::RoleName_Number).toString();
}

void BuildingMapModel::setEmgDeviceNum(QString DistrickNum, QString BuildingNum, QString FloorNum, int numEmg)
{

    Q_UNUSED(numEmg);

    QModelIndex DisIndex = m_DistrickModelList.createIndex(m_DisIndex.value(DistrickNum) , 0 , NULL);
    m_DistrickModelList.setData(DisIndex , true , MapModelList::RoleName_IsEmg);
    m_DistrickModelList.setData(DisIndex , m_DistrickModelList.data(DisIndex , MapModelList::RoleName_NumEmg).toInt()+1 , MapModelList::RoleName_NumEmg);

    QModelIndex BuildIndex = m_DistrickModelList.createIndex(m_BuildIndex.value(BuildingNum) , 0 , NULL);
    m_BuildingInfoList.value(DistrickNum)->setData(BuildIndex , true , MapModelList::RoleName_IsEmg);
    m_BuildingInfoList.value(DistrickNum)->setData(BuildIndex , m_BuildingInfoList.value(DistrickNum)->data(BuildIndex , MapModelList::RoleName_NumEmg).toInt()+1 ,
                                                   MapModelList::RoleName_NumEmg);

    QModelIndex FuildIndex = m_DistrickModelList.createIndex(getDisBuildFloorIndex(BMapModelType::Model_Floor ,FloorNum , BuildingNum) , 0 , NULL);
    m_FloorInfoList.value(BuildingNum)->setData(FuildIndex , true , MapModelList::RoleName_IsEmg);
    m_FloorInfoList.value(BuildingNum)->setData(FuildIndex , m_FloorInfoList.value(BuildingNum)->data(FuildIndex , MapModelList::RoleName_NumEmg).toInt()+1 ,
                                                   MapModelList::RoleName_NumEmg);

}
void BuildingMapModel::setCancelEmgDeviceNum(QString DistrickNum , QString BuildingNum , QString FloorNum , int NumEmg)
{
    Q_UNUSED(NumEmg);

    QModelIndex DisIndex = m_DistrickModelList.createIndex(m_DisIndex.value(DistrickNum) , 0 , NULL);
    int DistrickEmgCnt  =   m_DistrickModelList.data(DisIndex , MapModelList::RoleName_NumEmg).toInt();
    if(--DistrickEmgCnt <= 0){
        m_DistrickModelList.setData(DisIndex , false , MapModelList::RoleName_IsEmg);
    }
    m_DistrickModelList.setData(DisIndex , m_DistrickModelList.data(DisIndex , MapModelList::RoleName_NumEmg).toInt()-1 , MapModelList::RoleName_NumEmg);

    QModelIndex BuildIndex = m_DistrickModelList.createIndex(m_BuildIndex.value(BuildingNum) , 0 , NULL);
    int BuildEmgCnt = m_BuildingInfoList.value(DistrickNum)->data(BuildIndex , MapModelList::RoleName_NumEmg).toInt();
    if(--BuildEmgCnt <= 0){
        m_BuildingInfoList.value(DistrickNum)->setData(BuildIndex , false , MapModelList::RoleName_IsEmg);
    }
    m_BuildingInfoList.value(DistrickNum)->setData(BuildIndex , m_BuildingInfoList.value(DistrickNum)->data(BuildIndex , MapModelList::RoleName_NumEmg).toInt()-1 ,
                                                   MapModelList::RoleName_NumEmg);

    QModelIndex FuildIndex = m_DistrickModelList.createIndex(getDisBuildFloorIndex(BMapModelType::Model_Floor ,FloorNum , BuildingNum) , 0 , NULL);
    int FloorEmgCnt = m_FloorInfoList.value(BuildingNum)->data(FuildIndex , MapModelList::RoleName_NumEmg).toInt();
    if(--FloorEmgCnt  <= 0){
        m_FloorInfoList.value(BuildingNum)->setData(FuildIndex , false , MapModelList::RoleName_IsEmg);

    }
    m_FloorInfoList.value(BuildingNum)->setData(FuildIndex , m_FloorInfoList.value(BuildingNum)->data(FuildIndex , MapModelList::RoleName_NumEmg).toInt()-1 ,
                                                   MapModelList::RoleName_NumEmg);
}

void BuildingMapModel::clearEmgNum()
{
    for(int DisIndex=0 ; DisIndex < m_DistrickModelList.rowCount() ; DisIndex++){
        QModelIndex index = m_DistrickModelList.createIndex(DisIndex , 0 , NULL);
        m_DistrickModelList.setData(index , false , MapModelList::RoleName_IsEmg);
        m_DistrickModelList.setData(index , 0 , MapModelList::RoleName_NumEmg);
    }

    QStringList DisdNumList = m_BuildingInfoList.keys();
    for(int NumDis = 0 ; NumDis < DisdNumList.count() ; NumDis++){
        for(int NumBuild = 0 ; NumBuild < m_BuildingInfoList.value(DisdNumList.at(NumDis))->rowCount() ; NumBuild++){
            QModelIndex index = m_DistrickModelList.createIndex(NumBuild , 0 , NULL);
            m_BuildingInfoList.value(DisdNumList.at(NumDis))->setData(index , false , MapModelList::RoleName_IsEmg);
            m_BuildingInfoList.value(DisdNumList.at(NumDis))->setData(index , 0 , MapModelList::RoleName_NumEmg);
        }
    }

    QStringList BuildNumList = m_FloorInfoList.keys();
    for(int NumBuild = 0 ; NumBuild < BuildNumList.count() ; NumBuild++){
        for(int NumFloor = 0 ; NumFloor < BuildNumList.count() ; NumFloor++){
            QModelIndex index = m_DistrickModelList.createIndex(NumFloor , 0 , NULL);
            m_FloorInfoList.value(BuildNumList.at(NumBuild))->setData(index , false , MapModelList::RoleName_IsEmg);
            m_FloorInfoList.value(BuildNumList.at(NumBuild))->setData(index , 0 , MapModelList::RoleName_NumEmg);
        }
    }
}
QModelIndex BuildingMapModel::createIndex(int row, int column, void *data) const
{
    return m_DistrickModelList.createIndex(row , column , data);
}
QString BuildingMapModel::getDistrickNumberByBuildNumber(QString BuildNumber)
{
    if(!m_DisBuildNumberList.contains(BuildNumber))
        return "";

    return m_DisBuildNumberList.value(BuildNumber);
}
int BuildingMapModel::getDisBuildFloorIndex(BMapModelType Type, QString Number , QString BuildNumber)
{
    if(Type == Model_Districk && m_DisIndex.contains(Number)){
        return m_DisIndex.value(Number);
    }
    else if(Type == Model_Build && m_BuildIndex.contains(Number)){
        return m_BuildIndex.value(Number);
    }
    else if(Type == Model_Floor && !BuildNumber.isEmpty()){

        int FloorNum = Number.mid(1).toInt();
        int startFloorNum = getBuildInfoByBuildNumber(BuildNumber)->StartNumber.mid(1).toInt();
        if(FloorNum > 0 && startFloorNum < 0 ){
            return FloorNum-startFloorNum-1;
        }
        else{
            return FloorNum-startFloorNum;
        }
    }
    else{
        return -1;
    }
}
const BuildingMapModel::BuildingInfo* BuildingMapModel::getBuildInfoByBuildNumber(QString BuildNumber)
{
    if(!m_PerBuilkdInfoList.contains(BuildNumber))
        return NULL;

    return m_PerBuilkdInfoList.value(BuildNumber);
}
