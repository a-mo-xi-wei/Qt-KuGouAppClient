#include "../../includes/network/subandpubclient.h"
#include "../../includes/QsLog/QsLog.h"

#include <QDateTime>
#include <QUuid>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

subandpubclient::subandpubclient(QObject *parent)
    : QObject(parent),
      m_isAutosubMessage(true),
      m_isAutoSubMsgSuccesed(false)
{
    m_mainWebSocketClient.SetNetworkFrameManager(this);

    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(timer_system_update()));
    m_updateTimer.setSingleShot(true);
}

subandpubclient::~subandpubclient()
{
    this->Close();
}

/**
 * @brief subandpubclient::Open 打开指定地址的网络连接
 * @param url
 */
void subandpubclient::Open(QUrl url)
{
    if(m_clientUUID == "")
    {
        printLog(QsLogging::Level::InfoLevel,
                 QString::fromLocal8Bit("UUID不能为空,请先设置一个."));
        return;
    }

    m_mainWebSocketClient.Open(url);
}

/**
 * @brief subandpubclient::Open2
 * @param ip
 * @param port
 */
void subandpubclient::Open2(QString ip,int port)
{
    if(ip == "" || port <= 0)
        return;

    if(m_clientUUID == "")
    {
        printLog(QsLogging::Level::InfoLevel,
                 QString::fromLocal8Bit("UUID不能为空,请先设置一个."));
        return;
    }

    m_serverip = ip;
    m_serverport = port;

    m_mainWebSocketClient.Open(QUrl(QString::asprintf("ws://%s:%d",
                                                      ip.toStdString().c_str(),
                                                      port)));

    //m_updateTimer.start(2000);
}

/**
 * @brief subandpubclient::Close 关闭连接
 */
void subandpubclient::Close(void)
{
    m_updateTimer.stop();

    m_mainWebSocketClient.Close();
    m_subMessageList.clear();
}

/**
 * @brief subandpubclient::timer_system_update 系统更新定时器
 */
void subandpubclient::timer_system_update(void)
{
    if(m_isAutoSubMsgSuccesed &&
       m_pubMessageList.isEmpty() ||
       !m_mainWebSocketClient.isConnected())
        return;

    QList<tagPubMessage>::iterator iter = m_pubMessageList.begin();
    for(;iter != m_pubMessageList.end();)
    {
        if(this->pubMessage((*iter).message,
                            (*iter).content,
                            (*iter).password,
                            (*iter).time,
                            false))
            iter = m_pubMessageList.erase(iter);
        else
            ++iter;
    }
}

/**
 * @brief subandpubclient::isConnected 检测连接是否建立成功
 * @return
 */
bool subandpubclient::isConnected(void)
{
    return m_mainWebSocketClient.isConnected();
}

/**
 * @brief subandpubclient::getCurrentDate 得到当前时间
 * @return
 */
QString subandpubclient::getCurrentDate(void)
{
    QDateTime dateTime =QDateTime::currentDateTime();
    return dateTime.toString("<b>[yyyy-MM-dd hh:mm:ss]</b> ");
}

/**
 * @brief subandpubclient::subMessage 订阅消息
 * @param message
 * @param password
 */
void subandpubclient::subMessage(QString message,QString password)
{
    if(!m_mainWebSocketClient.isConnected())
        return;

    QJsonObject msgObj;
    msgObj["msgId"] = IDD_SUBANDPUB_SUBSCRIBE;
    msgObj["message"] = message;
    msgObj["pwd"] = password;

    m_mainWebSocketClient.Send(JsonToString(msgObj));
}

/**
 * @brief subandpubclient::unSubMessage 退订消息
 * @param message
 */
void subandpubclient::unSubMessage(QString message)
{
    if(!m_mainWebSocketClient.isConnected())
        return;

    QJsonObject msgObj;
    msgObj["msgId"] = IDD_SUBANDPUB_UNSUBSCRIBE;
    msgObj["message"] = message;

    m_mainWebSocketClient.Send(JsonToString(msgObj));
}

/**
 * @brief subandpubclient::pubMessage 发布消息
 * @param message
 * @param content
 * @param password
 */
bool subandpubclient::pubMessage(QString message,QString content,
                                 QString password,QString time,
                                 bool isAddofflinelist)
{
    if(!m_mainWebSocketClient.isConnected())
    {
        if(isAddofflinelist)
        {
            if(m_pubMessageList.size() > 100)
                m_pubMessageList.pop_front();

            m_pubMessageList.push_back(tagPubMessage(message,
                                                     content,
                                                     password,
                                                     QString::asprintf("%lld",QDateTime::currentSecsSinceEpoch())));
        }

        return false;
    }

    QJsonObject msgObj;
    msgObj["msgId"] = IDD_SUBANDPUB_PUBSCRIBE;
    msgObj["message"] = message;
    msgObj["content"] = content;
    msgObj["pwd"] = password;
    msgObj["time"] = (time != "" ? time : QString::asprintf("%lld",QDateTime::currentSecsSinceEpoch()));

    if(m_mainWebSocketClient.Send(JsonToString(msgObj)) <= 0)
    {
        if(isAddofflinelist)
        {
            if(m_pubMessageList.size() > 100)
                m_pubMessageList.pop_front();

            m_pubMessageList.push_back(tagPubMessage(message,
                                                     content,
                                                     password,
                                                     QString::asprintf("%lld",QDateTime::currentSecsSinceEpoch())));
        }

        return false;
    }

    return true;
}

/**
 * @brief subandpubclient::printLog 打印用户日志
 * @param type
 * @param msg
 */
void subandpubclient::printLog(QsLogging::Level type,QString msg)
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
                  "<b>SUBCLIENT-</b>"+
                  msg);
}

/**
 * @brief subandpubclient::deleteSubMessage 清除指定的订阅消息
 * @param message
 */
void subandpubclient::deleteSubMessage(QString message)
{
    if(message == "" || m_subMessageList.isEmpty())
        return;

    QHash<QString,QString>::iterator iter = m_subMessageList.find(message);
    if(iter != m_subMessageList.end())
        m_subMessageList.erase(iter);
}

/**
 * @brief subandpubclient::OnProcessNetText 处理网络字符串消息
 * @param conn
 * @param mes
 */
void subandpubclient::OnProcessNetText(QWebSocket *conn,QString mes)
{
    QJsonObject msgObj = StringToJson(mes);

    emit signalprocessmessage(msgObj);

    switch(msgObj["msgId"].toInt())
    {
    case IDD_SUBANDPUB_SUBSCRIBE:           // 消息订阅
    {
        QString message = msgObj["message"].toString();
        QString mesPWD = msgObj["pwd"].toString();

        switch(msgObj["subId"].toInt())
        {
        case IDD_SUBANDPUB_PASSWORD_ERROR:
        {
            printLog(QsLogging::Level::InfoLevel,
                     QString::fromLocal8Bit("密码不对,订阅失败:")+
                     mesPWD);
        }
            break;
        case IDD_SUBANDPUB_SUBMES_EXIST:
        {
            printLog(QsLogging::Level::InfoLevel,
                     QString::fromLocal8Bit("不能重复订阅,订阅失败!"));
        }
            break;
        case IDD_SUBANDPUB_SUCCESS:
        {
            printLog(QsLogging::Level::InfoLevel,
                     message+
                     QString::fromLocal8Bit(":订阅成功!"));

            m_subMessageList[message] = mesPWD;
        }
            break;
        default:
            break;
        }
    }
        break;
    case IDD_SUBANDPUB_UNSUBSCRIBE:         // 消息退订
    {
        QString message = msgObj["message"].toString();

        switch(msgObj["subId"].toInt())
        {
        case IDD_SUBANDPUB_SUBMES_NOEXIST:
        {
            printLog(QsLogging::Level::InfoLevel,
                     QString::fromLocal8Bit("当前消息不存在,退订失败!"));
        }
            break;
        case IDD_SUBANDPUB_SUCCESS:
        {
            printLog(QsLogging::Level::InfoLevel,
                     message+
                     QString::fromLocal8Bit(":退订成功!"));

            deleteSubMessage(message);
        }
            break;
        default:
            break;
        }
    }
        break;
    case IDD_SUBANDPUB_PUBSCRIBE:           // 发布订阅消息
    {
        switch(msgObj["subId"].toInt())
        {
        case IDD_SUBANDPUB_PASSWORD_ERROR:
        {
            printLog(QsLogging::Level::InfoLevel,
                     QString::fromLocal8Bit("密码不对,发布消息失败!"));
        }
            break;
        default:
            break;
        }
    }
        break;
    case IDD_SUBANDPUB_IDENTITY:           // 认证消息
    {
        switch(msgObj["subId"].toInt())
        {
        case IDD_SUBANDPUB_SUCCESS:
        {

        }
            break;
        case IDD_SUBANDPUB_CLIENT_EXIST:
        {
            printLog(QsLogging::Level::InfoLevel,
                     QString::fromLocal8Bit("当前UUID的客户端已经登录，不能重复登录!"));
        }
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
}

/**
 * @brief subandpubclient::RandomGenerateClientUUID 生成一个随机的ID
 */
QString subandpubclient::RandomGenerateClientUUID(void)
{
    return QUuid::createUuid().toString();
}

/**
 * @brief subandpubclient::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn
 */
void subandpubclient::OnProcessConnectedNetMes(QWebSocket *conn)
{
    printLog(QsLogging::Level::InfoLevel,
             QString::fromLocal8Bit("服务器:")+
             m_serverip+
             ":"+
             QString::asprintf("%d",m_serverport)+
             QString::fromLocal8Bit("连接成功,UUID:")+
             m_clientUUID);

    QJsonObject msgObj;
    msgObj["msgId"] = IDD_SUBANDPUB_IDENTITY;
    msgObj["type"] = IDENTITY_PUBLER;
    msgObj["uuid"] = m_clientUUID;

    m_mainWebSocketClient.Send(JsonToString(msgObj));

    // 是否选线重连后自动订阅消息
    if(m_isAutosubMessage && !m_subMessageList.isEmpty())
    {
        QHash<QString,QString>::iterator iter = m_subMessageList.begin();
        for(;iter != m_subMessageList.end();++iter)
        {
            this->subMessage(iter.key(),iter.value());
        }
    }

    m_isAutoSubMsgSuccesed = true;

    if(!m_pubMessageList.isEmpty())
        m_updateTimer.start(5000);
}

/**
 * @brief subandpubclient::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn
 */
void subandpubclient::OnProcessDisconnectedNetMes(QWebSocket *conn)
{
    printLog(QsLogging::Level::InfoLevel,
             QString::fromLocal8Bit("服务器连接断开!"));

    m_isAutoSubMsgSuccesed = false;
}

/**
 * @brief subandpubclient::loadConfigFile 通过配置文件
 * @param filepath
 * @return
 */
bool subandpubclient::loadConfigFile(QString filepath)
{
    QFileInfo pfileinfo(filepath);
    if(!pfileinfo.exists())
        return false;

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "subandpubclient::loadConfigFile:"+filepath+" open fail.");
        return false;
    }

    m_currentConfigPath = filepath;

    QXmlStreamReader reader(&file);

    while(!reader.atEnd() && !reader.hasError())
    {
        if(reader.isStartElement())
        {
            if(reader.name() == "brotherclient")
            {
                QXmlStreamAttributes attributes = reader.attributes();

                m_serverip = attributes.value("ip").toString();
                m_clientUUID = attributes.value("uuid").toString();
                m_serverport = attributes.value("port").toInt();
            }
        }

        reader.readNext();
    }

    file.close();

    return true;
}

/**
 * @brief subandpubclient::savaConfigFile 保存配置文件
 * @param filepath
 * @return
 */
bool subandpubclient::savaConfigFile(QString filepath)
{
    QString tmpCurPath = (filepath == "" ? m_currentConfigPath : filepath);

    QFileInfo pfileinfo(tmpCurPath);
    if(!pfileinfo.exists())
        return false;

    QFile file(tmpCurPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "subandpubclient::savaConfigFile:"+tmpCurPath+" open fail.");
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("brotherclient");
    xmlWriter.writeAttribute("port", QString::asprintf("%d",m_serverport));
    xmlWriter.writeAttribute("uuid", m_clientUUID);
    xmlWriter.writeAttribute("ip", m_serverip);

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();

    return true;
}
