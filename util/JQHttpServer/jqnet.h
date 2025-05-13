/**
 * @file jqnet.h
 * @brief JQNet 头文件，包含网络相关的实用函数和类。
 *
 * 此文件是 JQLibrary 的一部分，提供了网络地址、主机名、HTTP 请求等实用功能。
 *
 * @author Jason
 * @contact 188080501@qq.com
 * @copyright GNU Lesser General Public License
 */

#ifndef JQLIBRARY_INCLUDE_JQNET_H_
#define JQLIBRARY_INCLUDE_JQNET_H_

#ifndef QT_NETWORK_LIB
#   error("Please add network in pro file")
#endif

// C++ lib import
#include <functional>

// Qt lib import
#include <QSharedPointer>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QNetworkAddressEntry>

#include "jqdeclare.hpp"

namespace JQNet
{

/**
 * @brief 获取第一个网络地址条目。
 *
 * @return 第一个网络地址条目。
 */
QNetworkAddressEntry getFirstNetworkAddressEntry();

/**
 * @brief 获取第一个网络地址条目和接口。
 *
 * @param ridVm 是否排除虚拟机接口。
 * @return 第一个网络地址条目和接口。
 */
QPair< QNetworkAddressEntry, QNetworkInterface > getFirstNetworkAddressEntryAndInterface(const bool &ridVm = true);

/**
 * @brief 获取网络地址条目和接口列表。
 *
 * @param ridVm 是否排除虚拟机接口。
 * @return 网络地址条目和接口列表。
 */
QList< QPair< QNetworkAddressEntry, QNetworkInterface > > getNetworkAddressEntryAndInterface(const bool &ridVm = true);

/**
 * @brief 获取主机名。
 *
 * @return 主机名。
 */
QString getHostName();

/**
 * @brief 检查 TCP 是否可达。
 *
 * @param hostName 主机名。
 * @param port 端口。
 * @param timeout 超时时间。
 * @return 是否可达。
 */
bool tcpReachable(const QString &hostName, const quint16 &port, const int &timeout = 5000);

#ifdef JQFOUNDATION_LIB
/**
 * @brief 检查 ping 是否可达。
 *
 * @param address 地址。
 * @param timeout 超时时间。
 * @return 是否可达。
 */
bool pingReachable(const QString &address, const int &timeout = 300);
#endif

/**
 * @class HTTP
 * @brief HTTP 客户端类，提供了 HTTP 请求的封装。
 */
class HTTP
{
    Q_DISABLE_COPY( HTTP )

public:
    /**
     * @brief 构造函数。
     */
    HTTP() = default;

    /**
     * @brief 析构函数。
     */
    ~HTTP() = default;

public:
    /**
     * @brief 获取网络访问管理器。
     *
     * @return 网络访问管理器。
     */
    inline QNetworkAccessManager &manage() { return manage_; }

    /**
     * @brief 执行 HTTP GET 请求。
     *
     * @param request 网络请求。
     * @param receiveBuffer 接收缓冲区。
     * @param timeout 超时时间。
     * @return 是否成功。
     */
    bool get(
            const QNetworkRequest &request,
            QByteArray &receiveBuffer,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP GET 请求（异步）。
     *
     * @param request 网络请求。
     * @param onFinished 完成回调。
     * @param onError 错误回调。
     * @param timeout 超时时间。
     */
    void get(
            const QNetworkRequest &request,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QByteArray &data) > &onFinished,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QNetworkReply::NetworkError &code, const QByteArray &data) > &onError,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP DELETE 请求。
     *
     * @param request 网络请求。
     * @param receiveBuffer 接收缓冲区。
     * @param timeout 超时时间。
     * @return 是否成功。
     */
    bool deleteResource(
            const QNetworkRequest &request,
            QByteArray &receiveBuffer,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP DELETE 请求（异步）。
     *
     * @param request 网络请求。
     * @param onFinished 完成回调。
     * @param onError 错误回调。
     * @param timeout 超时时间。
     */
    void deleteResource(
            const QNetworkRequest &request,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QByteArray &data) > &onFinished,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QNetworkReply::NetworkError &code, const QByteArray &data) > &onError,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP POST 请求。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param receiveRawHeaderPairs 接收的原始头对。
     * @param receiveBuffer 接收缓冲区。
     * @param timeout 超时时间。
     * @return 是否成功。
     */
    bool post(
            const QNetworkRequest &request,
            const QByteArray &body,
            QList< QNetworkReply::RawHeaderPair > &receiveRawHeaderPairs,
            QByteArray &receiveBuffer,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP POST 请求（多部分）。
     *
     * @param request 网络请求。
     * @param multiPart 多部分指针。
     * @param receiveBuffer 接收缓冲区。
     * @param timeout 超时时间。
     * @return 是否成功。
     */
    bool post(
            const QNetworkRequest &request,
            const QSharedPointer< QHttpMultiPart > &multiPart,
            QByteArray &receiveBuffer,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP POST 请求（异步）。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param onFinished 完成回调。
     * @param onError 错误回调。
     * @param timeout 超时时间。
     */
    void post(
            const QNetworkRequest &request,
            const QByteArray &body,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QByteArray &data) > &onFinished,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QNetworkReply::NetworkError &code, const QByteArray &data) > &onError,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP PUT 请求。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param receiveBuffer 接收缓冲区。
     * @param timeout 超时时间。
     * @return 是否成功。
     */
    bool put(
            const QNetworkRequest &request,
            const QByteArray &body,
            QByteArray &receiveBuffer,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP PUT 请求（多部分）。
     *
     * @param request 网络请求。
     * @param multiPart 多部分指针。
     * @param receiveBuffer 接收缓冲区。
     * @param timeout 超时时间。
     * @return 是否成功。
     */
    bool put(
            const QNetworkRequest &request,
            const QSharedPointer< QHttpMultiPart > &multiPart,
            QByteArray &receiveBuffer,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP PUT 请求（异步）。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param onFinished 完成回调。
     * @param onError 错误回调。
     * @param timeout 超时时间。
     */
    void put(
            const QNetworkRequest &request,
            const QByteArray &body,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QByteArray &data) > &onFinished,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QNetworkReply::NetworkError &code, const QByteArray &data) > &onError,
            const int &timeout = 30 * 1000
        );

#if !( defined Q_OS_LINUX ) && ( QT_VERSION >= QT_VERSION_CHECK( 5, 9, 0 ) )
    /**
     * @brief 执行 HTTP PATCH 请求。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param receiveBuffer 接收缓冲区。
     * @param timeout 超时时间。
     * @return 是否成功。
     */
    bool patch(
            const QNetworkRequest &request,
            const QByteArray &body,
            QByteArray &receiveBuffer,
            const int &timeout = 30 * 1000
        );

    /**
     * @brief 执行 HTTP PATCH 请求（异步）。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param onFinished 完成回调。
     * @param onError 错误回调。
     * @param timeout 超时时间。
     */
    void patch(
            const QNetworkRequest &request,
            const QByteArray &body,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QByteArray &data) > &onFinished,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QNetworkReply::NetworkError &code, const QByteArray &data) > &onError,
            const int &timeout = 30 * 1000
        );
#endif

    /**
     * @brief 执行 HTTP GET 请求（静态）。
     *
     * @param url URL。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > get(const QString &url, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP GET 请求（静态）。
     *
     * @param request 网络请求。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > get(const QNetworkRequest &request, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP DELETE 请求（静态）。
     *
     * @param url URL。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > deleteResource(const QString &url, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP DELETE 请求（静态）。
     *
     * @param request 网络请求。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > deleteResource(const QNetworkRequest &request, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP POST 请求（静态）。
     *
     * @param url URL。
     * @param body 请求体。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > post(const QString &url, const QByteArray &body, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP POST 请求（静态）。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > post(const QNetworkRequest &request, const QByteArray &body, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP POST 请求（静态，带原始头对）。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QPair< QList< QNetworkReply::RawHeaderPair >, QByteArray > > post2(const QNetworkRequest &request, const QByteArray &body, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP POST 请求（静态，多部分）。
     *
     * @param request 网络请求。
     * @param multiPart 多部分指针。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > post(const QNetworkRequest &request, const QSharedPointer< QHttpMultiPart > &multiPart, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP PUT 请求（静态）。
     *
     * @param url URL。
     * @param body 请求体。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > put(const QString &url, const QByteArray &body, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP PUT 请求（静态）。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > put(const QNetworkRequest &request, const QByteArray &body, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP PUT 请求（静态，多部分）。
     *
     * @param request 网络请求。
     * @param multiPart 多部分指针。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > put(const QNetworkRequest &request, const QSharedPointer< QHttpMultiPart > &multiPart, const int &timeout = 30 * 1000);

#if !( defined Q_OS_LINUX ) && ( QT_VERSION >= QT_VERSION_CHECK( 5, 9, 0 ) )
    /**
     * @brief 执行 HTTP PATCH 请求（静态）。
     *
     * @param url URL。
     * @param body 请求体。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > patch(const QString &url, const QByteArray &body, const int &timeout = 30 * 1000);

    /**
     * @brief 执行 HTTP PATCH 请求（静态）。
     *
     * @param request 网络请求。
     * @param body 请求体。
     * @param timeout 超时时间。
     * @return 结果和数据。
     */
    static QPair< bool, QByteArray > patch(const QNetworkRequest &request, const QByteArray &body, const int &timeout = 30 * 1000);
#endif

private:
    void handle(
            QNetworkReply *reply,
            const int &timeout,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QByteArray &data) > &onFinished,
            const std::function< void(const QList< QNetworkReply::RawHeaderPair > &, const QNetworkReply::NetworkError &code, const QByteArray &data) > &onError,
            const std::function< void() > &onTimeout
        );

    QNetworkAccessManager manage_;
};

}

#endif//JQLIBRARY_INCLUDE_JQNET_H_