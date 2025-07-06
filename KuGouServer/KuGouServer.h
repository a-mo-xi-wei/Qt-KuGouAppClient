/**
 * @file Server.h
 * @brief HTTP 服务器主类头文件，定义了 Server 类的接口。
 *
 * 此文件定义了 Server 类，负责管理数据库、路由及请求处理。
 *
 * @author WeiWang
 * @date 2025-05-12
 */

#ifndef SERVER_H
#define SERVER_H

#include "chttpserver.h"
#include "sqlitedataprovider.h"

class KuGouServer : public QObject , public NetworkFrameManager {
    Q_OBJECT
public:
    /**
     * @brief 构造函数。
     */
    KuGouServer();

private:
    /**
     * @brief 初始化数据库。
     */
    void initDateBase();

    /**
     * @brief 初始化路由。
     */
    void initRouter();

    // 处理 HTTP 请求
    /**
     * @brief 处理 HTTP 接受的请求。
     *
     * @param obj 事件来源对象。
     * @param session HTTP 会话对象。
     * @return bool 是否已处理请求。
     */
    bool OnProcessHttpAccepted(QObject *obj, const QPointer<JQHttpServer::Session> &session) override;

    // 重排索引
    /**
     * @brief 重排歌曲索引。
     */
    void reorderIndex();

private:
    // 字符串类型安全处理
    /**
     * @brief 安全处理字符串，防止 SQL 注入。
     *
     * @param input 输入字符串。
     * @return 安全处理后的字符串。
     */
    static QString safeString(const QString &input);

    // 数值类型安全处理
    /**
     * @brief 安全处理数值类型。
     *
     * @param value 数值。
     * @return 安全处理后的字符串。
     */
    template<typename T>
    static QString safeNumber(T value) {
        static_assert(std::is_arithmetic<T>::value, "Numeric type required");
        return QString::number(value);
    }

    // 处理各种 API
    // 测试
    /**
     * @brief 处理测试 API。
     * @param session HTTP 会话对象。
     * @return bool 操作结果。
     */
    bool onApiTest(const QPointer<JQHttpServer::Session> &session);
    // 版本
    /**
     * @brief 处理版本 API。
     * @param session HTTP 会话对象。
     * @return bool 操作结果。
     */
    bool onApiVersion(const QPointer<JQHttpServer::Session> &session);
    // 获取本地歌曲列表
    /**
     * @brief 处理获取本地歌曲列表 API。
     * @param session HTTP 会话对象。
     * @return bool 操作结果。
     */
    bool onApiLocalSongList(const QPointer<JQHttpServer::Session> &session);
    //搜索建议
    /**
     * @brief 处理获取搜索建议 API。
     * @param session  HTTP 会话对象。
     * @return bool 操作结果。
     */
    bool onApiSearchSuggestion(const QPointer<JQHttpServer::Session> &session);
    // 搜索歌曲
    /**
     * @brief 处理搜索歌曲 API。
     * @param session HTTP 会话对象。
     * @return bool 操作结果。
     */
    bool onApiSearchSong(const QPointer<JQHttpServer::Session> &session);
    // 添加歌曲
    /**
     * @brief 处理添加歌曲 API。
     * @param session HTTP 会话对象。
     * @return bool 操作结果。
     */
    bool onApiAddSong(const QPointer<JQHttpServer::Session> &session);
    // 删除歌曲
    /**
     * @brief 处理删除歌曲 API。
     * @param session HTTP 会话对象。
     * @return bool 操作结果。
     */
    bool onApiDelSong(const QPointer<JQHttpServer::Session> &session);

private:
    /// HTTP 服务器
    chttpserver m_httpserver;
    /// 封装好的 SQLite 数据库操作类
    SqliteDataProvider m_SqliteDataProvider;
    /// 定义路由表
    using ApiHandler = std::function<bool(const QPointer<JQHttpServer::Session> &)>;
    QHash<QString, ApiHandler> apiRouter;
};

#endif //SERVER_H