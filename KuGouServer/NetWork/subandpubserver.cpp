#include "subandpubserver.h"
#include "common.h"

#include <QDateTime>
#include <QUuid>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QCryptographicHash>

subandpubserver::subandpubserver(QObject *parent)
    : QObject(parent),
      m_websocketServerPort(1101)
{
    m_MainWebSocketServer.SetNetworkFrameManager(this);

    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(timer_system_update()));
}

subandpubserver::~subandpubserver()
{
    stopServer(false);
}

/**
 * @brief subandpubserver::RandomGenerateServerUUID 生成一个随机的服务器ID
 */
QString subandpubserver::RandomGenerateServerUUID(void)
{
    return QUuid::createUuid().toString();
}

/**
 * @brief subandpubserver::getCurrentDate 得到当前时间
 * @return
 */
QString subandpubserver::getCurrentDate(void)
{
    QDateTime dateTime =QDateTime::currentDateTime();
    return dateTime.toString("<b>[yyyy-MM-dd hh:mm:ss]</b> ");
}

/**
 * @brief subandpubserver::isRunning 检测服务器是否已经启动
 * @return
 */
bool subandpubserver::isRunning(void)
{
    return m_MainWebSocketServer.isListening();
}

/**
 * @brief subandpubserver::addBrotherServer 添加一个兄弟服务器
 * @param ip
 * @param port
 * @return
 */
bool subandpubserver::addBrotherServer(QString ip,int port)
{
    if(ip.isEmpty() || port <= 0)
        return false;

    CWebSocketClient *pwebclient = new CWebSocketClient(this,this);
    pwebclient->Open2(ip,port);

    m_brotherServers[pwebclient->getWebSocket()] = pwebclient;

    return true;
}

/**
 * @brief subandpubserver::savaBrotherServers 保存配置文件
 * @param filepath
 * @return
 */
bool subandpubserver::savaBrotherServers(QString filepath)
{
    QString tmpCurPath = (filepath == "" ? m_currentConfigFile : filepath);

    QFileInfo pfileinfo(tmpCurPath);
    if(!pfileinfo.exists())
        return false;

    QFile file(tmpCurPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "savaBrotherServers:"+tmpCurPath+" open fail.");
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("brotherservers");
    xmlWriter.writeAttribute("port", QString::asprintf("%d",this->getWebsocketServerPort()));
    xmlWriter.writeAttribute("uuid", this->getServerUUID());

    QList<tagBrotherServerItem>::iterator iter = m_allBrotherServerList.begin();
    for(;iter != m_allBrotherServerList.end();++iter)
    {
        xmlWriter.writeStartElement("server");
        xmlWriter.writeAttribute("ip",(*iter).ip);
        xmlWriter.writeAttribute("port", QString::asprintf("%d",(*iter).port));
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();

    return true;
}

/**
 * @brief subandpubserver::loadBrotherServers 通过配置文件导入兄弟服务器
 * @param filepath
 * @return
 */
bool subandpubserver::loadBrotherServers(QString filepath)
{
    QFileInfo pfileinfo(filepath);
    if(!pfileinfo.exists())
        return false;

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "loadBrotherServers:"+filepath+" open fail.");
        return false;
    }

    m_currentConfigFile = filepath;
    m_allBrotherServerList.clear();

    QXmlStreamReader reader(&file);

    while(!reader.atEnd() && !reader.hasError())
    {
        if(reader.isStartElement())
        {
            if(reader.name().toString() == "server")
            {
                QXmlStreamAttributes attributes = reader.attributes();

                QString ip = attributes.value("ip").toString();
                int port = attributes.value("port").toInt();

                m_allBrotherServerList.push_back(tagBrotherServerItem(ip,port));

                printLog(QsLogging::Level::InfoLevel,
                         QString::fromLocal8Bit("加载服务器:")+
                         ip+
                         QString::asprintf(":%d",port));
            }
            else if(reader.name().toString() == "brotherservers")
            {
                QXmlStreamAttributes attributes = reader.attributes();

                m_serverUUID = attributes.value("uuid").toString();
                m_websocketServerPort = attributes.value("port").toInt();
            }
        }

        reader.readNext();
    }

    file.close();

    return true;
}

/**
 * @brief subandpubserver::isExistClientInBrotherServers 检测指定连接是否是兄弟结点
 * @param client
 * @return
 */
CWebSocketClient* subandpubserver::isExistClientInBrotherServers(QWebSocket* client)
{
    if(m_brotherServers.isEmpty() || client == NULL)
        return NULL;

    QHash<QWebSocket*,CWebSocketClient*>::iterator iter = m_brotherServers.find(client);
    if(iter == m_brotherServers.end())
        return NULL;

    return iter.value();
}

/**
 * @brief subandpubserver::closeAllBrotherServers 关闭所有的兄弟服务器
 */
void subandpubserver::closeAllBrotherServers(void)
{
    QHash<QWebSocket*,CWebSocketClient*>::iterator iter = m_brotherServers.begin();
    for(;iter != m_brotherServers.end();++iter)
    {
        printLog(QsLogging::Level::InfoLevel,
                 QString::fromLocal8Bit("关闭服务器连接:")+
                 iter.value()->getServerIP()+
                 "_"+
                 QString::asprintf("%d",iter.value()->getServerPort()));

        iter.value()->Close();
        iter.value()->deleteLater();
    }

    m_brotherServers.clear();
}

/**
 * @brief subandpubserver::printLog 打印用户日志
 * @param type
 * @param msg
 */
void subandpubserver::printLog(QsLogging::Level type,QString msg)
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

    emit showlog(getCurrentDate()+
            "<b>PUBSERVER-</b>"+
            msg);
}

/**
 * @brief subandpubserver::startServer 启动服务器
 */
bool subandpubserver::startServer(void)
{
    if(m_websocketServerPort <= 0)
        return false;

    if(m_serverUUID == "")
    {
        printLog(QsLogging::Level::InfoLevel,
                 QString::fromLocal8Bit("UUID不能为空,请先设置一个."));
        return false;
    }

    bool isOk = true;

    if(!m_MainWebSocketServer.OpenServer(m_websocketServerPort))
    {
        QString tmpStr = QString::fromLocal8Bit("Websocket服务器启动失败,端口:")+
                         QString::asprintf("%d;",m_websocketServerPort)+
                         QString::fromLocal8Bit("UUID:")+
                         m_serverUUID;
        printLog(QsLogging::Level::ErrorLevel,tmpStr);
        isOk = false;
    }

    closeAllBrotherServers();

    for(int i=0;i<m_allBrotherServerList.size();i++)
    {
        if(addBrotherServer(m_allBrotherServerList[i].ip,
                            m_allBrotherServerList[i].port))
        {

        }
    }

    printLog(QsLogging::Level::InfoLevel,
             QString::fromLocal8Bit("Websocket服务器启动成功,端口:")+
             QString::asprintf("%d;",m_websocketServerPort)+
             QString::fromLocal8Bit("UUID:")+
             m_serverUUID);

    m_updateTimer.start(1000);

    return isOk;
}

/**
 * @brief subandpubserver::stopServer 停止服务器
 */
void subandpubserver::stopServer(bool isShowlog)
{
    closeAllBrotherServers();

    m_MainWebSocketServer.CloseServer();
    m_allsubscribes.clear();
    m_allsubscribePWDs.clear();
    m_allsubscribeMessageList.clear();
    m_allsubscribeMessageHash.clear();
    m_allSendFailMessageList.clear();
    m_allSendFailClientList.clear();
    m_clientidentityinfos.clear();

    m_updateTimer.stop();

    if(isShowlog)
        printLog(QsLogging::Level::InfoLevel,QString::fromLocal8Bit("服务器关闭成功."));
}

/**
 * @brief subandpubserver::system_update 系统更新定时器
 */
void subandpubserver::timer_system_update(void)
{   
    QHash<QString,QList<tagSendFailMsgItem>>::iterator itermsg = m_allSendFailMessageList.begin();
    for(;itermsg != m_allSendFailMessageList.end();++itermsg)
    {
        if(itermsg.value().isEmpty())
            continue;

        QList<tagSendFailMsgItem>::iterator itersend = itermsg.value().begin();
        for(;itersend != itermsg.value().end();)
        {
            bool isSendOk = false;

            // 检测客户端是否在线
            QHash<QString,tagSendFailClient>::iterator iterclient = m_allSendFailClientList.find(itermsg.key());
            if(iterclient != m_allSendFailClientList.end() &&
               iterclient.value().socket != NULL &&
               iterclient.value().socket->state() == QAbstractSocket::ConnectedState)
            {
                if(iterclient.value().identity == IDENTITY_PUBLER)
                {
                    if(isExistClientSubMessage(iterclient.value().socket,(*itersend).message) &&
                       m_MainWebSocketServer.Send(iterclient.value().socket,
                                           (*itersend).contents) > 0)
                        isSendOk = true;
                }
                else if(iterclient.value().identity == IDENTITY_SERVER)
                {
                    QHash<QWebSocket*,CWebSocketClient*>::iterator iterbro = m_brotherServers.find(iterclient.value().socket);
                    if(iterbro != m_brotherServers.end())
                    {
                        if(iterbro.value()->Send((*itersend).contents) > 0)
                            isSendOk = true;
                    }
                }
            }

            if(isSendOk)
                itersend = itermsg.value().erase(itersend);
            else
                ++itersend;
        }
    }
}

/**
 * @brief subandpubserver::sendMessagetoBrother 给所有兄弟结点发送消息
 * @param data
 * @param conn
 */
void subandpubserver::sendMessagetoBrother(QString data,QWebSocket* client)
{
    if(data.isEmpty() || m_brotherServers.isEmpty())
        return;

    QHash<QWebSocket*,CWebSocketClient*>::iterator iter = m_brotherServers.begin();
    for(;iter != m_brotherServers.end();++iter)
    {
        if(client != NULL && iter.key() == client)
            continue;

        iter.value()->Send(data);
    }
}

/**
 * @brief subandpubserver::isExistSubscribeMessage 检测指定的消息是否存在
 * @param message
 * @return
 */
bool subandpubserver::isExistSubscribeMessage(QString message)
{
    if(message.isEmpty())
        return false;

    QHash<QString,QList<QWebSocket*>>::iterator iter = m_allsubscribes.find(message);
    if(iter == m_allsubscribes.end())
        return false;

    return true;
}

/**
 * @brief subandpubserver::isExistClientSubMessage 检测指定的客户端是否订阅了指定的消息
 * @param client
 * @param message
 * @return
 */
bool subandpubserver::isExistClientSubMessage(QWebSocket* client,QString message)
{
    if(client == NULL || message.isEmpty())
        return false;

    QHash<QString,QList<QWebSocket*>>::iterator iter = m_allsubscribes.find(message);
    if(iter == m_allsubscribes.end())
        return false;

    QList<QWebSocket*>::iterator iter2 = std::find(iter.value().begin(),
                                           iter.value().end(),
                                           client);
    if(iter2 == iter.value().end())
        return false;

    return true;
}

/**
 * @brief subandpubserver::deleteclientFromSubMessage 从指定订阅消息中删除指定的客户端
 * @param client
 * @param message
 * @return
 */
bool subandpubserver::deleteclientFromSubMessage(QWebSocket* client,QString message)
{
    if(client == NULL || message.isEmpty())
        return false;

    QHash<QString,QList<QWebSocket*>>::iterator iter = m_allsubscribes.find(message);
    if(iter == m_allsubscribes.end())
        return false;

    QList<QWebSocket*>::iterator iter2 = std::find(iter.value().begin(),
                                           iter.value().end(),
                                           client);
    if(iter2 == iter.value().end())
        return false;

    iter.value().erase(iter2);

    if(iter.value().isEmpty())
    {
        QHash<QString,QString>::iterator iter3 = m_allsubscribePWDs.find(iter.key());
        if(iter3 !=m_allsubscribePWDs.end())
            m_allsubscribePWDs.erase(iter3);

        m_allsubscribes.erase(iter);
    }

    return true;
}

/**
 * @brief subandpubserver::generateMessageID 根据用户发送消息生成一个消息ID
 * @param client
 * @param msg
 * @return
 */
QString subandpubserver::generateMessageID(QWebSocket* client,QString msg)
{
    if(client == NULL || msg == "")
        return m_serverUUID;

    QString tmpString = client->localAddress().toString()+
                        QString::asprintf("%d",client->localPort())+
                        msg+
                        QString::asprintf("%lld",QDateTime::currentMSecsSinceEpoch());

    return QCryptographicHash::hash(tmpString.toUtf8(),QCryptographicHash::Md5).toHex();
}

/**
 * @brief subandpubserver::isSubscribeMessagePWDSame 检测指定消息的密码是否一致
 * @param message
 * @param pwd
 * @return
 */
bool subandpubserver::isSubscribeMessagePWDSame(QString message,QString pwd)
{
    if(message == "" || pwd == "" || m_allsubscribePWDs.isEmpty())
        return false;

    QHash<QString,QString>::iterator iter = m_allsubscribePWDs.find(message);
    if(iter == m_allsubscribePWDs.end() ||
       iter.value() != pwd)
        return false;

    return true;
}

/**
 * @brief subandpubserver::isExistPWDsubMessage 检测指定消息是否有密码
 * @param message
 * @return
 */
bool subandpubserver::isExistPWDsubMessage(QString message)
{
    if(message == "" || m_allsubscribePWDs.isEmpty())
        return false;

    QHash<QString,QString>::iterator iter = m_allsubscribePWDs.find(message);
    if(iter == m_allsubscribePWDs.end())
        return false;

    return true;
}

/**
 * @brief subandpubserver::OnProcessNetText 处理网络字符串消息
 * @param conn
 * @param mes
 */
void subandpubserver::OnProcessNetText(QWebSocket *conn,QString mes)
{
    QJsonObject msgObj = StringToJson(mes);

    switch(msgObj["msgId"].toInt())
    {
    case IDD_SUBANDPUB_SUBSCRIBE:          // 消息订阅
    {
        QString message = msgObj["message"].toString();
        QString mesPWD = msgObj["pwd"].toString();

        if(message == "")
        {
            QJsonObject retObj;
            retObj["msgId"] = IDD_SUBANDPUB_SUBSCRIBE;
            retObj["subId"] = IDD_SUBANDPUB_FAIL;

            m_MainWebSocketServer.Send(conn,JsonToString(retObj));

            return;
        }

        if(mesPWD != "")
        {
            if(isExistSubscribeMessage(message))
            {
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_SUBSCRIBE;
                retObj["subId"] = IDD_SUBANDPUB_SUBMES_EXIST;
                retObj["message"] = message;

                m_MainWebSocketServer.Send(conn,JsonToString(retObj));

                return;
            }

            if(isExistPWDsubMessage(message) &&
               !isSubscribeMessagePWDSame(message,mesPWD))
            {
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_SUBSCRIBE;
                retObj["subId"] = IDD_SUBANDPUB_PASSWORD_ERROR;
                retObj["message"] = message;
                retObj["pwd"] = mesPWD;

                m_MainWebSocketServer.Send(conn,JsonToString(retObj));

                return;
            }
        }
        else
        {
            if(isExistPWDsubMessage(message))
            {
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_SUBSCRIBE;
                retObj["subId"] = IDD_SUBANDPUB_PASSWORD_ERROR;
                retObj["message"] = message;
                retObj["pwd"] = mesPWD;

                m_MainWebSocketServer.Send(conn,JsonToString(retObj));

                return;
            }
        }

        // 首先检测订阅的消息是否已经被订阅了
        if(isExistClientSubMessage(conn,message))
        {
            QJsonObject retObj;
            retObj["msgId"] = IDD_SUBANDPUB_SUBSCRIBE;
            retObj["subId"] = IDD_SUBANDPUB_SUBMES_EXIST;
            retObj["message"] = message;
            retObj["pwd"] = mesPWD;

            m_MainWebSocketServer.Send(conn,JsonToString(retObj));

            return;
        }

        m_allsubscribes[message].push_back(conn);

        if(mesPWD != "")
        {
            QHash<QString,QString>::iterator iterTwo = m_allsubscribePWDs.find(message);
            if(iterTwo == m_allsubscribePWDs.end())
            {
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_SYNCPWD;
                retObj["message"] = message;
                retObj["pwd"] = mesPWD;

                // 将这个消息发送给其它兄弟结点
                sendMessagetoBrother(JsonToString(retObj),conn);

                m_allsubscribePWDs[message] = mesPWD;
            }
        }

        tagSendFailClient *pSendFailClient = getSendFailClient(conn);

        printLog(QsLogging::Level::InfoLevel,
                 (pSendFailClient != NULL ? pSendFailClient->uuid : "")+
                 "["+
                 convert_to_ipv4_addr(conn->peerAddress())+
                 "_"+
                 QString::asprintf("%d",conn->peerPort())+
                 "]"+
                 QString::fromLocal8Bit("成功订阅消息:")+
                 message+
                 QString::fromLocal8Bit(" 密码:")+
                 mesPWD);

        QJsonObject retObj;
        retObj["msgId"] = IDD_SUBANDPUB_SUBSCRIBE;
        retObj["subId"] = IDD_SUBANDPUB_SUCCESS;
        retObj["message"] = message;
        retObj["pwd"] = mesPWD;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));
    }
        break;
    case IDD_SUBANDPUB_UNSUBSCRIBE:         // 消息退订
    {
        QString message = msgObj["message"].toString();

        if(message == "")
        {
            QJsonObject retObj;
            retObj["msgId"] = IDD_SUBANDPUB_UNSUBSCRIBE;
            retObj["subId"] = IDD_SUBANDPUB_FAIL;

            m_MainWebSocketServer.Send(conn,JsonToString(retObj));

            return;
        }

        // 首先检测订阅的消息是否已经被退订了
        if(!isExistClientSubMessage(conn,message))
        {
            QJsonObject retObj;
            retObj["msgId"] = IDD_SUBANDPUB_UNSUBSCRIBE;
            retObj["subId"] = IDD_SUBANDPUB_SUBMES_NOEXIST;
            retObj["message"] = message;

            m_MainWebSocketServer.Send(conn,JsonToString(retObj));

            return;
        }

        deleteclientFromSubMessage(conn,message);

        tagSendFailClient *pSendFailClient = getSendFailClient(conn);

        printLog(QsLogging::Level::InfoLevel,
                 (pSendFailClient != NULL ? pSendFailClient->uuid : "")+
                 "["+
                 convert_to_ipv4_addr(conn->peerAddress())+
                 "_"+
                 QString::asprintf("%d",conn->peerPort())+
                 "]"+
                 QString::fromLocal8Bit("成功退订消息:")+
                 message);

        QJsonObject retObj;
        retObj["msgId"] = IDD_SUBANDPUB_UNSUBSCRIBE;
        retObj["subId"] = IDD_SUBANDPUB_SUCCESS;
        retObj["message"] = message;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));
    }
        break;
    case IDD_SUBANDPUB_PUBSCRIBE:              // 发布订阅消息
    {
        QString message = msgObj["message"].toString();
        QString content = msgObj["content"].toString();
        QString msgid = msgObj["msgid"].toString();
        QString mesPWD = msgObj["pwd"].toString();

        if(message == "" || content == "")
        {
            QJsonObject retObj;
            retObj["msgId"] = IDD_SUBANDPUB_PUBSCRIBE;
            retObj["subId"] = IDD_SUBANDPUB_FAIL;

            m_MainWebSocketServer.Send(conn,JsonToString(retObj));

            return;
        }

        if(mesPWD != "")
        {
            if(isExistSubscribeMessage(message) &&
               !isSubscribeMessagePWDSame(message,mesPWD))
            {
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_PUBSCRIBE;
                retObj["subId"] = IDD_SUBANDPUB_PASSWORD_ERROR;
                retObj["message"] = message;
                retObj["pwd"] = mesPWD;

                m_MainWebSocketServer.Send(conn,JsonToString(retObj));

                return;
            }
        }
        else
        {
            if(isExistSubscribeMessage(message) &&
               isExistPWDsubMessage(message))
            {
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_PUBSCRIBE;
                retObj["subId"] = IDD_SUBANDPUB_PASSWORD_ERROR;
                retObj["message"] = message;
                retObj["pwd"] = mesPWD;

                m_MainWebSocketServer.Send(conn,JsonToString(retObj));

                return;
            }
        }

        // 如果消息没有ID的话，就给他分配一个
        if(msgid == "")
        {
            msgid = generateMessageID(conn,mes);
            msgObj["msgid"] = msgid;
        }

        // 检测当前消息是否已经处理过，如果处理过就不处理了
        QHash<QString,tagSubMsgItem>::iterator iter = m_allsubscribeMessageHash.find(msgid);
        if(iter != m_allsubscribeMessageHash.end())
            return;

        QString brotherMsg = JsonToString(msgObj);

        // 保存消息到断开连接的客户端消息列表中
        QHash<QString,tagSendFailClient>::iterator iterfail = m_allSendFailClientList.begin();
        for(;iterfail != m_allSendFailClientList.end();++iterfail)
        {
            if(iterfail.value().socket != NULL)
                continue;

            QHash<QString,QList<tagSendFailMsgItem>>::iterator itermsg = m_allSendFailMessageList.find(iterfail.value().uuid);
            if(itermsg != m_allSendFailMessageList.end())
            {
                if(itermsg.value().size() > 200)
                    itermsg.value().pop_front();

                if(iterfail.value().identity == IDENTITY_PUBLER)
                    itermsg.value().push_back(tagSendFailMsgItem(IDENTITY_PUBLER,mes,message));
                else if(iterfail.value().identity == IDENTITY_SERVER)
                    itermsg.value().push_back(tagSendFailMsgItem(IDENTITY_SERVER,brotherMsg,message));
            }
            else
            {
                m_allSendFailMessageList[iterfail.value().uuid].clear();

                if(iterfail.value().identity == IDENTITY_PUBLER)
                    m_allSendFailMessageList[iterfail.value().uuid].push_back(tagSendFailMsgItem(IDENTITY_PUBLER,mes,message));
                else if(iterfail.value().identity == IDENTITY_SERVER)
                    m_allSendFailMessageList[iterfail.value().uuid].push_back(tagSendFailMsgItem(IDENTITY_SERVER,brotherMsg,message));
            }
        }

        // 向订阅了这个消息的本机客户端发送这消息
        sendMessageToClient(message,mes);

        // 将这个消息发送给其它兄弟结点
        sendMessagetoBrother(brotherMsg,conn);

        // 将当前消息加入到已处理消息列表中
        if(m_allsubscribeMessageList.size() > 1000)
        {
            QJsonObject tmpObj = m_allsubscribeMessageList[m_allsubscribeMessageList.size()-1];

            iter = m_allsubscribeMessageHash.find(tmpObj["msgid"].toString());
            if(iter != m_allsubscribeMessageHash.end())
                m_allsubscribeMessageHash.erase(iter);

            m_allsubscribeMessageList.pop_back();
        }

        tagSendFailClient *pSendFailClient = getSendFailClient(conn);

        if(pSendFailClient)
        {
            printLog(QsLogging::Level::InfoLevel,
                     (pSendFailClient != NULL ? pSendFailClient->uuid : "")+
                     "["+
                     convert_to_ipv4_addr(conn->peerAddress())+
                     "_"+
                     QString::asprintf("%d",conn->peerPort())+
                     "]"+
                     QString::fromLocal8Bit("发布消息:")+
                     message+
                     QString::fromLocal8Bit(" 内容:")+
                     content);

            if(pSendFailClient->identity != IDENTITY_SERVER)
            {
                // 数据发送成功
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_PUBSCRIBE;
                retObj["subId"] = IDD_SUBANDPUB_SUCCESS;
                retObj["message"] = message;
                retObj["pwd"] = mesPWD;

                m_MainWebSocketServer.Send(conn,JsonToString(retObj));
            }
        }

        m_allsubscribeMessageList.push_front(msgObj);
        m_allsubscribeMessageHash[msgid] = tagSubMsgItem(msgid,message,content);
    }
        break;
    case IDD_SUBANDPUB_SYNCPWD:             //  同步订阅消息密码
    {
        QString message = msgObj["message"].toString();
        QString mesPWD = msgObj["pwd"].toString();

        if(mesPWD != "")
        {
            QHash<QString,QString>::iterator iterTwo = m_allsubscribePWDs.find(message);
            if(iterTwo == m_allsubscribePWDs.end())
            {
                QJsonObject retObj;
                retObj["msgId"] = IDD_SUBANDPUB_SYNCPWD;
                retObj["message"] = message;
                retObj["pwd"] = mesPWD;

                // 将这个消息发送给其它兄弟结点
                sendMessagetoBrother(JsonToString(retObj),conn);

                m_allsubscribePWDs[message] = mesPWD;
            }
        }
    }
        break;
    case IDD_SUBANDPUB_IDENTITY:             // 身份认证信息
    {
        QString clientuuid = msgObj["uuid"].toString();
        int subId = msgObj["subId"].isNull() ? -1 : msgObj["subId"].toInt();
        ClientIdentity pClientIdentity = (ClientIdentity)msgObj["type"].toInt();

        if(subId == IDD_SUBANDPUB_SUCCESS ||
           subId == IDD_SUBANDPUB_CLIENT_EXIST)
        {
            if(subId == IDD_SUBANDPUB_CLIENT_EXIST)
            {
                printLog(QsLogging::Level::InfoLevel,
                         clientuuid+
                         QString::fromLocal8Bit("已经登录，不能重复登录!"));
            }

            return;
        }

        tagSendFailClient *pSendFailClient = getSendFailClient2(clientuuid);
        if((pSendFailClient != NULL &&
           pSendFailClient->socket != NULL) ||
           clientuuid == this->getServerUUID())
        {
            QJsonObject retObj;
            retObj["msgId"] = IDD_SUBANDPUB_IDENTITY;
            retObj["subId"] = IDD_SUBANDPUB_CLIENT_EXIST;
            retObj["uuid"] = clientuuid;

            m_MainWebSocketServer.Send(conn,JsonToString(retObj));
            m_MainWebSocketServer.closeClient(conn);

            return;
        }

        m_clientidentityinfos[conn] = pClientIdentity;
        m_allSendFailClientList[clientuuid] = tagSendFailClient(pClientIdentity,clientuuid,conn);

        if(pClientIdentity == IDENTITY_PUBLER)
        {
            printLog(QsLogging::Level::InfoLevel,
                     QString::fromLocal8Bit("客户端:")+
                     clientuuid+
                     "["+
                     convert_to_ipv4_addr(conn->peerAddress())+
                     "_"+
                     QString::asprintf("%d",conn->peerPort())+
                     "]"+
                     QString::fromLocal8Bit("连接成功."));
        }

        QJsonObject retObj;
        retObj["msgId"] = IDD_SUBANDPUB_IDENTITY;
        retObj["subId"] = IDD_SUBANDPUB_SUCCESS;
        retObj["uuid"] = clientuuid;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));
    }
        break;
    default:
        break;
    }
}

tagSendFailClient* subandpubserver::getSendFailClient2(QString uuid)
{
    if(uuid == "") return NULL;

    QHash<QString,tagSendFailClient>::iterator iter = m_allSendFailClientList.find(uuid);
    if(iter != m_allSendFailClientList.end())
    {
        return &iter.value();
    }

    return NULL;
}

/**
 * @brief subandpubserver::getSendFailClient 得到客户端的UUID
 * @param client
 * @return
 */
tagSendFailClient* subandpubserver::getSendFailClient(QWebSocket *client)
{
    if(client == NULL) return NULL;

    QHash<QString,tagSendFailClient>::iterator iter = m_allSendFailClientList.begin();
    for(;iter != m_allSendFailClientList.end();++iter)
    {
        if(iter.value().socket == client)
            return &iter.value();
    }

    return NULL;
}

/**
 * @brief subandpubserver::sendMessageToClient 向订阅了指定消息的客户端发送指定消息
 * @param message
 * @param content
 */
bool subandpubserver::sendMessageToClient(QString message,QString content)
{
    if(message == "" || content == "" || m_allsubscribes.isEmpty())
        return false;

    QHash<QString,QList<QWebSocket*>>::iterator iter = m_allsubscribes.find(message);
    if(iter == m_allsubscribes.end())
        return false;

    QList<QWebSocket*>::iterator iter2 = iter.value().begin();
    for(;iter2 != iter.value().end();++iter2)
    {
        m_MainWebSocketServer.Send((*iter2),content);
    }

    return true;
}

/**
 * @brief subandpubserver::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn
 */
void subandpubserver::OnProcessConnectedNetMes(QWebSocket *conn)
{
    CWebSocketClient *pbrotherclient = isExistClientInBrotherServers(conn);
    if(pbrotherclient != NULL)
    {
        tagSendFailClient *pSendFailClient = getSendFailClient(conn);
        printLog(QsLogging::Level::InfoLevel,
                 QString::fromLocal8Bit("服务器:")+
                 (pSendFailClient != NULL ? pSendFailClient->uuid : "")+
                 "["+
                 pbrotherclient->getServerIP()+
                 "_"+
                 QString::asprintf("%d",pbrotherclient->getServerPort())+
                 "]"+
                 QString::fromLocal8Bit("连接成功."));

        QJsonObject msgObj;
        msgObj["msgId"] = IDD_SUBANDPUB_IDENTITY;
        msgObj["type"] = IDENTITY_SERVER;
        msgObj["uuid"] = m_serverUUID;

        pbrotherclient->Send(JsonToString(msgObj));

        return;
    }
}

/**
 * @brief subandpubserver::ResetSendFailClient 重置掉线客户端
 * @param client
 */
void subandpubserver::ResetSendFailClient(QWebSocket *client)
{
    if(m_allSendFailClientList.isEmpty() || client == NULL)
        return;

    QHash<QString,tagSendFailClient>::iterator iterfail = m_allSendFailClientList.begin();
    for(;iterfail != m_allSendFailClientList.end();++iterfail)
    {
        if(iterfail.value().socket == client)
            iterfail.value().socket = NULL;
    }
}

/**
 * @brief subandpubserver::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn
 */
void subandpubserver::OnProcessDisconnectedNetMes(QWebSocket *conn)
{
    CWebSocketClient *pbrotherclient = isExistClientInBrotherServers(conn);
    if(pbrotherclient != NULL)
    {
        ResetSendFailClient(conn);

        printLog(QsLogging::Level::InfoLevel,
                 QString::fromLocal8Bit("服务器:")+
                 pbrotherclient->getServerIP()+
                 "_"+
                 QString::asprintf("%d",pbrotherclient->getServerPort())+
                 QString::fromLocal8Bit("断开连接."));
        return;
    }

    // 检测在线用户是否有订阅消息，如果有，就取消订阅
    QHash<QString,QList<QWebSocket*>>::iterator iter2 = m_allsubscribes.begin();
    for(;iter2 != m_allsubscribes.end();)
    {
        QList<QWebSocket*>::iterator iter3 = std::find(iter2.value().begin(),
                                               iter2.value().end(),
                                               conn);
        if(iter3 == iter2.value().end())
        {
            ++iter2;
            continue;
        }

        tagSendFailClient *pSendFailClient = getSendFailClient((*iter3));

        printLog(QsLogging::Level::InfoLevel,
                 (pSendFailClient != NULL ? pSendFailClient->uuid : "")+
                 "["+
                 convert_to_ipv4_addr((*iter3)->peerAddress())+
                 "_"+
                 QString::asprintf("%d",(*iter3)->peerPort())+
                 "]"+
                 QString::fromLocal8Bit("成功退订消息:")+
                 iter2.key());

        iter2.value().erase(iter3);   

        if(iter2.value().isEmpty())
        {
            QHash<QString,QString>::iterator iter4 = m_allsubscribePWDs.find(iter2.key());
            if(iter4 !=m_allsubscribePWDs.end())
                m_allsubscribePWDs.erase(iter4);

            iter2 = m_allsubscribes.erase(iter2);
        }
        else
        {
            ++iter2;
        }
    }

    // 清除用户的身份认证信息
    QHash<QWebSocket*,ClientIdentity>::iterator iteriden = m_clientidentityinfos.find(conn);
    if(iteriden != m_clientidentityinfos.end())
    {
        if(iteriden.value() == IDENTITY_PUBLER)
        {
            tagSendFailClient *pSendFailClient = getSendFailClient(iteriden.key());

            printLog(QsLogging::Level::InfoLevel,
                     QString::fromLocal8Bit("客户端:")+
                     (pSendFailClient != NULL ? pSendFailClient->uuid : "")+
                     "["+
                     convert_to_ipv4_addr(conn->peerAddress())+
                     "_"+
                     QString::asprintf("%d",conn->peerPort())+
                     "]"+
                     QString::fromLocal8Bit("断开连接."));
        }

        m_clientidentityinfos.erase(iteriden);
    }

    // 重置客户端
    ResetSendFailClient(conn);
}
