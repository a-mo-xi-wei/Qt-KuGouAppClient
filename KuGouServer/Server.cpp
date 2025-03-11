//
// Created by WeiWang on 25-3-9.
//

#include "Server.h"

Server::Server() {
    initDateBase();
}

void Server::initDateBase() {
    m_SqliteDataProvider.connect(QCoreApplication::applicationDirPath()+QString("SQLite.db"));
    //先判断是否存在usertable表,不存在则创建
    if (m_SqliteDataProvider.execSql("SELECT name FROM sqlite_master WHERE type='table' AND name='user_table';").isEmpty()) {
        const QString sql =
           "CREATE TABLE \"usertable\" ("
           "\"portrait\" text,"
           "\"account\" text NOT NULL,"
           "\"password\" text NOT NULL,"
           "\"id\" text NOT NULL,"
           "\"nickname\" text NOT NULL DEFAULT '点击登录',"
           "\"gender\" integer DEFAULT -1,"
           "\"birthday\" text,"
           "\"area_province\" text,"
           "\"area_city\" text,"
           "\"signature\" text,"
           "PRIMARY KEY (\"account\", \"id\"));";
        m_SqliteDataProvider.execSql(sql);
    }
    //先判断是否存在local_song_table表,不存在则创建
    if (m_SqliteDataProvider.execSql("SELECT name FROM sqlite_master WHERE type='table' AND name='local_song_table';").isEmpty()) {
        const QString sql =
            "CREATE TABLE \"local_song_table\" ("
            "\"index\" integer NOT NULL,"
            "\"cover\" text,"
            "\"song\" text DEFAULT '未知歌曲',"
            "\"singer\" text DEFAULT '未知歌手',"
            "\"duration\" text NOT NULL,"
            "\"media_path\" text NOT NULL,"
            "\"add_time\" text NOT NULL,"
            "\"play_count\" integer NOT NULL DEFAULT 0,"
            "PRIMARY KEY (\"index\"));";
        m_SqliteDataProvider.execSql(sql);
    }

}

bool Server::OnProcessHttpAccepted(QObject *obj, const QPointer<JQHttpServer::Session> &session) {
    return NetworkFrameManager::OnProcessHttpAccepted(obj, session);
}
