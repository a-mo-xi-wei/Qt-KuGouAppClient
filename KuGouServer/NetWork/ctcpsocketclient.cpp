#include "ctcpsocketclient.h"
#include "QsLog.h"
#include "rc4.h"
#include "fastlz.h"

#include <QJsonDocument>
#include <QThread>
#include <QHostAddress>

CTcpSocket::CTcpSocket(NetworkFrameManager *pNetworkFrameManager,
                       QObject *parent,bool isReConnecting,
                       qintptr socketDescriptor,
                       bool enableencryption,
                       bool isencoded)
    : QTcpSocket (parent),
      m_NetworkFrameManager(pNetworkFrameManager),
      m_recvDataState(false),
      m_ServerPort(0),
      socketID(socketDescriptor),
      m_isReConnecting(isReConnecting),
      m_HeartCount(0),
      m_enableencryption(enableencryption),
      m_curThread(NULL),
      m_isencoded(isencoded)
{
    if(socketDescriptor != -1)
        this->setSocketDescriptor(socketDescriptor);

    connect(this, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);

    //connect(this, static_cast<QAbstractSocketErrorSignal>(&QTcpSocket::error), this, &CTcpSocket::error);
    // 兼容 Qt 5 和 Qt 6
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(this, &QAbstractSocket::errorOccurred, this, &CTcpSocket::error);
#else
    connect(this, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &CTcpSocket::error);
#endif
    dis = connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    if(m_isReConnecting)
    {
        QObject::connect(&m_TcpSocketReconnectTimer, &QTimer::timeout, this, &CTcpSocket::handleTcpSocketReconnect);
        m_TcpSocketReconnectTimer.setInterval(5000);
    }

    QObject::connect(&m_TcpSocketHeartTimeOutTimer, &QTimer::timeout, this, &CTcpSocket::handleTcpSocketHeartTimeOut);
    m_TcpSocketHeartTimeOutTimer.setInterval(2000);

    if(socketID != -1)
        m_TcpSocketHeartTimeOutTimer.start();
}

CTcpSocket::~CTcpSocket()
{
    //close();
}

/**
 * @brief CTcpSocket::disConTcp 断开socket的连接
 * @param socketDescriptor
 */
void CTcpSocket::disConTcp(qint64 socketDescriptor)
{
    if (socketDescriptor != -1 &&
            socketDescriptor == socketID)
    {
        this->disconnectFromHost();
    }
    else if (socketDescriptor == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        //this->deleteLater();
    }
}

/**
 * @brief CTcpSocket::connectTcpServer 连接tcp服务器
 * @param ip 服务器ip
 * @param port 服务器断开
 */
void CTcpSocket::connectTcpServer(const QString& ip, quint16 port)
{
    if(ip.isEmpty() || port <= 0 || this->isConnected())
        return;

    m_HeartCount=0;
    m_recvDataState=false;
    m_ServerPort = port;
    m_ServerIP = ip;

    this->connectToHost(ip, port);
    //this->waitForConnected();
}

/**
 * @brief CTcpSocket::close 关闭与tcp服务器的连接
 */
void CTcpSocket::close(void)
{
    disConTcp(socketID);
}

/**
 * @brief CTcpSocket::connectClient 发送新用户连接信息
 * @param socketDescriptor
 * @param ip
 * @param port
 */
void CTcpSocket::newconnectClient(qint64 socketDescriptor, const QString &ip, quint16 port)
{
    onConnected();
}

/**
 * @brief CWebSocketClient::onConnected 连接成功后的处理
 */
void CTcpSocket::onConnected()
{
    if(socketID == -1)
        m_TcpSocketHeartTimeOutTimer.start();
    m_TcpSocketReconnectTimer.stop();

    QLOG_INFO()<<"CTcpSocket::onConnected";

    if(m_NetworkFrameManager)
    {
        if(socketID != -1) m_NetworkFrameManager->lock();

        m_NetworkFrameManager->OnProcessConnectedNetMes(this);

        if(socketID != -1) m_NetworkFrameManager->unlock();
    }
}

/**
 * @brief CTcpSocket::error 错误处理
 * @param socketError
 */
void CTcpSocket::error(QAbstractSocket::SocketError socketError)
{
    QLOG_INFO()<<"CTcpSocket::error:"<<socketError;

     reconnectTcpServer();   
}

/**
 * @brief CTcpSocket::onDisconnected 连接断开后的处理
 */
void CTcpSocket::onDisconnected()
{
    if(m_NetworkFrameManager)
    {
        if(socketID != -1) m_NetworkFrameManager->lock();

        m_NetworkFrameManager->OnProcessDisconnectedNetMes(this);

        if(socketID != -1) m_NetworkFrameManager->unlock();
    }

    if(socketID != -1)
    {
        emit sockDisConnect(socketID,
                            this->peerAddress().toString(),
                            this->peerPort(),
                            m_curThread);//发送断开连接的用户信息
        //this->deleteLater();
    }

    m_TcpSocketHeartTimeOutTimer.stop();

    if(m_isReConnecting && !m_TcpSocketReconnectTimer.isActive())
        m_TcpSocketReconnectTimer.start();
}

/**
 * @brief CTcpSocket::reconnectTcpServer 重新连接服务器
 */
void CTcpSocket::reconnectTcpServer(void)
{
    if(!m_isReConnecting || m_TcpSocketReconnectTimer.isActive())
        return;

    if(this->state() != QTcpSocket::ConnectedState &&
       this->state() != QTcpSocket::ConnectingState)
    {
        this->close();
        onDisconnected();
    }
}

/**
 * @brief CWebSocketClient::handleTcpSocketReconnect 处理客户端断开后重连
 */
void CTcpSocket::handleTcpSocketReconnect()
{
    if(!m_isReConnecting || !this->isValid())
        return;

    QLOG_INFO()<<"CTcpSocket::handleTcpSocketReconnect()";

    this->connectTcpServer(m_ServerIP,m_ServerPort);
}

/**
 * @brief CWebSocketClient::handleTcpSocketHeartTimeOut 处理心跳消息
 */
void CTcpSocket::handleTcpSocketHeartTimeOut()
{
    if(!this->isConnected())
        return;

    if(socketID != -1)
    {
        if(m_HeartCount > 5)
        {
            this->close();
        }

        m_HeartCount+=1;
    }
    else
    {
        send(QByteArray("heart"));
    }
}

/**
 * @brief CTcpSocket::sendJson 发送json数据
 * @param mes 要发送的数据
 */
void CTcpSocket::sendJson(QJsonObject mes)
{
    if(mes.isEmpty() || !isConnected())
        return;

    QJsonDocument document=QJsonDocument(mes);
    QByteArray array = document.toJson();// 转换成QByteArray

    if(m_enableencryption)
    {
        unsigned char buf[10240];

        int declen = Rc4Encrypt(RC4_KEY,(unsigned char*)array.data(),buf,array.size());
        if(declen > 0)
            array = QByteArray((const char *)buf,declen);
    }

    this->send(array);
}

/**
 * @brief CTcpSocket::OnReadyRead 处理接收数据
 */
void CTcpSocket::OnReadyRead()
{
    m_dataPacket.append(this->readAll());

    // 循环解析包数据，数据中可能不只一包数据
    parsePacket();
}

/**
 * @brief CTcpSocket::parsePacket 解包
 * @param packet 要解包的数据
 *
 * @return 如果数据解包成功返回真，否则返回假
 */
bool CTcpSocket::parsePacket(void)
{
    if(!m_isencoded)
    {
        QByteArray precvDataBytes = m_dataPacket;

        this->m_HeartCount=0;

        if(m_NetworkFrameManager)
        {
            if(socketID != -1) m_NetworkFrameManager->lock();
            m_NetworkFrameManager->OnProcessNetBinary(this,precvDataBytes);
            if(socketID != -1) m_NetworkFrameManager->unlock();
        }

        emit signalsNetBinary(socketID,precvDataBytes);

        m_dataPacket.clear();

        return true;
    }

    while(!m_dataPacket.isEmpty())
    {
        if(!m_recvDataState && m_dataPacket.size() >= sizeof(tagDatagramHearder))
        {
            // 先取包头
            memcpy(&m_TcpPacketHearder,m_dataPacket.constData(),sizeof(tagDatagramHearder));

            // 检查版本号是否正确
            if(m_TcpPacketHearder.version != IDD_TCP_VERSION)
            {
                m_recvDataState = false;
                m_dataPacket.clear();
                break;
            }

            m_dataPacket.remove(0,sizeof(tagDatagramHearder));
            m_recvDataState = true;
        }

        if(!m_recvDataState || m_dataPacket.size() < m_TcpPacketHearder.compressdataSize)
            break;

        // 得到当前数据
        QByteArray precvDataData = m_dataPacket.mid(0,m_TcpPacketHearder.compressdataSize);
        m_dataPacket.remove(0,m_TcpPacketHearder.compressdataSize);

        // 获取文件校验码
        quint16 pdecchecknum = checksum((quint8*)precvDataData.constData(),precvDataData.size());

        if(m_TcpPacketHearder.checknum == pdecchecknum)
        {
            // 解压文件
            //QByteArray precvDataBytes = qUncompress(precvDataData);
            memset(uncompressed_buffer,0,IDD_COMPRESS_BUFFER_SIZE);
            fastlz_decompress(precvDataData.data(),precvDataData.size(),uncompressed_buffer,m_TcpPacketHearder.srcdataSize);

            QByteArray precvDataBytes = QByteArray((const char*)uncompressed_buffer,m_TcpPacketHearder.srcdataSize);

            /*if(m_enableencryption)
            {
                int declen = Rc4Decrypt(RC4_KEY, (unsigned char*)precvDataBytes.data(),(unsigned char*)buf, precvDataBytes.size() );
                if(declen > 0)
                    precvData = QByteArray((const char *)buf,declen);
            }*/

            this->m_HeartCount=0;

            if(precvDataBytes != "heart")
            {
                // 心跳消息就不处理了
                if(m_NetworkFrameManager)
                {
                    if(socketID != -1) m_NetworkFrameManager->lock();
                    m_NetworkFrameManager->OnProcessNetBinary(this,precvDataBytes);
                    if(socketID != -1) m_NetworkFrameManager->unlock();
                }

                emit signalsNetBinary(socketID,precvDataBytes);
            }
        }

        m_recvDataState=false;
    }

    return true;
}

/**
 * @brief CTcpSocket::isConnected 检测是否连接成功
 * @return 如果连接成功返回真，否则返回假
 */
bool CTcpSocket::isConnected(void)
{
    return this->state() ==  QTcpSocket::ConnectedState ? true : false;
}

/**
 * @brief CTcpSocket::send 封包并发送
 * @param data 要发送的数据
 *
 * @return 返回发送成功的数据
 */
qint64 CTcpSocket::send(QByteArray data)
{
    if(this->state() !=  QTcpSocket::ConnectedState ||
       data.isEmpty())
    {
        return -1;
    }

    QByteArray sendData;
    qint64 m_sendsize,m_totalsize;

    if(m_isencoded)
    {
        memset(compressed_buffer,0,IDD_COMPRESS_BUFFER_SIZE);
        int compressedsize = fastlz_compress(data.data(),data.size(),compressed_buffer);

        tagDatagramHearder pTcpPacketHearder;
        pTcpPacketHearder.version = IDD_TCP_VERSION;
        pTcpPacketHearder.srcdataSize = data.size();
        pTcpPacketHearder.compressdataSize = compressedsize;
        pTcpPacketHearder.checknum = checksum((quint8*)compressed_buffer,compressedsize);

        //添加包头
        sendData.append((const char*)&pTcpPacketHearder,sizeof(tagDatagramHearder));

        //添加包数据
        sendData.append((const char*)compressed_buffer,compressedsize);
    }
    else
    {
        //添加包数据
        sendData.append(data);
    }

    m_totalsize = sendData.size();
    m_sendsize=0;

    do
    {
        QByteArray psendbytearray;

        if(sendData.size()-m_sendsize < BUF_SIZE)
            psendbytearray = sendData.mid(m_sendsize);
        else
            psendbytearray = sendData.mid(m_sendsize,BUF_SIZE);

        m_sendsize += this->write(psendbytearray);
    }
    while(m_sendsize < m_totalsize);

    return m_sendsize;
}

