#include "../../includes/network/cwebsocketserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include "../../includes/QsLog/QsLog.h"

#include <QTextCodec>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QDir>

QHash<QWebSocket*,tagWebClient> m_webClients;                          /**< 用于客户端文件和二进制数据处理 */

//initialiseSingleton(CWebSocketServer);

QT_USE_NAMESPACE

CWebSocketServer::CWebSocketServer(QObject *parent) :
    QObject(parent),
    m_NetworkFrameManager(NULL),
    m_processrecvFile(false),
    m_isautoDeleteSocket(true),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WebSocketServer"),
                                            QWebSocketServer::NonSecureMode, this))
{
    connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
            this, &CWebSocketServer::onNewConnection);
    connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &CWebSocketServer::closed);

    QObject::connect(&m_WebSocketHeartTimeOutTimer, &QTimer::timeout, this, &CWebSocketServer::handleWebSocketHeartTimeOut);
    m_WebSocketHeartTimeOutTimer.setInterval(1000);
}

CWebSocketServer::~CWebSocketServer()
{
    CloseServer();
}

/**
 * @brief CWebSocketServer::SetNetworkFrameManager 设置网络消息处理框架
 * @param pNetworkFrameManager 要设置的网络消息处理框架
 */
void CWebSocketServer::SetNetworkFrameManager(NetworkFrameManager *pNetworkFrameManager)
{
    if(pNetworkFrameManager == NULL)
        return;

    m_NetworkFrameManager = pNetworkFrameManager;
}

/**
 * 设置服务器最大支持连接数量,默认为30个连接
 *
 * @param maxcount 要设置的最大可接受连接数量 
 */
void CWebSocketServer::setMaxPendingConnections(int maxcount)
{
    if(m_pWebSocketServer == NULL)
        return;

    m_pWebSocketServer->setMaxPendingConnections(maxcount);
}

/**
 * @brief CWebSocketServer::SendAllFile 给所有客户端发送文件
 * @param pwebsocket
 * @param filepath
 * @param isExcludeUserInputEvents
 * @param rootpath
 * @return
 */
bool CWebSocketServer::SendAllFile(QString filepath,bool isExcludeUserInputEvents,QString rootpath)
{
    if(filepath.isEmpty() || m_clients.isEmpty())
        return false;

    QMutexLocker tmpLocker(&m_clientsMutex);

    QHash<QWebSocket *,int>::iterator iter = m_clients.begin();
    for(;iter != m_clients.end();++iter)
        sendFile(iter.key(),filepath,isExcludeUserInputEvents,rootpath);

    return true;
}

/**
 * @brief CWebSocketClient::sendFile 发送文件
 *
 * @param isExcludeUserInputEvents 是否要做防卡死，如果是在网络中处理就不用，在界面上处理要做防卡死处理，初始是要做防卡死处理的
 * @param filepath 要发送的文件的完整路径
 * @param rootpath 主路径，主要去除发送的文件路径，得到文件的相对路径
 * @return 如果文件发送成功返回真，否则返回假
 */
bool CWebSocketServer::sendFile(QWebSocket *pwebsocket,QString filepath,bool isExcludeUserInputEvents,QString rootpath)
{
    if(pwebsocket == NULL || filepath.isEmpty())
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
        int pos = filepath.lastIndexOf("/");
        if(pos > 0)
            rootpath = filepath.mid(0,pos+1);
    }

    qint64 m_sendsize,m_totalfilesize;                                  /**< 当前发送的文件的数据大小和文件总大小 */

    QByteArray tmpByteArray = qCompress(m_sendfile.readAll());

    m_sendfile.close();

    m_totalfilesize = tmpByteArray.size();
    m_sendsize=0;
    quint16 pdecchecknum = qChecksum(tmpByteArray.constData(),tmpByteArray.size());

    QString tmpRealFilePath = filepath.mid(rootpath.size());

    tagFileStruct ptagFileStruct;
    memset(&ptagFileStruct,0,sizeof(ptagFileStruct));

    ptagFileStruct.mark[0] = 'F';
    ptagFileStruct.mark[1] = 'I';
    ptagFileStruct.mark[2] = 'L';

    strncpy(ptagFileStruct.fileName,tmpRealFilePath.toUtf8().data(),MAX_FILENAME);
    ptagFileStruct.compressfileSize = tmpByteArray.size();
    ptagFileStruct.srcfileSize = pfileinfo.size();
    ptagFileStruct.checknum = pdecchecknum;

    QByteArray pHeardBytes((char*)&ptagFileStruct,sizeof(ptagFileStruct));

    if(pwebsocket->sendBinaryMessage(pHeardBytes) != pHeardBytes.size())
    {
        QLOG_ERROR()<<filepath<<" heard send fail.";
        return false;
    }

    do
    {
        if(pwebsocket->state() != QAbstractSocket::ConnectedState)
        {
            QLOG_ERROR()<<"CWebSocketServer::sendFile send data fail:QAbstractSocket::ConnectedState.";
            return false;
        }

        QByteArray psendbytearray;

        if(tmpByteArray.size()-m_sendsize < BUF_SIZE)
            psendbytearray = tmpByteArray.mid(m_sendsize);
        else
            psendbytearray = tmpByteArray.mid(m_sendsize,BUF_SIZE);

        m_sendsize += pwebsocket->sendBinaryMessage(psendbytearray);

        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessSendFile(pwebsocket,filepath,m_sendsize,m_totalfilesize);

        // 上传进度
        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(pwebsocket,0,m_sendsize,m_totalfilesize);

        if(isExcludeUserInputEvents)
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    while(m_sendsize < m_totalfilesize);

    return true;
}

/**
 * @brief CWebSocketServer::Send 给指定客户端发送字符数据
 * @param pwebsocket 要接收数据的客户端
 * @param msg 要发送的数据
 * @return 返回发送成功的数据大小
 */
qint64 CWebSocketServer::Send(QWebSocket *pwebsocket,QString msg)
{
    if(pwebsocket == NULL ||
            msg.isEmpty() ||
            !pwebsocket->isValid())
        return -1;

    QMutexLocker pmutexlocker(&m_clientsMutex);     

    QHash<QWebSocket*,tagWebClient>::iterator iter2 = m_webClients.find(pwebsocket);
    if(iter2 == m_webClients.end())
        return -1;

    return pwebsocket->sendTextMessage(msg);
}

/**
 * @brief CWebSocketServer::SendByteArray 给指定客户端发送二进制数据
 * @param pwebsocket 要接收数据的客户端
 * @param data 要发送的数据
 * @return 返回发送成功的数据大小
 */
qint64 CWebSocketServer::SendByteArray(QWebSocket *pwebsocket,QByteArray &data,bool isExcludeUserInputEvents)
{
    if(pwebsocket == NULL ||
            !pwebsocket->isValid() ||
            data.isEmpty())
        return -1;

    QMutexLocker pmutexlocker(&m_clientsMutex);

    QHash<QWebSocket*,tagWebClient>::iterator iter2 = m_webClients.find(pwebsocket);
    if(iter2 == m_webClients.end())
        return -1;        

    qint64 m_sendsize,m_totaldatasize;

    QByteArray tmpByteArray = qCompress(data);

    m_totaldatasize = tmpByteArray.size();
    m_sendsize=0;
    quint16 pdecchecknum = qChecksum(tmpByteArray.constData(),tmpByteArray.size());

    tagDataStruct ptagDataStruct;
    memset(&ptagDataStruct,0,sizeof(ptagDataStruct));

    ptagDataStruct.mark[0] = 'D';
    ptagDataStruct.mark[1] = 'A';
    ptagDataStruct.mark[2] = 'T';

    ptagDataStruct.compressdataSize = tmpByteArray.size();
    ptagDataStruct.srcdataSize = m_totaldatasize;
    ptagDataStruct.checknum = pdecchecknum;

    QByteArray pHeardBytes = QByteArray((char*)&ptagDataStruct,sizeof(ptagDataStruct));

    if(pwebsocket->sendBinaryMessage(pHeardBytes) != pHeardBytes.size())
    {
        return -1;
    }

    do
    {
        if(pwebsocket->state() != QAbstractSocket::ConnectedState)
        {
            return -1;
        }

        QByteArray psendbytearray;

        if(tmpByteArray.size()-m_sendsize < BUF_SIZE)
            psendbytearray = tmpByteArray.mid(m_sendsize);
        else
            psendbytearray = tmpByteArray.mid(m_sendsize,BUF_SIZE);

        m_sendsize += pwebsocket->sendBinaryMessage(psendbytearray);

        if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessBinaryOperProcess(pwebsocket,0,m_sendsize,m_totaldatasize);

        if(isExcludeUserInputEvents)
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    while(m_sendsize < m_totaldatasize);

    qDebug()<<"CWebSocketServer::SendByteArray:"<<ptagDataStruct.compressdataSize<<" "<<ptagDataStruct.checknum;

    return 1;
}

/**
 * @brief CWebSocketServer::SendAll 给所有客户端发送字符数据
 * @param msg 要发送的字符串数据
 * @return 如果所有客户端发送成功返回真，否则返回假
 */
bool CWebSocketServer::SendAll(QString msg)
{
    if(msg.isEmpty() || m_clients.isEmpty())
        return false;

    bool pState = true;

    QMutexLocker pmutexlocker(&m_clientsMutex);

    QHash<QWebSocket *,int>::iterator iter = m_clients.begin();
    for(;iter != m_clients.end();++iter)
    {
        if(iter.key() == NULL ||
                !iter.key()->isValid() ||
                iter.key()->state() != QAbstractSocket::ConnectedState)
            continue;

        qint64 len = iter.key()->sendTextMessage(msg);

        if(len < 0)
        {
            pState = false;

            QHostAddress phostaddress = QHostAddress(iter.key()->peerAddress().toIPv4Address());
            QLOG_ERROR()<<(phostaddress.toString() + "send size:" + QString::asprintf("%ld",len) + " ");
        }
    }

    return pState;
}

/**
 * @brief CWebSocketServer::SendAllOther 给除了指定客户端的其它客户端发送字符数据
 * @param pwebsocket 要排除的客户端
 * @param msg 要发送的数据
 * @return 如果所有客户端发送成功返回真，否则返回假
 */
bool CWebSocketServer::SendAllOther(QWebSocket *pwebsocket,QString msg)
{
    if(pwebsocket == NULL || msg.isEmpty())
        return false;

    bool pState = true;

    QMutexLocker pmutexlocker(&m_clientsMutex);

    QHash<QWebSocket *,int>::iterator iter = m_clients.begin();
    for(;iter != m_clients.end();++iter)
    {
        if(iter.key() == NULL ||
                !iter.key()->isValid() ||
                iter.key()->state() != QAbstractSocket::ConnectedState || iter.key() == pwebsocket)
            continue;

        qint64 len = iter.key()->sendTextMessage(msg);

        if(len < 0)
        {
            pState = false;

            QHostAddress phostaddress = QHostAddress(iter.key()->peerAddress().toIPv4Address());
            QLOG_ERROR()<<(phostaddress.toString() + "send size:" + QString::asprintf("%ld",len) + " ");
        }
    }

    return pState;
}

/**
 * @brief CWebSocketServer::getHostAddress 得到服务器IP
 * @return
 */
QHostAddress CWebSocketServer::getHostAddress(void)
{
    return m_pWebSocketServer ? m_pWebSocketServer->serverAddress() : QHostAddress();
}

/**
 * @brief CWebSocketServer::SendAllByteArray 给所有客户端发送二进制数据
 * @param data 要发送的数据
 * @return 如果所有客户端发送成功返回真，否则返回假
 */
bool CWebSocketServer::SendAllByteArray(QByteArray &data)
{
    if(data.isEmpty() || m_clients.isEmpty())
        return false;

    bool pState = true;

    QMutexLocker pmutexlocker(&m_clientsMutex);

    QHash<QWebSocket *,int>::iterator iter = m_clients.begin();
    for(;iter != m_clients.end();++iter)
    {
        if(iter.key() == NULL ||
                !iter.key()->isValid() ||
                iter.key()->state() != QAbstractSocket::ConnectedState)
            continue;

        qint64 len = SendByteArray(iter.key(),data);

        if(len <= 0)
        {
            pState = false;

            QHostAddress phostaddress = QHostAddress(iter.key()->peerAddress().toIPv4Address());
            QLOG_ERROR()<<(phostaddress.toString() + "send size:" + QString::asprintf("%ld",len) + " ");
        }
    }

    return pState;
}

/**
 * @brief CWebSocketServer::SendAllOtherByteArray 给除了指定客户端的其它客户端发送二进制数据
 * @param pwebsocket 要排除的客户端
 * @param data 要发送的二进制数据
 * @return 如果所有客户端发送成功返回真，否则返回假
 */
bool CWebSocketServer::SendAllOtherByteArray(QWebSocket *pwebsocket,QByteArray &data)
{
    if(pwebsocket == NULL || data.isEmpty())
        return false;

    bool pState = true;

    QMutexLocker pmutexlocker(&m_clientsMutex);

    QHash<QWebSocket *,int>::iterator iter = m_clients.begin();
    for(;iter != m_clients.end();++iter)
    {
        if(iter.key() == NULL ||
                !iter.key()->isValid() ||
                iter.key()->state() != QAbstractSocket::ConnectedState ||
                iter.key() == pwebsocket)
            continue;

        qint64 len = SendByteArray(iter.key(),data);

        if(len <= 0)
        {
            pState = false;

            QHostAddress phostaddress = QHostAddress(iter.key()->peerAddress().toIPv4Address());
            QLOG_ERROR()<<(phostaddress.toString() + "send size:" + QString::asprintf("%ld",len) + " ");
        }
    }

    return pState;
}

/**
 * @brief CWebSocketServer::isListening 服务器是否启动
 * @return
 */
bool CWebSocketServer::isListening(void)
{
    if(m_pWebSocketServer == NULL)
        return false;

    return m_pWebSocketServer->isListening();
}

/**
 * @brief CWebSocketServer::OpenServer 打开一个指定端口的服务器
 * @param port 要连接的服务器地址
 */
bool CWebSocketServer::OpenServer(int port)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        QLOG_INFO()<<"CWebSocketServer::OpenServer:"<<port;
        qDebug()<<"CWebSocketServer::OpenServer:"<<port;

        m_WebSocketHeartTimeOutTimer.start();

        return true;
    }

    return false;
}

/**
 * @brief getServerPort 得到服务器打开的端口
 * @return
 */
quint16 CWebSocketServer::getServerPort(void)
{
    if(m_pWebSocketServer == NULL)
        return 0;

    return m_pWebSocketServer->serverPort();
}

/**
 * @brief CWebSocketServer::closeClient 关闭指定的客户端
 * @param pwebsocket 要关闭的客户端
 */
void CWebSocketServer::closeClient(QWebSocket *pwebsocket)
{
    if(pwebsocket == NULL)
        return;

    pwebsocket->close();

    QMutexLocker pmutexlocker(&m_clientsMutex);

    QHash<QWebSocket*,tagWebClient>::iterator iterTwo = m_webClients.find(pwebsocket);
    if(iterTwo != m_webClients.end())
    {
        m_webClients.erase(iterTwo);
    }

    QHash<QWebSocket *,int>::iterator iter = m_clients.find(pwebsocket);
    if(iter != m_clients.end())
    {
        iter.key()->deleteLater();
        m_clients.erase(iter);
    }

    QHash<QString,QWebSocket*>::iterator iterName = m_clientNames.find(pwebsocket->objectName());
    if(iterName != m_clientNames.end())
    {
        m_clientNames.erase(iterName);
    }
}

/**
 * @brief CWebSocketServer::CloseServer 关闭服务器
 */
void CWebSocketServer::CloseServer(void)
{
    m_WebSocketHeartTimeOutTimer.stop();
    if(m_pWebSocketServer) m_pWebSocketServer->close();

    QMutexLocker pmutexlocker(&m_clientsMutex);

    if(!m_clients.isEmpty())
    {
        QHash<QWebSocket *,int>::iterator iter = m_clients.begin();
        for(;iter != m_clients.end();++iter)
            if(iter.key()) iter.key()->deleteLater();

        m_clients.clear();
        m_webClients.clear();
        m_clientNames.clear();
        m_clientVariants.clear();
    }
}

/**
 * @brief CWebSocketServer::handleWebSocketHeartTimeOut 处理客户端心跳消息，如果客户端心跳超时就关闭相应客户端
 */
void CWebSocketServer::handleWebSocketHeartTimeOut()
{
    if(m_clients.isEmpty())
        return;

    QMutexLocker pmutexlocker(&m_clientsMutex);

    QHash<QWebSocket *,int>::iterator iter = m_clients.begin();
    for(;iter != m_clients.end();)
    {
        (*iter) += 1;

        if((*iter) > 10)
        {
            QWebSocket *pWebSocket = iter.key();

            // 从列表中清除这个客户端，免得它又去执行断开连接
            iter = m_clients.erase(iter);

            if(pWebSocket)
            {
                QHash<QWebSocket*,tagWebClient>::iterator iter2 = m_webClients.find(pWebSocket);
                if(iter2 != m_webClients.end()) m_webClients.erase(iter2);

                QHash<QString,QWebSocket*>::iterator iterName = m_clientNames.find(pWebSocket->objectName());
                if(iterName != m_clientNames.end()) m_clientNames.erase(iterName);

                QLOG_INFO()<<"CWebSocketServer::handleWebSocketHeartTimeOut():"
                             <<pWebSocket->peerAddress().toString()<<":"<<pWebSocket->peerPort();

                if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessDisconnectedNetMes(pWebSocket);

                // signalsDisconnectedNetMes 不可靠，可能会出现发射消息，但消息晚执行的情况出现，
                // 如果需要在外部操作socket,设置m_isautoDeleteSocket为false
                // 然后在外部使用完socket后，自行删除
                emit signalsDisconnectedNetMes(pWebSocket);

                // 删除客户端的关联数据
                deleteClientVariant(pWebSocket);

                if(m_isautoDeleteSocket) pWebSocket->deleteLater();
            }
        }
        else
        {
            ++iter;
        }
    }
}

/**
 * @brief CWebSocketServer::onNewConnection 处理新的连接到达
 */
void CWebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &CWebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &CWebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &CWebSocketServer::socketDisconnected);

    pSocket->setObjectName(pSocket->peerAddress().toString()+QString::asprintf("_%d",pSocket->peerPort()));

    m_clientsMutex.lock();

    m_clients[pSocket]=0;
    m_webClients[pSocket].reset();
    m_clientNames[pSocket->objectName()] = pSocket;

    m_clientsMutex.unlock();

    QLOG_INFO()<<"CWebSocketServer::onNewConnection():"
              <<pSocket->peerAddress().toString()<<":"<<pSocket->peerPort();

    emit signalsConnectedNetMes(pSocket);

    if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessConnectedNetMes(pSocket);
}

/**
 * @brief CWebSocketServer::processTextMessage 处理新的消息到达
 * @param message 到达的消息
 */
void CWebSocketServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if(message == tr("heart"))
    {
        m_clientsMutex.lock();

        QHash<QWebSocket *,int>::iterator iter = m_clients.find(pClient);
        if(iter != m_clients.end())
            (*iter) = 0;

        m_clientsMutex.unlock();

        return;
    }

    emit signalsNetText(pClient,message);

    if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessNetText(pClient,message);
}

/**
 * @brief CWebSocketServer::processBinaryMessage 处理新的二进制数据达到
 * @param message 到达的二进制数据
 */
void CWebSocketServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    // 是否处理接收文件
    if(m_processrecvFile)
    {
        onPrcessRecvFile(pClient,message);
    }
    else
    {
        // 处理二进制数据
        onPrcessRecvBinaryData(pClient,message);
    }
}

/**
 * @brief CWebSocketServer::socketDisconnected 某个客户端断开的消息处理
 */
void CWebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    QMutexLocker pmutexlocker(&m_clientsMutex);

    if (pClient) {
        QHash<QWebSocket*,tagWebClient>::iterator iter2 = m_webClients.find(pClient);
        if(iter2 != m_webClients.end()) m_webClients.erase(iter2);

        QHash<QString,QWebSocket*>::iterator iterName = m_clientNames.find(pClient->objectName());
        if(iterName != m_clientNames.end()) m_clientNames.erase(iterName);

        QHash<QWebSocket *,int>::iterator iter = m_clients.find(pClient);
        if(iter != m_clients.end())
        {
            m_clients.erase(iter);

            QLOG_INFO()<<"CWebSocketServer::socketDisconnected:"
                       <<pClient->peerAddress().toString()<<":"<<pClient->peerPort();

            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessDisconnectedNetMes(pClient);

            // signalsDisconnectedNetMes 不可靠，可能会出现发射消息，但消息晚执行的情况出现，
            // 如果需要在外部操作socket,设置m_isautoDeleteSocket为false
            // 然后在外部使用完socket后，自行删除
            emit signalsDisconnectedNetMes(pClient);

            // 删除客户端的关联数据
            deleteClientVariant(pClient);
        }    

        if(m_isautoDeleteSocket) pClient->deleteLater();
    }
}

/**
 * @brief CWebSocketServer::setIsProcessRecvFile 是否处理接收文件
 * @param isProcess 设置true表示处理接收文件，否则不处理
 * @param recvfiledir 接收文件保存路径
 */
void CWebSocketServer::setIsProcessRecvFile(bool isProcess,QString recvfiledir)
{
    m_processrecvFile = isProcess;
    m_recvFileSaveDir = recvfiledir;
}

/**
 * @brief CWebSocketServer::getAllClients 得到当前所有在线的客户端
 *
 * @return 返回所有在线的客户端
 */
QList<QWebSocket*> CWebSocketServer::getAllClients(void)
{
    return m_clients.keys();
}

/**
 * @brief CWebSocketServer::getClient 根据名称得到指定的客户端
 * @param objName 对象名称
 * @return
 */
QWebSocket* CWebSocketServer::getClient(QString objName)
{
    if(objName.isEmpty() || m_clientNames.isEmpty())
        return NULL;

    QHash<QString,QWebSocket*>::iterator iter = m_clientNames.find(objName);
    if(iter == m_clientNames.end())
        return NULL;

    return iter.value();
}

/**
 * @brief onPrcessRecvFile 处理文件接收
 * @param pwebsocket 要处理的客户端
 * @param data 要处理的数据
 */
void CWebSocketServer::onPrcessRecvFile(QWebSocket *pwebsocket,const QByteArray &data)
{
    m_webClients[pwebsocket].m_recvFileBytes.push_back(data);

    while(!m_webClients[pwebsocket].m_recvFileBytes.isEmpty())
    {
        if(!m_webClients[pwebsocket].m_recvFileState &&
            m_webClients[pwebsocket].m_recvFileBytes.size() >= sizeof(tagFileStruct))
        {
            memcpy(&m_webClients[pwebsocket].m_tagFileStruct,m_webClients[pwebsocket].m_recvFileBytes.constData(),sizeof(m_webClients[pwebsocket].m_tagFileStruct));

            if(m_webClients[pwebsocket].m_tagFileStruct.mark[0] != 'F' ||
               m_webClients[pwebsocket].m_tagFileStruct.mark[1] != 'I' ||
               m_webClients[pwebsocket].m_tagFileStruct.mark[2] != 'L')
            {
                m_webClients[pwebsocket].m_recvFileState = false;
                closeClient(pwebsocket);
                QLOG_ERROR()<<"CWebSocketServer::onPrcessRecvFile filestruct fail.";
                break;
            }

            QLOG_INFO()<<"CWebSocketServer::onPrcessRecvFile filestruct:"<<QString(m_webClients[pwebsocket].m_tagFileStruct.fileName)<<" "<<m_webClients[pwebsocket].m_tagFileStruct.compressfileSize<<" "<<m_webClients[pwebsocket].m_tagFileStruct.checknum;
            //qDebug()<<"filestruct:"<<QString(m_tagFileStruct.fileName)<<" "<<m_tagFileStruct.compressfileSize<<" "<<m_tagFileStruct.checknum;

            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_webClients[pwebsocket].m_tagFileStruct.fileName),"",STATE_START);

            m_webClients[pwebsocket].m_recvFileBytes.remove(0,sizeof(m_webClients[pwebsocket].m_tagFileStruct));
            m_webClients[pwebsocket].m_recvFileState = true;
        }

        if(!m_webClients[pwebsocket].m_recvFileState || m_webClients[pwebsocket].m_recvFileBytes.size() < m_webClients[pwebsocket].m_tagFileStruct.compressfileSize)
            break;

        // 得到当前文件数据
        QByteArray precvFileData = m_webClients[pwebsocket].m_recvFileBytes.mid(0,m_webClients[pwebsocket].m_tagFileStruct.compressfileSize);
        m_webClients[pwebsocket].m_recvFileBytes.remove(0,m_webClients[pwebsocket].m_tagFileStruct.compressfileSize);

        // 获取文件校验码
        quint16 pdecchecknum = qChecksum(precvFileData.constData(),precvFileData.size());

        // 解压文件
        QByteArray precvFileBytes = qUncompress(precvFileData);

        QLOG_INFO()<<"CWebSocketServer::onPrcessRecvFile:"<<QString(m_webClients[pwebsocket].m_tagFileStruct.fileName)<<" "<<pdecchecknum<<" finished.";
        //qDebug()<<"recv:"<<QString(m_tagFileStruct.fileName)<<" "<<pdecchecknum<<" "<<m_tagFileStruct.checknum<<" "<<precvFileData.size()<<" "<<m_tagFileStruct.compressfileSize<<" finished.";

        QString appfiledir = QCoreApplication::applicationDirPath() + QString("/tempData/tempData.tmp");

        if(m_webClients[pwebsocket].m_tagFileStruct.checknum == pdecchecknum)
        {
            if(!m_recvFileSaveDir.isEmpty())
                appfiledir = m_recvFileSaveDir + QString("/") + QString(m_webClients[pwebsocket].m_tagFileStruct.fileName);

            QString tmpFileDirPath = appfiledir.mid(0,appfiledir.lastIndexOf(tr("/")));

            QDir dir(tmpFileDirPath);
            if(!dir.exists())
            {
                if(!dir.mkpath(tmpFileDirPath))
                {
                    QLOG_ERROR()<<"CWebSocketServer::onPrcessRecvFile:"<<tmpFileDirPath<<" create fail.";
                    m_webClients[pwebsocket].m_recvFileState=false;
                    continue;
                }
            }

            QFile precvFile(appfiledir);
            if(precvFile.open(QIODevice::WriteOnly))
            {
                qint64 tmprecvfilesize = precvFile.write(precvFileBytes);

                if(tmprecvfilesize != m_webClients[pwebsocket].m_tagFileStruct.srcfileSize)
                {
                    if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_webClients[pwebsocket].m_tagFileStruct.fileName),appfiledir,STATE_SIZE);

                    QLOG_ERROR()<<"CWebSocketServer::onPrcessRecvFile write file:"<<appfiledir<<" "<<tmprecvfilesize<<" "<<m_webClients[pwebsocket].m_tagFileStruct.srcfileSize<<" "<<precvFileBytes.size()<<" error.";
                    precvFile.close();
                    m_webClients[pwebsocket].m_recvFileState=false;
                    continue;
                }

                precvFile.close();

                if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_webClients[pwebsocket].m_tagFileStruct.fileName),appfiledir,STATE_SUCCESS);
            }
            else
            {
                if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_webClients[pwebsocket].m_tagFileStruct.fileName),appfiledir,STATE_NOTWRITE);
                m_webClients[pwebsocket].m_recvFileState=false;
                continue;
            }
        }
        else
        {
            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessRecvFile(pwebsocket,QString(m_webClients[pwebsocket].m_tagFileStruct.fileName),appfiledir,STATE_CHECKNUM);
            QLOG_ERROR()<<"CWebSocketServer::onPrcessRecvFile recv:"<<QString(m_webClients[pwebsocket].m_tagFileStruct.fileName)<<" "<<pdecchecknum<<" "<<m_webClients[pwebsocket].m_tagFileStruct.checknum<<" error.";
        }

        m_webClients[pwebsocket].m_recvFileState=false;
    }
}

/**
 * @brief CWebSocketServer::onPrcessRecvBinaryData 处理二进制数据接收
 * @param pwebsocket 要处理的客户端
 * @param data 要处理的数据
 */
void CWebSocketServer::onPrcessRecvBinaryData(QWebSocket *pwebsocket,const QByteArray &data)
{
    m_webClients[pwebsocket].m_recvDataBytes.push_back(data);

    while(!m_webClients[pwebsocket].m_recvDataBytes.isEmpty())
    {
        if(!m_webClients[pwebsocket].m_recvDataState &&
            m_webClients[pwebsocket].m_recvDataBytes.size() >= sizeof(tagDataStruct))
        {
            memcpy(&m_webClients[pwebsocket].m_tagDataStruct,m_webClients[pwebsocket].m_recvDataBytes.constData(),sizeof(m_webClients[pwebsocket].m_tagDataStruct));

            if(m_webClients[pwebsocket].m_tagDataStruct.mark[0] != 'D' ||
               m_webClients[pwebsocket].m_tagDataStruct.mark[1] != 'A' ||
               m_webClients[pwebsocket].m_tagDataStruct.mark[2] != 'T')
            {
                m_webClients[pwebsocket].m_recvDataState = false;
                closeClient(pwebsocket);
                break;
            }

            m_webClients[pwebsocket].m_recvDataBytes.remove(0,sizeof(m_webClients[pwebsocket].m_tagDataStruct));
            m_webClients[pwebsocket].m_recvDataState = true;
        }

        if(!m_webClients[pwebsocket].m_recvDataState || m_webClients[pwebsocket].m_recvDataBytes.size() < m_webClients[pwebsocket].m_tagDataStruct.compressdataSize)
            break;

        // 得到当前数据
        QByteArray precvDataData = m_webClients[pwebsocket].m_recvDataBytes.mid(0,m_webClients[pwebsocket].m_tagDataStruct.compressdataSize);
        m_webClients[pwebsocket].m_recvDataBytes.remove(0,m_webClients[pwebsocket].m_tagDataStruct.compressdataSize);

        // 获取文件校验码
        quint16 pdecchecknum = qChecksum(precvDataData.constData(),precvDataData.size());

        if(m_webClients[pwebsocket].m_tagDataStruct.checknum == pdecchecknum)
        {
            // 解压文件
            QByteArray precvDataBytes = qUncompress(precvDataData);

            emit signalsNetBinary(pwebsocket,precvDataBytes);

            if(m_NetworkFrameManager) m_NetworkFrameManager->OnProcessNetBinary(pwebsocket,precvDataBytes);
        }

        m_webClients[pwebsocket].m_recvDataState=false;
    }
}

/**
 * @brief CWebSocketServer::setClientVariant 设置指定客户端的关联数据
 * @param variant
 * @return
 */
bool CWebSocketServer::setClientVariant(QWebSocket *client,QVariant variant)
{
    if(client == NULL || variant.isNull())
        return false;

    //QHash<QWebSocket *,QVariant>::iterator iter = m_clientVariants.find(client);
    //if(iter != m_clientVariants.end())
    //    return false;

    m_clientVariants[client] = variant;

    return true;
}

/**
 * @brief CWebSocketServer::getClientVariant 得到指定客户端的关联数据
 * @return
 */
QVariant CWebSocketServer::getClientVariant(QWebSocket *client)
{
    if(client == NULL || m_clientVariants.isEmpty())
        return QVariant();

    QHash<QWebSocket *,QVariant>::iterator iter = m_clientVariants.find(client);
    if(iter == m_clientVariants.end())
        return QVariant();

    return iter.value();
}

/**
 * @brief CWebSocketServer::deleteClientVariant 清除指定客户端的管理数据
 * @param client
 * @return
 */
bool CWebSocketServer::deleteClientVariant(QWebSocket *client)
{
    if(client == NULL || m_clientVariants.isEmpty())
        return false;

    QHash<QWebSocket *,QVariant>::iterator iter = m_clientVariants.find(client);
    if(iter == m_clientVariants.end())
        return false;

    m_clientVariants.erase(iter);

    return true;
}
