//
// Created by WeiWang on 25-3-9.
//

#ifndef SERVER_H
#define SERVER_H

#include "chttpserver.h"
#include "sqlitedataprovider.h"

class Server : public QObject , public NetworkFrameManager {
    Q_OBJECT
public:
    Server();

private:
    void initDateBase();

    void initRouter();

    // 处理http请求
    bool OnProcessHttpAccepted(QObject *obj, const QPointer<JQHttpServer::Session> &session) override;

private:
    //处理各种API
    //测试
    bool onApiTest(const QPointer<JQHttpServer::Session> &session);
    //版本
    bool onApiVersion(const QPointer<JQHttpServer::Session> &session);
    //获取本地歌曲列表
    bool onApiLocalSongList(const QPointer<JQHttpServer::Session> &session);
    //搜索歌曲
    bool onApiSearchSong(const QPointer<JQHttpServer::Session> &session);
    //添加歌曲
    bool onApiAddSong(const QPointer<JQHttpServer::Session> &session);
    //删除歌曲
    bool onApiDelSong(const QPointer<JQHttpServer::Session> &session);

private:
    chttpserver m_httpserver;
    SqliteDataProvider m_SqliteDataProvider;
    //定义路由表
    using ApiHandler = std::function<bool(const QPointer<JQHttpServer::Session> &)>;
    QHash<QString, ApiHandler> apiRouter;
};

#endif //SERVER_H