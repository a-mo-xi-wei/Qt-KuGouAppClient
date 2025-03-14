//
// Created by WeiWang on 25-3-9.
//

#include "Server.h"

Server::Server() {
    initDateBase();
    initRouter();
    // 初始日志系统，设置日志文件路径
    init_log_file(QCoreApplication::applicationDirPath() + QString("/logs/serve_%1.log").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    m_httpserver.setNetworkFrameManager(this);
    if (!m_httpserver.isRunning()) {
        m_httpserver.listen(8080);
        QLOG_INFO()<< QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") + "服务器启动成功";
    }
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

void Server::initRouter() {
    apiRouter["/api/test"] = &Server::onApiTest;
    apiRouter["/api/version"] = &Server::onApiVersion;
    apiRouter["/api/addSong"] = &Server::onApiAddSong;
    apiRouter["/api/delSong"] = &Server::onApiDelSong;
}

bool Server::OnProcessHttpAccepted(QObject *obj, const QPointer<JQHttpServer::Session> &session) {
    //qDebug()<<"看到我，你就有了";
    QString path = session->requestUrl();
    QString method = session->requestMethod(); // GET/POST/PUT/DELETE
    //QMap<QString,QString> header = session->requestUrlQuery();

    bool isProcessed = false;

    if(obj == &m_httpserver)
    {
        // 2. 根据路径和方法处理请求
        if (method == "GET") {
            if (apiRouter.contains(path)) {
                isProcessed = apiRouter[path](session);
            }
        }

    }

    QLOG_INFO()<< QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ")+
                      "Server::OnProcessHttpAccepted:"+
                      session->requestSourceIp();

    return isProcessed ? isProcessed : NetworkFrameManager::OnProcessHttpAccepted(obj,session);
}

bool Server::onApiTest(const QPointer<JQHttpServer::Session> &session) {
    // 解析请求数据（假设是 JSON）
    //QJsonDocument requestDoc = QJsonDocument::fromJson(session->requestBody());
    //QJsonObject requestData = requestDoc.object();

    // 返回成功响应
    QJsonObject response;
    response["status"] = "success";
    session->replyBytes(QJsonDocument(response).toJson(),"application/json");
    return true;
}

bool Server::onApiVersion(const QPointer<JQHttpServer::Session> &session) {
    QJsonObject response;
    response["version"] = "1.0";
    session->replyBytes(QJsonDocument(response).toJson(),"application/json");
    return true;
}

bool Server::onApiSearchSong(const QPointer<JQHttpServer::Session> &session) {
    return false;
}

bool Server::onApiAddSong(const QPointer<JQHttpServer::Session> &session) {
    return false;
}

bool Server::onApiDelSong(const QPointer<JQHttpServer::Session> &session) {
    return false;
}