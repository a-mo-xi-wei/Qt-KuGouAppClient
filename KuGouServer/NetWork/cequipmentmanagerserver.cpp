#include "cequipmentmanagerserver.h"
#include "QsLog.h"
#include "common.h"
#include "ndbpool.h"

#include <QJsonArray>
#include <QDebug>
#include <QCryptographicHash>
#include <QWebSocket>
#include <QSQlError>

#define IDD_USER 0     // 用户
#define IDD_DEVIE 1    // 设备

CEquipmentManagerServer::CEquipmentManagerServer(QObject *parent)
    : QObject(parent),
      m_AgentServerPort(1001),
      m_AgentServerMaxClients(1000),
      m_ReverseProxyServerPort(1002)
{
    m_AgentServer.SetNetworkFrameManager(this);

    connect(&m_ReverseProxyServer,SIGNAL(readyRead()),this,SLOT(ReverseProxyServer_reciverPendingDatagram()));
}

CEquipmentManagerServer::~CEquipmentManagerServer()
{
    stopService();
}

/**
 * @brief CEquipmentManagerServer::setDataBase 设置版本库所使用数据库
 * @param dbPath 数据库文件路径
 */
void CEquipmentManagerServer::setDataBase(QString dbPath)
{
    if(dbPath.isEmpty())
        return;

    m_sqliteDataProvider.connect(dbPath);
}

/**
 * @brief CEquipmentManagerServer::isExistUser 检测指定用户ID和密码的用户是否存在
 * @param id 用户ID
 * @param pwd 用户密码
 * @return 如果用户存在返回真，否则返回假
 */
bool CEquipmentManagerServer::isExistUser(QWebSocket *conn,int id,QString pwd)
{
    if(id <= 0 || pwd.isEmpty())
        return false;

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from users where id=%1 and userpwd='%2'")
                                                         .arg(id).arg(pwd));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_EMS_MESSAGE_LOGIN;
        mesObjReturn["state"] = IDD_EMS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return false;
    }

    return true;
}

/**
 * @brief CEquipmentManagerServer::setAgentServerStartupParams 设置代理服务器启动参数
 * @param serverPort 代理服务器启动的端口
 * @param maxClients 代理服务器能接受的最大客户端数量
 */
void CEquipmentManagerServer::setAgentServerStartupParams(int serverPort,int maxClients)
{
    m_AgentServerPort = serverPort;
    m_AgentServerMaxClients = maxClients;
}

/**
 * @brief CEquipmentManagerServer::OnProcessNetText 处理网络字符串消息
 * @param conn
 * @param mes
 */
void CEquipmentManagerServer::OnProcessNetText(QWebSocket *conn,QString mes)
{
    QJsonObject mesObj = StringToJson(mes);

    switch (mesObj["mesid"].toInt())
    {
    case IDD_JS_MESSAGE_REGISTER:            // 设备注册
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetEquipmentRegister(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_LOGIN:                // 用户登录
    {
        onProcessNetEquipmentUserLogin(conn,mesObj);
    }
        break;
    case IDD_JS_MESSAGE_PUTRESOURCEDATA:       // 推送资源到设备
    {
        onProcessNetPutResourceToDevice(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_GETDEVICELIST:        // 得到设备列表
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetEquipmentGetDeviceList(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_GETDEVICEDATA:        // 得到设备数据
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetEquipmentGetData(conn,mesObj);
    }
        break;        
    case IDD_JS_MESSAGE_COMMAND:               // 用户命令
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetUserCommand(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_ADD_DEVICE:           // 添加设备
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetAddDevice(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_DELETE_DEVICE:      // 删除设备
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetDeleteDevice(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_UPDATE_DEVICE:      // 更新设备信息
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetUpdateDevice(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_PROJECTINFO:      // 得到所有项目的信息
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetGetAllProjectInfo(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_P2P_DEVICE:      // 和设备建立P2P连接
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetP2pDevice(conn,mesObj);
    }
        break;
    case IDD_EMS_MESSAGE_VERSIONTODEVICE:      // 下载版本库到设备上
    {
        if(isExistUser(conn,mesObj["myid"].toInt(),mesObj["mypwd"].toString()))
            onProcessNetDownloadVersiontoDevice(conn,mesObj);
    }
        break;
    default:
    {
        int pUserID = mesObj["userid"].toInt();

        // 将设备的其它信息发送到它相应的控制设备上
        QHash<QWebSocket*,tagEquipmentItem>::iterator iterDevice = m_Onlineequipmentlist.find(conn);
        if(iterDevice != m_Onlineequipmentlist.end() &&
                iterDevice.value().type == 1)
        {
            if(pUserID > 0)
            {
                QHash<int,QHash<int,QWebSocket*>>::iterator iterUser = m_onlineDevicelist.find(pUserID);
                if(iterUser != m_onlineDevicelist.end())
                {
                    QHash<int,QWebSocket*>::iterator iterFindDevice = iterUser.value().find(iterDevice.value().deviceID);
                    if(iterFindDevice != iterUser.value().end())
                    {
                        QHash<int,QWebSocket*>::iterator iterolUser = m_onlineUserList.find(iterUser.key());
                        if(iterolUser != m_onlineUserList.end())
                        {
                            iterolUser.value()->sendTextMessage(mes);
                        }
                    }
                }
            }
            else
            {
                QHash<int,QHash<int,QWebSocket*>>::iterator iterUser = m_onlineDevicelist.begin();
                for(;iterUser != m_onlineDevicelist.end();++iterUser)
                {
                    QHash<int,QWebSocket*>::iterator iterFindDevice = iterUser.value().find(iterDevice.value().deviceID);
                    if(iterFindDevice == iterUser.value().end())
                        continue;

                    QHash<int,QWebSocket*>::iterator iterolUser = m_onlineUserList.find(iterUser.key());
                    if(iterolUser != m_onlineUserList.end())
                    {
                        iterolUser.value()->sendTextMessage(mes);
                    }
                }
            }
        }
    }
        break;
    }
}

/**
 * @brief CEquipmentManagerServer 得到指定ID保存的版本数据
 * @param id 要得到保存的版本数据的ID,如果ID为-1，返回最新的数据
 * @return 如果操作成功返回真，否则返回假
 */
QByteArray CEquipmentManagerServer::getVersionData(int id)
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

/// 下载版本到设备上
void CEquipmentManagerServer::onProcessNetDownloadVersiontoDevice(QWebSocket *conn,QJsonObject &mesObj)
{
    int pversion = mesObj["version"].toInt();
    int pdeviceid = mesObj["deviceid"].toInt();

    QByteArray VersionData = getVersionData(pversion);

    if(VersionData.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_JS_MESSAGE_PUTRESOURCEDATA;
        mesObjReturn["state"] = IDD_JS_MESSAGE_NODATA;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));
        return;
    }

    tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,pdeviceid);
    if(pEquipmentItem == NULL)
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_JS_MESSAGE_PUTRESOURCEDATA;
        mesObjReturn["state"] = IDD_JS_MESSAGE_NODEVICE;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));
        return;
    }

    m_onlinecontrolClientlist[pEquipmentItem->webClient] = conn;

    tagVersionOper pVersionOper;
    memset(&pVersionOper,0,sizeof(tagVersionOper));

    pVersionOper.tagIndex = IDD_JS_MESSAGE_PUTRESOURCEDATA;
    pVersionOper.mainid = pdeviceid;      /**< 这里是设备ID */
    pVersionOper.subid = pversion;       /**< 当前版本号 */

    QByteArray sendData;
    sendData.append((const char*)&pVersionOper,sizeof(tagVersionOper));
    sendData.append(VersionData);

    m_AgentServer.SendByteArray(pEquipmentItem->webClient,sendData);
}

/// 用户命令
void CEquipmentManagerServer::onProcessNetUserCommand(QWebSocket *conn,QJsonObject &mesObj)
{
    int pdeviceId = mesObj["deviceid"].toInt();

    // 查找当前设备是否在线
    tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,pdeviceId);
    if(pEquipmentItem == NULL)
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_JS_MESSAGE_COMMAND;
        mesObjReturn["state"] = IDD_EMS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    pEquipmentItem->webClient->sendTextMessage(JsonToString(mesObj));
}

/**
 * @brief CEquipmentManagerServer::onProcessNetPutResourceToDevice 推送资源到指定设备
 * @param conn
 * @param mesObj
 */
void CEquipmentManagerServer::onProcessNetPutResourceToDevice(QWebSocket *conn,QJsonObject &mesObj)
{
    int deviceid = mesObj["deviceid"].toInt();
    int pversion = mesObj["version"].toInt();
    int state = mesObj["state"].toInt();

    QHash<QWebSocket*,QWebSocket*>::iterator iter = m_onlinecontrolClientlist.find(conn);
    if(iter != m_onlinecontrolClientlist.end())
    {
        if(state == IDD_JS_MESSAGE_SUCCESS)
        {
            m_sqliteDataProvider.execSql(QString("update equipments set curvers=%1 where id=%2")
                                                                     .arg(pversion)
                                                                     .arg(deviceid));
        }

        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_JS_MESSAGE_PUTRESOURCEDATA;
        mesObjReturn["deviceid"] = deviceid;
        mesObjReturn["version"] = pversion;
        mesObjReturn["state"] = state;
        m_AgentServer.Send(iter.value(),JsonToString(mesObjReturn));

        m_onlinecontrolClientlist.erase(iter);
    }
}

/**
 * @brief CEquipmentManagerServer::onProcessNetEquipmentUserLogin 用户登录
 * @param conn
 * @param mesObj
 */
void CEquipmentManagerServer::onProcessNetEquipmentUserLogin(QWebSocket *conn,QJsonObject &mesObj)
{
    QString username = mesObj["username"].toString();
    QString userpwd = QCryptographicHash::hash(mesObj["userpwd"].toString().toLatin1(),QCryptographicHash::Md5).toHex();

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from users where username='%1' and userpwd='%2'")
                                                         .arg(username)
                                                         .arg(userpwd));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_EMS_MESSAGE_LOGIN;
        mesObjReturn["state"] = IDD_EMS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    int puserid = pRecord(0)(0,0).toInt();
    m_Onlineequipmentlist[conn] = tagEquipmentItem(tagP2pDevice(),conn,0,puserid);
    m_onlineUserList[puserid] = conn;

    QVector<int> pDeviceList = getUserControlDevices(puserid);

    if(!pDeviceList.isEmpty())
    {
        for(int i=0;i<pDeviceList.size();i++)
        {
            tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,pDeviceList[i]);

            if(pEquipmentItem)
                m_onlineDevicelist[puserid][pDeviceList[i]] = pEquipmentItem->webClient;
            else
                m_onlineDevicelist[puserid][pDeviceList[i]] = NULL;
        }
    }

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_EMS_MESSAGE_LOGIN;
    mesObjReturn["state"] = IDD_EMS_MESSAGE_SUCCESS;
    mesObjReturn["userid"] = pRecord(0)(0,0).toInt();
    mesObjReturn["username"] = pRecord(0)(0,1).toStdString().c_str();
    mesObjReturn["userpwd"] = pRecord(0)(0,2).toStdString().c_str();
    mesObjReturn["type"] = pRecord(0)(0,3).toInt();
    mesObjReturn["devices"] = pRecord(0)(0,5).toStdString().c_str();

    m_AgentServer.Send(conn,JsonToString(mesObjReturn));
}

/**
 * @brief CEquipmentManagerServer::onProcessNetEquipmentGetData 得到指定设备的数据
 * @param conn
 * @param mesObj
 */
void CEquipmentManagerServer::onProcessNetEquipmentGetData(QWebSocket *conn,QJsonObject &mesObj)
{
    int pdeviceId = mesObj["deviceid"].toInt();

    // 查找当前设备是否在线
    tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,pdeviceId);
    if(pEquipmentItem == NULL)
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_EMS_MESSAGE_GETDEVICEDATA;
        mesObjReturn["state"] = IDD_EMS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    m_onlinecontrolClientlist[pEquipmentItem->webClient] = conn;

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_JS_MESSAGE_GETRESOURCEDATA;
    mesObjReturn["deviceid"] = pdeviceId;
    mesObjReturn["userid"] = mesObj["myid"].toInt();
    mesObjReturn["userpwd"] = mesObj["mypwd"].toString();
    pEquipmentItem->webClient->sendTextMessage(JsonToString(mesObjReturn));
}

/**
 * @brief CEquipmentManagerServer::onProcessNetEquipmentGetDeviceList 得到设备列表
 * @param conn
 * @param mesObj
 */
void CEquipmentManagerServer::onProcessNetEquipmentGetDeviceList(QWebSocket *conn,QJsonObject &mesObj)
{
    QJsonArray pdevices = mesObj["devices"].toArray();

    RecordSetList pRecord;

    if(!pdevices.isEmpty())
    {
        QString sqlStr = "select equ.*,mainpro.name as mainproname,subpro.name as subproname from equipments equ join mainproject mainpro,subproject subpro on (equ.id=";

        for(int i=0;i<pdevices.size();i++)
        {
            if(i+1 < pdevices.size())
                sqlStr += QString::asprintf("%d or equ.id=",pdevices[i].toInt());
            else
                sqlStr += QString::asprintf("%d",pdevices[i].toInt());
        }

        sqlStr += ") and mainpro.id=equ.mproid and subpro.id=equ.sproid";

        pRecord = m_sqliteDataProvider.execSql(sqlStr);
    }
    else
        pRecord = m_sqliteDataProvider.execSql(QString("select equ.*,mainpro.name as mainproname,subpro.name as subproname from equipments "
                                                       "equ join mainproject mainpro,subproject subpro on mainpro.id=equ.mproid and subpro.id=equ.sproid"));

    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_EMS_MESSAGE_GETDEVICELIST;
        mesObjReturn["state"] = IDD_EMS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_EMS_MESSAGE_GETDEVICELIST; 
    mesObjReturn["state"] = IDD_EMS_MESSAGE_SUCCESS;

    QJsonArray deviceArray;

    for(int k=0;k<(int)pRecord(0).rows();k++)
    {
        QString deviceName = pRecord(0)(k,1).toStdString().c_str();
        int deviceId = pRecord(0)(k,0).toInt();

        QJsonObject deviceObj;
        deviceObj["id"] = deviceId;
        deviceObj["name"] = deviceName;
        deviceObj["ip"] = pRecord(0)(k,2).toStdString().c_str();
        deviceObj["mproid"] = pRecord(0)(k,3).toInt();
        deviceObj["sproid"] = pRecord(0)(k,4).toInt();
        deviceObj["describe"] = pRecord(0)(k,5).toStdString().c_str();
        deviceObj["curvers"] = pRecord(0)(k,7).toInt();
        deviceObj["mainproname"] = pRecord(0)(k,8).toStdString().c_str();
        deviceObj["subproname"] = pRecord(0)(k,9).toStdString().c_str();

        tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,deviceId);
        if(pEquipmentItem != NULL)
        {
            deviceObj["udpaddress"] = pEquipmentItem->udpClient.sender.toString();
            deviceObj["udpport"] = pEquipmentItem->udpClient.senderPort;
            deviceObj["state"] = 1;  // 在线
        }
        else
        {
            deviceObj["state"] = 0;
        }

        deviceArray.push_back(deviceObj);
    }

    mesObjReturn["devices"] = deviceArray;

    m_AgentServer.Send(conn,JsonToString(mesObjReturn));
}

/// 向所有控制设备广播指定ID的受控设备的状态
void CEquipmentManagerServer::sendDeviceStateToAllUsers(int devicdID,int state)
{
    // 更新设备数据库中的状态
    m_sqliteDataProvider.execSql(QString("update equipments set state=%1 where id=%2")
                                                             .arg(state)
                                                             .arg(devicdID));

    QHash<QWebSocket*,tagEquipmentItem>::iterator iter = m_Onlineequipmentlist.begin();
    for(;iter != m_Onlineequipmentlist.end();++iter)
    {
        if(iter.value().type == 1)
            continue;

        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_EMS_MESSAGE_DEVICE_STATE;
        mesObjReturn["deviceid"] = devicdID;
        mesObjReturn["state"] = state;

        m_AgentServer.Send(iter.value().webClient,JsonToString(mesObjReturn));
    }
}

/**
 * @brief CEquipmentManagerServer::getDeviceInfoByID 根据设备ID得到设备信息
 * @param usertype 0 - 控制设备；1 - 受控设备
 * @param deviceID
 * @return
 */
tagEquipmentItem* CEquipmentManagerServer::getDeviceInfoByID(int usertype,int deviceID)
{
    if(usertype == -1)
    {
        QHash<QWebSocket*,tagEquipmentItem>::iterator iter = m_Onlineequipmentlist.begin();
        for(;iter != m_Onlineequipmentlist.end();++iter)
        {
            if(iter.value().deviceID == deviceID)
                return &iter.value();
        }

        return NULL;
    }

    QHash<QWebSocket*,tagEquipmentItem>::iterator iter = m_Onlineequipmentlist.begin();
    for(;iter != m_Onlineequipmentlist.end();++iter)
    {
        if(iter.value().type == usertype && iter.value().deviceID == deviceID)
            return &iter.value();
    }

    return NULL;
}

/// 得到指定玩家的设备列表
QVector<int> CEquipmentManagerServer::getUserControlDevices(int userId)
{
    QVector<int> returnDevices;

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select devices from users where id=%1")
                                                         .arg(userId));
    if(!pRecord.isEmpty())
    {
        QString devices = pRecord(0)(0,0).toStdString().c_str();

        QStringList pDeviceList = devices.split(";");

        for(int i=0;i<pDeviceList.size();i++)
        {
            returnDevices.push_back(pDeviceList[i].toInt());
        }
    }

    return returnDevices;
}

/**
 * @brief CEquipmentManagerServer::onProcessNetEquipmentRegister 设备注册
 * @param conn
 * @param mesObj
 */
void CEquipmentManagerServer::onProcessNetEquipmentRegister(QWebSocket *conn,QJsonObject &mesObj)
{
    int deviceid = mesObj["deviceid"].toInt();

    RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from equipments where id=%1")
                                                         .arg(deviceid));
    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_JS_MESSAGE_REGISTER;
        mesObjReturn["state"] = IDD_JS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    m_Onlineequipmentlist[conn] = tagEquipmentItem(tagP2pDevice(),conn,1,deviceid);

    QHash<int,QHash<int,QWebSocket*>>::iterator iter = m_onlineDevicelist.begin();
    for(;iter != m_onlineDevicelist.end();++iter)
    {
        QHash<int,QWebSocket*>::iterator iterDevice = iter.value().find(deviceid);
        if(iterDevice != iter.value().end())
        {
            iterDevice.value() = conn;
        }
    }

    // 如果设备注册，假如当前相应控制设备的用户在线，设备和控制用户要相互依赖
    QHash<int,QWebSocket*>::iterator iterolUser = m_onlineUserList.begin();
    for(;iterolUser != m_onlineUserList.end();++iterolUser)
    {
        QVector<int> pDeviceList = getUserControlDevices(iterolUser.key());
        if(pDeviceList.isEmpty())
            continue;

        QVector<int>::iterator iterDevDevice = std::find(pDeviceList.begin(),pDeviceList.end(),deviceid);

        if(iterDevDevice != pDeviceList.end())
        {
            tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,deviceid);

            if(pEquipmentItem)
                m_onlineDevicelist[iterolUser.key()][deviceid] = pEquipmentItem->webClient;
            else
                m_onlineDevicelist[iterolUser.key()][deviceid] = NULL;
        }
    }

    QString clientIP = conn->localAddress().toString() + ":" + QString::fromLocal8Bit("%d",conn->localPort());

    m_sqliteDataProvider.execSql(QString("update equipments set ip=%1,state=1 where id=%2")
                                                             .arg(clientIP)
                                                             .arg(deviceid));

    sendDeviceStateToAllUsers(deviceid,1);

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_JS_MESSAGE_REGISTER;
    mesObjReturn["state"] = IDD_JS_MESSAGE_SUCCESS;
    mesObjReturn["udpserip"] = getLocalIP().toString();
    mesObjReturn["udpserport"] = m_ReverseProxyServerPort;
    m_AgentServer.Send(conn,JsonToString(mesObjReturn));
}

/**
 * @brief CEquipmentManagerServer::OnProcessNetBinary 处理网络二进制消息
 * @param conn
 * @param data
 */
void CEquipmentManagerServer::OnProcessNetBinary(QWebSocket *conn,QByteArray &data)
{
    tagVersionOper pVersionOper;
    QByteArray reciverData = data;

    memset(&pVersionOper,0,sizeof(tagVersionOper));
    memcpy(&pVersionOper,reciverData.data(),sizeof(tagVersionOper));
    reciverData.remove(0,sizeof(tagVersionOper));

    switch(pVersionOper.tagIndex)
    {
    case IDD_JS_MESSAGE_GETRESOURCEDATA:
    {
        QHash<QWebSocket*,QWebSocket*>::iterator iter = m_onlinecontrolClientlist.find(conn);
        if(iter != m_onlinecontrolClientlist.end())
        {
            m_AgentServer.SendByteArray(iter.value(),data);

            m_onlinecontrolClientlist.erase(iter);
        }
    }
        break;
    case IDD_JS_MESSAGE_PUTRESOURCEDATA:
    {
        int pDeviceID = pVersionOper.mainid;

        tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,pDeviceID);
        if(pEquipmentItem == NULL)
        {
            QJsonObject mesObjReturn;
            mesObjReturn["mesid"] = IDD_JS_MESSAGE_PUTRESOURCEDATA;
            mesObjReturn["state"] = IDD_JS_MESSAGE_NODEVICE;
            m_AgentServer.Send(conn,JsonToString(mesObjReturn));
            return;
        }

        m_onlinecontrolClientlist[pEquipmentItem->webClient] = conn;

        m_AgentServer.SendByteArray(pEquipmentItem->webClient,data);
    }
        break;
    default:
        break;
    }
}

/**
 * @brief CEquipmentManagerServer::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn
 */
void CEquipmentManagerServer::OnProcessConnectedNetMes(QWebSocket *conn)
{

}

/**
 * @brief CEquipmentManagerServer::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn
 */
void CEquipmentManagerServer::OnProcessDisconnectedNetMes(QWebSocket *conn)
{
    QHash<QWebSocket*,tagEquipmentItem>::iterator iter = m_Onlineequipmentlist.find(conn);
    if(iter != m_Onlineequipmentlist.end())
    {
        sendDeviceStateToAllUsers(iter.value().deviceID,0);

        // 删除当前设备列表
        if(iter.value().type == 0)
        {
            QHash<int,QHash<int,QWebSocket*>>::iterator iterUser = m_onlineDevicelist.find(iter.value().deviceID);
            if(iterUser != m_onlineDevicelist.end())
            {
                m_onlineDevicelist.erase(iterUser);
            }
        }
        else if(iter.value().type == 1)
        {
            QHash<int,QHash<int,QWebSocket*>>::iterator iterUser = m_onlineDevicelist.begin();
            for(;iterUser != m_onlineDevicelist.end();)
            {
                QHash<int,QWebSocket*>::iterator iterDevice = iterUser.value().find(iter.value().deviceID);
                if(iterDevice != iterUser.value().end())
                {
                    iterUser.value().erase(iterDevice);
                }

                if(iterUser.value().isEmpty())
                    iterUser = m_onlineDevicelist.erase(iterUser);
                else
                    ++iterUser;
            }
        }

        // 删除用户和设备的关系列表
        QHash<QWebSocket*,QWebSocket*>::iterator iter2 = m_onlinecontrolClientlist.find(conn);
        if(iter2 != m_onlinecontrolClientlist.end())
        {
            m_onlinecontrolClientlist.erase(iter2);
        }
        else
        {
            iter2 = m_onlinecontrolClientlist.begin();
            for(;iter2 != m_onlinecontrolClientlist.end();)
            {
                if(iter2.key() == conn || iter2.value() == conn)
                    iter2 = m_onlinecontrolClientlist.erase(iter2);
                else
                    ++iter;
            }
        }

        // 删除在线用户列表
        QHash<int,QWebSocket*>::iterator iterolUser = m_onlineUserList.begin();
        for(;iterolUser != m_onlineUserList.end();++iterolUser)
        {
            if(iterolUser.value() != conn)
                continue;

            m_onlineUserList.erase(iterolUser);
            break;
        }

        m_Onlineequipmentlist.erase(iter);
    }
}

/**
 * @brief CEquipmentManagerServer::ReverseProxyServer_ClientReceiveMsg 处理收到的客户端信息
 * @param socketid
 * @param datagramdata
 */
void CEquipmentManagerServer::ReverseProxyServer_reciverPendingDatagram()
{
    while(m_ReverseProxyServer.hasPendingDatagrams())
    {
        QByteArray preciverDatagramData;
        QHostAddress sender;
        quint16 senderPort = 0;

        preciverDatagramData.resize(m_ReverseProxyServer.pendingDatagramSize());

        if(m_ReverseProxyServer.readDatagram(preciverDatagramData.data(),preciverDatagramData.size(),
                                    &sender, &senderPort) > 0)
        {
            QJsonObject mesObj = StringToJson(preciverDatagramData);

            RecordSetList pRecord = m_sqliteDataProvider.execSql(QString("select * from users where id=%1 and userpwd='%2'")
                                                                 .arg(mesObj["myid"].toInt())
                                                                 .arg(mesObj["mypwd"].toString()));
            if(pRecord.isEmpty())
            {
                int pdeviceid = mesObj["deviceid"].toInt();
                int pUserId = mesObj["userid"].toInt();

                tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,pdeviceid);
                if(pEquipmentItem != NULL)
                {
                    pEquipmentItem->udpClient.sender = sender;
                    pEquipmentItem->udpClient.senderPort = senderPort;
                }

                QHash<int,QWebSocket*>::iterator iterolUser = m_onlineUserList.find(pUserId);
                if(iterolUser != m_onlineUserList.end())
                {
                    QJsonObject mesObjReturn;
                    mesObjReturn["mesid"] = IDD_EMS_MESSAGE_P2P_DEVICE;
                    mesObjReturn["state"] = IDD_EMS_MESSAGE_SUCCESS;
                    mesObjReturn["deviceid"] = pdeviceid;
                    mesObjReturn["udpdeviceip"] = sender.toString().simplified();
                    mesObjReturn["udpdeviceport"] = senderPort;

                    iterolUser.value()->sendTextMessage(JsonToString(mesObjReturn));
                }
            }
        }
    }
}

/**
 * @brief CEquipmentManagerServer::startupService 启动服务
 */
void CEquipmentManagerServer::startupService(void)
{
    m_AgentServer.setMaxPendingConnections(m_AgentServerMaxClients);
    m_AgentServer.OpenServer(m_AgentServerPort);
    m_ReverseProxyServer.bind(QHostAddress::Any,m_ReverseProxyServerPort);

    QLOG_INFO()<<"CEquipmentManagerServer startupService succesed.";
}

/// 服务器是否已经启动
bool CEquipmentManagerServer::isStartup(void)
{
    return m_AgentServer.isListening();
}

/**
 * @brief CEquipmentManagerServer::stopService 停止服务
 */
void CEquipmentManagerServer::stopService(void)
{
    m_ReverseProxyServer.close();
    m_AgentServer.CloseServer();
}

/// 添加设备
void CEquipmentManagerServer::onProcessNetAddDevice(QWebSocket *conn,QJsonObject &mesObj)
{
    QString pName = mesObj["name"].toString();
    int mproid = mesObj["mproid"].toInt();
    int sproid = mesObj["sproid"].toInt();
    QString describe = mesObj["describe"].toString();
    int state = mesObj["state"].toInt();
    int version = mesObj["version"].toInt();

    RecordSetList pRecord = m_sqliteDataProvider.execInsertSql(QString("insert into equipments (name,mproid,sproid,describe,state,curvers) values ('%1',%2,%3,'%4',%5,%6)")
                                 .arg(pName)
                                 .arg(mproid)
                                 .arg(sproid)
                                 .arg(describe)
                                 .arg(state)
                                 .arg(version));

    if(pRecord.isEmpty())
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_EMS_MESSAGE_ADD_DEVICE;
        mesObjReturn["state"] = IDD_EMS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    int lastId = pRecord(0)(0,0).toInt();

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_EMS_MESSAGE_ADD_DEVICE;
    mesObjReturn["state"] = IDD_EMS_MESSAGE_SUCCESS;
    mesObjReturn["deviceid"] = lastId;
    mesObjReturn["name"] = pName;
    mesObjReturn["mproid"] = mproid;
    mesObjReturn["sproid"] = sproid;
    mesObjReturn["describe"] = describe;
    mesObjReturn["state"] = state;
    mesObjReturn["version"] = version;
    m_AgentServer.Send(conn,JsonToString(mesObjReturn));
}

/// 删除设备
void CEquipmentManagerServer::onProcessNetDeleteDevice(QWebSocket *conn,QJsonObject &mesObj)
{
    QJsonArray pDeviceID = mesObj["deviceid"].toArray();

    QString sqlStr = QString("delete from equipments where id=");

    for(int i=0;i<pDeviceID.size()-1;i++)
        sqlStr += QString::asprintf("%d and id=",pDeviceID[i].toInt());
    sqlStr += QString::asprintf("%d",pDeviceID[pDeviceID.size()-1].toInt());

    m_sqliteDataProvider.execSql(sqlStr);

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_EMS_MESSAGE_DELETE_DEVICE;
    mesObjReturn["state"] = IDD_EMS_MESSAGE_SUCCESS;
    mesObjReturn["deviceids"] = pDeviceID;

    m_AgentServer.Send(conn,JsonToString(mesObjReturn));
}

/// 和设备建立P2P连接
void CEquipmentManagerServer::onProcessNetP2pDevice(QWebSocket *conn,QJsonObject &mesObj)
{
    int pdeviceId = mesObj["deviceid"].toInt();

    // 查找当前设备是否在线
    tagEquipmentItem *pEquipmentItem = getDeviceInfoByID(IDD_DEVIE,pdeviceId);
    if(pEquipmentItem == NULL)
    {
        QJsonObject mesObjReturn;
        mesObjReturn["mesid"] = IDD_EMS_MESSAGE_P2P_DEVICE;
        mesObjReturn["state"] = IDD_EMS_MESSAGE_FAIL;
        m_AgentServer.Send(conn,JsonToString(mesObjReturn));

        return;
    }

    pEquipmentItem->webClient->sendTextMessage(JsonToString(mesObj));
}

/// 得到所有项目的信息
void CEquipmentManagerServer::onProcessNetGetAllProjectInfo(QWebSocket *conn,QJsonObject &mesObj)
{
    RecordSetList pmainRecord = m_sqliteDataProvider.execSql(QString("select id,name from mainproject"));
    RecordSetList psubRecord = m_sqliteDataProvider.execSql(QString("select id,name,mainid from subproject"));

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_EMS_MESSAGE_PROJECTINFO;

    QJsonArray mainprojects;

    for(int k=0;k<(int)pmainRecord(0).rows();k++)
    {
        QJsonObject proObj;
        proObj["id"] = pmainRecord(0)(k,0).toInt();
        proObj["name"] = pmainRecord(0)(k,1).toStdString().c_str();

        mainprojects.push_back(proObj);
    }

    QJsonArray subprojects;

    for(int k=0;k<(int)psubRecord(0).rows();k++)
    {
        QJsonObject proObj;
        proObj["id"] = psubRecord(0)(k,0).toInt();
        proObj["name"] = psubRecord(0)(k,1).toStdString().c_str();
        proObj["mainid"] = psubRecord(0)(k,2).toInt();

        subprojects.push_back(proObj);
    }

    mesObjReturn["mpro"] = mainprojects;
    mesObjReturn["spro"] = subprojects;

    m_AgentServer.Send(conn,JsonToString(mesObjReturn));
}

/// 更新设备信息
void CEquipmentManagerServer::onProcessNetUpdateDevice(QWebSocket *conn,QJsonObject &mesObj)
{
    int pDeviceID = mesObj["deviceid"].toInt();
    QString pName = mesObj["name"].toString();
    int mproid = mesObj["mproid"].toInt();
    int sproid = mesObj["sproid"].toInt();
    QString describe = mesObj["describe"].toString();
    int state = mesObj["state"].toInt();
    int version = mesObj["version"].toInt();

    m_sqliteDataProvider.execSql(QString("update equipments set name='%1',mproid=%2,sproid=%3,describe='%4',state=%5,curvers=%6 where id=%7")
                                                         .arg(pName)
                                                         .arg(mproid)
                                                         .arg(sproid)
                                                         .arg(describe)
                                                         .arg(state)
                                                         .arg(version)
                                                         .arg(pDeviceID));

    QJsonObject mesObjReturn;
    mesObjReturn["mesid"] = IDD_EMS_MESSAGE_UPDATE_DEVICE;
    mesObjReturn["state"] = IDD_EMS_MESSAGE_SUCCESS;
    mesObjReturn["deviceid"] = pDeviceID;
    mesObjReturn["name"] = pName;
    mesObjReturn["mproid"] = mproid;
    mesObjReturn["sproid"] = sproid;
    mesObjReturn["describe"] = describe;
    mesObjReturn["state"] = state;
    mesObjReturn["version"] = version;
    m_AgentServer.Send(conn,JsonToString(mesObjReturn));
}
