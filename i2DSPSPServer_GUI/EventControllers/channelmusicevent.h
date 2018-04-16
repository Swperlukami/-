#ifndef CHANNELMUSICEVENT_H
#define CHANNELMUSICEVENT_H

#include <QObject>
#include "SysDataBase/emgbroadcastdatabase.h"

class ChannelMusicEvent : public QObject
{
    Q_OBJECT
public:
    explicit ChannelMusicEvent(QObject *parent = nullptr);

    void getChannelMusicInfo();
signals:
    void initChannelMusicList(int channelIndex , QString songName , QString singer ,QString album , int duration);
public slots:
    void userAddOrDelSong(bool isAdd , int channelNum, QString songName);

private:

};

#endif // CHANNELMUSICEVENT_H
