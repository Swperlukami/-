#ifndef SPCONTROLLER_H
#define SPCONTROLLER_H

#include <QObject>
#include <QDir>
#include <QSound>
#include <QMediaService>
#include <QMediaMetaData>
#include <QMediaContent>
#include <QMediaPlayer>
#include "BuildingMapModel/buildingmapmodel.h"

class SPController : public QObject
{
    Q_OBJECT
public:
    explicit SPController(QObject *parent = nullptr);
    void processData(QByteArray data);

    void setSPQmlViewRootItem(const QObject *rootqmlview);

signals:
    void spEvent(QString ltisn , QString spisn , int port ,QString state , QString apid , int apspno);
    void spInit(QString ltisn ,QString spisn , int port ,QString state , QString addr , QString apid , int apspno);

    void initAudioFileList(QString filename , int duration , bool isrecordfile);
public slots:
    void getFloorDevice(int buildtype, QVariant buildindex);

    QString getSPDetailInfo(QString ltid , int spno);

    void resAPSPStateEvent(QString APIp , QString SPId ,int SPNo, int SPState);
private:
    void initSPDevices();    


    BuildingMapModel *mp_SPBuildingMapModel;
};

#endif // SPCONTROLLER_H
