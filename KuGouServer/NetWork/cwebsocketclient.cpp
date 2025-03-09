#include "cwebsocketclient.h"
#include "QsLog.h"
#include <QDir>
#include <utility>

initialiseSingleton(CWebSocketClientManager);

QT_USE_NAMESPACE

CWebSocketClient::CWebSocketClient(NetworkFrameManager *pNetworkFrameManager,QObject *parent)
    :QObject(parent),
    m_serverPort(0),
    m_isCloseSocket(false),
    m_isDisreconnected(true),
    m_NetworkFrameManager(pNetworkFrameManager),
    m_recvFileState(false),
    m_processrecvFile(false),
    m_recvDataState(false),
    m_processrecvData(false)
{
    //connect(&m_webSocket, &QWebSocket::connected, this, &CWebSocketClient::onConnected);
    //connect(&m_webSocket, &QWebSocket::disconnected, this, &CWebSocketClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &CWebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &CWebSocketClient::onBinaryReceived);

    QObject::connect(&m_WebSocketHeartTimeOutTimer, &QTimer::timeout, this, &CWebSocketClient::handleWebSocketHeartTimeOut);
    m_WebSocketHeartTimeOutTimer.setInterval(2000);

    QObject::connect(&m_WebSocketReconnectTimer, &QTimer::timeout, this, &CWebSocketClient::handleWebSocketReconnect);
    m_WebSocketReconnectTimer.setInterval(5000);
}

CWebSocketClient::~CWebSocketClient()
{
    m_webSocket.close();
}

/**
 * @brief CWebSocketClient::Open2 打开指定地址的网络连接
 * @param serverip
 * @param serverport
 */
void CWebSocketClient::Open2(const QString& serverip,int serverport)
{
    if(serverip == "" || serverport <= 0)
        return;

    m_serverIP = serverip;
    m_serverPort = serverport;

    Open(QUrl("ws://"+m_serverIP+QString::asprintf(":%d",m_serverPort)));
}

/**
 * @brief CWebSocketClient::Open 打开指定地址的网络连接
 * @param url 要打开的网址
 */
void CWebSocketClient::Open(const QUrl& url)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &CWebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &CWebSocketClient::onDisconnected);

    m_isCloseSocket=false;
    m_websocketurl = url;
    m_webSocket.open(m_websocketurl);

    QLOG_INFO()<<"CWebSocketClient::Open:"<<url;
    qDebug()<<"CWebSocketClient::Open:"<<url;
}

/**
 * @brief CWebSocketClient::isConnected 检测连接是否建立成功
 * @return
 */
bool CWebSocketClient::isConnected()
{
    return m_webSocket.state() == QAbstractSocket::ConnectedState ? true : false;
}

/**
 * @brief CWebSocketClient::Send 发送字符串
 * @param msg 要发送的字符串
 * @return 返回发送成功的字符串长度
 */
qint64 CWebSocketClient::Send(const QString& msg)
{
    if(msg.isEmpty())
        return 0;

    return m_webSocket.sendTextMessage(msg);
}

/**
 * @brief CWebSocketClient::SetNetworkFrameManager 设置网络消息处理框架
 * @param pNetworkFrameManager 要设置的消息处理框架
 */
void CWebSocketClient::SetNetworkFrameManager(NetworkFrameManager *pNetworkFrameManager)
{
    if(pNetworkFrameManager == nullptr)
        return;

    m_NetworkFrameManager = pNetworkFrameManager;
}

/**
 * @brief CWebSocketClient::sendFile 发送文件
 *
 * @param isExcludeUserInputEvents 是否要做防卡死，如果是在网络中处理就不用，在界面上处理要做防卡死处理，初始是要做防卡死处理的
 * @param filepath 要发送的文件的完整路径
 * @param rootpath 主路径，主要去除发送的文件路径，得到文件的相对路径
 * @return 如果文件发送成功返回真，否则返回假
 */
bool CWebSocketClient::sendFile(const QString& filepath,bool isExcludeUserInputEvents,QString rootpath)
{
    if(filepath.isEmpty())
        return false;

    QFileInfo pfileinfo(filepath);
    if(!pfileinfo.exists())
    {
        QLOG_ERROR()<<filepath<<" is exist.";
        return false;
    }

    QFile m_sendfile;
    m_sendfile.setFileName(filepath);

    if(!m_sendfile.open(QIODevice::ReadOnly))
    {
        QLOG_ERROR()<<filepath<<" open fail.";
        return false;
    }

    if(rootpath.isEmpty())
    {
        int pos = static_cast<int>(filepath.lastIndexOf("/"));
        if(pos > 0)
            rootpath = filepath.mid(0,pos+1);
    }

    qint64 m_sendsize,m_totalfilesize;                                  /**< 当前发送的文件的数据大小和文件总大小 */

    QByteArray tmpByteArray = qCompress(m_sendfile.readAll());

    m_sendfile.close();

    m_totalfilesize = tmpByteArray.size();
    m_sendsize=0;
    //quint16 pdecchecknum = qChecksum(tmpByteArray.constData(),tmpByteArray.size());
    quint16 pdecchecknum = qChecksum(QByteArrayView(tmpByteArray));

    QString tmpRealFilePath = filepath.mid(rootpath.size());

    tagFileStruct ptagFileStruct{};
    memset(&ptagFileStruct,0,sizeof(ptagFileStruct));

    ptagFileStruct.mark[0] = 'F';
    ptagFileStruct.mark[1] = 'I';
    ptagFileStruct.mark[2] = 'L';

    strncpy(ptagFileStruct.fileName,tmpRealFilePath.toUtf8().data(),MAX_FILENAME);
    ptagFileStruct.compressfileSize = tmpByteArray.size();
    ptagFileStruct.srcfileSize = pfileinfo.size();
    ptagFileStruct.checknum = pdecchecknum;

    QByteArray pHeardBytes = QByteArray((char*)&ptagFileStruct,sizeof(ptagFileStruct));

    if(m_webSocket.sendBinaryMessage(pHeardBytes) != pHeardBytes.size())
    {
        QLOG_ERROR()<<filepath<<" heard send fail.";
        return false;
    }

    do
    {       
        if(m_webSocket.state() != QAbstractSocket::ConnectedState)
        {
            QLOG_ERROR()<<"CWebSocketClient::sendFile send data fail:QAbstractSocket::ConnectedState.";
            return false;
        }

        QByteArray psendbytearray;

        if(tmpByteArray.size()-m_sendsize < BUF_SIZE)
            psendbytearray = tmpByteArray.mid(m_sendsize);
        else
            psendbytearray = tmpByteArray.mid(m_sendsize,BUF_SIZE);

        m_sendsize += m_webSocket.sendBinaryMessage(psendbytearray);

        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessSendFile(&m_webSocket,filepath,m_sendsize,m_totalfilesize);

        // 上传进度
        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(&m_webSocket,0,m_sendsize,m_totalfilesize);

        if(isExcludeUserInputEvents)
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    while(m_sendsize < m_totalfilesize);

    return true;
}

/**
 * @brief CWebSocketClient::sendBinaryMessage 发送二进制数据
 * @param data 要发送的二进制数据
 * @return 发送成功返回1，否则返回0
 */
qint64 CWebSocketClient::sendBinaryMessage(const QByteArray &data,bool isExcludeUserInputEvents)
{
    if(data.isEmpty())
        return 0;

    qint64 m_sendsize,m_totaldatasize;

    QByteArray tmpByteArray = qCompress(data);

    m_totaldatasize = tmpByteArray.size();
    m_sendsize=0;
    quint16 pdecchecknum = qChecksum(QByteArrayView(tmpByteArray));

    tagDataStruct ptagDataStruct{};
    memset(&ptagDataStruct,0,sizeof(ptagDataStruct));

    ptagDataStruct.mark[0] = 'D';
    ptagDataStruct.mark[1] = 'A';
    ptagDataStruct.mark[2] = 'T';

    ptagDataStruct.compressdataSize = tmpByteArray.size();
    ptagDataStruct.srcdataSize = m_totaldatasize;
    ptagDataStruct.checknum = pdecchecknum;

    QByteArray pHeardBytes = QByteArray((char*)&ptagDataStruct,sizeof(ptagDataStruct));

    if(m_webSocket.sendBinaryMessage(pHeardBytes) != pHeardBytes.size())
    {
        return 0;
    }

    do
    {
        if(m_webSocket.state() != QAbstractSocket::ConnectedState)
        {
            return 0;
        }

        QByteArray psendbytearray;

        if(tmpByteArray.size()-m_sendsize < BUF_SIZE)
            psendbytearray = tmpByteArray.mid(m_sendsize);
        else
            psendbytearray = tmpByteArray.mid(m_sendsize,BUF_SIZE);

        m_sendsize += m_webSocket.sendBinaryMessage(psendbytearray);

        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(&m_webSocket,0,m_sendsize,m_totaldatasize);

        // 上传进度
        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(&m_webSocket,0,m_sendsize,m_totaldatasize);

        if(isExcludeUserInputEvents)
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    while(m_sendsize < m_totaldatasize);

    return 1;
}

/**
 * @brief CWebSocketClient::Close 关闭连接
 */
void CWebSocketClient::Close(bool isClosed)
{
    if(isClosed)
    {
        disconnect(&m_webSocket, &QWebSocket::connected, this, &CWebSocketClient::onConnected);
        disconnect(&m_webSocket, &QWebSocket::disconnected, this, &CWebSocketClient::onDisconnected);
    }

    m_WebSocketHeartTimeOutTimer.stop();
    m_WebSocketReconnectTimer.stop();
    m_webSocket.close();
    m_isCloseSocket = isClosed;
}

/**
 * @brief CWebSocketClient::onConnected 连接成功后的处理
 */
void CWebSocketClient::onConnected()
{
    m_WebSocketHeartTimeOutTimer.start();
    m_WebSocketReconnectTimer.stop();

    QLOG_INFO()<<"CWebSocketClient::onConnected";

    emit signalsConnectedNetMes(&m_webSocket);

    if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessConnectedNetMes(&m_webSocket);
}

/**
 * @brief CWebSocketClient::onDisconnected 连接断开后的处理
 */
void CWebSocketClient::onDisconnected()
{
    m_WebSocketHeartTimeOutTimer.stop();
    if(!m_isCloseSocket)
        m_WebSocketReconnectTimer.start();

    QLOG_INFO()<<m_webSocket.state()<<" "<<m_webSocket.errorString()<<" "<<m_isCloseSocket;

    emit signalsDisconnectedNetMes(&m_webSocket);

    if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessDisconnectedNetMes(&m_webSocket);
}

/**
 * @brief CWebSocketClient::handleWebSocketReconnect 处理客户端断开后重连
 */
void CWebSocketClient::handleWebSocketReconnect()
{
    QLOG_INFO()<<"CWebSocketClient::handleWebSocketReconnect():"<<m_isDisreconnected<<" "<<m_websocketurl;

    if(m_isDisreconnected && m_websocketurl.isValid())
    {
        QLOG_INFO()<<"CWebSocketClient::handleWebSocketReconnect()";

        m_webSocket.open(m_websocketurl);
    }
}

/**
 * @brief CWebSocketClient::handleWebSocketHeartTimeOut 处理心跳消息
 */
void CWebSocketClient::handleWebSocketHeartTimeOut()
{
    m_webSocket.sendTextMessage("heart");
}

/**
 * @brief CWebSocketClient::onTextMessageReceived 处理接收到字符串时的情况
 * @param message 接收到的字符串
 */
void CWebSocketClient::onTextMessageReceived(const QString& message)
{
    QLOG_INFO()<<"CWebSocketClient::onTextMessageReceived:"<<message;

    emit signalsNetText(&m_webSocket,message);

    if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessNetText(&m_webSocket,message);
}

/**
 * @brief CWebSocketClient::onBinaryReceived 处理接收到二进制数据的情况
 * @param message 接收到的二进制数据
 */
void CWebSocketClient::onBinaryReceived(const QByteArray& message)
{
    // 是否处理接收文件
    if(m_processrecvFile)
    {
        onPrcessRecvFile(&m_webSocket,message);
    }
    else
    {
        // 处理二进制数据
        onPrcessRecvBinaryData(&m_webSocket,message);
    }
}

/// 是否处理接收文件
void CWebSocketClient::setIsProcessRecvFile(bool isProcess,QString recvfiledir)
{
    m_processrecvFile = isProcess;
    m_recvFileSaveDir = std::move(recvfiledir);

    if(m_processrecvFile) m_recvFileState=false;
}

/**
 * @brief CWebSocketClient::onPrcessRecvBinaryData 处理二进制数据接收
 * @param pwebsocket 要处理的客户端
 * @param data 要处理的数据
 */
void CWebSocketClient::onPrcessRecvBinaryData(QWebSocket *pwebsocket,const QByteArray &data)
{
    m_recvDataBytes.push_back(data);

    while(!m_recvDataBytes.isEmpty())
    {
        if(!m_recvDataState &&
            m_recvDataBytes.size() >= sizeof(tagDataStruct))
        {
            memcpy(&m_tagDataStruct,m_recvDataBytes.constData(),sizeof(m_tagDataStruct));

            if(m_tagDataStruct.mark[0] != 'D' ||
               m_tagDataStruct.mark[1] != 'A' ||
               m_tagDataStruct.mark[2] != 'T')
            {
                m_recvDataState = false;
                break;
            }

            // 下载进度
            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(pwebsocket,1,m_recvDataBytes.size(),m_tagDataStruct.compressdataSize);

            m_recvDataBytes.remove(0,sizeof(m_tagDataStruct));
            m_recvDataState = true;
        }

        // 下载进度
        if(m_NetworkFrameManager && m_recvDataBytes.size() > 0)
            m_NetworkFrameManager->OnProcessBinaryOperProcess(pwebsocket,1,m_recvDataBytes.size() < m_tagDataStruct.compressdataSize ? m_recvDataBytes.size() : m_tagDataStruct.compressdataSize,m_tagDataStruct.compressdataSize);

        if(!m_recvDataState || m_recvDataBytes.size() < m_tagDataStruct.compressdataSize)
            break;

        // 下载进度
        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(pwebsocket,1,m_tagDataStruct.compressdataSize,m_tagDataStruct.compressdataSize);

        // 得到当前数据
        QByteArray precvDataData = m_recvDataBytes.mid(0,m_tagDataStruct.compressdataSize);
        m_recvDataBytes.remove(0,m_tagDataStruct.compressdataSize);

        // 获取文件校验码
        quint16 pdecchecknum = qChecksum(QByteArrayView(precvDataData));

        if(m_tagDataStruct.checknum == pdecchecknum)
        {
            // 解压文件
            QByteArray precvDataBytes = qUncompress(precvDataData);

            emit signalsNetBinary(&m_webSocket,precvDataBytes);

            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessNetBinary(&m_webSocket,precvDataBytes);
        }

        qDebug()<<"CWebSocketClient::onPrcessRecvBinaryData:"<<m_recvDataBytes.size();
        m_recvDataState=false;
    }
}

/**
 * @brief onPrcessRecvFile 处理文件接收
 * @param pwebsocket 要处理的客户端
 * @param data 要处理的数据
 */
void CWebSocketClient::onPrcessRecvFile(QWebSocket *pwebsocket,const QByteArray &data)
{
    m_recvFileBytes.push_back(data);

    while(!m_recvFileBytes.isEmpty())
    {
        if(!m_recvFileState &&
            m_recvFileBytes.size() >= sizeof(tagFileStruct))
        {
            memcpy(&m_tagFileStruct,m_recvFileBytes.constData(),sizeof(m_tagFileStruct));

            if(m_tagFileStruct.mark[0] != 'F' ||
               m_tagFileStruct.mark[1] != 'I' ||
               m_tagFileStruct.mark[2] != 'L')
            {
                m_recvFileState = false;
                QLOG_ERROR()<<"CWebSocketClient::onPrcessRecvFile filestruct fail.";
                break;
            }

            QLOG_INFO()<<"CWebSocketClient::onPrcessRecvFile filestruct:"<<QString(m_tagFileStruct.fileName)<<" "<<m_tagFileStruct.compressfileSize<<" "<<m_tagFileStruct.checknum;
            //qDebug()<<"filestruct:"<<QString(m_tagFileStruct.fileName)<<" "<<m_tagFileStruct.compressfileSize<<" "<<m_tagFileStruct.checknum;

            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_tagFileStruct.fileName),"",STATE_START);

            // 下载进度
            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(&m_webSocket,1,m_recvFileBytes.size(),m_tagFileStruct.compressfileSize);

            m_recvFileBytes.remove(0,sizeof(m_tagFileStruct));
            m_recvFileState = true;
        }

        // 下载进度
        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(&m_webSocket,1,m_recvFileBytes.size() < m_tagFileStruct.compressfileSize ? m_recvFileBytes.size() : m_tagFileStruct.compressfileSize,m_tagFileStruct.compressfileSize);

        if(!m_recvFileState || m_recvFileBytes.size() < m_tagFileStruct.compressfileSize)
            break;

        // 下载进度
        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(&m_webSocket,1,m_tagFileStruct.compressfileSize,m_tagFileStruct.compressfileSize);

        // 得到当前文件数据
        QByteArray precvFileData = m_recvFileBytes.mid(0,m_tagFileStruct.compressfileSize);
        m_recvFileBytes.remove(0,m_tagFileStruct.compressfileSize);

        // 获取文件校验码
        quint16 pdecchecknum = qChecksum(QByteArrayView(precvFileData));

        // 解压文件
        QByteArray precvFileBytes = qUncompress(precvFileData);

        QLOG_INFO()<<"CWebSocketClient::onPrcessRecvFile recv:"<<QString(m_tagFileStruct.fileName)<<" "<<pdecchecknum<<" finished.";
        //qDebug()<<"recv:"<<QString(m_tagFileStruct.fileName)<<" "<<pdecchecknum<<" "<<precvFileData.size()<<" "<<m_tagFileStruct.compressfileSize<<" finished.";

        QString appfiledir = QCoreApplication::applicationDirPath() + QString("/tempData/tempData.tmp");

        if(m_tagFileStruct.checknum == pdecchecknum)
        {
            if(!m_recvFileSaveDir.isEmpty())
                appfiledir = m_recvFileSaveDir + QString("/") + QString(m_tagFileStruct.fileName);

            QString tmpFileDirPath = appfiledir.mid(0,appfiledir.lastIndexOf(tr("/")));

            QDir dir(tmpFileDirPath);
            if(!dir.exists())
            {
                if(!dir.mkpath(tmpFileDirPath))
                {
                    QLOG_ERROR()<<"CWebSocketClient::onPrcessRecvFile:"<<tmpFileDirPath<<" create fail.";
                    m_recvFileState=false;
                    continue;
                }
            }

            QFile precvFile(appfiledir);
            if(precvFile.open(QIODevice::WriteOnly))
            {
                qint64 tmprecvfilesize = precvFile.write(precvFileBytes);

                if(tmprecvfilesize != m_tagFileStruct.srcfileSize)
                {
                    if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_tagFileStruct.fileName),appfiledir,STATE_SIZE);

                    QLOG_ERROR()<<"CWebSocketClient::onPrcessRecvFile write file:"<<appfiledir<<" "<<tmprecvfilesize<<" "<<m_tagFileStruct.srcfileSize<<" "<<precvFileBytes.size()<<" error.";
                    precvFile.close();
                    m_recvFileState=false;
                    continue;
                }

                precvFile.close();

                if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_tagFileStruct.fileName),appfiledir,STATE_SUCCESS);
            }
            else
            {
                QLOG_ERROR()<<"CWebSocketClient::onPrcessRecvFile recv:"<<appfiledir<<" "<<pdecchecknum<<" "<<m_tagFileStruct.checknum<<" write error.";

                if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_tagFileStruct.fileName),appfiledir,STATE_NOTWRITE);
                m_recvFileState=false;
                continue;
            }
        }
        else
        {
            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_tagFileStruct.fileName),appfiledir,STATE_CHECKNUM);
            QLOG_ERROR()<<"CWebSocketClient::onPrcessRecvFile recv:"<<QString(m_tagFileStruct.fileName)<<" "<<pdecchecknum<<" "<<m_tagFileStruct.checknum<<" error.";
        }

        m_recvFileState=false;
    }
}

CWebSocketClientManager::~CWebSocketClientManager()
{
    clearAllClients();
}

/**
 * @brief CWebSocketClientManager::clearAllClients 清除所有的客户端
 */
void CWebSocketClientManager::clearAllClients()
{
    QHash<QString,CWebSocketClient*>::iterator iter = m_WebSocketClients.begin();
    for(;iter != m_WebSocketClients.end();++iter)
    {
        if((*iter)) delete (*iter);
        (*iter) = nullptr;
    }

    m_WebSocketClients.clear();
}

/**
 * @brief CWebSocketClientManager::addClient 添加一个客户端
 * @param clientName 要添加的客户端的名称
 * @param pClient 要添加的客户端
 * @return 如果客户端添加成功返回真，否则返回假
 */
bool CWebSocketClientManager::addClient(const QString& clientName,CWebSocketClient* pClient)
{
    if(clientName.isEmpty() || pClient == nullptr)
        return false;

    CWebSocketClient *pReturnClient = getClient(clientName);
    if(pReturnClient)
        return false;

    m_WebSocketClients[clientName] = pClient;

    return true;
}

/**
 * @brief CWebSocketClientManager::getClient 得到一个客户端
 * @param clientName 要得到的客户端的名称
 * @return 如果存在这个客户端返回这个客户端，否则返回nullptr
 */
CWebSocketClient* CWebSocketClientManager::getClient(const QString& clientName)
{
    QHash<QString,CWebSocketClient*>::iterator iter = m_WebSocketClients.find(clientName);
    if(iter != m_WebSocketClients.end())
        return (*iter);

    return nullptr;
}

/**
 * @brief CWebSocketClientManager::deleteClient 清除指定的客户端
 * @param clientName
 * @return
 */
bool CWebSocketClientManager::deleteClient(const QString& clientName)
{
    if(clientName.isEmpty() || m_WebSocketClients.isEmpty())
        return false;

    QHash<QString,CWebSocketClient*>::iterator iter = m_WebSocketClients.find(clientName);
    if(iter != m_WebSocketClients.end())
    {
        delete (*iter);
        (*iter) = nullptr;

        m_WebSocketClients.erase(iter);

        return true;
    }

    return false;
}

