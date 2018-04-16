#ifndef BUILDINGMAPMODEL_H
#define BUILDINGMAPMODEL_H

#include <QObject>
#include <QQuickItem>
#include <QFileSystemModel>
#include <QQmlContext>
#include <QDir>
#include <QMap>
#include "mapmodellist.h"

class BuildingMapModel : public QObject
{
    Q_OBJECT
public:
    explicit BuildingMapModel(QObject *parent);
    enum BMapModelType{Model_Districk = 0 , Model_Build , Model_Floor};

    struct BuildingInfo{
        QString Name;
        QString Number;
        QString StartNumber;
        int     ListIndex;
        QString DisNumber;
        MapModelList *FloorInfoList;
    };

    void setBuildingMapInfoFilePath(QString filePath);
    const MapModelList *getDistrickModel();
    const MapModelList *getBuildingModel(QString DistrickNum);
    const MapModelList *getFloorModel(QString DistrickNum , QString BuildingNum);
    void setNumInfoDevices(QMap<QString, int> Districk , QMap<QString, int> Building, QMap<QString, int> Floor);
    void setEmgDeviceNum(QString DistrickNum , QString BuildingNum , QString FloorNum , int NumEmg);
    void setCancelEmgDeviceNum(QString DistrickNum , QString BuildingNum , QString FloorNum , int NumEmg);
    void clearEmgNum();

    void initTwoMapImage(QString &Dis , QString &Build , QString &Floor);
    QString getDistrickNumberByBuildNumber(QString BuildNumber);

    int getDisBuildFloorIndex(BMapModelType Type , QString Number, QString BuildNumber = "");

    QModelIndex createIndex(int row, int column, void *data) const;
    const BuildingInfo* getBuildInfoByBuildNumber(QString BuildNumber);

    QString m_CurrentDistrcik;
    QString m_CurrentBuild;
    QString m_CurrentFloor;

signals:

public slots:
    void userClickedDistrickItem(int Type, QVariant Number);

private:

    MapModelList m_DistrickModelList;
    QMap<QString , int>m_DisIndex;                      //记录区号与索引的链表
    QMap<QString , MapModelList*>m_BuildingInfoList;    //提供分区编号，获取该区楼栋列表。

    QMap<QString , int>m_BuildIndex;                    //记录建筑编号与索引的链表
    QMap<QString , MapModelList*>m_FloorInfoList;       //提供建筑编号，获取该建筑的楼层列表。

    QMap<QString , QString>m_DisBuildNumberList;        //根据建筑编号索引区号，建筑编号为Key，建筑编号不会重复

    QMap<QString , BuildingInfo*>m_PerBuilkdInfoList;    //没栋建筑的编号对应其先关信息链表

    QQuickItem *pTwoMapView;
    QQuickItem *pDisListView;
    QQuickItem *pBuildListView;
    QQuickItem *pFloorListView;



    void initQuickItem();

};

#endif // BUILDINGMAPMODEL_H
