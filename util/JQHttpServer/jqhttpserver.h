/*
    This file is part of JQLibrary

    Copyright: Jason

    Contact email: 188080501@qq.com

    GNU Lesser General Public License Usage
    Alternatively, this file may be used under the terms of the GNU Lesser
    General Public License version 2.1 or version 3 as published by the Free
    Software Foundation and appearing in the file LICENSE.LGPLv21 and
    LICENSE.LGPLv3 included in the packaging of this file. Please review the
    following information to ensure the GNU Lesser General Public License
    requirements will be met: https://www.gnu.org/licenses/lgpl.html and
    http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*/

#ifndef JQLIBRARY_INCLUDE_JQHTTPSERVER_H_
#define JQLIBRARY_INCLUDE_JQHTTPSERVER_H_

#ifndef QT_NETWORK_LIB
#   error("Please add network in pro file")
#endif

#ifndef QT_CONCURRENT_LIB
#   error("Please add concurrent in pro file")
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

class JQHTTPSERVER_EXPORT Session: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY( Session )

public:
    Session( const QPointer<QIODevice> &socket );

    ~Session();

    void setHandleAcceptedCallback(const std::function<void(const QPointer<Session> &)> &callback) { handleAcceptedCallback_ = callback; }

    QPointer<QIODevice> ioDevice() { return ioDevice_; }

    QString requestSourceIp() const;

    QString requestMethod() const;

    QString requestUrl() const;

    QString requestCrlf() const;

    QMap<QString, QString> requestHeader() const;

    QByteArray requestBody() const;

    QString requestUrlPath() const;

    QStringList requestUrlPathSplitToList() const;

    QMap<QString, QString> requestUrlQuery() const;

    int replyHttpCode() const;

    qint64 replyBodySize() const;

#ifndef QT_NO_SSL
    QSslCertificate peerCertificate() const;
#endif

    volatile int m_isSafeExit;

public slots:
    void replyText(const QString &replyData, const int &httpStatusCode = 200);

    void replyRedirects(const QUrl &targetUrl, const int &httpStatusCode = 200);

    void replyJsonObject(const QJsonObject &jsonObject, const int &httpStatusCode = 200);

    void replyJsonArray(const QJsonArray &jsonArray, const int &httpStatusCode = 200);

    void replyFile(const QString &filePath, const int &httpStatusCode = 200);

    void replyFile(const QString &fileName, const QByteArray &fileData, const int &httpStatusCode = 200);

    void replyImage(const QImage &image, const QString &format = "PNG", const int &httpStatusCode = 200);

    void replyImage(const QString &imageFilePath, const int &httpStatusCode = 200);

    void replyBytes(const QByteArray &bytes, const QString &contentType = "application/octet-stream", const int &httpStatusCode = 200);

    void replyOptions();

private:
    void inspectionBufferSetup1();
    void inspectionBufferSetup2();

    void onBytesWritten(const qint64 &written);
    void waitWorkingForFinished();

private:
    static QAtomicInt remainSession_;

    QPointer<QIODevice>                                   ioDevice_;
    std::function<void( const QPointer<Session> & )>      handleAcceptedCallback_;
    QSharedPointer<QTimer>                                autoCloseTimer_;

    QByteArray receiveBuffer_;

    QString                  requestSourceIp_;
    QString                  requestMethod_;
    QString                  requestUrl_;
    QString                  requestCrlf_;
    QByteArray               requestBody_;
    QMap<QString, QString> requestHeader_;

    bool   headerAcceptedFinished_  = false;
    bool   contentAcceptedFinished_ = false;
    qint64 contentLength_           = -1;

    int        replyHttpCode_ = -1;
    QByteArray replyBuffer_;
    qint64     replyBodySize_ = -1;

    qint64                      waitWrittenByteCount_ = -1;
    QSharedPointer<QIODevice> replyIoDevice_;
};

class JQHTTPSERVER_EXPORT AbstractManage: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY( AbstractManage )

public:
    AbstractManage(const int &handleMaxThreadCount);

    virtual ~AbstractManage();

    void setMainObject(QObject *obj) { m_mainObj = obj; }
    QObject* getMainObject() { return m_mainObj; }

    void setHttpAcceptedCallback(const std::function<void(const QPointer<Session> &session,QObject *mainObj)> &httpAcceptedCallback)
    {
        httpAcceptedCallback_ = httpAcceptedCallback;
    }

    QSharedPointer<QThreadPool> handleThreadPool() { return handleThreadPool_; }

    QSharedPointer<QThreadPool> serverThreadPool() { return serverThreadPool_; }

    virtual bool isRunning() = 0;

protected Q_SLOTS:
    bool initialize();

    void deinitialize();

protected:
    virtual bool onStart() = 0;

    virtual void onFinish() = 0;

    bool startServerThread();

    void stopHandleThread();

    void stopServerThread();

    void newSession(const QPointer<Session> &session);

    void handleAccepted(const QPointer<Session> &session);

signals:
    void readyToClose();
    //void onRedReady(const QPointer<JQHttpServer::Session> &session);

protected:
    QSharedPointer<QThreadPool> serverThreadPool_;
    QSharedPointer<QThreadPool> handleThreadPool_;

    QMutex mutex_;

    std::function<void(const QPointer<Session> &session,QObject *mainObj)> httpAcceptedCallback_;

    QSet<Session *> availableSessions_;

    QObject *m_mainObj;
};

class JQHTTPSERVER_EXPORT TcpServerManage: public AbstractManage
{
    Q_OBJECT
    Q_DISABLE_COPY( TcpServerManage )

public:
    TcpServerManage(const int &handleMaxThreadCount = 2);

    ~TcpServerManage();

    bool listen( const QHostAddress &address, const quint16 &port );
    quint16 getListenPort() { return listenPort_; }

private:
    bool isRunning();

    bool onStart();

    void onFinish();

private:
    QPointer<QTcpServer> tcpServer_;

    QHostAddress listenAddress_ = QHostAddress::Any;
    quint16 listenPort_ = 0;
};

#ifndef QT_NO_SSL
class SslServerHelper;

class JQHTTPSERVER_EXPORT SslServerManage: public AbstractManage
{
    Q_OBJECT
    Q_DISABLE_COPY( SslServerManage )

public:
    SslServerManage(const int &handleMaxThreadCount = 2);

    ~SslServerManage();

    bool listen( const QHostAddress &                                   address,
                 const quint16 &                                        port,
                 const QString &                                        crtFilePath,
                 const QString &                                        keyFilePath,
                 const QList<QPair<QString, QSsl::EncodingFormat>> &caFileList = {},    // [ { filePath, format } ]
                 const QSslSocket::PeerVerifyMode &                     peerVerifyMode = QSslSocket::VerifyNone );
    quint16 getListenPort() { return listenPort_; }

private:
    bool isRunning();

    bool onStart();

    void onFinish();

private:
    QPointer<SslServerHelper> tcpServer_;

    QHostAddress listenAddress_ = QHostAddress::Any;
    quint16      listenPort_    = 0;

    QSharedPointer<QSslConfiguration> sslConfiguration_;
};


enum ServiceConfigEnum
{
    ServiceUnknownConfig,
    ServiceHttpListenPort,
    ServiceHttpsListenPort,
    ServiceProcessor, // QPointer<QObject> or QList<QPointer<QObject>>
    ServiceUuid,
    ServiceSslCrtFilePath,
    ServiceSslKeyFilePath,
    ServiceSslCAFilePath,
    ServiceSslPeerVerifyMode,
};

class JQHTTPSERVER_EXPORT Service: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY( Service )

private:
    enum ReceiveDataType
    {
        UnknownReceiveDataType,
        NoReceiveDataType,
        VariantListReceiveDataType,
        VariantMapReceiveDataType,
        ListVariantMapReceiveDataType,
    };

    struct ApiConfig
    {
        QPointer<QObject> process;
        QString             apiMethod;
        QString             apiName;
        QString             slotName;
        ReceiveDataType     receiveDataType = UnknownReceiveDataType;
    };

    class Recoder
    {
    public:
        Recoder( const QPointer<JQHttpServer::Session> &session );

        ~Recoder();

        QPointer<JQHttpServer::Session> session_;
        QDateTime                         acceptedTime_;
        QString                           serviceUuid_;
        QString                           apiName;
    };

protected:
    Service() = default;

public:
    ~Service() = default;


    static QSharedPointer<Service> createService( const QMap<ServiceConfigEnum, QVariant> &config );


    void registerProcessor( const QPointer<QObject> &processor );


    virtual QJsonDocument extractPostJsonData( const QPointer<JQHttpServer::Session> &session );

    static void reply(
        const QPointer<JQHttpServer::Session> &session,
        const QJsonObject &data,
        const bool &isSucceed = true,
        const QString &message = { },
        const int &httpStatusCode = 200 );

    static void reply(
        const QPointer<JQHttpServer::Session> &session,
        const bool &isSucceed = true,
        const QString &message = { },
        const int &httpStatusCode = 200 );


    virtual void httpGetPing( const QPointer<JQHttpServer::Session> &session );

    virtual void httpGetFaviconIco( const QPointer<JQHttpServer::Session> &session );

    virtual void httpOptions( const QPointer<JQHttpServer::Session> &session );

protected:
    bool initialize( const QMap<ServiceConfigEnum, QVariant> &config );

private:
    void onSessionAccepted( const QPointer<JQHttpServer::Session> &session );


    static QString snakeCaseToCamelCase(const QString &source, const bool &firstCharUpper = false);

    static QList<QVariantMap> variantListToListVariantMap(const QVariantList &source);

private:
    QSharedPointer<JQHttpServer::TcpServerManage> httpServerManage_;
    QSharedPointer<JQHttpServer::SslServerManage> httpsServerManage_;

    QString                                     serviceUuid_;
    QMap<QString, QMap<QString, ApiConfig>> schedules_;    // apiMethod -> apiName -> API
    QMap<QString, std::function<void( const QPointer<JQHttpServer::Session> &session )>> schedules2_; // apiPathPrefix -> callback
    QPointer<QObject> certificateVerifier_;
};
#endif

}

#endif//JQLIBRARY_INCLUDE_JQHTTPSERVER_H_
