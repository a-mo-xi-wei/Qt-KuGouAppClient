#ifndef LIBHTTP_H
#define LIBHTTP_H

#include <QNetworkReply>
#include <QHttpPart>

#if defined(MYHTTPCLIENT_LIBRARY)
#define LIBHTTP_EXPORT Q_DECL_EXPORT
#else
#define LIBHTTP_EXPORT Q_DECL_IMPORT
#endif

class LIBHTTP_EXPORT CLibhttp : public QObject
{
    Q_OBJECT

public:
    explicit CLibhttp(QObject *parent = nullptr);
    ~CLibhttp() override;

    /// 向一个http发送一个post请求
    Q_INVOKABLE QString UrlRequestPost(const QString& url,const QString &data,int timeout=500000);
    /// 向一个http发送一个get请求
    Q_INVOKABLE QString UrlRequestGet(const QString& url,const QString& data,int timeout=500000);
    /// http上传一个文件
    Q_INVOKABLE bool UploadFile(const QString& strServerUrl,const QString& srcFilePath,int timeout=500000);
    /// 从http上下载一个文件
    Q_INVOKABLE bool DownloadFile(const QString& strServerUrl,const QString& decFilePath,int timeout=500000);

signals:
    void httpProgress(qint64 bytesSent,qint64 bytesTotal);

    void httpTimeout();

public slots:
    void Progress(qint64 bytesSent,qint64 bytesTotal);
    void Error(QNetworkReply::NetworkError reply);
};

#endif // LIBHTTP_H
