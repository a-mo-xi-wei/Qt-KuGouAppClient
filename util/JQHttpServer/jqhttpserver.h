/**
 * @file jqhttpserver.h
 * @brief JQHttpServer 头文件，包含 HTTP 服务器的类和枚举定义。
 *
 * 此文件是 JQLibrary 的一部分，提供了 HTTP 服务器的实现。
 *
 * @author Jason
 * @contact 188080501@qq.com
 * @copyright GNU Lesser General Public License
 */

#ifndef JQLIBRARY_INCLUDE_JQHTTPSERVER_H_
#define JQLIBRARY_INCLUDE_JQHTTPSERVER_H_

#ifndef QT_NETWORK_LIB
#   error("请在CMake文件中添加network模块")
#endif

#ifndef QT_CONCURRENT_LIB
#   error("请在CMake文件中添加concurrent模块")
#endif

#if defined(JQHTTPSERVER_LIBRARY)
#define JQHTTPSERVER_EXPORT Q_DECL_EXPORT
#else
#define JQHTTPSERVER_EXPORT Q_DECL_IMPORT
#endif

// C++ lib import
#include <functional>

// Qt lib import
#include <QVector>
#include <QFuture>
#include <QMutex>
#include <QUrl>
#ifndef QT_NO_SSL
#   include <QSslCertificate>
#   include <QSslSocket>
#endif

#include "jqdeclare.hpp"

class QIODevice;
class QThreadPool;
class QHostAddress;
class QTimer;
class QImage;
class QTcpServer;
class QLocalServer;
class QSslKey;
class QSslConfiguration;

namespace JQHttpServer
{

/**
 * @class Session
 * @brief HTTP会话类，用于处理单个HTTP请求/响应周期
 * @details 管理HTTP请求解析、响应生成及网络通信
 */
class JQHTTPSERVER_EXPORT Session: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY( Session )

public:
    /**
     * @brief 构造函数。
     *
     * @param socket IO 设备指针。
     */
    Session( const QPointer<QIODevice> &socket );

    /**
     * @brief 安全退出标志位
     * @details 原子变量，用于标记是否处于安全退出状态
     */
    ~Session();

    /**
     * @brief 设置处理接受的回调函数。
     *
     * @param callback 回调函数。
     */
    void setHandleAcceptedCallback(const std::function<void(const QPointer<Session> &)> &callback) { handleAcceptedCallback_ = callback; }

    /**
     * @brief 获取 IO 设备指针。
     *
     * @return IO 设备指针。
     */
    QPointer<QIODevice> ioDevice() { return ioDevice_; }

    /**
     * @brief 获取请求源 IP。
     *
     * @return 请求源 IP。
     */
    QString requestSourceIp() const;

    /**
     * @brief 获取请求方法。
     *
     * @return 请求方法。
     */
    QString requestMethod() const;

    /**
     * @brief 获取请求 URL。
     *
     * @return 请求 URL。
     */
    QString requestUrl() const;

    /**
     * @brief 获取请求 CRLF。
     *
     * @return 请求 CRLF。
     */
    QString requestCrlf() const;

    /**
     * @brief 获取请求头。
     *
     * @return 请求头。
     */
    QMap<QString, QString> requestHeader() const;

    /**
     * @brief 获取请求体。
     *
     * @return 请求体。
     */
    QByteArray requestBody() const;

    /**
     * @brief 获取请求 URL 路径。
     *
     * @return 请求 URL 路径。
     */
    QString requestUrlPath() const;

    /**
     * @brief 获取请求 URL 路径分割列表。
     *
     * @return 请求 URL 路径分割列表。
     */
    QStringList requestUrlPathSplitToList() const;

    /**
     * @brief 获取请求 URL 查询参数。
     *
     * @return 请求 URL 查询参数。
     */
    QMap<QString, QString> requestUrlQuery() const;

    /**
     * @brief 获取回复的 HTTP 代码。
     *
     * @return 回复的 HTTP 代码。
     */
    int replyHttpCode() const;

    /**
     * @brief 获取回复的主体大小。
     *
     * @return 回复的主体大小。
     */
    qint64 replyBodySize() const;

#ifndef QT_NO_SSL
    /**
     * @brief 获取对等证书。
     *
     * @return 对等证书。
     */
    QSslCertificate peerCertificate() const;
#endif

    volatile int m_isSafeExit;

public slots:
    /**
     * @brief 回复文本。
     *
     * @param replyData 回复数据。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyText(const QString &replyData, const int &httpStatusCode = 200);

    /**
     * @brief 回复重定向。
     *
     * @param targetUrl 目标 URL。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyRedirects(const QUrl &targetUrl, const int &httpStatusCode = 200);

    /**
     * @brief 回复 JSON 对象。
     *
     * @param jsonObject JSON 对象。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyJsonObject(const QJsonObject &jsonObject, const int &httpStatusCode = 200);

    /**
     * @brief 回复 JSON 数组。
     *
     * @param jsonArray JSON 数组。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyJsonArray(const QJsonArray &jsonArray, const int &httpStatusCode = 200);

    /**
     * @brief 回复文件。
     *
     * @param filePath 文件路径。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyFile(const QString &filePath, const int &httpStatusCode = 200);

    /**
     * @brief 回复文件。
     *
     * @param fileName 文件名。
     * @param fileData 文件数据。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyFile(const QString &fileName, const QByteArray &fileData, const int &httpStatusCode = 200);

    /**
     * @brief 回复图像。
     *
     * @param image 图像。
     * @param format 格式。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyImage(const QImage &image, const QString &format = "PNG", const int &httpStatusCode = 200);

    /**
     * @brief 回复图像。
     *
     * @param imageFilePath 图像文件路径。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyImage(const QString &imageFilePath, const int &httpStatusCode = 200);

    /**
     * @brief 回复字节。
     *
     * @param bytes 字节。
     * @param contentType 内容类型。
     * @param httpStatusCode HTTP 状态码。
     */
    void replyBytes(const QByteArray &bytes, const QString &contentType = "application/octet-stream", const int &httpStatusCode = 200);

    /**
     * @brief 回复选项。
     */
    void replyOptions();

private:
    /**
     * @brief 检查缓冲区设置 1。
     */
    void inspectionBufferSetup1();

    /**
     * @brief 检查缓冲区设置 2。
     */
    void inspectionBufferSetup2();

    /**
     * @brief 处理字节写入。
     *
     * @param written 写入的字节数。
     */
    void onBytesWritten(const qint64 &written);

    /**
     * @brief 等待工作完成。
     */
    void waitWorkingForFinished();

private:
    /// 当前存活的会话数统计
    static QAtomicInt remainSession_;

    /// 网络IO设备指针
    QPointer<QIODevice>                                   ioDevice_;

    /// 会话接受回调函数
    std::function<void( const QPointer<Session> & )>      handleAcceptedCallback_;

    /// 自动关闭定时器
    QSharedPointer<QTimer>                                autoCloseTimer_;

    /// 接收缓冲区
    QByteArray receiveBuffer_;

    // 请求相关字段
    QString                  requestSourceIp_;   ///< 客户端IP地址
    QString                  requestMethod_;     ///< HTTP请求方法
    QString                  requestUrl_;        ///< 完整请求URL
    QString                  requestCrlf_;       ///< 换行符类型(CRLF/LF)
    QByteArray               requestBody_;       ///< 请求体原始数据
    QMap<QString, QString> requestHeader_;     ///< 请求头键值对

    /// 头部解析完成标记
    bool   headerAcceptedFinished_  = false;

    /// 内容接收完成标记
    bool   contentAcceptedFinished_ = false;

    /// 内容长度(根据Content-Length头)
    qint64 contentLength_           = -1;

    // 响应相关字段
    int        replyHttpCode_ = -1;   ///< HTTP状态码
    QByteArray replyBuffer_;          ///< 响应缓冲区
    qint64     replyBodySize_ = -1;   ///< 响应体大小

    /// 等待写入的字节数
    qint64                      waitWrittenByteCount_ = -1;

    /// 响应IO设备指针(用于文件传输等)
    QSharedPointer<QIODevice> replyIoDevice_;
};

/**
 * @class AbstractManage
 * @brief 服务器管理抽象基类
 * @details 提供线程池管理、会话处理等基础功能
 */
class JQHTTPSERVER_EXPORT AbstractManage: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY( AbstractManage )

public:
    /**
     * @brief 构造函数。
     *
     * @param handleMaxThreadCount 处理线程的最大数量。
     */
    AbstractManage(const int &handleMaxThreadCount);

    /**
     * @brief 虚析构函数。
     */
    virtual ~AbstractManage();

    /**
     * @brief 设置主对象。
     *
     * @param obj 主对象。
     */
    void setMainObject(QObject *obj) { m_mainObj = obj; }

    /**
     * @brief 获取主对象。
     *
     * @return 主对象。
     */
    QObject* getMainObject() { return m_mainObj; }

    /**
     * @brief 设置 HTTP 接受回调。
     *
     * @param httpAcceptedCallback HTTP 接受回调。
     */
    void setHttpAcceptedCallback(const std::function<void(const QPointer<Session> &session,QObject *mainObj)> &httpAcceptedCallback)
    {
        httpAcceptedCallback_ = httpAcceptedCallback;
    }

    /**
     * @brief 获取处理线程池。
     *
     * @return 处理线程池。
     */
    QSharedPointer<QThreadPool> handleThreadPool() { return handleThreadPool_; }

    /**
     * @brief 获取服务器线程池。
     *
     * @return 服务器线程池。
     */
    QSharedPointer<QThreadPool> serverThreadPool() { return serverThreadPool_; }

    /**
     * @brief 检查是否正在运行。
     *
     * @return 是否正在运行。
     */
    virtual bool isRunning() = 0;

protected Q_SLOTS:
    /**
     * @brief 初始化。
     *
     * @return 是否成功。
     */
    bool initialize();

    /**
     * @brief 反初始化。
     */
    void deinitialize();

protected:
    /**
     * @brief 开始时调用。
     *
     * @return 是否成功。
     */
    virtual bool onStart() = 0;

    /**
     * @brief 完成时调用。
     */
    virtual void onFinish() = 0;

    /**
     * @brief 启动服务器线程。
     *
     * @return 是否成功。
     */
    bool startServerThread();

    /**
     * @brief 停止处理线程。
     */
    void stopHandleThread();

    /**
     * @brief 停止服务器线程。
     */
    void stopServerThread();

    /**
     * @brief 创建新会话。
     *
     * @param session 会话。
     */
    void newSession(const QPointer<Session> &session);

    /**
     * @brief 处理接受的会话。
     *
     * @param session 会话。
     */
    void handleAccepted(const QPointer<Session> &session);

signals:
    /**
     * @brief 准备关闭信号。
     */
    void readyToClose();

protected:
    /// 服务器线程池(用于接受连接)
    QSharedPointer<QThreadPool> serverThreadPool_;

    /// 处理线程池(用于业务逻辑)
    QSharedPointer<QThreadPool> handleThreadPool_;

    /// 线程同步互斥锁
    QMutex mutex_;

    /// HTTP请求接受回调函数
    std::function<void(const QPointer<Session> &session,QObject *mainObj)> httpAcceptedCallback_;

    /// 当前活跃会话集合
    QSet<Session *> availableSessions_;

    /// 主业务逻辑对象指针
    QObject *m_mainObj;
};

/**
 * @class TcpServerManage
 * @brief TCP协议HTTP服务器管理类
 * @details 实现基于TCP的HTTP服务器管理功能
 */
class JQHTTPSERVER_EXPORT TcpServerManage: public AbstractManage
{
    Q_OBJECT
    Q_DISABLE_COPY( TcpServerManage )

public:
    /**
     * @brief 构造函数。
     *
     * @param handleMaxThreadCount 处理线程的最大数量。
     */
    TcpServerManage(const int &handleMaxThreadCount = 2);

    /**
     * @brief 析构函数。
     */
    ~TcpServerManage();

    /**
     * @brief 监听。
     *
     * @param address 地址。
     * @param port 端口。
     * @return 是否成功。
     */
    bool listen( const QHostAddress &address, const quint16 &port );

    /**
     * @brief 获取监听端口。
     *
     * @return 监听端口。
     */
    quint16 getListenPort() { return listenPort_; }

private:
    /**
     * @brief 检查是否正在运行。
     *
     * @return 是否正在运行。
     */
    bool isRunning();

    /**
     * @brief 开始时调用。
     *
     * @return 是否成功。
     */
    bool onStart();

    /**
     * @brief 完成时调用。
     */
    void onFinish();

private:
    /// TCP服务器实例指针
    QPointer<QTcpServer> tcpServer_;

    /// 监听地址
    QHostAddress listenAddress_ = QHostAddress::Any;

    /// 监听端口号
    quint16 listenPort_ = 0;
};

#ifndef QT_NO_SSL
/**
 * @class SslServerHelper
 * @brief SSL 服务器助手类。
 */
class SslServerHelper;

/**
 * @class SslServerManage
 * @brief SSL 服务器管理类，继承自 AbstractManage。
 */
class JQHTTPSERVER_EXPORT SslServerManage: public AbstractManage
{
    Q_OBJECT
    Q_DISABLE_COPY( SslServerManage )

public:
    /**
     * @brief 构造函数。
     *
     * @param handleMaxThreadCount 处理线程的最大数量。
     */
    SslServerManage(const int &handleMaxThreadCount = 2);

    /**
     * @brief 析构函数。
     */
    ~SslServerManage();

    /**
     * @brief 监听。
     *
     * @param address 地址。
     * @param port 端口。
     * @param crtFilePath 证书文件路径。
     * @param keyFilePath 密钥文件路径。
     * @param caFileList CA 文件列表。
     * @param peerVerifyMode 对等验证模式。
     * @return 是否成功。
     */
    bool listen( const QHostAddress &address,
                 const quint16 &port,
                 const QString &crtFilePath,
                 const QString &keyFilePath,
                 const QList<QPair<QString, QSsl::EncodingFormat>> &caFileList = {},    // [ { filePath, format } ]
                 const QSslSocket::PeerVerifyMode &peerVerifyMode = QSslSocket::VerifyNone );

    /**
     * @brief 获取监听端口。
     *
     * @return 监听端口。
     */
    quint16 getListenPort() { return listenPort_; }

private:
    /**
     * @brief 检查是否正在运行。
     *
     * @return 是否正在运行。
     */
    bool isRunning();

    /**
     * @brief 开始时调用。
     *
     * @return 是否成功。
     */
    bool onStart();

    /**
 * @enum ServiceConfigEnum
 * @brief 服务配置枚举类型
 * @details 定义服务初始化时的配置项类型
     */
    void onFinish();

private:
    QPointer<SslServerHelper> tcpServer_;

    QHostAddress listenAddress_ = QHostAddress::Any;
    quint16 listenPort_ = 0;

    QSharedPointer<QSslConfiguration> sslConfiguration_;
};


enum ServiceConfigEnum
{
    ServiceUnknownConfig,         ///< 未知配置项
    ServiceHttpListenPort,        ///< HTTP监听端口
    ServiceHttpsListenPort,       ///< HTTPS监听端口
    ServiceProcessor,             ///< 业务处理对象(QObject指针或列表)
    ServiceUuid,                  ///< 服务唯一标识
    ServiceSslCrtFilePath,        ///< SSL证书文件路径
    ServiceSslKeyFilePath,        ///< SSL密钥文件路径
    ServiceSslCAFilePath,         ///< CA证书文件路径
    ServiceSslPeerVerifyMode,     ///< 客户端验证模式
};

/**
 * @class Service
 * @brief HTTP服务综合管理类
 * @details 整合HTTP/HTTPS服务，提供路由注册、请求处理等功能
 */
class JQHTTPSERVER_EXPORT Service: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY( Service )

private:
    /**
     * @enum ReceiveDataType
     * @brief 请求体数据类型枚举
     * @details 定义POST请求中接收数据的格式类型
     */
    enum ReceiveDataType
    {
        UnknownReceiveDataType,        ///< 未知数据类型
        NoReceiveDataType,              ///< 无请求体数据
        VariantListReceiveDataType,     ///< QVariantList类型数据
        VariantMapReceiveDataType,      ///< QVariantMap类型数据
        ListVariantMapReceiveDataType,  ///< QList<QVariantMap>类型数据
    };

    /**
     * @struct ApiConfig
     * @brief API接口配置结构体
     * @details 存储单个API接口的元数据配置
     */
    struct ApiConfig
    {
        QPointer<QObject> process;      ///< 处理请求的QObject对象指针
        QString           apiMethod;    ///< HTTP请求方法（GET/POST等）
        QString           apiName;     ///< API接口名称（路由路径）
        QString           slotName;     ///< 处理槽函数名称
        ReceiveDataType   receiveDataType = UnknownReceiveDataType; ///< 请求体数据类型
    };

    /**
     * @class Recoder
     * @brief 请求记录器类
     * @details 用于记录HTTP请求的上下文信息
     */
    class Recoder
    {
    public:
        /**
         * @brief 构造函数
         * @param session 关联的HTTP会话对象
         */
        Recoder( const QPointer<JQHttpServer::Session> &session );

        /**
         * @brief 析构函数
         * @details 自动记录请求处理时长
         */
        ~Recoder();

        QPointer<JQHttpServer::Session> session_;   ///< 关联的会话对象指针
        QDateTime                       acceptedTime_; ///< 请求接收时间
        QString                         serviceUuid_; ///< 服务唯一标识
        QString                         apiName;     ///< 调用的API名称
    };

protected:
    /**
     * @brief 默认构造函数
     * @details 保护类型，需通过createService工厂方法创建实例
     */
    Service() = default;

public:
    ~Service() = default;

    /**
     * @brief 创建服务实例的工厂方法
     * @param config 服务配置参数表
     * @return 共享指针包装的服务实例
     */
    static QSharedPointer<Service> createService( const QMap<ServiceConfigEnum, QVariant> &config );

    /**
     * @brief 注册业务处理器
     * @param processor 包含业务处理槽函数的QObject对象
     * @details 自动扫描processor的槽函数进行路由注册
     */
    void registerProcessor( const QPointer<QObject> &processor );

    /**
     * @brief 提取POST请求中的JSON数据
     * @param session HTTP会话对象
     * @return 解析后的JSON文档对象
     */
    virtual QJsonDocument extractPostJsonData( const QPointer<JQHttpServer::Session> &session );

    /**
     * @brief 发送JSON格式响应（带数据内容）
     * @param session HTTP会话对象
     * @param data 要返回的JSON数据
     * @param isSucceed 业务处理是否成功
     * @param message 附加消息
     * @param httpStatusCode HTTP状态码
     */
    static void reply(
        const QPointer<JQHttpServer::Session> &session,
        const QJsonObject &data,
        const bool &isSucceed = true,
        const QString &message = { },
        const int &httpStatusCode = 200 );

    /**
     * @brief 发送JSON格式响应（无数据内容）
     * @param session HTTP会话对象
     * @param isSucceed 业务处理是否成功
     * @param message 附加消息
     * @param httpStatusCode HTTP状态码
     */
    static void reply(
        const QPointer<JQHttpServer::Session> &session,
        const bool &isSucceed = true,
        const QString &message = { },
        const int &httpStatusCode = 200 );

    /**
     * @brief 处理GET /ping请求
     * @param session HTTP会话对象
     * @details 默认返回pong响应
     */
    virtual void httpGetPing( const QPointer<JQHttpServer::Session> &session );

    /**
     * @brief 处理GET /favicon.ico请求
     * @param session HTTP会话对象
     * @details 默认返回404状态
     */
    virtual void httpGetFaviconIco( const QPointer<JQHttpServer::Session> &session );

    /**
     * @brief 处理OPTIONS请求
     * @param session HTTP会话对象
     * @details 默认实现CORS支持
     */
    virtual void httpOptions( const QPointer<JQHttpServer::Session> &session );

protected:
    /**
     * @brief 初始化服务
     * @param config 服务配置参数表
     * @return 是否初始化成功
     */
    bool initialize( const QMap<ServiceConfigEnum, QVariant> &config );

private:
    /**
     * @brief 处理新会话接入
     * @param session HTTP会话对象
     * @details 路由请求到注册的处理函数
     */
    void onSessionAccepted( const QPointer<JQHttpServer::Session> &session );

    /**
     * @brief 蛇形命名转驼峰命名
     * @param source 原始字符串
     * @param firstCharUpper 首字母是否大写
     * @return 转换后的字符串
     */
    static QString snakeCaseToCamelCase(const QString &source, const bool &firstCharUpper = false);

    /**
     * @brief QVariantList转QList<QVariantMap>
     * @param source 原始数据列表
     * @return 转换后的数据结构
     */
    static QList<QVariantMap> variantListToListVariantMap(const QVariantList &source);

private:
    /// HTTP服务器管理实例
    QSharedPointer<JQHttpServer::TcpServerManage> httpServerManage_;
    /// HTTPS服务器管理实例
    QSharedPointer<JQHttpServer::SslServerManage> httpsServerManage_;
    /// 服务唯一标识
    QString                                     serviceUuid_;
    /// 路由配置映射表(方法->路径->配置)
    QMap<QString, QMap<QString, ApiConfig>> schedules_;    // apiMethod -> apiName -> API
    /// 路径前缀回调映射
    QMap<QString, std::function<void( const QPointer<JQHttpServer::Session> &session )>> schedules2_; // apiPathPrefix -> callback
    /// 证书验证器对象指针
    QPointer<QObject> certificateVerifier_;
};
#endif

}

#endif//JQLIBRARY_INCLUDE_JQHTTPSERVER_H_