#include "cchatserver.h"

#include <QDateTime>
#include <QWebSocket>

cchatserver::cchatserver(QObject *parent)
    : QObject(parent)
{
    m_MainWebSocketServer.SetNetworkFrameManager(this);
    m_MainWebSocketServer.setAutoDeleteSocket(false);

    init_dbpool(false,this);
}

cchatserver::~cchatserver()
{
    closeServer(false);
    clean_dbpool();
}

/**
 * @brief cchatserver::startServer 启动服务器
 * @param port
 * @return
 */
bool cchatserver::startServer(int port)
{
    if(m_MainWebSocketServer.isListening() ||
       port <= 0 ||
       m_SqliteDataProvider.getdbPath().isEmpty())
    {
        printLog(QsLogging::Level::ErrorLevel,
                 QString::asprintf("cchatserver start failed:")+
                 "port <=0|server is Listening|db is emtpy.");
        return false;
    }

    bool isOk = m_MainWebSocketServer.OpenServer(port);

    printLog(QsLogging::Level::InfoLevel,
             isOk ? QString::asprintf("cchatserver start successed,port:%d.",port) : "cchatserver start failed.");

    m_onlineUsers.clear();
    m_allgrounps.clear();

    if(isOk)
    {
        getAllGroup();
    }

    return isOk;
}

/**
 * @brief cchatserver::closeServer 关闭服务器
 */
void cchatserver::closeServer(bool isprintlog)
{
    if(isprintlog)
        printLog(QsLogging::Level::InfoLevel,"cchatserver close successed.");

    m_MainWebSocketServer.CloseServer();
    m_SqliteDataProvider.disconnect();
}

/**
 * @brief cchatserver::isListening 检测服务器是否启动
 * @return
 */
bool cchatserver::isListening(void)
{
    return m_MainWebSocketServer.isListening();
}

/**
 * @brief cchatserver::openDB 打开指定的数据库
 * @param dbpath
 * @return
 */
bool cchatserver::openDB(QString dbpath)
{
    if(dbpath.isEmpty())
        return false;

    return m_SqliteDataProvider.connect(dbpath);
}

/**
 * @brief cchatserver::getCurrentServerInfo 得到当前服务器信息
 * @return
 */
QString cchatserver::getCurrentServerInfo(void)
{
    QString tmpString = QString::fromLocal8Bit("在线用户总数:")+
                        QString::asprintf("%d",m_onlineUsers.size())+
                        QString::fromLocal8Bit(" 在线群数量:")+
                        QString::asprintf("%d",m_allgrounps.size());

    printLog(QsLogging::Level::InfoLevel,tmpString);

    return tmpString;
}

/**
 * @brief cchatserver::getCurrentDate 得到当前时间
 * @return
 */
QString cchatserver::getCurrentDate(void)
{
    QDateTime dateTime =QDateTime::currentDateTime();
    return dateTime.toString("<b>[yyyy-MM-dd hh:mm:ss]</b> ");
}

/**
 * @brief cchatserver::printLog 打印用户日志
 * @param type
 * @param msg
 */
void cchatserver::printLog(QsLogging::Level type,QString msg)
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
            "<b>CHATSERVER-</b>"+
            msg);
}

/**
 * @brief cchatserver::deletegroup 解散指定ID的群
 * @param groupId
 * @return
 */
bool cchatserver::deletegroup(int groupId)
{
    if(m_allgrounps.isEmpty() || groupId <= 0)
        return false;

    bool isOk = false;

    QHash<QString,tagGroupInfo>::iterator iter = m_allgrounps.begin();
    for(;iter != m_allgrounps.end();++iter)
    {
        if(iter.value().id == groupId)
        {
            m_allgrounps.erase(iter);
            isOk = true;
            break;
        }
    }

    return isOk;
}

/**
 * @brief cchatserver::getGroupInfo 得到指定ID的群信息
 * @param groupId
 * @return
 */
tagGroupInfo* cchatserver::getGroupInfo(int groupId)
{
    if(m_allgrounps.isEmpty() || groupId <= 0)
        return NULL;

    QHash<QString,tagGroupInfo>::iterator iter = m_allgrounps.begin();
    for(;iter != m_allgrounps.end();++iter)
    {
        if(iter.value().id == groupId)
            return &iter.value();
    }

    return NULL;
}

/**
 * @brief cchatserver::getUserInfo 根据用户ID得到用户数据
 * @param userId
 * @return
 */
tagUserInfo* cchatserver::getUserInfo(int userId)
{
    if(m_onlineUsers.isEmpty() || userId <= 0)
        return NULL;

    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.begin();
    for(;iter != m_onlineUsers.end();++iter)
    {
        if(iter.value().id == userId)
            return &iter.value();
    }

    return NULL;
}

/**
 * @brief cchatserver::getAllGroup 得到所有的群
 */
void cchatserver::getAllGroup(void)
{
    RecordSetList record = m_SqliteDataProvider.execSql("select * from chatgroup");

    if(!record.isEmpty())
    {
        for(int i=0;i<record(0).rows();i++)
        {
            tagGroupInfo pGroupInfo(record(0)(i,0).toInt(),
                                    record(0)(i,1),
                                    record(0)(i,2).toInt(),
                                    record(0)(i,3));
            QString memberStr = record(0)(i,4);

            if(!memberStr.isEmpty())
            {
                QStringList memberlist = memberStr.split(",");
                for(int j=0;j<memberlist.size();j++)
                {
                    pGroupInfo.members.push_back(memberlist[j].toInt());
                }
            }

            m_allgrounps[record(0)(i,1)] = pGroupInfo;
        }
    }
}

/**
 * @brief cchatserver::isUserOnline 检测指定ID的用户是否在线
 * @param userId
 * @return
 */
bool cchatserver::isUserOnline(int userId)
{
    if(m_onlineUsers.isEmpty() || userId <= 0)
        return false;

    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.begin();
    for(;iter != m_onlineUsers.end();++iter)
    {
        if(iter.value().id == userId)
            return true;
    }

    return false;
}

/**
 * @brief cchatserver::inClientOnline 检测指定客户端是否在线
 * @param conn
 * @return
 */
bool cchatserver::inClientOnline(QWebSocket *conn)
{
    if(conn == NULL || m_onlineUsers.isEmpty())
        return false;

    // 检查发送者是否在线
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
        return false;

    return true;
}

/**
 * @brief cchatserver::OnProcessNetText 处理网络字符串消息
 * @param conn
 * @param mes
 */
void cchatserver::OnProcessNetText(QWebSocket *conn,QString mes)
{
    QJsonObject msgObj = StringToJson(mes);

    switch(msgObj["msgId"].toInt())
    {
    case IDD_CHATSERVER_USER:                     // 用户操作
    {
        switch(msgObj["subId"].toInt())
        {
        case IDD_CHATSERVER_USER_REGISTER:        // 新用户注册
        {
            onProcessNetUserRegister(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_UPDATEINFO:      // 修改用户信息
        {
            onProcessNetUserUpdateInfo(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_CANCEL:          // 注销用户
        {
            onProcessNetUserCancel(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_LOGIN:           // 用户登录
        {
            onProcessNetUserLogin(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_GETONLINEUSERS:  // 得到所有在线用户信息
        {
            onProcessNetUserGetAllonlineUsers(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_ADD_FRIEND:      // 加好友
        {
            onProcessNetUserAddFriends(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_DEL_FRIEND:      // 删除好友
        {
            onProcessNetUserDeleteFriends(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_GETFRIENDS:      // 得到好友
        {
            onProcessNetUserGetFriends(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_CHAT:            // 聊天消息
        {
            onProcessNetUserChat(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_USER_GETCHATRECORD:   // 得到聊天记录
        {
            onProcessNetUserGetChatRecord(conn,msgObj);
        }
            break;
        default:
            break;
        }
    }
        break;
    case IDD_CHATSERVER_GROUP:                    // 群操作
    {
        switch(msgObj["subId"].toInt())
        {
        case IDD_CHATSERVER_GROUP_CREATE:         // 建立新群
        {
            onProcessNetUserCreateGroup(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_GROUP_DELETE:         // 解散群
        {
            onProcessNetUserDeleteGroup(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_GROUP_JOIN:           // 加入群
        {
            onProcessNetUserJoinGroup(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_GROUP_LEAVE:          // 离开群
        {
            onProcessNetUserLeaveGroup(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_GROUP_CHAT:          // 群聊天
        {
            onProcessNetUserGroupChat(conn,msgObj);
        }
            break;
        case IDD_CHATSERVER_GROUP_GETCHATRECORD:         // 得到群聊天记录
        {
            onProcessNetUserGroupGetChatRecord(conn,msgObj);
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
 * @brief cchatserver::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn
 */
void cchatserver::OnProcessConnectedNetMes(QWebSocket *conn)
{

}

/**
 * @brief cchatserver::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn
 */
void cchatserver::OnProcessDisconnectedNetMes(QWebSocket *conn)
{
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter != m_onlineUsers.end())
        m_onlineUsers.erase(iter);

    conn->deleteLater();
}

/**
 * @brief cchatserver::onProcessNetUserGroupGetChatRecord 处理用户获取聊天记录消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserGroupGetChatRecord(QWebSocket *conn,QJsonObject &msgObj)
{
    int groupid = msgObj["groupid"].toInt();
    QString time = msgObj["time"].toString();
    int limit = msgObj["limit"].toInt();

    // 检查发送者是否在线
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_GETCHATRECORD;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检测指定的群是否存在
    tagGroupInfo *pGroupInfo = getGroupInfo(groupid);
    if(pGroupInfo == NULL)
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_GETCHATRECORD;
        retObj["state"] = IDD_CHATSERVER_OPER_GROUP_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    RecordSetList record = m_SqliteDataProvider.execSql(QString("select * from grouprecord where groupid=%1 and time<%2 limit %3").arg(
                                                            QString::asprintf("%d",groupid),
                                                            time,
                                                            QString::asprintf("%d",limit)));

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_GROUP;
    retObj["subId"] = IDD_CHATSERVER_GROUP_GETCHATRECORD;

    if(!record.isEmpty())
    {
        QJsonArray relArray;
        for(int i=0;i<record(0).rows();i++)
        {
            QJsonObject datObj;
            datObj["id"] = record(0)(i,0).toInt();
            datObj["groupid"] = record(0)(i,1).toInt();
            datObj["senderid"] = record(0)(i,2).toInt();
            datObj["content"] = record(0)(i,3);
            datObj["time"] = record(0)(i,4);

            relArray.push_back(datObj);
        }

        retObj["records"] = relArray;
    }

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserGetChatRecord 处理用户获取聊天记录消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserGetChatRecord(QWebSocket *conn,QJsonObject &msgObj)
{
    int senderid = msgObj["senderid"].toInt();
    int receiverid = msgObj["receiverid"].toInt();
    QString time = msgObj["time"].toString();
    int limit = msgObj["limit"].toInt();

    // 检查发送者是否在线
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_GETCHATRECORD;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查双方参与者是否存在
    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                        QString::asprintf("%d",senderid))).isEmpty() ||
       m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                        QString::asprintf("%d",receiverid))).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_GETCHATRECORD;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    RecordSetList record = m_SqliteDataProvider.execSql(QString("select * from chatrecord where senderid=%1 and receiverid=%2 and time<%3 limit %4").arg(
                                                            QString::asprintf("%d",senderid),
                                                            QString::asprintf("%d",receiverid),
                                                            time,
                                                            QString::asprintf("%d",limit)));

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_GETCHATRECORD;

    if(!record.isEmpty())
    {
        QJsonArray relArray;
        for(int i=0;i<record(0).rows();i++)
        {
            QJsonObject datObj;
            datObj["id"] = record(0)(i,0).toInt();
            datObj["senderid"] = record(0)(i,1).toInt();
            datObj["receiverid"] = record(0)(i,2).toInt();
            datObj["content"] = record(0)(i,3);
            datObj["time"] = record(0)(i,4);

            relArray.push_back(datObj);
        }

        retObj["records"] = relArray;
    }

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserGroupChat 处理用户聊天消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserGroupChat(QWebSocket *conn,QJsonObject &msgObj)
{
    int senderid = msgObj["senderid"].toInt();
    int groupid = msgObj["groupid"].toInt();
    QString content = msgObj["content"].toString();

    // 检查发送者是否在线
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_CHAT;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查发送者是否存在
    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                        QString::asprintf("%d",senderid))).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_CHAT;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检测群存在，而且用户在群中
    tagGroupInfo *pGroupInfo = getGroupInfo(groupid);
    if(pGroupInfo && pGroupInfo->isExistMember(senderid))
    {
        // 向群中所有在线的用户发送信息
        for(int i=0;i<pGroupInfo->members.size();i++)
        {
            tagUserInfo *pUserInfo = getUserInfo(pGroupInfo->members[i]);
            if(pUserInfo == NULL || pUserInfo->conn == NULL)
                continue;

            m_MainWebSocketServer.Send(pUserInfo->conn,JsonToString(msgObj));
        }

        // 先清除超时的聊天记录，聊天记录只保存一周
        QString sqlStrs = QString("delete from grouprecord where (julianday('now','localtime') - julianday(datetime(time, 'unixepoch'))) >= %1 and senderid=%2 and groupid=%3;")
                .arg("7",
                     QString::asprintf("%d",senderid),
                     QString::asprintf("%d",groupid));
        m_SqliteDataProvider.execSql(sqlStrs);

        // 保存聊天记录
        if(m_SqliteDataProvider.execInsertSql(QString("insert into grouprecord (senderid,groupid,content,time) values(%1,%2,'%3',%4);").arg(
                                               QString::asprintf("%d",senderid),
                                               QString::asprintf("%d",groupid),
                                               content,
                                               QString::asprintf("%lld",QDateTime::currentSecsSinceEpoch()))).isEmpty())
        {
            QJsonObject retObj;
            retObj["msgId"] = IDD_CHATSERVER_GROUP;
            retObj["subId"] = IDD_CHATSERVER_GROUP_CHAT;
            retObj["state"] = IDD_CHATSERVER_OPER_FAILED;

            m_MainWebSocketServer.Send(conn,JsonToString(retObj));

            return;
        }

        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_CHAT;
        retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_GROUP;
    retObj["subId"] = IDD_CHATSERVER_GROUP_CHAT;
    retObj["state"] = IDD_CHATSERVER_OPER_FAILED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserChat 处理用户聊天消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserChat(QWebSocket *conn,QJsonObject &msgObj)
{
    int senderid = msgObj["senderid"].toInt();
    int receiverid = msgObj["receiverid"].toInt();
    QString content = msgObj["content"].toString();

    // 检查发送者是否在线
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_CHAT;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查双方参与者是否存在
    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                        QString::asprintf("%d",senderid))).isEmpty() ||
       m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                        QString::asprintf("%d",receiverid))).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_CHAT;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 如果接收者在线，发送给接收者
    tagUserInfo *preciverInfo = getUserInfo(receiverid);
    if(preciverInfo)
    {
        m_MainWebSocketServer.Send(conn,JsonToString(msgObj));
    }

    // 先清除超时的聊天记录，聊天记录只保存一周
    QString sqlStrs = QString("delete from chatrecord where (julianday('now','localtime') - julianday(datetime(time, 'unixepoch'))) >= %1 "
                              "and senderid=%2 and receiverid=%3;")
                            .arg("7",
                                 QString::asprintf("%d",senderid),
                                 QString::asprintf("%d",receiverid));
    m_SqliteDataProvider.execSql(sqlStrs);

    // 保存聊天记录
    m_SqliteDataProvider.execInsertSql(QString("insert into chatrecord (senderid,receiverid,content,time) values(%1,%2,'%3',%4);").arg(
                                           QString::asprintf("%d",senderid),
                                           QString::asprintf("%d",receiverid),
                                           content,
                                           QString::asprintf("%lld",QDateTime::currentSecsSinceEpoch())));

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_CHAT;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserGetFriends 处理用户得到好友消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserGetFriends(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_GETFRIENDS;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查用户是否在线，只有在线用户才能得到好友列表
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_GETFRIENDS;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    QJsonArray msgArray;
    for(int i=0;i<iter.value().friends.size();i++)
    {
        QJsonObject userObj;
        userObj["userid"] = iter.value().friends[i];

        RecordSetList record = m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                                                QString::asprintf("%d",iter.value().friends[i])));
        if(!record.isEmpty())
        {
            userObj["name"] = record(0)(0,1);
        }

        msgArray.push_back(userObj);
    }

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_GETFRIENDS;
    retObj["friends"] = msgArray;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserUpdateInfo 处理用户修改信息消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserUpdateInfo(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString olduserpwd = msgObj["olduserpwd"].toString();
    QString newuserpwd = msgObj["newuserpwd"].toString();

    RecordSetList record = m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                                            QString::asprintf("%d",userid),
                                                            olduserpwd));
    if(record.isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_UPDATEINFO;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    m_SqliteDataProvider.execSql(QString("update user set userpwd='%1' where id=%2").arg(
                                        newuserpwd,
                                        QString::asprintf("%d",userid)));

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_UPDATEINFO;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserCancel 处理用户注销消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserCancel(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_CANCEL;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 从相应群中删除这个用户
    QHash<QString,tagGroupInfo>::iterator itergroup = m_allgrounps.begin();
    for(;itergroup != m_allgrounps.end();++itergroup)
    {
        if(itergroup.value().deleteMember(userid))
        {
            QString tmpStrgroup;
            for(int k=0;k<itergroup.value().members.size();k++)
            {
                tmpStrgroup += QString::asprintf("%d,",itergroup.value().members[k]);
            }

            if(!tmpStrgroup.isEmpty())
                tmpStrgroup = tmpStrgroup.mid(0,tmpStrgroup.length()-1);

            m_SqliteDataProvider.execSql(QString("update chatgroup set members='%1' where id=%2").arg(
                                                tmpStrgroup,
                                                QString::asprintf("%d",itergroup.value().id)));
        }
    }

    m_SqliteDataProvider.execSql(QString("delete from user where id=%1").arg(
                                        QString::asprintf("%d",userid)));

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_CANCEL;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserLogin 处理用户登录消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserLogin(QWebSocket *conn,QJsonObject &msgObj)
{
    QString username = msgObj["username"].toString();
    QString userpwd = msgObj["userpwd"].toString();

    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter != m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_LOGIN;
        retObj["state"] = IDD_CHATSERVER_OPER_EXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    RecordSetList precord = m_SqliteDataProvider.execSql(QString("select * from user where username='%1' and userpwd='%2'").arg(
                                                            username,
                                                            userpwd));

    if(precord.isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_LOGIN;
        retObj["state"] = IDD_CHATSERVER_OPER_FAILED;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    tagUserInfo pUserInfo(precord(0)(0,0).toInt(),
                          precord(0)(0,1),
                          precord(0)(0,2));

    if(!precord(0)(0,3).isEmpty())
    {
        QStringList pfriends = precord(0)(0,3).split(",");
        for(int i=0;i<pfriends.size();i++)
        {
            pUserInfo.friends.push_back(pfriends[i].toInt());
        }
    }

    pUserInfo.conn = conn;
    m_onlineUsers[conn] = pUserInfo;

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_LOGIN;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserGetAllonlineUsers 处理用户得到当前所有在线用户信息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserGetAllonlineUsers(QWebSocket *conn,QJsonObject &msgObj)
{
    QJsonObject returnObj;
    returnObj["msgId"] = IDD_CHATSERVER_USER;
    returnObj["subId"] = IDD_CHATSERVER_USER_GETONLINEUSERS;

    QJsonArray msgArray;
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.begin();
    for(;iter != m_onlineUsers.end();++iter)
    {
        QJsonObject userObj;
        userObj["id"] = iter.value().id;
        userObj["name"] = iter.value().name;

        msgArray.push_back(userObj);
    }

    returnObj["users"] = msgArray;

    m_MainWebSocketServer.Send(conn,JsonToString(returnObj));
}

/**
 * @brief cchatserver::onProcessNetUserRegister 处理用户注册消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserRegister(QWebSocket *conn,QJsonObject &msgObj)
{
    QString username = msgObj["username"].toString();
    QString userpwd = msgObj["userpwd"].toString();

    RecordSetList record = m_SqliteDataProvider.execSql(QString("select * from user where username='%1'").arg(
                                                            username));

    if(!record.isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_REGISTER;
        retObj["state"] = IDD_CHATSERVER_OPER_EXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(!m_SqliteDataProvider.execInsertSql(QString("insert into user (username,userpwd) values('%1','%2')").arg(
                                        username,
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_REGISTER;
        retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_REGISTER;
    retObj["state"] = IDD_CHATSERVER_OPER_FAILED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserDeleteFriends 处理用户删除好友消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserDeleteFriends(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();
    int friendid = msgObj["friendid"].toInt();

    // 检查用户是否在线，只有在线用户才能添加
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_DEL_FRIEND;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_DEL_FRIEND;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查好友是否存在
    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                        QString::asprintf("%d",friendid))).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_DEL_FRIEND;
        retObj["state"] = IDD_CHATSERVER_OPER_FRIEND_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(iter.value().deleteFriend(friendid))
    {
        QString tmpStr;
        for(int i=0;i<iter.value().friends.size();i++)
        {
            tmpStr += QString::asprintf("%d,",iter.value().friends[i]);
        }

        tmpStr = tmpStr.mid(0,tmpStr.length()-1);

        // 更新数据库
        m_SqliteDataProvider.execSql(QString("update user set friends='%1' where id=%2").arg(
                                                tmpStr,
                                                QString::asprintf("%d",userid)));
    }

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_DEL_FRIEND;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserAddFriends 处理用户加好友消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserAddFriends(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();
    int friendid = msgObj["friendid"].toInt();

    // 检查用户是否在线，只有在线用户才能添加
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_ADD_FRIEND;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_ADD_FRIEND;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查好友是否存在
    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1").arg(
                                        QString::asprintf("%d",friendid))).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_USER;
        retObj["subId"] = IDD_CHATSERVER_USER_ADD_FRIEND;
        retObj["state"] = IDD_CHATSERVER_OPER_FRIEND_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(!iter.value().isExistFriend(friendid))
    {
        iter.value().friends.push_back(friendid);

        QString tmpStr;
        for(int i=0;i<iter.value().friends.size();i++)
        {
            tmpStr += QString::asprintf("%d,",iter.value().friends[i]);
        }

        tmpStr = tmpStr.mid(0,tmpStr.length()-1);

        // 更新数据库
        m_SqliteDataProvider.execSql(QString("update user set friends='%1' where id=%2").arg(
                                                tmpStr,
                                                QString::asprintf("%d",userid)));
    }

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_USER;
    retObj["subId"] = IDD_CHATSERVER_USER_ADD_FRIEND;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserDeleteGroup 处理解散群消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserDeleteGroup(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();
    int groupid = msgObj["groupid"].toInt();

    // 检查用户是否在线，只有在线用户才能添加
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_DELETE;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_DELETE;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检测要解散的群是否存在
    if(getGroupInfo(groupid) == NULL)
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_DELETE;
        retObj["state"] = IDD_CHATSERVER_OPER_GROUP_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    m_SqliteDataProvider.execInsertSql(QString("delete from chatgroup where id=%1").arg(
                                           QString::asprintf("%d",groupid)));

    bool isOk = deletegroup(groupid);

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_GROUP;
    retObj["subId"] = IDD_CHATSERVER_GROUP_DELETE;
    retObj["state"] = (!isOk ? IDD_CHATSERVER_OPER_FAILED : IDD_CHATSERVER_OPER_SUCCESSED);

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserCreateGroup 处理建立新群消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserCreateGroup(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();
    QString groupname = msgObj["groupname"].toString();
    QString introduce = msgObj["introduce"].toString();

    // 检查用户是否在线，只有在线用户才能添加
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_CREATE;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_CREATE;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检测要建立的群是否存在
    QHash<QString,tagGroupInfo>::iterator itergroup = m_allgrounps.find(groupname);
    if(itergroup != m_allgrounps.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_CREATE;
        retObj["state"] = IDD_CHATSERVER_OPER_EXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    RecordSetList record = m_SqliteDataProvider.execInsertSql(QString("insert into chatgroup (name,creatorid,introduce,members) values('%1',%2,'%3','%4')").arg(
                                           groupname,
                                           QString::asprintf("%d",userid),
                                           introduce,
                                           QString::asprintf("%d",userid)));

    if(!record.isEmpty())
    {
        tagGroupInfo pGroupInfo = tagGroupInfo(record(0)(0,0).toInt(),
                                               groupname,
                                               userid,
                                               introduce);

        pGroupInfo.members.push_back(userid);

        m_allgrounps[groupname] = pGroupInfo;
    }

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_GROUP;
    retObj["subId"] = IDD_CHATSERVER_GROUP_CREATE;
    retObj["state"] = (record.isEmpty() ? IDD_CHATSERVER_OPER_FAILED : IDD_CHATSERVER_OPER_SUCCESSED);

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserLeaveGroup 处理离开群消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserLeaveGroup(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();
    int groupid = msgObj["groupid"].toInt();

    // 检查用户是否在线，只有在线用户才能添加
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_LEAVE;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_LEAVE;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查指定ID的群是否存在
    tagGroupInfo *pGroupInfo = getGroupInfo(groupid);
    if(pGroupInfo == NULL)
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_LEAVE;
        retObj["state"] = IDD_CHATSERVER_OPER_GROUP_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查群里面是否已经有用户存在
    if(!pGroupInfo->isExistMember(userid))
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_JOIN;
        retObj["state"] = IDD_CHATSERVER_OPER_GROUP_USERNOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(pGroupInfo->deleteMember(userid))
    {
        // 更新数据库
        m_SqliteDataProvider.execSql(QString("update chatgroup set members='%1' where id=%2").arg(
                                                pGroupInfo->getMemberString(),
                                                QString::asprintf("%d",pGroupInfo->id)));
    }

    // 如果群里一个人都没有，就清除掉这个群
    if(pGroupInfo->members.isEmpty())
    {
        QHash<QString,tagGroupInfo>::iterator itergroup = m_allgrounps.find(pGroupInfo->name);
        if(itergroup != m_allgrounps.end())
        {
            // 更新数据库
            m_SqliteDataProvider.execSql(QString("delete from chatgroup where id=%1").arg(
                                                    QString::asprintf("%d",pGroupInfo->id)));

            m_allgrounps.erase(itergroup);
        }
    }

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_GROUP;
    retObj["subId"] = IDD_CHATSERVER_GROUP_JOIN;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}

/**
 * @brief cchatserver::onProcessNetUserJoinGroup 处理加入群消息
 * @param conn
 * @param msgObj
 */
void cchatserver::onProcessNetUserJoinGroup(QWebSocket *conn,QJsonObject &msgObj)
{
    int userid = msgObj["userid"].toInt();
    QString userpwd = msgObj["userpwd"].toString();
    int groupid = msgObj["groupid"].toInt();

    // 检查用户是否在线，只有在线用户才能添加
    QHash<QWebSocket*,tagUserInfo>::iterator iter = m_onlineUsers.find(conn);
    if(iter == m_onlineUsers.end())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_JOIN;
        retObj["state"] = IDD_CHATSERVER_OPER_OFFLINE;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    if(m_SqliteDataProvider.execSql(QString("select * from user where id=%1 and userpwd='%2'").arg(
                                        QString::asprintf("%d",userid),
                                        userpwd)).isEmpty())
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_JOIN;
        retObj["state"] = IDD_CHATSERVER_OPER_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查指定ID的群是否存在
    tagGroupInfo *pGroupInfo = getGroupInfo(groupid);
    if(pGroupInfo == NULL)
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_JOIN;
        retObj["state"] = IDD_CHATSERVER_OPER_GROUP_NOEXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    // 检查群里面是否已经有用户存在
    if(pGroupInfo->isExistMember(userid))
    {
        QJsonObject retObj;
        retObj["msgId"] = IDD_CHATSERVER_GROUP;
        retObj["subId"] = IDD_CHATSERVER_GROUP_JOIN;
        retObj["state"] = IDD_CHATSERVER_OPER_GROUP_USEREXIST;

        m_MainWebSocketServer.Send(conn,JsonToString(retObj));

        return;
    }

    pGroupInfo->members.push_back(userid);

    // 更新数据库
    m_SqliteDataProvider.execSql(QString("update chatgroup set members='%1' where id=%2").arg(
                                            pGroupInfo->getMemberString(),
                                            QString::asprintf("%d",pGroupInfo->id)));

    QJsonObject retObj;
    retObj["msgId"] = IDD_CHATSERVER_GROUP;
    retObj["subId"] = IDD_CHATSERVER_GROUP_JOIN;
    retObj["state"] = IDD_CHATSERVER_OPER_SUCCESSED;

    m_MainWebSocketServer.Send(conn,JsonToString(retObj));
}
