#ifndef AUDIOFILESMANAGECONTROLLER_H
#define AUDIOFILESMANAGECONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QTextCodec>
#include "SysDataBase/database.h"
#include "SysDataBase/emgbroadcastdatabase.h"


class AudioFilesManageController : public QObject
{
    Q_OBJECT
public:
    explicit AudioFilesManageController(QObject *parent = nullptr);
    ~AudioFilesManageController();
    void initSystemAudoFiles();

signals:
    void initSyaAudioFilesInfo(int fileType , QString fileName , QString singer , QString albnum , int duration);

public slots:
    bool addAudioFileToSysFloder(int fileType , QString filePath);
    void deleteTheAudioFile(QString fileName , QString fileType);

    void userSetAudioManageEvents(QString cmdType , QVariant params = 0 , QVariant params2 = 0);

    void initSysEmgRecordFiles();
    void initSysTalkRecordFiles();
private:
    QString getTheMp3ID3Info(QString mp3FilePath);

    QString mediainfoGetAudioInfo(QString mp3FilePath);

    QProcess *m_pMplayerGetMp3InfoProcess;

};

#endif // AUDIOFILESMANAGECONTROLLER_H
