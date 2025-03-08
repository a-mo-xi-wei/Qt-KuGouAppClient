#include "../../includes/network/libhttp.h"
#include "../../includes/QsLog/QsLog.h"

#include <QEventLoop>
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>
#include <QDateTime>
#include <QStandardPaths>
#include <QProgressBar>
#include <QFileDialog>
#include <QHostAddress>
#include <QTimer>

CLibhttp::CLibhttp(QObject *parent)
{

}

CLibhttp::~CLibhttp(void)
{

}

/**
 * @brief UrlRequestGet 向一个http发送一个get请求
 * @param url 要请求的url
 * @param timeout 连接超时时间
 * @return
 */
QString CLibhttp::UrlRequestGet(const QString url,const QString data,int timeout)
{
    if(url == "" || timeout <= 0 || data == "")
        return "";

    emit signalshowlog("CLibhttp::UrlRequestGet:"+url);

    QTimer timeout_timer;
    QNetworkAccessManager qnam;
    const QUrl aurl(url+data);
    QNetworkRequest qnr(aurl);
    //qnr.setRawHeader("Content-Type","application/x-www-form-urlencoded;charset=utf-8");
    qnr.setRawHeader("Content-Type","application/json;charset=utf-8");

    timeout_timer.setInterval(timeout);
    timeout_timer.setSingleShot(true);

    //qint64 currenttime = QDateTime::currentSecsSinceEpoch();

    QNetworkReply *reply = qnam.get(qnr);
    QEventLoop eventloop;

    QObject::connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    QObject::connect(&timeout_timer,SIGNAL(timeout()),&eventloop,SLOT(quit()));

    timeout_timer.start();
    eventloop.exec();

    if(timeout_timer.isActive()) {
        timeout_timer.stop();

        if(reply->error() != QNetworkReply::NoError) {
            QLOG_ERROR() << "http error:" << reply->errorString();
            emit signalshowlog("CLibhttp::UrlRequestGet:http error:" + reply->errorString());
        }
        else {
            QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();

            //QLOG_INFO()<<"status code:"<<nStatusCode;
            //QLOG_INFO()<<"UrlRequestPost:"<<(QDateTime::currentSecsSinceEpoch()-currenttime);
            emit signalshowlog(QString::asprintf("CLibhttp::UrlRequestGet:status code:%d",nStatusCode));

            QTextCodec *codec = QTextCodec::codecForName("utf8");

            QString replyData = codec->toUnicode(reply->readAll());
            reply->deleteLater();
            reply=NULL;

            return replyData;
        }
    }
    else {
        QObject::disconnect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
        reply->abort();
        reply->deleteLater();
        //QLOG_WARN()<<"timeout";
        emit signalshowlog("CLibhttp::UrlRequestGet:timeout");
    }

    //emit signalshowlog("CLibhttp::UrlRequestGet:finished.");

    return "";
}

/**
 * @brief UrlRequestPost 向一个http发送一个post请求
 * @param url 要请求的url
 * @param data 要发送的http数据
 * @return 如果请求成功返回请求成功的数据，否则返回空
 */
QString CLibhttp::UrlRequestPost(const QString url,const QString data,int timeout)
{
    if(url == "" || data == "" || timeout <= 0)
        return "";

    emit signalshowlog("CLibhttp::UrlRequestPost:"+url);

    QTimer timeout_timer;
    QNetworkAccessManager qnam;
    const QUrl aurl(url);
    QNetworkRequest qnr(aurl);
    //qnr.setRawHeader("Content-Type","application/x-www-form-urlencoded;charset=utf-8");
    qnr.setRawHeader("Content-Type","application/json;charset=utf-8");

    timeout_timer.setInterval(timeout);
    timeout_timer.setSingleShot(true);

    qint64 currenttime = QDateTime::currentSecsSinceEpoch();

    QNetworkReply *reply = qnam.post(qnr,data.toUtf8());
    QEventLoop eventloop;

    QObject::connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    QObject::connect(&timeout_timer,SIGNAL(timeout()),&eventloop,SLOT(quit()));

    timeout_timer.start();
    eventloop.exec();

    if(timeout_timer.isActive()) {
        timeout_timer.stop();

        if(reply->error() != QNetworkReply::NoError) {
            QLOG_ERROR() << "http error:" << reply->errorString();
            emit signalshowlog("CLibhttp::UrlRequestPost:http error:"+reply->errorString());
        }
        else {
            QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();

            //QLOG_INFO()<<"status code:"<<nStatusCode;
            //QLOG_INFO()<<"UrlRequestPost:"<<(QDateTime::currentSecsSinceEpoch()-currenttime);
            emit signalshowlog(QString::asprintf("CLibhttp::UrlRequestPost:status code:%d",nStatusCode));

            QTextCodec *codec = QTextCodec::codecForName("utf8");

            QString replyData = codec->toUnicode(reply->readAll());
            reply->deleteLater();
            reply=NULL;

            return replyData;
        }
    }
    else {
        QObject::disconnect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
        reply->abort();
        reply->deleteLater();
        //QLOG_WARN()<<"timeout";
        emit signalshowlog("CLibhttp::UrlRequestPost:timeout");
    }

    //emit signalshowlog("CLibhttp::UrlRequestPost:finished.");

    return "";
}
/*static QString UrlRequestPost(const QString url,const QString data)
{
    QNetworkAccessManager qnam;
    const QUrl aurl(url);
    QNetworkRequest qnr(aurl);
    //qnr.setRawHeader("Content-Type","application/x-www-form-urlencoded;charset=utf-8");
    qnr.setRawHeader("Content-Type","application/json;charset=utf-8");

    QNetworkReply *reply = qnam.post(qnr,data.toUtf8());
    QEventLoop eventloop;

    QObject::connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));

    eventloop.exec(QEventLoop::ExcludeUserInputEvents);

    QTextCodec *codec = QTextCodec::codecForName("utf8");

    QString replyData = codec->toUnicode(reply->readAll());
    reply->deleteLater();
    reply=NULL;

    return replyData;
}*/

/**
 * @brief CLibhttp::UploadFile http上传一个文件
 * @param strServerUrl 要上传的http服务器路径
 * @param srcFilePath 要上传的文件路径
 * @param timeout 上传文件超时时间
 *
 * @return 如果文件上传成功返回真，否则返回假
 */
bool CLibhttp::UploadFile(const QString strServerUrl,const QString srcFilePath,int timeout)
{
    if(strServerUrl.isEmpty() || srcFilePath.isEmpty())
        return false;

    QNetworkAccessManager manager;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;

    QFileInfo fileInfo(srcFilePath);
    if(!fileInfo.exists())
    {
        QLOG_ERROR()<<"CLibhttp::UploadFile:"<<srcFilePath<<" is not exists.";
        return false;
    }

    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+ fileInfo.fileName() + "\"")); // file为后端定义的key，filename即为excel文件名

    QFile *file = new QFile(srcFilePath);
    if(!file->open(QIODevice::ReadOnly))
    {
        QLOG_ERROR()<<"CLibhttp::UploadFile:"<<srcFilePath<<" open fail.";
        return false;
    }

    filePart.setBodyDevice(file);

    file->setParent(multiPart);
    multiPart->append(filePart);

    QUrl url(strServerUrl);
    QNetworkRequest netReq;
    netReq.setUrl(url);

    QNetworkReply *reply = manager.post(netReq,multiPart);
    //connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(RequestFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(Error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(Progress(qint64 ,qint64)));

    QEventLoop eventloop;
    QObject::connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));

    eventloop.exec(QEventLoop::ExcludeUserInputEvents);

    file->close();
    reply->abort();
    reply->deleteLater();

    return false;
}

/**
 * @brief CLibhttp::DownloadFile 从http上下载一个文件
 * @param strServerUrl 要下载的服务器地址
 * @param decFilePath 保存文件地址
 * @param timeout 超时时间
 *
 * @return 如果文件下载成功返回真，否则返回假
 */
bool CLibhttp::DownloadFile(const QString strServerUrl,const QString decFilePath,int timeout)
{
    if(strServerUrl.isEmpty() || decFilePath.isEmpty())
        return false;

    QTimer timeout_timer;
    QNetworkAccessManager manager;
    QNetworkRequest netReq;
    netReq.setUrl(QUrl(strServerUrl));

    timeout_timer.setInterval(timeout);
    timeout_timer.setSingleShot(true);

    qint64 currenttime = QDateTime::currentSecsSinceEpoch();

    QNetworkReply *reply = manager.get(netReq);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(Error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(Progress(qint64 ,qint64)));

    QEventLoop eventloop;

    QObject::connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    QObject::connect(&timeout_timer,SIGNAL(timeout()),&eventloop,SLOT(quit()));

    timeout_timer.start();
    eventloop.exec(QEventLoop::ExcludeUserInputEvents);

    if(timeout_timer.isActive()) {
        timeout_timer.stop();

        if(reply->error() != QNetworkReply::NoError) {
            QLOG_ERROR() << "http DownloadFile error:" << reply->errorString();
        }
        else {
            QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            int nStatusCode = variant.toInt();

            QLOG_INFO()<<"DownloadFile status code:"<<nStatusCode;
            QLOG_INFO()<<"DownloadFile UrlRequestPost:"<<(QDateTime::currentSecsSinceEpoch()-currenttime);

            QFile precvFile(decFilePath);
            if(!precvFile.open(QIODevice::WriteOnly))
            {
                QLOG_ERROR()<<"CLibhttp::DownloadFile:"<<decFilePath<<" open fail.";
                return false;
            }

            precvFile.write(reply->readAll());
            precvFile.close();

            reply->deleteLater();
            reply=NULL;

            return true;
        }
    }
    else {
        QObject::disconnect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
        reply->abort();
        reply->deleteLater();
        QLOG_WARN()<<"DownloadFile timeout.";
    }

    return false;
}

void CLibhttp::Progress(qint64 bytesSent,qint64 bytesTotal)
{
    emit httpProgress(bytesSent,bytesTotal);
}

void CLibhttp::Error(QNetworkReply::NetworkError reply)
{
    QLOG_ERROR()<<"CLibhttp:"<<reply;
}

