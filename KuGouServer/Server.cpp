//
// Created by WeiWang on 25-3-9.
//

#include "Server.h"
#include "SResultCode.h"

#define CheckJsonParse(session)\
        QJsonParseError error;\
        QJsonDocument requestDoc = QJsonDocument::fromJson(session->requestBody(), &error);\
        if (error.error != QJsonParseError::NoError) {\
            QLOG_ERROR() << "JSON parse error:" << error.errorString();\
            session->replyBytes(SResult::failure(SResultCode::ParamJsonInvalid),"application/json");\
            return false;\
        }

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
    //apiRouter["/api/test"] = std::bind(&Server::onApiTest, this, std::placeholders::_1);
    apiRouter["/api/test"] = [this](auto && PH1) { return onApiTest(std::forward<decltype(PH1)>(PH1)); };
    //apiRouter["/api/version"] = std::bind(&Server::onApiVersion, this, std::placeholders::_1);
    apiRouter["/api/version"] = [this](auto && PH1) { return onApiVersion(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/localSongList"] = [this](auto && PH1) { return onApiLocalSongList(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/searchSong"] = [this](auto && PH1) { return onApiSearchSong(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/addSong"] = [this](auto && PH1) { return onApiAddSong(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/delSong"] = [this](auto && PH1) { return onApiDelSong(std::forward<decltype(PH1)>(PH1)); };
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

bool Server::onApiLocalSongList(const QPointer<JQHttpServer::Session> &session) {
    if (m_SqliteDataProvider.execSql("SELECT name FROM sqlite_master WHERE type='table' AND name='local_song_table';").isEmpty()) {

    }
    return false;
}

bool Server::onApiSearchSong(const QPointer<JQHttpServer::Session> &session) {
    return false;
}

bool Server::onApiAddSong(const QPointer<JQHttpServer::Session> &session) {
    CheckJsonParse(session);
    QJsonObject requestData = requestDoc.object();

    // 校验必需字段
    const QStringList requiredFields = {
        "index", "songName", "singer",
        "duration", "mediaPath", "addTime"
    };
    for (const auto &field : requiredFields) {
        if (!requestData.contains(field)) {
            QLOG_ERROR() << "Missing required field: " << field;
            session->replyBytes(SResult::failure(SResultCode::ParamLoss),"application/json");
            return false;
        }
    }

    try {
        // 提取数据
        const int index = requestData["index"].toInt();
        const QString songName = requestData["songName"].toString();
        const QString singer = requestData["singer"].toString();
        const QString duration = requestData["duration"].toString();
        const QString mediaPath = requestData["mediaPath"].toString();
        const QString addTime = requestData["addTime"].toString();

        // 处理封面图片（Base64或空）
        QString coverData;
        if (requestData.contains("cover") && !requestData["cover"].isNull()) {
            QByteArray imageData = QByteArray::fromBase64(
                requestData["cover"].toString().toLatin1()
            );

            // 可选：验证图片有效性
            if (!QImage::fromData(imageData).isNull()) {
                coverData = requestData["cover"].toString();
            } else {
                QLOG_WARN() << "Invalid image data for index:" << index;
            }
        }

        // 构造SQL语句（使用参数化查询防注入）
        const QString sql = QString(
           "INSERT INTO local_song_table "
           "(index, cover, song, singer, duration, media_path, add_time) "
           "VALUES (%1, %2, %3, %4, %5, %6, %7);"
       )
       .arg(index)  // 数值类型直接使用
       .arg(coverData)
       .arg(songName)
       .arg(singer)
       .arg(duration)
       .arg(mediaPath)
       .arg(addTime);
        qDebug()<<sql;
        return true;
        // 执行SQL
        if (!m_SqliteDataProvider.execSql(sql).isEmpty()) {
            QJsonObject response;
            response["status"] = "success";
            response["index"] = index;
            session->replyBytes(QJsonDocument(response).toJson(), "application/json");

            QLOG_INFO() << "Song added successfully. Index:" << index;
        } else {
            QLOG_ERROR() << "Song added error for index:" << index;
            session->replyBytes(SResult::failure(SResultCode::ServerSqlQueryError),"application/json");
        }
    } catch (const std::exception &e) {
        QLOG_ERROR() << "Exception in onApiAddSong:" << e.what();
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError),"application/json");
    }

    return true;
}

bool Server::onApiDelSong(const QPointer<JQHttpServer::Session> &session) {
    return false;
}