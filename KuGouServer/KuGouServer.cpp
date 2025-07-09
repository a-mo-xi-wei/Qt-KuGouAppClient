//
// Created by WeiWang on 25-3-9.
//
/**
 * @file Server.cpp
 * @brief HTTP 服务器核心实现，处理请求路由、数据库操作及业务逻辑。
 *
 * 此文件是服务器应用程序的核心，负责处理 HTTP 请求、数据库操作和业务逻辑。
 *
 * @author WeiWang
 * @date 2025-05-12
 */

//-----------------------------------------------------------------------------
// Server类实现
//-----------------------------------------------------------------------------

/**
 * @class KuGouServer
 * @brief HTTP服务器主类，管理数据库、路由及请求处理
 * @details
 * - 使用SQLite作为本地数据库存储用户和歌曲数据
 * - 基于JQHttpServer实现HTTP协议解析和路由分发
 * - 支持JWT令牌验证的鉴权机制
 * - 提供歌曲列表管理、搜索、增删等RESTful API
 */

#include "KuGouServer.h"
#include "SJwt.h"
#include "SResultCode.h"

#include <QRegularExpression>
#include <QSqlError>
#include <QNetworkReply>



//-----------------------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------------------

/**
 * @def CheckJsonParse(session)
 * @brief JSON 解析检查宏，快速验证请求体合法性
 * @param session HTTP 会话对象
 * @note
 * - 解析失败时自动返回 400 错误
 * - 使用 QJsonParseError 检测具体错误类型
 * - 宏内包含直接 return，需谨慎使用作用域
 */
#define CheckJsonParse(session)\
        QJsonParseError error;\
        QJsonDocument requestDoc = QJsonDocument::fromJson(session->requestBody(), &error);\
        if (error.error != QJsonParseError::NoError) {\
            QLOG_ERROR() << "JSON parse error:" << error.errorString();\
            session->replyBytes(SResult::failure(SResultCode::ParamJsonInvalid),"application/json");\
            return false;\
        }

const char* SECRET = "weisang666";

/**
 * @brief 检查令牌。
 *
 * @param session HTTP 会话对象。
 * @return std::optional<QByteArray> 检查结果。
 */
std::optional<QByteArray> CheckToken(const QPointer<JQHttpServer::Session> &session) {
    // 验证 token
	auto auth = session->requestHeader().value("Authorization");
    // 如果没有认证头
	if (auth.isEmpty()) {
		return SResult::failure(SResultCode::UserUnauthorized);
	}
    // 必须以 Bearer 开头
	if (!auth.startsWith("Bearer")) {
		return SResult::failure(SResultCode::UserAuthFormatError);
	}
    // 拿到 token
	auto token = auth.mid(strlen("Bearer")).toUtf8();
    // 验证 token
	auto jwtObject = SJwt::SJwtObject::decode(token, SJwt::SAlgorithm::HS256, SECRET);
	if (jwtObject.status() == SJwt::SJwtObject::Status::Expired) {
		return SResult::failure(SResultCode::UserAuthTokenExpired);
	}
	if (!jwtObject.isValid()) {
		return SResult::failure(SResultCode::UserAuthTokenInvalid);
	}
	return {};
}

/**
 * @brief 构造函数。
 */
KuGouServer::KuGouServer() {
    initDateBase();
    initRouter();
    // 初始日志系统，设置日志文件路径
    init_log_file(QCoreApplication::applicationDirPath() + QString("/../logs/serve_%1.log").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    m_httpserver.setNetworkFrameManager(this);
    if (!m_httpserver.isRunning()) {
        m_httpserver.listen(8080);
        QLOG_INFO() << "服务器启动成功";
    }
}

//-----------------------------------------------------------------------------
// 数据库初始化
//-----------------------------------------------------------------------------

/**
 * @brief 初始化 SQLite 数据库表结构。
 * @details
 * 1. 检查 user_table 表是否存在，不存在则创建用户表。
 * 2. 检查 local_song_table 表是否存在，不存在则创建歌曲表。
 * 表结构说明：
 * - user_table: 存储用户账户、昵称、头像等基本信息。
 * - local_song_table: 存储歌曲索引、元数据及播放统计。
 * @warning 主键设计使用复合键保证数据唯一性。
 */
void KuGouServer::initDateBase() {
    init_dbpool(false, this);
    m_SqliteDataProvider.connect(QCoreApplication::applicationDirPath() + QString("SQLite.db"));
    //先判断是否存在 user_table 表,不存在则创建
    if (m_SqliteDataProvider.execSql("SELECT name FROM sqlite_master WHERE type='table' AND name='user_table';","find_user_able",false).isEmpty()) {
        const QString sql =
           "CREATE TABLE \"user_table\" ("
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
           "PRIMARY KEY (\"account\", \"password\"));";
        m_SqliteDataProvider.execSql(sql,"create_user_table",false);
    }
    // 先判断是否存在 local_song_table 表,不存在则创建
    if (m_SqliteDataProvider.execSql("SELECT name FROM sqlite_master WHERE type='table' AND name='local_song_table';","find_local_song_able",false).isEmpty()) {
        const QString sql =
            "CREATE TABLE \"local_song_table\" ("
            "\"index\" integer NOT NULL,"
            "\"cover\" text,"
            "\"song\" text DEFAULT '网络歌曲',"
            "\"singer\" text DEFAULT '网络歌手',"
            "\"duration\" text NOT NULL,"
            "\"media_path\" text NOT NULL,"
            "\"add_time\" text NOT NULL,"
            "\"play_count\" integer NOT NULL DEFAULT 0,"
            "PRIMARY KEY (\"song\", \"singer\", \"duration\"));";//用歌曲和歌手和时长唯一标识
        m_SqliteDataProvider.execSql(sql,"create_local_song_table",false);
    }

}

/**
 * @brief 初始化路由。
 */
void KuGouServer::initRouter() {
    m_SqliteDataProvider.connect(QCoreApplication::applicationDirPath()+QString("SQLite.db"));
    //apiRouter["/api/test"] = std::bind(&Server::onApiTest, this, std::placeholders::_1);
    ///< 应用完美转发！！！
    apiRouter["/api/test"] = [this](auto && PH1) { return onApiTest(std::forward<decltype(PH1)>(PH1)); };
    //apiRouter["/api/version"] = std::bind(&Server::onApiVersion, this, std::placeholders::_1);
    apiRouter["/api/version"] = [this](auto && PH1) { return onApiVersion(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/localSongList"] = [this](auto && PH1) { return onApiLocalSongList(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/searchSuggestion"] = [this](auto && PH1) { return onApiSearchSuggestion(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/searchSong"] = [this](auto && PH1) { return onApiSearchSong(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/addSong"] = [this](auto && PH1) { return onApiAddSong(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/delSong"] = [this](auto && PH1) { return onApiDelSong(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/login"] = [this](auto && PH1) { return onApiLogin(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/register"] = [this](auto && PH1) { return onApiRegister(std::forward<decltype(PH1)>(PH1)); };
    apiRouter["/api/userDestroy"] = [this](auto && PH1) { return onApiUserDestroy(std::forward<decltype(PH1)>(PH1)); };
}

//-----------------------------------------------------------------------------
// 路由处理
//-----------------------------------------------------------------------------

/**
 * @brief 处理 HTTP 请求入口
 * @param obj 事件来源对象
 * @param session HTTP会话对象
 * @return bool 是否已处理请求
 * @retval true 请求已被处理
 * @retval false 请求未处理，转交默认处理
 * @note
 * - 路由匹配优先级：精确路径 > 正则兜底
 * - 支持GET/POST/PUT/DELETE方法
 * - 记录客户端 IP 用于审计日志
 */
bool KuGouServer::OnProcessHttpAccepted(QObject *obj, const QPointer<JQHttpServer::Session> &session) {
    //qDebug()<<"看到我，你就有了";
    QString path = session->requestUrlPath();
    QString method = session->requestMethod(); // GET/POST/PUT/DELETE
    // QMap<QString,QString> header = session->requestUrlQuery();
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
                //qWarning() << "非法访问路径: "<<path;
                QLOG_ERROR() << "非法访问路径: "<<path;
                session->replyBytes(SResult::failure(SResultCode::PathIllegal), "application/json");
                isProcessed = true;
            }
        }
    }

    QLOG_INFO()<< "Server::OnProcessHttpAccepted: "+ session->requestSourceIp();

    return isProcessed ? isProcessed : NetworkFrameManager::OnProcessHttpAccepted(obj,session);
}

/**
 * @brief 重排歌曲索引。
 */
void KuGouServer::reorderIndex() {
    const QString sql =
       R"(
        WITH sorted AS (
            SELECT rowid, ROW_NUMBER() OVER (ORDER BY add_time ASC) - 1 AS new_index
            FROM local_song_table
        )
        UPDATE local_song_table
        SET "index" = sorted.new_index
        FROM sorted
        WHERE local_song_table.rowid = sorted.rowid;
        )";

    m_SqliteDataProvider.execSql(sql, "reindex_songs", false);
}

/**
 * @brief 安全处理字符串，防止 SQL 注入。
 *
 * @param input 输入字符串。
 * @return 安全处理后的字符串。
 */
QString KuGouServer::safeString(const QString &input) {
    // 创建输入字符串的副本
    QString escaped = input;

    // 使用 QString 参数进行替换
    escaped.replace(QStringLiteral("'"), QStringLiteral("''"));

    // 包裹结果在单引号中
    return QString("'%1'").arg(escaped);
}

/**
 * @brief 处理测试 API。
 *
 * @param session HTTP 会话对象。
 * @return bool 操作结果。
 */
bool KuGouServer::onApiTest(const QPointer<JQHttpServer::Session> &session) {
    // 解析请求数据（假设是 JSON）
    // QJsonDocument requestDoc = QJsonDocument::fromJson(session->requestBody());
    // QJsonObject requestData = requestDoc.object();

    // 返回成功响应
    QJsonObject response;
    response["status"] = "success";
    session->replyBytes(QJsonDocument(response).toJson(), "application/json");
    return true;
}

/**
 * @brief 处理版本 API。
 *
 * @param session HTTP 会话对象。
 * @return bool 操作结果。
 */
bool KuGouServer::onApiVersion(const QPointer<JQHttpServer::Session> &session) {
    QJsonObject response;
    response["App-version"] = "1.0";
    response["App-name"] = "我的酷狗";
    response["App-datatime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    response["App-copyright"] = "威桑版权所有";
    session->replyBytes(QJsonDocument(response).toJson(), "application/json");
    return true;
}

/**
 * @brief 处理获取本地歌曲列表 API。
 *
 * @param session HTTP 会话对象。
 * @return bool 操作结果。
 */
bool KuGouServer::onApiLocalSongList(const QPointer<JQHttpServer::Session> &session) {
    try {
        // 构造带字段别名的 SQL 查询语句
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
        auto resultRecord = m_SqliteDataProvider.execSql(sql, "get_song_list", false);
        if (!resultRecord.isEmpty()) {
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
                            song["index"] = resultSet(i, j).toInt();
                            continue;
                        }
                        song[resultSet.getField(j)] = resultSet(i, j);
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
    } catch (const std::exception &e) {
        QLOG_ERROR() << "Exception in onApiLocalSongList:" << e.what();
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError), "application/json");
        return false;
    }
    return false;
}

bool KuGouServer::onApiSearchSuggestion(const QPointer<JQHttpServer::Session> &session) {   ///< TODO 性能优化方向：分批次发送，传统HTTP只能返回单次响应需多次响应，需要改用：WebSocket（全双工通信）
    const QString key = session->requestUrlQuery().value("key");
    if (key.isEmpty()) {
        session->replyBytes(SResult::failure(SResultCode::ParamLoss), "application/json");
        return false;
    }

    // 使用共享指针保持状态
    auto suggestions = QSharedPointer<QJsonArray>::create();
    auto weakSession = QPointer<JQHttpServer::Session>(session);

    // 创建工作线程
    QThread *workerThread = new QThread();
    QObject *worker = new QObject();
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, [=]() {
        // 在工作线程中创建 QNetworkAccessManager，确保与请求在同一线程
        auto manager = QSharedPointer<QNetworkAccessManager>::create();

        // QQ 音乐请求
        QNetworkRequest qqRequest(QUrl("https://c6.y.qq.com/splcloud/fcgi-bin/smartbox_new.fcg?key=" + key + "&format=json"));
        qqRequest.setRawHeader("Accept", "application/json");
        qqRequest.setRawHeader("Accept-Language", "zh-CN");
        qqRequest.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/137.0.0.0 Safari/537.36");
        qqRequest.setRawHeader("Content-Type", "text/html");
        qqRequest.setRawHeader("Accept-Encoding", "deflate");

        QNetworkReply *qqReply = manager->get(qqRequest);
        QEventLoop qqLoop;
        QTimer qqTimer;
        qqTimer.setSingleShot(true);
        connect(&qqTimer, &QTimer::timeout, &qqLoop, &QEventLoop::quit);
        connect(qqReply, &QNetworkReply::finished, &qqLoop, &QEventLoop::quit);
        qqTimer.start(5000); // 5秒超时
        qqLoop.exec();

        if (qqReply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(qqReply->readAll());
            if (!doc.isNull()) {
                QJsonArray items = doc["data"].toObject()["song"].toObject()["itemlist"].toArray();
                for (const auto& item : items) {
                    auto obj = item.toObject();
                    suggestions->append(obj["singer"].toString() + " - " + obj["name"].toString());
                }
            } else {
                qWarning() << "QQ音乐JSON解析失败";
            }
        } else {
            qWarning() << "QQ音乐请求失败:" << qqReply->errorString();
        }
        qqReply->deleteLater();

        // 网易云音乐请求
        QNetworkRequest neteaseRequest(QUrl("http://music.163.com/api/search/get/web"));
        neteaseRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        neteaseRequest.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
        neteaseRequest.setRawHeader("Referer", "https://music.163.com/");
        neteaseRequest.setRawHeader("Origin", "https://music.163.com");

        QByteArray postData = "s=" + QUrl::toPercentEncoding(key) + "&type=1";
        QNetworkReply *neteaseReply = manager->post(neteaseRequest, postData);
        QEventLoop neteaseLoop;
        QTimer neteaseTimer;
        neteaseTimer.setSingleShot(true);
        connect(&neteaseTimer, &QTimer::timeout, &neteaseLoop, &QEventLoop::quit);
        connect(neteaseReply, &QNetworkReply::finished, &neteaseLoop, &QEventLoop::quit);
        neteaseTimer.start(5000); // 5秒超时
        neteaseLoop.exec();

        if (neteaseReply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(neteaseReply->readAll());
            if (!doc.isNull()) {
                QJsonArray songs = doc["result"].toObject()["songs"].toArray();
                for (const auto& song : songs) {
                    auto obj = song.toObject();
                    QString artist = !obj["artists"].toArray().isEmpty()
                        ? obj["artists"].toArray()[0].toObject()["name"].toString()
                        : "";
                    suggestions->append(artist + " - " + obj["name"].toString());
                }
            } else {
                qWarning() << "网易云音乐JSON解析失败";
            }
        } else {
            qWarning() << "网易云音乐请求失败:" << neteaseReply->errorString();
        }
        neteaseReply->deleteLater();

        // 发送响应
        if (!weakSession.isNull()) {
            QJsonObject response;
            response["status"] = "success";
            response["data"] = *suggestions;
            weakSession->replyBytes(QJsonDocument(response).toJson(), "application/json");
        }

        // 清理资源并退出线程
        manager->deleteLater();
        worker->deleteLater();
        workerThread->quit();
        // 注意：不在工作线程中调用 wait()
    });

    // 在主线程中清理线程
    connect(workerThread, &QThread::finished, [=] {
        workerThread->deleteLater();
    });

    // 启动线程
    workerThread->start();
    return true;  // 已启动异步处理
}

/**
 * @brief 处理搜索歌曲 API。
 *
 * @param session HTTP 会话对象。
 * @return bool 操作结果。
 */
bool KuGouServer::onApiSearchSong(const QPointer<JQHttpServer::Session> &session) {
    // 获取查询参数
    const QString keyword = session->requestUrlQuery().value("keyword");
    if (keyword.isEmpty()) {
        session->replyBytes(SResult::failure(SResultCode::ParamLoss), "application/json");
        return false;
    }

    // 创建共享指针保存结果
    auto weakSession = QPointer<JQHttpServer::Session>(session);

    // 创建工作线程
    QThread *workerThread = new QThread();
    QObject *worker = new QObject();
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, [=] {
        // 在工作线程中创建网络管理器
        auto manager = QSharedPointer<QNetworkAccessManager>::create();

        QTimer timer1;
        timer1.setSingleShot(true);

        // 第一步：搜索歌曲
        QNetworkRequest searchRequest(QUrl(
            QString("http://songsearch.kugou.com/song_search_v2?keyword=%1&page=1&pagesize=20")
                .arg(QUrl::toPercentEncoding(keyword))
        ));

        QNetworkReply *searchReply = manager->get(searchRequest);
        QEventLoop loop1;
        connect(searchReply, &QNetworkReply::finished, &loop1, &QEventLoop::quit);
        connect(&timer1, &QTimer::timeout, &loop1, &QEventLoop::quit);
        timer1.start(3000); // 3秒超时
        loop1.exec();

        QList<QJsonObject> songList;

        if (searchReply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(searchReply->readAll());
            if (!doc.isNull()) {
                QJsonObject root = doc.object();
                QJsonObject data = root.value("data").toObject();
                QJsonArray lists = data.value("lists").toArray();

                for (const auto &item : lists) {
                    QJsonObject songObj = item.toObject();
                    QJsonObject songInfo;

                    songInfo["hash"] = songObj.value("FileHash").toString();
                    songInfo["songName"] = songObj.value("SongName").toString();
                    songInfo["singer"] = songObj.value("SingerName").toString();
                    songInfo["album"] = songObj.value("AlbumName").toString();

                    int duration = songObj.value("Duration").toInt();
                    songInfo["duration"] = QTime(0, duration / 60, duration % 60).toString("mm:ss");

                    QString coverUrl = songObj.value("Image").toString().replace("{size}", "400");
                    songInfo["coverUrl"] = coverUrl;

                    songList.append(songInfo);
                }
            }
        }
        else {
            qWarning() << "歌曲搜索失败:" << searchReply->errorString();
        }
        searchReply->deleteLater();

        // 发送响应
        if (!weakSession.isNull()) {
            QJsonObject response;
            response["status"] = "success";

            QJsonArray songsArray;
            for (const auto &song : songList) {
                songsArray.append(song);
                //qDebug()<<"netUrl : "<<song["netUrl"].toString();
            }
            response["data"] = songsArray;

            weakSession->replyBytes(QJsonDocument(response).toJson(), "application/json");
        }

        // 清理资源
        manager->deleteLater();
        worker->deleteLater();
        workerThread->quit();
    });

    // 清理线程
    connect(workerThread, &QThread::finished, [=] {
        workerThread->deleteLater();
    });

    workerThread->start();
    return true;
}

/**
 * @brief 处理添加歌曲 API。
 *
 * @param session HTTP 会话对象。
 * @return bool 操作结果。
 * @par 请求参数示例:
 * @code{json}
 * {
 *   "index": 1,
 *   "songName": "Hello",
 *   "singer": "Adele",
 *   "duration": "04:55",
 *   "mediaPath": "/music/hello.mp3",
 *   "addTime": "2025-05-13"
 * }
 * @endcode
 * @warning 使用 safeString 防止 SQL 注入攻击。
 */
bool KuGouServer::onApiAddSong(const QPointer<JQHttpServer::Session> &session) {
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
            session->replyBytes(SResult::failure(SResultCode::ParamLoss), "application/json");
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

        // 构造 SQL 语句（使用参数化查询防注入）
        const QString sql = QString(
           "INSERT INTO local_song_table "
           "(\"index\", cover, song, singer, duration, media_path, add_time) "
           "VALUES (%1, %2, %3, %4, %5, %6, %7);"
        )
        .arg(safeNumber(index))         // 下标
        .arg(safeString(coverData))     // Base64 图像数据
        .arg(safeString(songName))
        .arg(safeString(singer))
        .arg(safeString(duration))
        .arg(safeString(mediaPath))
        .arg(safeString(addTime));

        // 执行 SQL
        if (!m_SqliteDataProvider.execInsertSql(sql, "add_song", false).isEmpty()) {
            QJsonObject response;
            response["status"] = "success";
            response["index"] = index;
            session->replyBytes(QJsonDocument(response).toJson(), "application/json");

            QLOG_INFO() << "Song added successfully. Index:" << index;
        } else {
            QLOG_ERROR() << "Song added error for index:" << index;
            session->replyBytes(SResult::failure(SResultCode::ServerSqlQueryError), "application/json");
        }
    } catch (const std::exception &e) {
        QLOG_ERROR() << "Exception in onApiAddSong:" << e.what();
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError), "application/json");
    }

    return true;
}

bool KuGouServer::onApiDelSong(const QPointer<JQHttpServer::Session> &session) {
    CheckJsonParse(session);

    QJsonObject requestData = requestDoc.object();

    // 校验必需字段
    const QStringList requiredFields = {
        "song", "singer", "duration"
    };
    for (const auto &field : requiredFields) {
        if (!requestData.contains(field)) {
            QLOG_ERROR() << "Missing required field: " << field;
            session->replyBytes(SResult::failure(SResultCode::ParamLoss), "application/json");
            return false;
        }
    }
    try {
        QJsonDocument doc = QJsonDocument::fromJson(session->requestBody());
        QString sql = QString(
            "DELETE FROM local_song_table "
            "WHERE song = '%1' AND singer = '%2' AND duration = '%3'")
            .arg(doc["song"].toString().replace("'", "''"))
            .arg(doc["singer"].toString().replace("'", "''"))
            .arg(doc["duration"].toString());
        auto result = m_SqliteDataProvider.execDeleteSql(sql, "delete_song", false);
        if (!result.isEmpty()){
            //QJsonObject response;
            //response["status"] = "success";
            //response["message"] = "Song deleted successfully";
            //session->replyBytes(QJsonDocument(response).toJson(), "application/json");
            session->replyBytes(SResult::success(), "application/json");
            reorderIndex();
            //qDebug() << "Song deleted successfully."<< " 删除了" <<  result(0)(0, 0)<< "条记录";
            QLOG_INFO() << "Song deleted successfully.";
        } else {
            qWarning() << "Song deletion failed. SQL:" << sql;
            QLOG_ERROR() << "Song deletion failed. SQL:" << sql;
            session->replyBytes(SResult::failure(SResultCode::ServerSqlQueryError), "application/json");
        }
    }
    catch (const std::exception& e) {
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError),"application/json");
        return false;
    }
    return true;
}

bool KuGouServer::onApiLogin(const QPointer<JQHttpServer::Session> &session) {
    try {
        // 解析请求JSON数据
        CheckJsonParse(session);

        QJsonObject reqObj = requestDoc.object();
        QString account = reqObj.value("account").toString();
        QString password = reqObj.value("password").toString();

        // 验证必需参数
        if (account.isEmpty() || password.isEmpty()) {
            session->replyBytes(SResult::failure(SResultCode::ParamJsonInvalid), "application/json");
            QLOG_WARN() << "Missing account or password";
            return false;
        }

        // 安全构造SQL查询（防止SQL注入）
        QString safeAccount = account.replace("'", "''");
        QString sql = QString(
            "SELECT "
            "portrait, account, id, nickname, gender, birthday, "
            "area_province, area_city, signature "
            "FROM user_table "
            "WHERE account = '%1' AND password = '%2';"
        ).arg(safeAccount).arg(password.replace("'", "''"));

        // 执行数据库查询
        auto resultRecord = m_SqliteDataProvider.execSql(sql, "user_login", false);
        if (!resultRecord.isEmpty() && resultRecord(0).rows() > 0) {
            RecordSet resultSet = resultRecord(0);
            QJsonObject userData;

            // 提取用户数据（第0行）
            userData["portrait"] = resultSet(0, "portrait");
            userData["account"] = resultSet(0, "account");
            userData["id"] = resultSet(0, "id");
            userData["nickname"] = resultSet(0, "nickname");
            userData["gender"] = resultSet(0, "gender").toInt();
            userData["birthday"] = resultSet(0, "birthday");
            userData["area_province"] = resultSet(0, "area_province");
            userData["area_city"] = resultSet(0, "area_city");
            userData["signature"] = resultSet(0, "signature");

            // 构造成功响应
            QJsonObject response;
            response["status"] = "success";
            response["data"] = userData;
            session->replyBytes(QJsonDocument(response).toJson(), "application/json");

            QLOG_INFO() << "User logged in:" << account;
            qDebug() << "User logged in:" << account;
            return true;
        }

        // 认证失败处理
        session->replyBytes(SResult::failure(SResultCode::UserUnauthorized), "application/json");
        QLOG_WARN() << "Login failed for account:" << account;
        qWarning() << "Login failed for account:" << account;
        return false;

    } catch (const std::exception &e) {
        QLOG_ERROR() << "Exception in onApiLogin:" << e.what();
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError), "application/json");
        return false;
    }
}

bool KuGouServer::onApiRegister(const QPointer<JQHttpServer::Session> &session) {
    try {
        CheckJsonParse(session);

        QJsonObject reqObj = requestDoc.object();
        QString account = reqObj.value("account").toString();
        QString password = reqObj.value("password").toString();
        QString nickname = reqObj.value("nickname").toString();

        // 验证必需参数
        if (account.isEmpty() || password.isEmpty()) {
            session->replyBytes(SResult::failure(SResultCode::ParamJsonInvalid), "application/json");
            QLOG_WARN() << "Missing account or password";
            return false;
        }

        // 设置默认昵称（如果未提供）
        if (nickname.isEmpty()) {
            nickname = "新用户";
        }

        // 安全处理输入
        QString safeAccount = account.replace("'", "''");
        QString safePassword = password.replace("'", "''");
        QString safeNickname = nickname.replace("'", "''");

        // 检查账号是否已存在
        QString checkSql = QString(
            "SELECT account FROM user_table WHERE account = '%1';"
        ).arg(safeAccount);

        auto checkResult = m_SqliteDataProvider.execSql(checkSql, "check_account_exists", false);
        if (!checkResult.isEmpty() && checkResult(0).rows() > 0) {
            session->replyBytes(SResult::failure(SResultCode::UserAccountExists), "application/json");
            QLOG_WARN() << "Account already exists:" << account;
            return false;
        }

        // 生成用户ID（使用QUuid）
        QString userId = QUuid::createUuid().toString(QUuid::WithoutBraces);

        // 准备插入SQL
        QString insertSql = QString(
            "INSERT INTO user_table ("
            "portrait, account, password, id, nickname, gender, "
            "birthday, area_province, area_city, signature"
            ") VALUES ("
            "'', '%1', '%2', '%3', '%4', -1, "
            "'', '', '', '');"
        ).arg(safeAccount, safePassword, userId, safeNickname);

        // 执行注册
        m_SqliteDataProvider.execSql(insertSql, "register_user", false);

        // 构造成功响应
        QJsonObject response;
        response["status"] = "success";
        response["data"] = QJsonObject({
            {"account", account},
            {"nickname", nickname},
            {"id", userId}
        });

        session->replyBytes(QJsonDocument(response).toJson(), "application/json");
        QLOG_INFO() << "User registered:" << account;
        return true;

    }
    catch (const std::exception &e) {
        QLOG_ERROR() << "Exception in onApiRegister:" << e.what();
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError), "application/json");
        return false;
    }
}

bool KuGouServer::onApiUserDestroy(const QPointer<JQHttpServer::Session> &session) {
    try {
        CheckJsonParse(session);

        QJsonObject reqObj = requestDoc.object();
        QString account = reqObj.value("account").toString();
        QString password = reqObj.value("password").toString();

        // 验证必需参数
        if (account.isEmpty() || password.isEmpty()) {
            session->replyBytes(SResult::failure(SResultCode::ParamJsonInvalid), "application/json");
            QLOG_WARN() << "Missing account or password for user destroy";
            return false;
        }

        // 安全处理输入
        QString safeAccount = account.replace("'", "''");
        QString safePassword = password.replace("'", "''");

        // 验证账号和密码（防止未授权删除）
        QString verifySql = QString(
            "SELECT id FROM user_table "
            "WHERE account = '%1' AND password = '%2';"
        ).arg(safeAccount, safePassword);

        auto verifyResult = m_SqliteDataProvider.execSql(verifySql, "verify_user_destroy", false);
        if (verifyResult.isEmpty() || verifyResult(0).rows() == 0) {
            session->replyBytes(SResult::failure(SResultCode::UserUnauthorized), "application/json");
            QLOG_WARN() << "Authentication failed for user destroy:" << account;
            return false;
        }

        // 获取用户ID用于后续操作
        QString userId = verifyResult(0)(0, "id");

        // 开始事务（确保数据一致性）
        m_SqliteDataProvider.execSql("BEGIN TRANSACTION;", "begin_transaction", false);

        try {
            // 删除用户相关数据（根据实际需求扩展）
            // 1. 删除用户基本信息
            QString deleteUserSql = QString(
                "DELETE FROM user_table "
                "WHERE account = '%1' AND password = '%2';"
            ).arg(safeAccount, safePassword);
            m_SqliteDataProvider.execSql(deleteUserSql, "delete_user", false);

            // 2. 删除用户相关歌曲数据（可选）
            // QString deleteSongsSql = QString(
            //     "DELETE FROM local_song_table WHERE owner_id = '%1';"
            // ).arg(userId);
            // m_SqliteDataProvider.execSql(deleteSongsSql, "delete_user_songs", false);

            // 提交事务
            m_SqliteDataProvider.execSql("COMMIT;", "commit_transaction", false);

            QLOG_INFO() << "User destroyed:" << account;

            // 构造成功响应
            QJsonObject response;
            response["status"] = "success";
            session->replyBytes(QJsonDocument(response).toJson(), "application/json");
            return true;

        } catch (const std::exception& e) {
            // 回滚事务
            m_SqliteDataProvider.execSql("ROLLBACK;", "rollback_transaction", false);
            throw; // 重新抛出异常
        }

    } catch (const std::exception &e) {
        QLOG_ERROR() << "Exception in onApiUserDestroy:" << e.what();
        session->replyBytes(SResult::failure(SResultCode::ServerInnerError), "application/json");
        return false;
    }
}