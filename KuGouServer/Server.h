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
    // 字符串类型安全处理
    static QString safeString(const QString &input) {
        // 创建输入字符串的副本
        QString escaped = input;

        // 使用QString参数进行替换
        escaped.replace(QStringLiteral("'"), QStringLiteral("''"));

        // 包裹结果在单引号中
        return QString("'%1'").arg(escaped);
    }

    // 数值类型安全处理
    template<typename T>
    static QString safeNumber(T value) {
        static_assert(std::is_arithmetic<T>::value, "Numeric type required");
        return QString::number(value);
    }
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