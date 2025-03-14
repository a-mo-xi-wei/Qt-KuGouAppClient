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
    static bool onApiTest(const QPointer<JQHttpServer::Session> &session);
    //版本
    static bool onApiVersion(const QPointer<JQHttpServer::Session> &session);
    //搜索歌曲
    static bool onApiSearchSong(const QPointer<JQHttpServer::Session> &session);
    //添加歌曲
    static bool onApiAddSong(const QPointer<JQHttpServer::Session> &session);
    //删除歌曲
    static bool onApiDelSong(const QPointer<JQHttpServer::Session> &session);

private:
    chttpserver m_httpserver;
    SqliteDataProvider m_SqliteDataProvider;
    //定义路由表
    using ApiHandler = std::function<bool(const QPointer<JQHttpServer::Session> &)>;
    QHash<QString, ApiHandler> apiRouter;
};

#endif //SERVER_H