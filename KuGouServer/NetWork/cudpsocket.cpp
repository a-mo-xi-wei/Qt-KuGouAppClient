#include "cudpsocket.h"
#include "QsLog.h"
#include "common.h"

#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QCryptographicHash>

/**
 * @brief CUdpSocket::CUdpSocket
 * @param parent
 * @param isEnableKcp  是否启用kcp，默认是启用
 * @param isEnableReciver 是否接收数据，默认是启用
 * @param receivePort 接收数据的端口，如果设置成-1表示产生一个随机端口,否则启动指定的端口,
 *        随机端口号在1000-2000之间
 */
CUdpSocket::CUdpSocket(QObject *parent,bool isEnableKcp,bool isEnableReciver,
                       int receivePort,bool isProcessDatagramHearder,
                       bool isServerUse)
    : QObject(parent),
      m_isProcessDatagramHearder(false),
      m_ikcp(NULL),
      m_address(QHostAddress::Broadcast),
      m_port(-1),
      m_enableKcp(isEnableKcp),
      m_msgHeartCount(0),
      m_currentHeartTime(0),
      m_isEnableReciver(isEnableReciver),
      m_receivePort(receivePort),
      m_isSendHeart(true),
      m_currentUseKcp(true),
      m_isProcessBinaryData(isProcessDatagramHearder),
      m_isConnected(false),
      m_isServerUsing(isServerUse)
{
    m_ikcp = new HYKT::KcpObj(this);

    if(isServerUse) this->InitKcp();

    if(m_isEnableReciver)
    {
        if(m_receivePort == -1)
        {/*
            qsrand(time(NULL));
            m_receivePort = qrand() % 1000 + 1000;*/
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            // Qt 6 使用 C++ 标准库
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1000, 1999);
            m_receivePort = dis(gen);
#else
            // Qt 5 使用 qsrand 和 qrand
            qsrand(time(NULL));
            m_receivePort = qrand() % 1000 + 1000;
#endif
        }

        m_UdpSocket.bind(m_receivePort,QUdpSocket::ShareAddress);

        QLOG_INFO()<<"CUdpSocket bind port:"<<QString::asprintf("%d",m_receivePort);

        connect(&m_UdpSocket,
                &QUdpSocket::readyRead,
                this,&CUdpSocket::reciverPendingDatagram);
    }

    QObject::connect(&m_updateTimer, &QTimer::timeout,
        this, &CUdpSocket::KCPUpdate);
    m_updateTimer.start(10);
}

CUdpSocket::~CUdpSocket(void)
{
    ReleaseKcp();
    if(m_ikcp) m_ikcp->deleteLater();
    m_ikcp=NULL;
}

/**
 * @brief CUdpSocket::connect 设置要连接的地址和端口
 * @param address 要设置的电脑IP地址
 * @param port 电脑端口
 */
void CUdpSocket::Connect(QHostAddress address,int port)
{
    m_address = address;
    m_port = port;
}

/**
 * @brief CUdpSocket::getName 得到socket的名称
 * @return
 */
QString CUdpSocket::getName(void)
{
    return convert_to_ipv4_addr(m_address) + "_" + QString::asprintf("%ld",m_port);
}

/**
 * @brief CUdpSocket::Send 发送报文
 * @param buf 要发送的报文数据
 * @param len 要发送的报文数据长度
 * @return 返回发送成功的数据长度
 */
void CUdpSocket::onProcessSendData(const char * buf, int len)
{
    if(buf == NULL || len <= 0)
        return;

    m_UdpSocket.writeDatagram(buf,len,m_address,m_port);
}

/**
 * @brief CUdpSocket::Send 发送报文
 * @param Datagramdata 要发送的报文数据
 * @return 返回发送成功的数据长度
 */
qint64 CUdpSocket::SendByte(QByteArray Datagramdata)
{
    if(Datagramdata.isEmpty() || m_port <= 0)
        return -1;

    QByteArray pSendDatagramData;

    tagUdpDatagramHearder ptagDatagramHearder;
    ptagDatagramHearder.version[0] = 'K';
    ptagDatagramHearder.version[1] = 'C';
    ptagDatagramHearder.version[2] = 'P';
    ptagDatagramHearder.srcdataSize = Datagramdata.size();
    ptagDatagramHearder.compressdataSize = ptagDatagramHearder.srcdataSize;

    // 压缩数据
    QByteArray tmpDecData;

    if(FastlzCompressData(Datagramdata,tmpDecData))
        ptagDatagramHearder.compressdataSize = tmpDecData.size();
    else
        tmpDecData = Datagramdata;

    pSendDatagramData.append((char*)&ptagDatagramHearder,sizeof(tagUdpDatagramHearder));
    pSendDatagramData.append(tmpDecData);

    qint64 m_sendsize,m_totalfilesize;                                  /**< 当前发送的文件的数据大小和文件总大小 */

    m_sendsize = 0;
    m_totalfilesize = pSendDatagramData.size();

    do
    {
        QByteArray psendbytearray;

        if(pSendDatagramData.size()-m_sendsize < BUF_SIZE)
            psendbytearray = pSendDatagramData.mid(m_sendsize);
        else
            psendbytearray = pSendDatagramData.mid(m_sendsize,BUF_SIZE);

        m_sendsize += m_UdpSocket.writeDatagram(psendbytearray.data(),
                                         psendbytearray.size(),
                                         m_address,m_port);
    }
    while(m_sendsize < m_totalfilesize);

    return m_sendsize;
}

/**
 * @brief CUdpSocket::Send 发送报文
 * @param buf 要发送的报文数据
 * @param len 要发送的报文数据长度
 * @return 返回发送成功的数据长度
 */
int CUdpSocket::SendKcpData(QByteArray Datagramdata)
{
    if(!m_isConnected &&
        Datagramdata.isEmpty() ||
        m_enableKcp == false ||
        (m_ikcp == NULL || !m_ikcp->IsOpen()) ||
        m_port <= 0)
        return -1;

    m_kcpMutex.lock();

    QByteArray pSendDatagramData;

    tagUdpDatagramHearder ptagDatagramHearder;
    ptagDatagramHearder.version[0] = 'K';
    ptagDatagramHearder.version[1] = 'C';
    ptagDatagramHearder.version[2] = 'P';
    ptagDatagramHearder.srcdataSize = Datagramdata.size();
    ptagDatagramHearder.compressdataSize = ptagDatagramHearder.srcdataSize;

    // 压缩数据
    QByteArray tmpDecData;

    if(FastlzCompressData(Datagramdata,tmpDecData))
        ptagDatagramHearder.compressdataSize = tmpDecData.size();
    else
        tmpDecData = Datagramdata;

    pSendDatagramData.append((char*)&ptagDatagramHearder,sizeof(tagUdpDatagramHearder));
    pSendDatagramData.append(tmpDecData);

    m_ikcp->KcpSend(pSendDatagramData.data(),pSendDatagramData.size());

    m_kcpMutex.unlock();

    return 1;
}

void CUdpSocket::KCPUpdate()
{
    // 处理心跳(必须在用户设置了接收以后才能处理，否则会堆积大堆数据)
    if(!m_isServerUsing &&
        m_isSendHeart &&
        m_isEnableReciver &&
        m_port > 0)
    {
        qint64 tempTime = (qint64)time(NULL);

        //qDebug()<<tempTime<<" "<<m_currentHeartTime<<" "<<tempTime-m_currentHeartTime;

        if(tempTime-m_currentHeartTime > 1)
        {
            m_currentHeartTime = tempTime;

            SendByte(QByteArray("100"));

            // 心跳计数
            this->heart();

            // 如果心跳大于2次表示连接断开
            if(this->GetHeartCount() > 2 &&
               (m_ikcp != NULL && m_ikcp->IsOpen()))
            {
                this->ReleaseKcp();

                m_isConnected = false;
                m_msgHeartCount = 0;
                m_datagramData.clear();
                memset(&m_DatagramHearder,0,sizeof(m_DatagramHearder));
                m_isProcessDatagramHearder=false;

                emit signal_Disconnected();
            }
        }
    }
}

/**
 * @brief CUdpSocket::InitKcp 初始Kcp
 */
void CUdpSocket::InitKcp(void)
{
    if(m_ikcp == NULL ||
       m_ikcp->IsOpen())
        return;

    m_kcpMutex.lock();

    connect(m_ikcp, &HYKT::KcpObj::onKcpOutput, this,
            &CUdpSocket::onProcessSendData,
            Qt::QueuedConnection);
    connect(m_ikcp, &HYKT::KcpObj::onKcpRecv, this,
            &CUdpSocket::DoKcpRecv,
            Qt::QueuedConnection);
    m_ikcp->Open();

    m_kcpMutex.unlock();
}

/**
 * @brief CUdpSocket::ReleaseKcp 释放Kcp
 */
void CUdpSocket::ReleaseKcp(void)
{
    if(m_ikcp == NULL ||
       !m_ikcp->IsOpen())
        return;

    m_kcpMutex.lock();

    disconnect(m_ikcp, &HYKT::KcpObj::onKcpOutput, this, &CUdpSocket::onProcessSendData);
    disconnect(m_ikcp, &HYKT::KcpObj::onKcpRecv, this, &CUdpSocket::DoKcpRecv);
    m_ikcp->Close();

    m_kcpMutex.unlock();
}

/**
 * @brief CUdpSocket::onProcessReciverDatagramdata 处理接收到的报文
 * @param Datagramdata 要处理的接收到的报文
 */
void CUdpSocket::onProcessReciverDatagramdata(void)
{
    while(!m_datagramData.isEmpty())
    {
        if(m_isProcessBinaryData)
        {
            if(!m_isProcessDatagramHearder && m_datagramData.size() > sizeof(tagUdpDatagramHearder))
            {
                memcpy(&m_DatagramHearder,m_datagramData.constData(),sizeof(tagUdpDatagramHearder));

                if(m_DatagramHearder.version[0] != 'K' &&
                   m_DatagramHearder.version[1] != 'C' &&
                   m_DatagramHearder.version[2] != 'P')
                {
                    m_datagramData.clear();
                    QLOG_ERROR()<<"CUdpSocket::onProcessReciverDatagramdata:Version inconsistency";
                    return;
                }

                m_datagramData.remove(0,sizeof(tagUdpDatagramHearder));
                m_isProcessDatagramHearder = true;
            }

            if(!m_isProcessDatagramHearder || m_datagramData.size() < m_DatagramHearder.compressdataSize)
            {
                m_msgHeartCount = 0;
                break;
            }

            // 得到当前文件数据
            QByteArray precvDatagramData = m_datagramData.mid(0,m_DatagramHearder.compressdataSize);
            m_datagramData.remove(0,m_DatagramHearder.compressdataSize);

            // 解压数据
            QByteArray tmpDecData;
            //tmpDecData.reserve(m_DatagramHearder.srcdataSize);

            if(!FastlzDecompressData(precvDatagramData,tmpDecData,m_DatagramHearder.srcdataSize) ||
               tmpDecData.size() != m_DatagramHearder.srcdataSize)
            {
                m_datagramData.clear();
                m_isProcessDatagramHearder=false;
                m_msgHeartCount = 0;
                QLOG_ERROR()<<"CUdpSocket::onProcessReciverDatagramdata:size is not same.";
                return;
            }

            precvDatagramData = tmpDecData;
            m_msgHeartCount = 0;

            //qDebug()<<tmpDecData.size()<<" "<<precvDatagramData.size();

            // 心跳消息不处理
            if(QString(precvDatagramData) != "100")
                emit processPendingDatagram(this,precvDatagramData);

            m_isProcessDatagramHearder=false;
        }
        else
        {
            m_msgHeartCount = 0;

            QByteArray precvDatagramData = m_datagramData;
            m_datagramData.clear();

            // 心跳消息不处理
            if(QString(precvDatagramData) != "100")
                emit processPendingDatagram(this,precvDatagramData);
        }
    }
}

/**
 * @brief CUdpSocket::onProcessReciverKcpDatagramdata kcp处理接收到的报文
 * @param Datagramdata 要处理的报文
 * @param isenableKcp 是否是kcp报文
 */
void CUdpSocket::onProcessReciverKcpDatagramdata(QByteArray& Datagramdata,bool isenableKcp)
{
    if(Datagramdata.isEmpty())
        return;

    if(isenableKcp &&
       (m_ikcp && m_ikcp->IsOpen()))
    {
        m_kcpMutex.lock();
        m_ikcp->KcpInput(Datagramdata.data(), Datagramdata.size());
        m_kcpMutex.unlock();
    }
    else
    {
        m_datagramData.append(Datagramdata);
    }
}

void CUdpSocket::reciverPendingDatagram()
{
    while(m_UdpSocket.hasPendingDatagrams())
    {
        QByteArray preciverDatagramData;

        preciverDatagramData.resize(m_UdpSocket.pendingDatagramSize());

        if(m_UdpSocket.readDatagram(preciverDatagramData.data(),preciverDatagramData.size()) > 0)
        {
            bool isprocessKcp = m_enableKcp;

            if(!m_isProcessDatagramHearder)
            {
                // 心跳消息使用普通处理方式
                const char *tmpData = preciverDatagramData.data();
                if(tmpData &&
                   (tmpData[0] == 'K' && tmpData[1] == 'C' && tmpData[2] == 'P'))
                {
                    isprocessKcp = false;
                }

                this->setCurrentUsingKcp(isprocessKcp);
            }

            onProcessReciverKcpDatagramdata(preciverDatagramData,
                                            this->isCurrentUsingKcp());

            // 如果心跳是1，是连接成功
            if(!m_isServerUsing &&
               this->GetHeartCount() == 1 &&
               (m_ikcp != NULL && !m_ikcp->IsOpen()))
            {
                this->InitKcp();

                m_datagramData.clear();
                memset(&m_DatagramHearder,0,sizeof(m_DatagramHearder));
                m_isProcessDatagramHearder=false;

                emit signal_Connected();
            }

            m_isConnected = true;
            m_msgHeartCount = 0;
        }
    }

    onProcessReciverDatagramdata();
}

/**
 * @brief CUdpSocket::DoKcpRecv kcp转换后消息处理
 * @param buf
 * @param len
 */
void CUdpSocket::DoKcpRecv(const char *buf, int len)
{
    if(buf == NULL || len <= 0)
        return;

    m_datagramData.append(buf,len);

    onProcessReciverDatagramdata();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CUdpServer::CUdpServer(QObject *parent,bool isEnableKcp)
    : m_enableKcp(isEnableKcp)
{
    connect(&m_UdpSocket,&QUdpSocket::readyRead,
            this,&CUdpServer::reciverPendingDatagram/*,
            Qt::QueuedConnection*/);

    QObject::connect(&m_updateTimer, &QTimer::timeout,
        this, &CUdpServer::KCPUpdate);
    m_updateTimer.start(1000);
}

CUdpServer::~CUdpServer(void)
{
    Clear();
}

/**
 * @brief CUdpServer::open 设置要打开的端口
 * @param port 要打开的端口
 * @param isenableKcp 是否启动KCP
 *
 * @return 如果端口绑定成功返回真，否则返回假
 */
bool CUdpServer::open(int port,bool isenableKcp)
{
    if(port <= 0)
        return false;

    m_enableKcp = isenableKcp;

    return m_UdpSocket.bind(QHostAddress::Any,port);
}

/// 关闭指定的socket
void CUdpServer::closeSocket(CUdpSocket *socketid)
{
    if(socketid == NULL) return;

    m_ClientMutex.lock();

    QHash<QString,CUdpSocket*>::iterator iter = m_udpClients.begin();
    for(;iter != m_udpClients.end();)
    {
        if(iter.value() != socketid)
            continue;

        CUdpSocket *pClient = (*iter);

        QHash<QUdpSocket*,CUdpSocket*>::iterator iterTwo = m_udpKcpClients.find(pClient->getSocketID());
        if(iterTwo != m_udpKcpClients.end())
            m_udpKcpClients.erase(iterTwo);

        disconnect((*iter),SIGNAL(processPendingDatagram(CUdpSocket*,QByteArray)),
                   this,SLOT(on_process_processPendingDatagram(CUdpSocket*,QByteArray)));

        emit signal_ClientLeave(pClient);

        pClient->deleteLater();
        m_udpClients.erase(iter);

        break;
    }

    m_ClientMutex.unlock();
}

/**
 * @brief CUdpServer::stop 停止服务器
 */
void CUdpServer::stop(void)
{
    m_UdpSocket.close();
}

/**
 * @brief CUdpServer::Clear 清除所有的数据
 */
void CUdpServer::Clear(void)
{
    m_ClientMutex.lock();

    QHash<QString,CUdpSocket*>::iterator iter = m_udpClients.begin();
    for(;iter != m_udpClients.end();++iter)
    {
        delete (*iter);
    }

    m_udpClients.clear();
    m_udpKcpClients.clear();

    m_ClientMutex.unlock();
}

void CUdpServer::KCPUpdate()
{
    if(m_udpClients.isEmpty())
        return;

    m_ClientMutex.lock();

    QHash<QString,CUdpSocket*>::iterator iter = m_udpClients.begin();
    for(;iter != m_udpClients.end();)
    {
        CUdpSocket *pClient = (*iter);

        if(pClient->GetHeartCount() > 3)
        {
            QHash<QUdpSocket*,CUdpSocket*>::iterator iterTwo = m_udpKcpClients.find(pClient->getSocketID());
            if(iterTwo != m_udpKcpClients.end())
                m_udpKcpClients.erase(iterTwo);

            disconnect((*iter),SIGNAL(processPendingDatagram(CUdpSocket*,QByteArray)),
                       this,SLOT(on_process_processPendingDatagram(CUdpSocket*,QByteArray)));

            emit signal_ClientLeave(pClient);

            pClient->deleteLater();
            iter = m_udpClients.erase(iter);
        }
        else
        {
            (*iter)->heart();
            ++iter;
        }
    }

    m_ClientMutex.unlock();
}

/**
 * @brief CUdpServer::SendAll 发送消息给所有人
 * @param datagramdata 要发送的数据包
 */
void CUdpServer::SendAll(QByteArray datagramdata)
{
    if(m_udpKcpClients.isEmpty())
        return;

    m_ClientMutex.lock();

    QHash<QUdpSocket*,CUdpSocket*>::iterator iterTwo = m_udpKcpClients.begin();
    for(;iterTwo != m_udpKcpClients.end();++iterTwo)
    {
        if(m_enableKcp) (*iterTwo)->SendKcpData(datagramdata);
        else (*iterTwo)->SendByte(datagramdata);
    }

    m_ClientMutex.unlock();
}

/// 发送除了指定socketid的其它人
void CUdpServer::SendOther(QUdpSocket* socketid,QByteArray datagramdata)
{
    if(m_udpKcpClients.isEmpty())
        return;

    m_ClientMutex.lock();

    QHash<QUdpSocket*,CUdpSocket*>::iterator iterTwo = m_udpKcpClients.begin();
    for(;iterTwo != m_udpKcpClients.end();++iterTwo)
    {
        if((*iterTwo)->getSocketID() == socketid)
            continue;

        if(m_enableKcp) (*iterTwo)->SendKcpData(datagramdata);
        else (*iterTwo)->SendByte(datagramdata);
    }

    m_ClientMutex.unlock();
}

/// 发送给指定socketid
void CUdpServer::SendTo(QUdpSocket* socketid,QByteArray datagramdata)
{
    if(m_udpKcpClients.isEmpty())
        return;

    m_ClientMutex.lock();

    QHash<QUdpSocket*,CUdpSocket*>::iterator iterTwo = m_udpKcpClients.find(socketid);
    if(iterTwo != m_udpKcpClients.end())
    {
        if(m_enableKcp) (*iterTwo)->SendKcpData(datagramdata);
        else (*iterTwo)->SendByte(datagramdata);
    }

    m_ClientMutex.unlock();
}

void CUdpServer::on_process_processPendingDatagram(CUdpSocket* socketid,QByteArray datagramdata)
{
    emit signal_ClientReceiveMsg(socketid,datagramdata);
}

void CUdpServer::reciverPendingDatagram()
{
    while(m_UdpSocket.hasPendingDatagrams())
    {
        QByteArray preciverDatagramData;
        QHostAddress sender;
        quint16 senderPort = 0;

        preciverDatagramData.resize(m_UdpSocket.pendingDatagramSize());

        if(m_UdpSocket.readDatagram(preciverDatagramData.data(),preciverDatagramData.size(),
                                    &sender, &senderPort) > 0)
        {
            m_ClientMutex.lock();

            QString kcpName = sender.toString() + "_" + QString::asprintf("%ld",senderPort);
            QByteArray md5Str = QCryptographicHash::hash(kcpName.toUtf8(), QCryptographicHash::Md5);
            CUdpSocket *pUdpSocket = NULL;

            QHash<QString,CUdpSocket*>::iterator iter = m_udpClients.find(md5Str.toHex());
            if(iter != m_udpClients.end())
                pUdpSocket = iter.value();

            if(pUdpSocket == NULL)
            {
                pUdpSocket = new CUdpSocket(nullptr,m_enableKcp,false,-1,true,true);
                pUdpSocket->Connect(sender,senderPort);

                connect(pUdpSocket,SIGNAL(processPendingDatagram(CUdpSocket*,QByteArray)),this,
                        SLOT(on_process_processPendingDatagram(CUdpSocket*,QByteArray)),
                        Qt::QueuedConnection);

                m_udpClients[md5Str.toHex()] = pUdpSocket;
                m_udpKcpClients[pUdpSocket->getSocketID()] = pUdpSocket;

                emit signal_ClientArrive(pUdpSocket);
            }

            bool isprocessKcp = m_enableKcp;

            if(!pUdpSocket->isProcessDatagramHearder())
            {
                // 心跳消息使用普通处理方式
                const char *tmpData = preciverDatagramData.data();
                if(tmpData &&
                   (tmpData[0] == 'K' && tmpData[1] == 'C' && tmpData[2] == 'P'))
                {
                    isprocessKcp = false;
                }

                pUdpSocket->setCurrentUsingKcp(isprocessKcp);
            }

            pUdpSocket->onProcessReciverKcpDatagramdata(preciverDatagramData,
                                                        pUdpSocket->isCurrentUsingKcp());

            // 这里和CUdpSocket处理不太一样，这里是处理多个客户端发送的消息，如果放在外面处理可能会存在多个客户端的
            // 消息，如果放在外面处理可能会导致只处理一个客户端的消息，所以需要在每个客户端收到消息后就自行处理自己客户端的
            // 消息.
            pUdpSocket->onProcessReciverDatagramdata();

            pUdpSocket->resetHeartCount();

            // 回复心跳消息
            //if(pUdpSocket->GetHeartCount() <= 0)
                pUdpSocket->SendByte(QByteArray("100"));

            m_ClientMutex.unlock();
        }
    }
}

/**
 * @brief CUdpServer::getSocket 根据socketid得到相应的socket
 * @param socketid socket的id
 * @return
 */
CUdpSocket* CUdpServer::getSocket(QUdpSocket *socketid)
{
    if(m_udpKcpClients.isEmpty() || socketid == NULL)
        return NULL;

    m_ClientMutex.lock();

    QHash<QUdpSocket*,CUdpSocket*>::iterator iter = m_udpKcpClients.find(socketid);
    if(iter != m_udpKcpClients.end())
    {
        m_ClientMutex.unlock();
        return (*iter);
    }

    m_ClientMutex.unlock();

    return NULL;
}

/**
 * @brief CUdpServer::addSocket 添加一个新的客户端
 * @param address 要添加的客户端的IP
 * @param port 要添加的客户端的端口
 *
 * @return 返回新添加的客户端
 */
CUdpSocket* CUdpServer::addSocket(QHostAddress address,int port)
{
    m_ClientMutex.lock();

    QString kcpName = address.toString() + "_" + QString::asprintf("%d",port);
    CUdpSocket *pUdpSocket = NULL;

    QHash<QString,CUdpSocket*>::iterator iter = m_udpClients.find(kcpName);
    if(iter == m_udpClients.end())
    {
        pUdpSocket = new CUdpSocket(nullptr,true,false);
        pUdpSocket->Connect(address,port);

        connect(pUdpSocket,
                SIGNAL(processPendingDatagram(CUdpSocket*,QByteArray)),
                this,SLOT(on_process_processPendingDatagram(CUdpSocket*,QByteArray)),
                Qt::QueuedConnection);

        m_udpClients[kcpName] = pUdpSocket;
        m_udpKcpClients[pUdpSocket->getSocketID()] = pUdpSocket;
    }
    else
    {
        pUdpSocket = (*iter);
    }

    m_ClientMutex.unlock();

    return pUdpSocket;
}

/**
 * @brief CUdpServer::getSocketByName 根据名称得到客户端
 * @param name 客户端的名称
 * @return 如果客户端存在返回这个客户端，否则返回NULL
 */
CUdpSocket* CUdpServer::getSocketByName(QString name)
{
    if(name.isEmpty())
        return NULL;

    m_ClientMutex.lock();

    QHash<QString,CUdpSocket*>::iterator iter = m_udpClients.find(name);
    if(iter != m_udpClients.end())
    {
        m_ClientMutex.unlock();
        return (*iter);
    }

    m_ClientMutex.unlock();

    return NULL;
}
