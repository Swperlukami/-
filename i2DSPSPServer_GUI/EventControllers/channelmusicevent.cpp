#include "channelmusicevent.h"

ChannelMusicEvent::ChannelMusicEvent(QObject *parent) : QObject(parent)
{

}
void ChannelMusicEvent::getChannelMusicInfo(){
    QSqlQuery query;
    for(int channelIndex = 0 ; channelIndex < 5 ; channelIndex++){
        QString strQuery = QString("select songName from channelmusic where channelNum=%1").arg(channelIndex+1);
        query.clear();
        query_broadcast_database(strQuery , query);
        while (query.next()) {
            emit initChannelMusicList(channelIndex , query.value("songName").toString() , "Adel" , "Adel Album" , 240);
        }
    }
}
void ChannelMusicEvent::userAddOrDelSong(bool isAdd, int channelNum , QString songName)
{
    QString execStr = "";
    if(isAdd){
        execStr = QString("insert into channelmusic values(%1 , '%2')").arg(channelNum).arg(songName);
    }
    else{
        execStr = QString("delete from channelmusic where songName='%1' and channelNum=%2").arg(songName).arg(channelNum);
    }
    exec_broadcast_database(execStr);
}
