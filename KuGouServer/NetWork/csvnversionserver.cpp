#include "csvnversionserver.h"
#include "QsLog.h"
#include "ndbpool.h"
#include "QSqlError.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QDateTime>
#include <QJsonArray>

/**
 * @brief CSVNVersionServer::CSVNVersionServer 构造函数
 * @param parent
 */
CSVNVersionServer::CSVNVersionServer(QObject *parent)
    : QObject(parent)
{
    m_webSocketServer.SetNetworkFrameManager(this);
}

/**
 * @brief CSVNVersionServer::~CSVNVersionServer 析构函数
 */
CSVNVersionServer::~CSVNVersionServer()
{
    stopServer();
}

/**
 * @brief CSVNVersionServer::setDataBase 设置版本库所使用数据库
 * @param dbPath 数据库文件路径
 */
void CSVNVersionServer::setDataBase(const QString& dbPath)
{
    if(dbPath.isEmpty())
        return;

    m_sqliteDataProvider.connect(dbPath);
}

/**
 * @brief CSVNVersionServer::startServer 启动服务器
 * @param serverPort 要启动的服务器的端口
 * @param maxClients 服务器能接受的最大客户端个数，初始时30
 *
 */
void CSVNVersionServer::startServer(int serverPort,int maxClients)
{
    m_webSocketServer.setMaxPendingConnections(maxClients);
    m_webSocketServer.OpenServer(serverPort);
}

/**
 * @brief CSVNVersionServer::stopServer 停止服务器
 */
void CSVNVersionServer::stopServer()
{
    m_webSocketServer.CloseServer();
}

/**
 * @brief CSVNVersionServer::insertuseroperatorlog 插入一条用户操作日志信息
 * @param userid 操作用户的ID
 * @param type 日志类型
 * @param grade 日志等级：0-普通；1-警告；2-错误；3-致命
 * @param logmsg 日志内容
 */
void CSVNVersionServer::insertuseroperatorlog(int userid,int type,int grade,const QString& logmsg)
{
    QDateTime ptime = QDateTime::currentDateTime();
    m_sqliteDataProvider.execSql(QString("insert into operationlog (userid,grade,type,log,time) values(%1,%2,%3,'%4',%5)")
                                  .arg(userid).arg(grade).arg(type).arg(logmsg).arg(ptime.toSecsSinceEpoch()));

    emit on_signalemitLogMessage(QString::asprintf("%d(%d)[%d]:",userid,type,grade)+
                                 logmsg);
}

/**
 * @brief CSVNVersionServer::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn 到达的新的连接
 */
void CSVNVersionServer::OnProcessConnectedNetMes(QWebSocket *conn)
{
    qDebug()<<"CSVNVersionServer::OnProcessConnectedNetMes";
}

/**
 * @brief CSVNVersionServer::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn 要断开的连接
 */
void CSVNVersionServer::OnProcessDisconnectedNetMes(QWebSocket *conn)
{
    qDebug()<<"CSVNVersionServer::OnProcessDisconnectedNetMes";
}

/**
 * @brief CSVNVersionServer::OnProcessNetBinary 处理网络二进制消息
 * @param conn 要处理的客户端
 * @param data 到达的二进制消息
 */
void CSVNVersionServer::OnProcessNetBinary(QWebSocket *conn,QByteArray &data)
{
    tagVersionOper pVersionOper;
    QByteArray reciverData = data;

    memset(&pVersionOper,0,sizeof(tagVersionOper));
    memcpy(&pVersionOper,reciverData.data(),sizeof(tagVersionOper));
    reciverData.remove(0,sizeof(tagVersionOper));

    switch(pVersionOper.tagIndex)
    {
    case IDD_SVNVERSION_VERSION_DAT_ADD:
    {
        RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from verstions where id=%1 and mainid=%2 and subid=%3")
                                                             .arg(pVersionOper.versionid)
                                                             .arg(pVersionOper.mainid)
                                                             .arg(pVersionOper.subid));
        if(pRecord.isEmpty())
        {
            QJsonObject mesObjReturn;
            mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
            mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_DAT_ADD;
            mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
            m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

            return;
        }

        if(!addVersionData(pVersionOper.versionid,reciverData))
        {
            QJsonObject mesObjReturn;
            mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
            mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_DAT_ADD;
            mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
            m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

            return;
        }

        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_DAT_ADD;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_SUC;
        mesObjReturn["mainproid"] = pVersionOper.mainid;
        mesObjReturn["subproid"] = pVersionOper.subid;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));
    }
        break;
    default:
        break;
    }
}

/**
 * @brief CSVNVersionServer::addVersionData 添加指定ID保存的版本数据
 * @param id 要保存的版本数据的ID
 * @param data 要保存的数据
 * @return 如果操作成功返回真，否则返回假
 */
bool CSVNVersionServer::addVersionData(int id,const QByteArray& data)
{
    if(id <=0 || data.isEmpty())
        return false;

    QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_sqliteDataProvider.getdbPath());
    QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";

    if(!tempDB.isOpen())
        return false;

    QSqlQuery queryresult(tempDB);

    queryresult.prepare("update verstions set content=:content where id="+QString::asprintf("%d",id));
    queryresult.bindValue(":content", data);

    if(!queryresult.exec())
    {
        QLOG_ERROR()<<"query error:"<<queryresult.lastError().text();
        return false;
    }

    return true;
}

/**
 * @brief getVersionData 得到指定ID保存的版本数据
 * @param id 要得到保存的版本数据的ID,如果ID为-1，返回最新的数据
 * @return 如果操作成功返回真，否则返回假
 */
QByteArray CSVNVersionServer::getVersionData(int id)
{
    QByteArray returndata;

    QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_sqliteDataProvider.getdbPath());
    QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";

    if(!tempDB.isOpen())
        return returndata;

    QSqlQuery queryresult(tempDB);

    if(id > 0)
        queryresult.prepare("select content from verstions where id="+QString::asprintf("%d",id));
    else
        queryresult.prepare("select * from verstions order by updatetime desc limit 1");

    if(!queryresult.exec())
    {
        QLOG_ERROR()<<"query error:"<<queryresult.lastError().text();
        return returndata;
    }

    queryresult.next();
    returndata = queryresult.value(0).toByteArray();

    return returndata;
}

/**
 * @brief CSVNVersionServer::isExistUser 检测指定用户ID和密码的用户是否存在
 * @param id 用户ID
 * @param pwd 用户密码
 * @return 如果用户存在返回真，否则返回假
 */
bool CSVNVersionServer::isExistUser(QWebSocket *conn,int id, const QString &pwd)
{
    if(id <= 0 || pwd.isEmpty())
        return false;

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from users where id=%1 and userpwd='%2'")
                                                         .arg(id).arg(pwd));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERION_USER;
        mesObjReturn["subid"] = IDD_SVNVERION_LOGIN_USER;
        mesObjReturn["state"] = IDD_SVNVERION_USER_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return false;
    }

    return true;
}

/**
 * @brief CSVNVersionServer::OnProcessNetText 处理网络字符串消息
 * @param conn 要处理的客户端
 * @param mes 到达的字符串消息
 */
void CSVNVersionServer::OnProcessNetText(QWebSocket *conn,QString mes)
{
    if(mes.isEmpty())
        return;

    qDebug()<<"CSVNVersionServer::OnProcessNetText:"<<mes;

    //emit on_signalemitLogMessage("CSVNVersionServer::OnProcessNetText:"+
    //                             mes);

    QJsonObject mesObj = StringToJson(mes);

    switch (mesObj["subid"].toInt())
    {
    case IDD_SVNVERION_REGISTER_USER:            // 注册用户
    {
        onProcessNetRegiterUser(conn,mesObj);
    }
        break;
    case IDD_SVNVERION_UPDATE_USER:            // 更新用户
    {
        onProcessNetUpdateUser(conn,mesObj);
    }
        break;
    case IDD_SVNVERION_DELETE_USER:            // 删除用户
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetDeleteUser(conn,mesObj);
    }
        break;
    case IDD_SVNVERION_LOGIN_USER:            // 用户登陆
    {
        onProcessNetLoginUser(conn,mesObj);
    }
        break;
    case IDD_SVNVERSION_PROJECT_ADD:           // 项目添加
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetProjectAdd(conn,mesObj);
    }
        break;
    case IDD_SVNVERSION_PROJECT_DEL:           // 项目删除
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetProjectDel(conn,mesObj);
    }
        break;
    case IDD_SVNVERSION_VERSION_ADD:           // 版本添加
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetVersionAdd(conn,mesObj);
    }
        break;
    case IDD_SVNVERSION_VERSION_DAT_GET:       // 得到版本数据
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetVersionGetData(conn,mesObj);
    }
        break;
    case IDD_SVNVERSION_PROJECT_INFO_GET:       // 得到项目信息
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetProjectGetInfo(conn,mesObj);
    }
        break;
    case IDD_SVNVERSION_PROJECT_VERSIONS_GET:       // 得到版本信息
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetProjectGetVersionsInfo(conn,mesObj);
    }
        break;
    case IDD_SVNVERION_GET_USERS:                   // 得到用户的信息
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetProjectGetUserInfo(conn,mesObj);
    }
        break;
    default:
        break;
    }
}

/**
 * @brief CSVNVersionServer::onProcessNetProjectGetUserInfo 处理项目得到用户信息
 * @param conn
 * @param mesObj
 */
void CSVNVersionServer::onProcessNetProjectGetUserInfo(QWebSocket *conn,QJsonObject &mesObj)
{
    int userid = mesObj["userid"].toInt();

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERION_USER;
    mesObjReturn["subid"] = IDD_SVNVERION_GET_USERS;

    RecordSetList pRecord;

    if(userid == -1)
        pRecord = m_sqliteDataProvider.execSql(QString("select * from users order by updatetime desc;"));
    else
        pRecord = m_sqliteDataProvider.execSql(QString("select * from users where id=%1;")
                                               .arg(userid));

    QJsonArray usersArray;

    for(int k=0;k<static_cast<int>(pRecord(0).rows());k++)
    {
        QJsonObject subproObj;
        subproObj["id"] = pRecord(0)(k,0).toInt();
        subproObj["username"] = pRecord(0)(k,1).toStdString().c_str();
        subproObj["userpwd"] = pRecord(0)(k,2).toStdString().c_str();
        subproObj["type"] = pRecord(0)(k,3).toInt();
        subproObj["projects"] = pRecord(0)(k,4).toStdString().c_str();
        subproObj["devices"] = pRecord(0)(k,5).toStdString().c_str();
        subproObj["updatetime"] = pRecord(0)(k,6).toStdString().c_str();

        usersArray.push_back(subproObj);
    }

    mesObjReturn["users"] = usersArray;

    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));
}

/**
 * @brief CSVNVersionServer::onProcessNetProjectGetVersionsInfo 处理项目得到版本信息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetProjectGetVersionsInfo(QWebSocket *conn,QJsonObject &mesObj)
{
    int mainid = mesObj["mainproid"].toInt();
    int subid = mesObj["subproid"].toInt();

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_VERSIONS_GET;

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select id,mainid,subid,type,changefiles,log,updatetime "
                                                                 "from verstions where mainid=%1 and subid=%2 order by updatetime desc")
                                                         .arg(mainid)
                                                         .arg(subid));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_VERSIONS_GET;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    mesObjReturn["mainproid"] = mainid;
    mesObjReturn["subproid"] = subid;

    QJsonArray verstionsArray;

    for(int k=0;k<static_cast<int>(pRecord(0).rows());k++)
    {
        QJsonObject subproObj;
        subproObj["id"] = pRecord(0)(k,0).toInt();
        subproObj["mainid"] = mainid;
        subproObj["subid"] = pRecord(0)(k,2).toInt();
        subproObj["type"] = pRecord(0)(k,3).toInt();
        subproObj["changefiles"] = pRecord(0)(k,4).toStdString().c_str();
        subproObj["log"] = pRecord(0)(k,5);
        subproObj["updatetime"] = pRecord(0)(k,6).toInt();

        verstionsArray.push_back(subproObj);
    }

    mesObjReturn["vertions"] = verstionsArray;

    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

    insertuseroperatorlog(mesObj["myid"].toInt(),
                          IDD_SVNVERSION_PROJECT_VERSIONS_GET,
                          0,
                          QString::asprintf("%d Get version information:%d",mesObj["myid"].toInt(),mainid));
}

/**
 * @brief CSVNVersionServer::onProcessNetProjectGetInfo 处理项目得到项目信息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetProjectGetInfo(QWebSocket *conn,QJsonObject &mesObj)
{
    int mainid = mesObj["mainid"].toInt();
    QJsonArray subids = mesObj["subids"].toArray();

    RecordSetList pRecord;

    // 获取所有的项目
    if(mainid == -1 && subids.isEmpty())
        pRecord = m_sqliteDataProvider.execSql(QString("select * from mainproject"));
    else
        pRecord = m_sqliteDataProvider.execSql(QString("select * from mainproject where id=%1").arg(mainid));

    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_INFO_GET;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_INFO_GET;

    QJsonArray mainproArray;

    for(int i=0;i<static_cast<int>(pRecord(0).rows());i++)
    {
        QJsonObject mainproObj;
        mainproObj["id"] = pRecord(0)(i,0).toInt();
        mainproObj["name"] = pRecord(0)(i,1);
        mainproObj["describe"] = pRecord(0)(i,2);

        mainproArray.push_back(mainproObj);
    }

    mesObjReturn["mainPros"] = mainproArray;

    QJsonArray subproArray;

    if(subids.isEmpty())
    {
        if(mainid == -1)
            pRecord = m_sqliteDataProvider.execSql(QString("select * from subproject"));
        else
            pRecord = m_sqliteDataProvider.execSql(QString("select * from subproject where mainid=%1").arg(mainid));

        if(!pRecord.isEmpty())
        {
            for(int i=0;i<static_cast<int>(pRecord(0).rows());i++)
            {
                QJsonObject subproObj;
                subproObj["id"] = pRecord(0)(i,0).toInt();
                subproObj["mainid"] = pRecord(0)(i,1).toInt();
                subproObj["name"] = pRecord(0)(i,2);
                subproObj["describe"] = pRecord(0)(i,3);

                subproArray.push_back(subproObj);
            }
        }
    }
    else
    {
        for(auto && subid : subids)
        {
            pRecord = m_sqliteDataProvider.execSql(QString("select * from subproject where mainid=%1 and id=%2").arg(mainid).arg(subid.toObject()["subid"].toInt()));
            if(!pRecord.isEmpty())
            {
                QJsonObject subproObj;
                subproObj["id"] = pRecord(0)(0,0).toInt();
                subproObj["mainid"] = mainid;
                subproObj["name"] = pRecord(0)(0,2);
                subproObj["describe"] = pRecord(0)(0,3);

                subproArray.push_back(subproObj);
            }
        }
    }

    mesObjReturn["subPros"] = subproArray;

    /*QJsonArray verstionsArray;

    for(int i=0;i<subproIndexs.size();i++)
    {
        pRecord = m_sqliteDataProvider.execSql(QString("select id,mainid,subid,type,changefiles,log,updatetime from verstions where mainid=%1 and subid=%2").arg(mainid).arg(subproIndexs[i]));
        if(!pRecord.isEmpty())
        {
            for(int k=0;k<(int)pRecord(0).rows();k++)
            {
                QJsonObject subproObj;
                subproObj["id"] = pRecord(0)(k,0).toInt();
                subproObj["mainid"] = mainid;
                subproObj["subid"] = pRecord(0)(k,2).toInt();
                subproObj["type"] = pRecord(0)(k,3).toInt();
                subproObj["changefiles"] = pRecord(0)(k,4).toStdString().c_str();
                subproObj["log"] = pRecord(0)(k,5);
                subproObj["updatetime"] = pRecord(0)(k,6).toInt();

                verstionsArray.push_back(subproObj);
            }
        }
    }

    mesObjReturn["vertions"] = verstionsArray;*/

    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

    insertuseroperatorlog(mesObj["myid"].toInt(),
                          IDD_SVNVERSION_PROJECT_INFO_GET,
                          0,
                          QString::asprintf("%d Get project information:%d",mesObj["myid"].toInt(),mainid));
}

/**
 * @brief CSVNVersionServer::onProcessNetVersionGetData 处理项目得到版本
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetVersionGetData(QWebSocket *conn,QJsonObject &mesObj)
{
    int id = mesObj["id"].toInt();
    int mainId = mesObj["mainId"].toInt();
    int subId = mesObj["subId"].toInt();

    QByteArray VersionData;

    if(id > 0)
    {
        VersionData = getVersionData(id);
    }
    else
    {
        RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select id from verstions where mainid=%1 and subid=%2 order by updatetime desc limit 1")
                                                             .arg(mainId)
                                                             .arg(subId));
        if(!pRecord.isEmpty())
        {
            VersionData = getVersionData(pRecord(0)(0,0).toInt());
        }
    }

    if(VersionData.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_DAT_GET;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;

        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    tagVersionOper pVersionOper;
    memset(&pVersionOper,0,sizeof(tagVersionOper));

    pVersionOper.tagIndex = IDD_SVNVERSION_VERSION_DAT_GET;

    QByteArray sendData;
    sendData.append((const char*)&pVersionOper,sizeof(tagVersionOper));
    sendData.append(VersionData);

    m_webSocketServer.SendByteArray(conn,sendData);

    insertuseroperatorlog(mesObj["myid"].toInt(),
                          IDD_SVNVERSION_VERSION_DAT_GET,
                          0,
                          QString::asprintf("%d Get version data:%d",mesObj["myid"].toInt(),id));
}

/**
 * @brief CSVNVersionServer::onProcessNetVersionAdd 处理项目添加版本
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetVersionAdd(QWebSocket *conn,QJsonObject &mesObj)
{
    int mainid = mesObj["mainid"].toInt();
    int subid = mesObj["subproid"].toInt();
    int type = mesObj["type"].toInt();
    QString log = mesObj["log"].toString();
    QJsonArray filechangedata = mesObj["filechangedata"].toArray();

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from mainproject where id=%1").arg(mainid));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_ADD;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    pRecord = m_sqliteDataProvider.execSql(QString("select * from subproject where id=%1").arg(subid));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_ADD;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    QString changefiles;

    for(auto && i : filechangedata)
    {
        changefiles += i.toString();
        changefiles += "&";
    }

    if(!changefiles.isEmpty())
        changefiles = changefiles.mid(0,changefiles.length()-1);

    QDateTime time = QDateTime::currentDateTime();
    pRecord = m_sqliteDataProvider.execInsertSql(QString("insert into verstions (mainid,subid,type,changefiles,log,updatetime) values (%1,%2,%3,'%4','%5',%6)")
                                 .arg(mainid).arg(subid).arg(type).arg(changefiles, log).arg(time.toSecsSinceEpoch()));

    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_ADD;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    int lastId = pRecord(0)(0,0).toInt();

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObjReturn["subid"] = IDD_SVNVERSION_VERSION_ADD;
    mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_SUC;
    mesObjReturn["mainproid"] = mainid;
    mesObjReturn["subproid"] = subid;
    mesObjReturn["versionid"] = lastId;
    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

    insertuseroperatorlog(mesObj["myid"].toInt(),
                          IDD_SVNVERSION_VERSION_ADD,
                          0,
                          QString::asprintf("%d Add version:%d",mesObj["myid"].toInt(),lastId));
}

/**
 * @brief CSVNVersionServer::onProcessNetProjectDel 处理项目删除消息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetProjectDel(QWebSocket *conn,QJsonObject &mesObj)
{
    int id = mesObj["id"].toInt();
    int mainproid = mesObj["mainproid"].toInt();
    int subproid = mesObj["subproid"].toInt();
    int type = mesObj["type"].toInt();

    bool isOk = false;

    switch(type)
    {
    case 0:           // 主项目
    {
        RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from subproject where mainid=%1").arg(mainproid));
        if(!pRecord.isEmpty())
        {
            for(int i=0;i<static_cast<int>(pRecord(0).rows());i++)
            {
                m_sqliteDataProvider.execSql(QString("delete from verstions where mainid=%1 and subid=%2")
                                             .arg(mainproid).arg(pRecord(0)(i,0).toInt()));

                m_sqliteDataProvider.execSql(QString("delete from subproject where id=%1")
                                             .arg(pRecord(0)(i,0).toInt()));
            }
        }

        m_sqliteDataProvider.execSql(QString("delete from mainproject where id=%1")
                                     .arg(mainproid));

        isOk = true;

        insertuseroperatorlog(mesObj["myid"].toInt(),
                              IDD_SVNVERSION_PROJECT_DEL,
                              0,
                              QString::asprintf("%d Delete main item:%d",mesObj["myid"].toInt(),mainproid));
    }
        break;
    case 1:           // 子项目
    {
        m_sqliteDataProvider.execSql(QString("delete from verstions where mainid=%1 and subid=%2")
                                     .arg(mainproid).arg(subproid));

        m_sqliteDataProvider.execSql(QString("delete from subproject where id=%1")
                                     .arg(subproid));

        isOk = true;

        insertuseroperatorlog(mesObj["myid"].toInt(),
                              IDD_SVNVERSION_PROJECT_DEL,
                              0,
                              QString::asprintf("%d Delete sub item:%d",mesObj["myid"].toInt(),subproid));
    }
        break;
    case 2:           // 版本
    {
        m_sqliteDataProvider.execSql(QString("delete from verstions where id=%1")
                                     .arg(id));

        isOk = true;

        insertuseroperatorlog(mesObj["myid"].toInt(),
                              IDD_SVNVERSION_PROJECT_DEL,
                              0,
                              QString::asprintf("%d Delete version:%d",mesObj["myid"].toInt(),id));
    }
        break;
    default:
        break;
    }

    if(isOk)
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_DEL;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_SUC;
        mesObjReturn["proid"] = id;
        mesObjReturn["mainproid"] = mainproid;
        mesObjReturn["subproid"] = subproid;
        mesObjReturn["protype"] = type;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));
    }
    else
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_DEL;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
        mesObjReturn["proid"] = id;
        mesObjReturn["mainproid"] = mainproid;
        mesObjReturn["subproid"] = subproid;
        mesObjReturn["protype"] = type;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));
    }
}

/**
 * @brief CSVNVersionServer::onProcessNetProjectAdd 处理项目添加消息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetProjectAdd(QWebSocket *conn,QJsonObject &mesObj)
{
    int type = mesObj["type"].toInt();
    QString name = mesObj["name"].toString();
    QString describe = mesObj["describe"].toString();

    switch(type)
    {
    case 1:      // 添加主项目
    {
        RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from mainproject where name='%1'").arg(name));
        if(!pRecord.isEmpty())
        {
            QJsonObject mesObjReturn;
            mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
            mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_ADD;
            mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_EXIST;
            m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

            return;
        }

        pRecord = m_sqliteDataProvider.execInsertSql(QString("insert into mainproject (name,describe) values ('%1','%2')")
                                     .arg(name, describe));

        if(pRecord.isEmpty())
        {
            QJsonObject mesObjReturn;
            mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
            mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_ADD;
            mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
            m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

            return;
        }

        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_ADD;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_SUC;
        mesObjReturn["mainproid"] = pRecord(0)(0,0).toInt();
        mesObjReturn["proname"] = name;
        mesObjReturn["prodescribe"] = describe;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        insertuseroperatorlog(mesObj["myid"].toInt(),
                              IDD_SVNVERSION_PROJECT_ADD,
                              0,
                              QString::asprintf("%d Add master project:",mesObj["myid"].toInt())+name);
    }
        break;
    case 2:        // 添加子项目
    {
        int mainid = mesObj["mainid"].toInt();

        RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from subproject where name='%1'").arg(name));
        if(!pRecord.isEmpty())
        {
            QJsonObject mesObjReturn;
            mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
            mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_ADD;
            mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
            m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

            return;
        }

        pRecord = m_sqliteDataProvider.execInsertSql(QString("insert into subproject (mainid,name,describe) values (%1,'%2','%3')")
                                     .arg(mainid).arg(name, describe));

        if(pRecord.isEmpty())
        {
            QJsonObject mesObjReturn;
            mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
            mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_ADD;
            mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_FAIL;
            m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

            return;
        }

        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERSION_PROJECT;
        mesObjReturn["subid"] = IDD_SVNVERSION_PROJECT_ADD;
        mesObjReturn["state"] = IDD_SVNVERSION_PROJECT_SUC;
        mesObjReturn["mainproid"] = mainid;
        mesObjReturn["subproid"] = pRecord(0)(0,0).toInt();
        mesObjReturn["proname"] = name;
        mesObjReturn["prodescribe"] = describe;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        insertuseroperatorlog(mesObj["myid"].toInt(),
                              IDD_SVNVERSION_PROJECT_ADD,
                              0,
                              QString::asprintf("%d Add subproject:",mesObj["myid"].toInt())+name);
    }
        break;
    default:
        break;
    }
}

/**
 * @brief CSVNVersionServer::onProcessNetLoginUser 处理用户登陆消息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetLoginUser(QWebSocket *conn,QJsonObject &mesObj)
{
    QString username = mesObj["username"].toString();
    QString userpwd = QCryptographicHash::hash(mesObj["userpwd"].toString().toLatin1(),QCryptographicHash::Md5).toHex();

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from users where username='%1' and userpwd='%2'")
                                                         .arg(username, userpwd));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERION_USER;
        mesObjReturn["subid"] = IDD_SVNVERION_LOGIN_USER;
        mesObjReturn["state"] = IDD_SVNVERION_USER_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERION_USER;
    mesObjReturn["subid"] = IDD_SVNVERION_LOGIN_USER;
    mesObjReturn["state"] = IDD_SVNVERION_USER_SUC;
    mesObjReturn["userid"] = pRecord(0)(0,0).toInt();;
    mesObjReturn["username"] = pRecord(0)(0,1).toStdString().c_str();
    mesObjReturn["userpwd"] = pRecord(0)(0,2).toStdString().c_str();
    mesObjReturn["type"] = pRecord(0)(0,3).toInt();
    mesObjReturn["projects"] = pRecord(0)(0,4).toStdString().c_str();
    mesObjReturn["devices"] = pRecord(0)(0,5).toStdString().c_str();
    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

    int puserid = pRecord(0)(0,0).simplified().toInt();
    insertuseroperatorlog(puserid,
                          IDD_SVNVERION_LOGIN_USER,
                          0,
                          QString::asprintf("%d Login system.",puserid));
}

/**
 * @brief CSVNVersionServer::onProcessNetRegiterUser 处理用户注册消息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetRegiterUser(QWebSocket *conn,QJsonObject &mesObj)
{
    QString username = mesObj["username"].toString();
    QString userpwd = QCryptographicHash::hash(mesObj["userpwd"].toString().toLatin1(),QCryptographicHash::Md5).toHex();
    int type = mesObj["type"].toInt();
    QString projects = mesObj["projects"].toString();
    QString devices = mesObj["devices"].toString();

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from users where username='%1' and userpwd='%2'").arg(username, userpwd));
    if(!pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERION_USER;
        mesObjReturn["subid"] = IDD_SVNVERION_REGISTER_USER;
        mesObjReturn["state"] = IDD_SVNVERION_REGISTER_USER_EXIST;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    QDateTime time = QDateTime::currentDateTime();
    pRecord = m_sqliteDataProvider.execInsertSql(QString("insert into users (username,userpwd,type,projects,devices,updatetime) values ('%1','%2',%3,'%4','%6',%5)")
                                 .arg(username, userpwd)
                                 .arg(type)
                                 .arg(projects)
                                 .arg(time.toSecsSinceEpoch())
                                 .arg(devices));

    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_SVNVERION_USER;
        mesObjReturn["subid"] = IDD_SVNVERION_REGISTER_USER;
        mesObjReturn["state"] = IDD_SVNVERION_USER_FAIL;
        m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERION_USER;
    mesObjReturn["subid"] = IDD_SVNVERION_REGISTER_USER;
    mesObjReturn["state"] = IDD_SVNVERION_USER_SUC;
    mesObjReturn["id"] = pRecord(0)(0,0).toInt();
    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));
}

/**
 * @brief CSVNVersionServer::onProcessNetUpdateUser 处理用户更新消息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetUpdateUser(QWebSocket *conn,QJsonObject &mesObj)
{
    int userid = mesObj["userid"].toInt();
    QString username = mesObj["username"].toString();
    QString userpwd = mesObj["userpwd"].toString().toLatin1();
    //QString userpwd = QCryptographicHash::hash(mesObj["userpwd"].toString().toLatin1(),QCryptographicHash::Md5).toHex();
    int type = mesObj["type"].toInt();
    QString projects = mesObj["projects"].toString();
    QString devices = mesObj["devices"].toString();

    m_sqliteDataProvider.execSql(QString("update users set username='%1',userpwd='%2',type=%3,projects='%4',devices='%6' where id=%5")
                                 .arg(username, userpwd).arg(type).arg(projects).arg(userid).arg(devices));

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERION_USER;
    mesObjReturn["subid"] = IDD_SVNVERION_UPDATE_USER;
    mesObjReturn["state"] = IDD_SVNVERION_USER_SUC;
    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

    insertuseroperatorlog(userid,
                          IDD_SVNVERION_UPDATE_USER,
                          0,
                          QString::asprintf("%d Update user information.",userid));
}

/**
 * @brief CSVNVersionServer::onProcessNetDeleteUser 处理用户删除消息
 * @param conn 接收到消息的客户端
 * @param mesObj 接收到的客户端的消息
 */
void CSVNVersionServer::onProcessNetDeleteUser(QWebSocket *conn,QJsonObject &mesObj)
{
    int userid = mesObj["userid"].toInt();

    m_sqliteDataProvider.execSql(QString("delete from users where id=%1")
                                 .arg(userid));

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_SVNVERION_USER;
    mesObjReturn["subid"] = IDD_SVNVERION_DELETE_USER;
    mesObjReturn["state"] = IDD_SVNVERION_USER_SUC;
    m_webSocketServer.Send(conn,JsonToString(mesObjReturn));

    insertuseroperatorlog(userid,
                          IDD_SVNVERION_DELETE_USER,
                          0,
                          QString::asprintf("%d delete user.",userid));
}

/**
 * @brief CSVNVersionServer::OnProcessSendFile 处理文件发送（客户端使用）
 * @param conn 要处理的客户端
 * @param file 要处理的文件的绝对路径
 * @param sendsize 当前发送的文件数据大小
 * @param totalsize 当前文件总的大小
 */
void CSVNVersionServer::OnProcessSendFile(QWebSocket *conn,QString file,qint64 sendsize,qint64 totalsize)
{

}

/**
 * @brief CSVNVersionServer::OnProcessRecvFile 处理文件接收
 * @param conn 要处理的客户端
 * @param srcfile 原始文件名称
 * @param decfile 保存的文件
 * @param pFileRecvError 状态码
 */
void CSVNVersionServer::OnProcessRecvFile(QWebSocket *conn,QString srcfile,QString decfile,FileRecvError pFileRecvError)
{

}
