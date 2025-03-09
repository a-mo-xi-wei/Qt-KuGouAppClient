#include "qenet.h"
#include "QsLog.h"
#include "common.h"
#include "enet/enet.h"
#include "networkframemanager.h"

#include <QUuid>
#include <QDebug>

CEnetClient::CEnetClient(QObject *parent)
    : QThread(parent),
      m_isClientRunning(false),
      m_client(NULL),
      m_decserver(NULL),
      m_NetworkFrameManager(NULL)
{
    if (enet_initialize () != 0)
    {
        QLOG_ERROR()<<"An error occurred while initializing ENet.";
        emit signalSendLog("An error occurred while initializing ENet.");
    }
    else
    {
        QLOG_ERROR()<<"initialize ENet successed.";
        emit signalSendLog("initialize ENet successed.");
    }

}

CEnetClient::~CEnetClient()
{
    disconServer(false);
    enet_deinitialize();
}

/**
 * @brief CEnetClient::connectServer 连接服务器
 * @param serverip
 * @param port
 * @return
 */
bool CEnetClient::connectServer(QString serverip,int port)
{
    if(m_isClientRunning ||
       serverip.isEmpty() || port <= 0)
        return false;

    m_client = enet_host_create(NULL,
                                  1,   //只允许连接一个服务器
                                  2,
                                  0,
                                  0);
    if(m_client == NULL)
    {
        QLOG_ERROR()<<"create enet client fail.";
        emit signalSendLog("create enet client fail.");
        return false;
    }

    //连接到服务器
    ENetAddress svraddr;
    enet_address_set_host(&svraddr,serverip.toStdString().c_str());
    svraddr.port=port;

    m_decserver = enet_host_connect(m_client,&svraddr,3,0); //client连接到svraddr对象，共分配三个通道
    if(m_decserver == NULL)
    {
        QLOG_ERROR()<<"server connect fail.";
        emit signalSendLog("server connect fail.");
        return false;
    }

    m_isClientRunning = true;
    this->start();

    emit signalSendLog("ENet client start successed,port:"+
                       QString::asprintf("%d",port)+
                       ",serve ip:"+
                       serverip);


    return true;
}

/**
 * @brief CEnetClient::disconServer 断开服务器
 */
void CEnetClient::disconServer(bool islog)
{
    if(!m_isClientRunning ||
        m_client == NULL)
        return;

    //if(islog)
    //    emit signalSendLog("ENet client stoping...");

    if(m_decserver)
        enet_peer_disconnect(m_decserver,0);

    ENetEvent event;

    //等待关闭成功
    while (enet_host_service (m_client, &event, 5000)>0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy (event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            emit signalSendLog("server disconnect successed.");
            break;
        }
    }

    //这里就是关闭失败，强制重置
    if(m_decserver)
        enet_peer_reset(m_decserver);

    m_isClientRunning = false;
    this->wait();

    enet_host_destroy(m_client);

    if(islog)
        emit signalSendLog("ENet client stop successed.");
}

/**
 * @brief CEnetClient::sendData 发送数据
 * @param data
 */
bool CEnetClient::sendData(QByteArray data)
{
    if(m_decserver == NULL || data.isEmpty() || m_client == NULL)
        return false;

    QMutexLocker locker(&m_lockclient);

    ENetPacket *packet=enet_packet_create(NULL,data.size()+1,ENET_PACKET_FLAG_RELIABLE); //创建包
    strcpy((char*)packet->data,data.constData());

    bool isOk = enet_peer_send(m_decserver,1,packet) < 0 ? false : true;

    enet_host_flush (m_client);

    return isOk;
}

void CEnetClient::run()
{
    if(m_client == NULL)
        return;

    ENetEvent event;
    int state = enet_host_service(m_client,&event,10);

    while(m_isClientRunning && state>=0)
    {
        if(event.type==ENET_EVENT_TYPE_CONNECT) //有客户机连接成功
        {
            emit signalSendLog("server connect successed.");

            if(m_NetworkFrameManager)
                m_NetworkFrameManager->OnProcessEnetConnectedNetMes(event.peer);
        }
        else if(event.type==ENET_EVENT_TYPE_RECEIVE) //收到数据
        {
             if(m_NetworkFrameManager)
                 m_NetworkFrameManager->OnProcessEnetNetBinary(event.peer,
                                                               QByteArray((const char *)event.packet->data,
                                                                          event.packet->dataLength));
        }
        else if(event.type==ENET_EVENT_TYPE_DISCONNECT) //失去连接
        {
            emit signalSendLog("server disconnect successed.");

             if(m_NetworkFrameManager)
                 m_NetworkFrameManager->OnProcessEnetDisconnectedNetMes(event.peer);
        }

        m_lockclient.lock();
        state = enet_host_service(m_client,&event,10);
        m_lockclient.unlock();

        QThread::msleep(1);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

CEnetServer::CEnetServer(QObject *parent)
    : QThread(parent),
      m_server(NULL),
      m_NetworkFrameManager(NULL),
      m_isServerRunning(false)
{
    if (enet_initialize () != 0)
    {
        QLOG_ERROR()<<"An error occurred while initializing ENet.";
        emit signalSendLog("An error occurred while initializing ENet.");
    }
    else
    {
        QLOG_ERROR()<<"initialize ENet successed.";
        emit signalSendLog("initialize ENet successed.");
    }
}

CEnetServer::~CEnetServer()
{
    closeServer(false);
    enet_deinitialize();
}

/**
 * @brief CEnetServer::openServer 打开服务器
 * @param port
 * @return
 */
bool CEnetServer::openServer(int port)
{
    if(m_isServerRunning ||
       m_server != NULL ||
       port <= 0)
        return false;

    ENetAddress address;

    address.host = ENET_HOST_ANY;
    address.port = port;

    m_server = enet_host_create (& address /* the address to bind the server host to */,
                                 32      /* allow up to 32 clients and/or outgoing connections */,
                                  2      /* allow up to 2 channels to be used, 0 and 1 */,
                                  0      /* assume any amount of incoming bandwidth */,
                                  0      /* assume any amount of outgoing bandwidth */);
    if (m_server == NULL)
    {
        QLOG_ERROR()<<"An error occurred while trying to create an ENet server host.";
        emit signalSendLog("An error occurred while trying to create an ENet server host.");
        return false;
    }

    m_isServerRunning = true;
    this->start();

    emit signalSendLog("ENet server start successed,port:"+QString::asprintf("%d",port));

    return true;
}

/**
 * @brief CEnetServer::closeServer 关闭服务器
 */
void CEnetServer::closeServer(bool islog)
{
    if(!m_isServerRunning ||
        m_server == NULL)
        return;

    //if(islog)
    //    emit signalSendLog("ENet server stoping...");

    m_isServerRunning = false;
    this->wait();

    enet_host_destroy(m_server);
    m_allsockets.clear();

    if(islog)
        emit signalSendLog("ENet server stop successed.");
}

/**
 * @brief CEnetServer::addClient 添加一个客户端到列表中
 * @param uuidStr
 * @param peer
 * @return
 */
bool CEnetServer::addClient(QString uuidStr,ENetPeer *peer)
{
    if(uuidStr.isEmpty() || peer == NULL)
        return false;

    QMutexLocker locker(&m_lockserver);

    QHash<ENetPeer*,QString>::iterator iter = m_allsockets.find(peer);
    if(iter != m_allsockets.end())
        return false;

    m_allsockets[peer] = uuidStr;

    return true;
}

/**
 * @brief CEnetServer::delClient 从列表中删除一个客户端
 * @param peer
 * @return
 */
bool CEnetServer::delClient(ENetPeer *peer)
{
    if(peer == NULL)
        return false;

    QMutexLocker locker(&m_lockserver);

    QHash<ENetPeer*,QString>::iterator iter = m_allsockets.find(peer);
    if(iter == m_allsockets.end())
        return false;

    emit signalSendLog("client:"+
                       iter.value()+
                       " leaved.");

    m_allsockets.erase(iter);

    return true;
}

/**
 * @brief CEnetServer::sendData 发送数据
 * @param peer
 * @param data
 */
bool CEnetServer::sendData(ENetPeer *peer,QByteArray data)
{
    if(peer == NULL || data.isEmpty() || m_server == NULL)
        return false;

    if(!isExistClient(peer))
        return false;

    QMutexLocker locker(&m_lockserver);

    ENetPacket *packet=enet_packet_create(NULL,data.size()+1,ENET_PACKET_FLAG_RELIABLE); //创建包
    strcpy((char*)packet->data,data.constData());

    bool isOk = enet_peer_send(peer,1,packet) < 0 ? false : true;

    enet_host_flush (m_server);

    return isOk;
}

/**
 * @brief CEnetServer::sendAll 给所有客户端发送数据
 * @param data
 */
bool CEnetServer::sendAll(QByteArray data)
{
    if(m_allsockets.isEmpty() || data.isEmpty() || m_server == NULL)
        return false;

    bool isOk = true;
    QMutexLocker locker(&m_lockserver);

    ENetPacket *packet=enet_packet_create(NULL,data.size(),ENET_PACKET_FLAG_RELIABLE); //创建包
    strcpy((char*)packet->data,data.constData());

    QHash<ENetPeer*,QString>::iterator iter = m_allsockets.begin();
    for(;iter != m_allsockets.end();++iter)
    {
        isOk = enet_peer_send(iter.key(),1,packet) < 0 ? false : true;
    }

    enet_host_flush (m_server);

    return isOk;
}

/**
 * @brief CEnetServer::getClientID 得到指定客户端的id
 * @param peer
 * @return
 */
QString CEnetServer::getClientID(ENetPeer *peer)
{
    if(peer == NULL || m_allsockets.isEmpty())
        return "";

    QMutexLocker locker(&m_lockserver);

    QHash<ENetPeer*,QString>::iterator iter = m_allsockets.find(peer);
    if(iter == m_allsockets.end())
        return "";

    return iter.value();
}

/**
 * @brief CEnetServer::isExistClient 检测指定客户端是否存在
 * @param peer
 * @return
 */
bool CEnetServer::isExistClient(ENetPeer *peer)
{
    if(peer == NULL || m_allsockets.isEmpty())
        return false;

    QMutexLocker locker(&m_lockserver);

    QHash<ENetPeer*,QString>::iterator iter = m_allsockets.find(peer);
    if(iter == m_allsockets.end())
        return false;

    return true;
}

void CEnetServer::run()
{
   if(m_server == NULL)
       return;

   ENetEvent event;
   int state = enet_host_service(m_server,&event,10);

   while(m_isServerRunning && state>=0)
   {
       if(event.type==ENET_EVENT_TYPE_CONNECT) //有客户机连接成功
       {
           QString uuidString = QUuid::createUuid().toString();
           ENetAddress remote=event.peer->address; //远程地址
           char ip[256];
           enet_address_get_host_ip(&remote,ip,256);
           QLOG_INFO() << "id:" << uuidString << " ip:" << ip;
           emit signalSendLog("client arrived,id:"+
                              uuidString+
                              " ip:"+
                              ip);
           //event.peer->data=(void*)(&uuidString);

           addClient(uuidString,event.peer);

           if(m_NetworkFrameManager)
               m_NetworkFrameManager->OnProcessEnetConnectedNetMes(event.peer);
       }
       else if(event.type==ENET_EVENT_TYPE_RECEIVE) //收到数据
       {
            if(m_NetworkFrameManager)
                m_NetworkFrameManager->OnProcessEnetNetBinary(event.peer,
                                                              QByteArray((const char *)event.packet->data,
                                                                         event.packet->dataLength));
       }
       else if(event.type==ENET_EVENT_TYPE_DISCONNECT) //失去连接
       {
            delClient(event.peer);

            if(m_NetworkFrameManager)
                m_NetworkFrameManager->OnProcessEnetDisconnectedNetMes(event.peer);
       }

       m_lockserver.lock();
       state = enet_host_service(m_server,&event,10);
       m_lockserver.unlock();

       QThread::msleep(1);
   }
}
