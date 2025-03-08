#include "../../includes/network/chttpserver.h"
//#include "../../includes/QsLog/QsLog.h"
#include "../../includes/common/common.h"

#include <QFileInfo>
#include <QImageReader>
#include <QBuffer>

chttpserver::chttpserver(QObject *parent,bool enablessl)
    : QObject(parent),
      m_NetworkFrameManager(NULL),
      m_serverisRunning(false),
      m_isEnableSSL(enablessl)
{
    m_currentWorkingPath = QCoreApplication::applicationDirPath();

    if(!m_isEnableSSL)
    {
        m_htmlServerManage.setMainObject(this);
        m_htmlServerManage.setHttpAcceptedCallback( std::bind( onHttpAccepted, std::placeholders::_1,std::placeholders::_2) );
    }
    else
    {
#ifndef QT_NO_SSL
        m_htmlSslServerManage.setMainObject(this);
        m_htmlSslServerManage.setHttpAcceptedCallback( std::bind( onHttpAccepted, std::placeholders::_1,std::placeholders::_2) );
#endif
    }
}

chttpserver::~chttpserver()
{

}

/**
 * @brief chttpserver::listen 开始侦听
 * @param port
 * @return
 */
bool chttpserver::listen(int port,QString server_crt,QString server_key)
{
    if(!m_isEnableSSL)
        m_serverisRunning = m_htmlServerManage.listen( QHostAddress::Any,
                                                       port);
    else
#ifndef QT_NO_SSL
        m_serverisRunning = m_htmlSslServerManage.listen(QHostAddress::Any,
                                                         port,
                                                         server_crt,
                                                         server_key);
#endif

    printLog(QsLogging::Level::InfoLevel,
             tr("chttpserver::listen:%1;").arg(port)+
             tr("isEnableSSL:%1;").arg((int)m_isEnableSSL)+
             tr("workingPath:")+m_currentWorkingPath+
             tr(";startup status:%1").arg((int)m_serverisRunning));

    return m_serverisRunning;
}

/**
 * @brief chttpserver::onProcessHttpAccepted 处理http传输过来的数据
 * @param session
 */
void chttpserver::onProcessHttpAccepted(const QPointer< JQHttpServer::Session > &session,QObject *mainObj)
{
    QMutexLocker locker(&m_htmlServerMutex);

    if(onprocesslocalfileload(session) || m_NetworkFrameManager == NULL)
        return;

    if(!m_NetworkFrameManager->OnProcessHttpAccepted(this,session))
    {
        session->replyBytes(QString::fromLocal8Bit("当前页面不存在!").toUtf8(),"text/html; charset=UTF-8");
    }
}

/**
 * @brief chttpserver::onprocesslocalfileload 处理内部文件导入
 * @param session
 */
bool chttpserver::onprocesslocalfileload(const QPointer< JQHttpServer::Session > &session)
{
    QString prequestUrl = session->requestUrl();
    bool isOk = false;

    if(prequestUrl.contains(".js") ||
       prequestUrl.contains(".html") ||
       prequestUrl.contains(".ttf") ||
       prequestUrl.contains(".woff") ||
       prequestUrl.contains(".woff2") ||
       prequestUrl.contains(".css"))
    {
        QString tmpFilePath = m_currentWorkingPath+prequestUrl;
        int pos = tmpFilePath.indexOf("?v=");
        if(pos > 0)
        {
            tmpFilePath = tmpFilePath.mid(0,pos);
        }
        pos = tmpFilePath.indexOf("?t=");
        if(pos > 0)
        {
            tmpFilePath = tmpFilePath.mid(0,pos);
        }

        if(prequestUrl.contains(".js"))
            session->replyBytes(getResourceManager()->getFile(tmpFilePath),
                                "application/javascript; charset=UTF-8");
        else if(prequestUrl.contains(".html"))
            session->replyBytes(getResourceManager()->getFile(tmpFilePath),
                                "text/html; charset=UTF-8");
        else if(prequestUrl.contains(".css"))
            session->replyBytes(getResourceManager()->getFile(tmpFilePath),
                                "text/css; charset=UTF-8");
        else if(prequestUrl.contains(".woff2"))
            session->replyBytes(getResourceManager()->getFile(tmpFilePath),
                                "application/font-woff2;");
        else if(prequestUrl.contains(".woff"))
            session->replyBytes(getResourceManager()->getFile(tmpFilePath),
                                "application/font-woff;");
        else if(prequestUrl.contains(".ttf"))
            session->replyBytes(getResourceManager()->getFile(tmpFilePath),
                                "application/font-sfnt otf ttf;");
        else
        {
            printLog(QsLogging::Level::ErrorLevel,
                     tr("File type not supported:")+tmpFilePath);
        }

        isOk = true;
    }
    else if(prequestUrl.contains(".jpg") ||
            prequestUrl.contains(".gif") ||
            prequestUrl.contains(".png") ||
            prequestUrl.contains(".ico"))
    {
        QFileInfo pFileInfo(m_currentWorkingPath+prequestUrl);
        if(pFileInfo.exists())
        {
            if(prequestUrl.contains(".gif"))
            {
                session->replyBytes(getResourceManager()->getFile(pFileInfo.absoluteFilePath()),
                                    "image/gif");
            }
            else
            {
                QByteArray tmpArray = getResourceManager()->getFile(pFileInfo.absoluteFilePath());
                QBuffer buffer(&tmpArray);
                buffer.open(QIODevice::ReadOnly);

                session->replyImage(QImageReader(&buffer).read(),pFileInfo.suffix());
            }
        }
        else
        {
            if(!pFileInfo.absoluteFilePath().contains("favicon.ico"))
            {
                printLog(QsLogging::Level::ErrorLevel,
                         tr("file does not exist:")+pFileInfo.absoluteFilePath());
            }
        }

        isOk = true;
    }
    else if(prequestUrl.contains(".wav"))
    {
        QString tmpFilePath = m_currentWorkingPath+prequestUrl;
        session->replyBytes(getResourceManager()->getFile(tmpFilePath),
                            "audio/x-wav");

        isOk = true;
    }

    return isOk;
}

/**
 * @brief chttpserver::onHttpAccepted 处理http请求
 * @param session
 */
void chttpserver::onHttpAccepted(const QPointer< JQHttpServer::Session > &session,QObject *mainObj)
{
    //QStringList requestUrlPath = session->requestUrlPathSplitToList();
    //QMap<QString,QString> header = session->requestUrlQuery();
    //session->replyBytes(session->requestUrl().toUtf8(),"text/html; charset=UTF-8");

    chttpserver *pchttpserver = dynamic_cast<chttpserver*>(mainObj);
    if(pchttpserver) pchttpserver->onProcessHttpAccepted(session,mainObj);
}

/**
 * @brief chttpserver::printLog 打印用户日志
 * @param type
 * @param msg
 */
void chttpserver::printLog(QsLogging::Level type,QString msg)
{
    switch (type)
    {
    case QsLogging::Level::InfoLevel:
        QLOG_INFO()<<msg;
        break;
    case QsLogging::Level::WarnLevel:
        QLOG_WARN()<<msg;
        break;
    case QsLogging::Level::ErrorLevel:
        QLOG_ERROR()<<msg;
        break;
    case QsLogging::Level::FatalLevel:
        QLOG_FATAL()<<msg;
        break;
    default:
        break;
    }

    emit signalshowlog(getCurrentDate()+
                  tr("<b>HTTPSER[%1]-</b>").arg(m_isEnableSSL ?
#ifndef QT_NO_SSL
                                                    m_htmlSslServerManage.getListenPort()
#else
                                                  -1
#endif
                                                  : m_htmlServerManage.getListenPort())+
                  msg);
}

/**
 * @brief CModbusClient::getCurrentDate 得到当前时间
 * @return
 */
QString chttpserver::getCurrentDate(void)
{
    QDateTime dateTime =QDateTime::currentDateTime();
    return dateTime.toString("<b>[yyyy-MM-dd hh:mm:ss]</b> ");
}
