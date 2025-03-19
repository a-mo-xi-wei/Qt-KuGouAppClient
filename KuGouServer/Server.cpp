//
// Created by WeiWang on 25-3-9.
//

#include "Server.h"
#include "SJwt.h"
#include "SResultCode.h"

#include <QRegularExpression>
#include <QSqlError>


#define CheckJsonParse(session)\
        QJsonParseError error;\
        QJsonDocument requestDoc = QJsonDocument::fromJson(session->requestBody(), &error);\
        if (error.error != QJsonParseError::NoError) {\
            QLOG_ERROR() << "JSON parse error:" << error.errorString();\
            session->replyBytes(SResult::failure(SResultCode::ParamJsonInvalid),"application/json");\
            return false;\
        }

const char* SECRET = "weisang666";

std::optional<QByteArray>CheckToken(const QPointer<JQHttpServer::Session> &session) {
	//验证token
	auto auth = session->requestHeader().value("Authorization");
	//如果没有认证头
	if (auth.isEmpty()) {
		return SResult::failure(SResultCode::UserUnauthorized);
	}
	//必须以Bearer开头
	if (!auth.startsWith("Bearer")) {
		return SResult::failure(SResultCode::UserAuthFormatError);
	}
	//拿到token
	auto token = auth.mid(strlen("Bearer")).toUtf8();
	//验证token
	auto jwtObject = SJwt::SJwtObject::decode(token, SJwt::SAlgorithm::HS256, SECRET);
	if (jwtObject.status() == SJwt::SJwtObject::Status::Expired) {
		return SResult::failure(SResultCode::UserAuthTokenExpired);
	}
	if (!jwtObject.isValid()) {
		return SResult::failure(SResultCode::UserAuthTokenInvalid);
	}
	return {};
}

Server::Server() {
    initDateBase();
    initRouter();
    // 初始日志系统，设置日志文件路径
    init_log_file(QCoreApplication::applicationDirPath() + QString("/../logs/serve_%1.log").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    m_httpserver.setNetworkFrameManager(this);
    if (!m_httpserver.isRunning()) {
        m_httpserver.listen(8080);
        QLOG_INFO()<< "服务器启动成功";
    }
}

void Server::initDateBase() {
    init_dbpool(false,this);
    m_SqliteDataProvider.connect(QCoreApplication::applicationDirPath()+QString("SQLite.db"));
    //先判断是否存在usertable表,不存在则创建
    if (m_SqliteDataProvider.execSql("SELECT name FROM sqlite_master WHERE type='table' AND name='user_table';","find_user_able",false).isEmpty()) {
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
        m_SqliteDataProvider.execSql(sql,"create_user_table",false);
    }
    //先判断是否存在local_song_table表,不存在则创建
    if (m_SqliteDataProvider.execSql("SELECT name FROM sqlite_master WHERE type='table' AND name='local_song_table';","find_local_song_able",false).isEmpty()) {
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
            "PRIMARY KEY (\"song\", \"singer\", \"duration\"));";//用歌曲和歌手和时长唯一标识
        m_SqliteDataProvider.execSql(sql,"create_local_song_table",false);
    }

}

void Server::initRouter() {
    m_SqliteDataProvider.connect(QCoreApplication::applicationDirPath()+QString("SQLite.db"));
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
        if (apiRouter.contains(path)) {
            isProcessed = apiRouter[path](session);
        }
        else
        {
            // 兜底正则匹配（如果找不到路径）
            static const QRegularExpression fallbackRegex(".*");
            if (fallbackRegex.match(path).hasMatch())
            {
                session->replyBytes(SResult::failure(SResultCode::PathIllegal), "application/json");
                isProcessed = true;
            }
        }
    }

    QLOG_INFO()<< "Server::OnProcessHttpAccepted: "+ session->requestSourceIp();

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
    response["App-version"] = "1.0";
    response["App-name"] = "我的酷狗";
    response["App-datatime"] =  QDateTime::currentDateTime().toString("yyyy-MM-dd");
    response["App-copyright"] = "威桑版权所有";
    session->replyBytes(QJsonDocument(response).toJson(),"application/json");
    return true;
}

bool Server::onApiLocalSongList(const QPointer<JQHttpServer::Session> &session) {
    try {
        // 构造带字段别名的SQL查询语句
        const QString sql =
            "SELECT "
            "\"index\", "
            "cover, "
            "song, "
            "singer, "
            "duration, "
            "media_path, "
            "add_time "
            "FROM local_song_table ;";

        // 执行查询
        auto resultRecord  = m_SqliteDataProvider.execSql(sql, "get_song_list", false);
        if (!resultRecord .isEmpty()) {
            // 构建响应数据
            QJsonArray songsArray;
            /*  //未重载迭代器
            for (const auto& record : resultRecord) {
                QJsonObject song;
                song["index"] = record.value("index").toInt();
                song["songName"] = record.value("songName").toString();
                song["singer"] = record.value("singer").toString();
                song["duration"] = record.value("duration").toString();
                song["mediaPath"] = record.value("mediaPath").toString();
                song["addTime"] = record.value("addTime").toString();

                // 处理封面数据（Base64字符串或空）
                if (record.contains("cover") && !record.value("cover").isNull()) {
                    song["cover"] = record.value("cover").toString();
                }

                songsArray.append(song);
            }
            */
            for (int k = 0; k < resultRecord.Count(); k++) {//一般只有一组结果
                RecordSet resultSet = resultRecord(k);
                for (int i = 0; i < resultSet.rows(); i++) {
                    QJsonObject song;
                    for (int j = 0; j < resultSet.cols(); j++) {
                        // 遍历每一列，根据列名填充 QJsonObject
                        if (resultSet.getField(j) == "index") {
                            song["index"] = resultSet(i,j).toInt();
                            continue;
                        }
                         song[resultSet.getField(j)] = resultSet(i,j);
                    }
                    songsArray.append(song);
                }
            }


            // 构造成功响应
            QJsonObject response;
            response["status"] = "success";
            response["data"] = songsArray;
            session->replyBytes(QJsonDocument(response).toJson(), "application/json");

            QLOG_INFO() << "Fetched" << songsArray.size() << "songs";
            return true;
        }
        QLOG_INFO() << "Fetched Empty songs";
    }
    catch (const std::exception &e) {
        QLOG_ERROR() << "Exception in onApiLocalSongList:" << e.what();
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError), "application/json");
        return false;
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
           "(\"index\", cover, song, singer, duration, media_path, add_time) "
           "VALUES (%1, %2, %3, %4, %5, %6, %7);"
       )
        .arg(safeNumber(index))         // 数值类型
        .arg(safeString(coverData))         // Base64图像数据
        .arg(safeString(songName))
        .arg(safeString(singer))
        .arg(safeString(duration))
        .arg(safeString(mediaPath))
        .arg(safeString(addTime));

        // 执行SQL
        if (!m_SqliteDataProvider.execInsertSql(sql,"add_song",false).isEmpty()) {
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
    try {
        QJsonDocument doc = QJsonDocument::fromJson(session->requestBody());
        int index = doc.object()["index"].toInt();

        QString sql = QString("DELETE FROM local_song_table WHERE \"\" = %1").arg(index);
        m_SqliteDataProvider.execSql(sql, "delete_song", true);

        session->replyBytes(SResult::success(), "application/json");
        return true;
    }
    catch (const std::exception& e) {
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError),"application/json");
        return false;
    }
    return false;
}