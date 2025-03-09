#include "ctcpsocketserver.h"
#include "QsLog.h"

#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

//initialiseSingleton(CTcpServer);

ThreadHandle::ThreadHandle()
{
    initfist = false;
}

ThreadHandle::~ThreadHandle() //停止所有线程，并释放资源
{
    QThread * tmp;
    for (auto it = threadSize.begin(); it != threadSize.end(); ++it)
    {
        tmp = it.key();
        tmp->exit();
        tmp->wait();
        delete tmp;
    }
}

ThreadHandle & ThreadHandle::getClass()
{
    static ThreadHandle th;
    return th;
}

QThread *ThreadHandle::getThread()
{
    if (!initfist)
    {
        initThreadType(THREADSIZE,10);
    }
    if (type == THREADSIZE)
        return findThreadSize();
    else
        return findHandleSize();
}

void ThreadHandle::removeThread(QThread * thread)
{
    auto t = threadSize.find(thread);
    if (t != threadSize.end())
    {
        t.value() --;
        if (type == THREADSIZE && t.value() == 0 && threadSize.size() > 1)
        {
            threadSize.remove(thread);
            thread->exit();
            thread->wait();
            delete thread;
        }
    }
}

void ThreadHandle::initThreadType(ThreadType type, unsigned int max)
{
    if (!initfist)
    {
        this->type = type;
        this->size = max;
        if (this->size == 0)
        {
            if(type == THREADSIZE)
                this->size = 10;
            else
                this->size = 1000;
        }

        if (type == THREADSIZE)
            initThreadSize();
        else
        {
            QThread * tmp = new QThread;
            threadSize.insert(tmp,0);
            tmp->start();
        }
    }
    initfist = true;
}

void ThreadHandle::initThreadSize() //建立好线程并启动，
{
    QThread * tmp;
    for (unsigned int i = 0; i < size;++i)
    {
        tmp = new QThread;
        threadSize.insert(tmp,0);
        tmp->start();
    }
}

QThread * ThreadHandle::findHandleSize() //查找到线程里的连接数小于最大值就返回查找到的，找不到就新建一个线程
{
    for (auto it  = threadSize.begin();it != threadSize.end() ;++it)
    {
        if (it.value() < size)
        {
            it.value() ++;
            return it.key();
        }
    }
    QThread * tmp = new QThread;
    threadSize.insert(tmp,1);
    tmp->start();
    return tmp;
}

QThread *ThreadHandle::findThreadSize() //遍历查找所有线程中连接数最小的那个，返回
{
    auto it = threadSize.begin();
    auto ite = threadSize.begin();
    for (++it ; it != threadSize.end(); ++it)
    {
        if (it.value() < ite.value())
        {
            ite = it;
        }
    }
    ite.value() ++;
    return ite.key();
}

/**
 * @brief ThreadHandle::clear 清除所有的线程
 */
void ThreadHandle::clear()
{
    for (auto it  = threadSize.begin();it != threadSize.end() ;++it)
    {
        it.value()  = 0;

        removeThread(it.key());
    }

    threadSize.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CTcpServer::CTcpServer(NetworkFrameManager *pNetworkFrameManager,
                       int numConnections,
                       bool isencoded,
                       QObject *parent)
    : QTcpServer(parent),
      m_NetworkFrameManager(pNetworkFrameManager),
      m_isUsingThread(false),
      m_isencoded(isencoded)
{
    setMaxPendingConnections(numConnections);
    nConnectCount = 0;

    connect(this, &QTcpServer::acceptError, this, &CTcpServer::displayError);
}

CTcpServer::~CTcpServer()
{
    CloseServer();
}

/**
 * @brief CTcpServer::OpenServer 打开一个指定端口的服务器
 * @param port 端口号
 */
bool CTcpServer::OpenServer(int port)
{
    if(this->isListening() || port <= 0)
        return false;

    if (!this->listen(QHostAddress::Any, port))
    {
        QLOG_INFO()<<"CTcpServer::OpenServer:"<<port<<" fail.";
        qDebug()<<"CTcpServer::OpenServer:"<<port<<" fail.";
        return false;
    }

    qDebug()<<"CTcpServer::OpenServer:"<<port;
    return true;
}

void CTcpServer::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "CTcpServer::displayError:" << socketError;
}

/**
 * @brief CloseServer 关闭服务器
 */
void CTcpServer::CloseServer()
{
    clear();

    this->close();
}

/**
 * @brief SendByteArray 给指定客户端发送二进制数据
 * @param socketDescriptor
 * @param data
 * @return
 */
qint64 CTcpServer::SendByteArray(qintptr socketDescriptor, const QByteArray &data)
{
    QHash<qint64, CTcpSocket *>::iterator iter = tcpClient.find(socketDescriptor);
    if(iter == tcpClient.end())
        return -1;

    return iter.value()->send(data);
}

/**
 * @brief CTcpServer::SendAllByteArray 给所有客户端发送二进制数据
 * @param data
 * @return
 */
bool CTcpServer::SendAllByteArray(const QByteArray& data)
{
    QHash<qint64, CTcpSocket *>::iterator iter = tcpClient.begin();
    for(;iter != tcpClient.end();++iter)
    {
        iter.value()->send(data);
    }

    return true;
}

/**
 * @brief CTcpServer::SendAllOtherByteArray 给除了指定客户端的其它客户端发送二进制数据
 * @param socketDescriptor
 * @param data
 * @return
 */
bool CTcpServer::SendAllOtherByteArray(qintptr socketDescriptor,const QByteArray& data)
{
    QHash<qint64, CTcpSocket *>::iterator iter = tcpClient.begin();
    for(;iter != tcpClient.end();++iter)
    {
        if(iter.key() == socketDescriptor)
            continue;

        iter.value()->send(data);
    }

    return true;
}

void CTcpServer::setMaxPendingConnections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections);//调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->maxConnections = numConnections;
}

void CTcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    if (tcpClient.size() > maxConnections)//继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }

    QThread* th = nullptr;

    if(m_isUsingThread)
        th = ThreadHandle::getClass().getThread();

    CTcpSocket* tcpTemp = new CTcpSocket(m_NetworkFrameManager,
                                         nullptr,
                                         false,
                                         socketDescriptor,
                                         true,
                                         m_isencoded);
    QString ip =  tcpTemp->peerAddress().toString();
    quint16 port = tcpTemp->peerPort();

    if(m_isUsingThread && th)
    {
        tcpTemp->setUsingThread(th);
        tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    }

    //NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    qRegisterMetaType<QThread*>();
    connect(tcpTemp,&CTcpSocket::sockDisConnect,
            this,&CTcpServer::sockDisConnectSlot,
            Qt::QueuedConnection);
    connect(tcpTemp,&CTcpSocket::signalsNetBinary,
            this,&CTcpServer::sockBinMessageSlot,
            Qt::QueuedConnection);
    connect(this, &CTcpServer::sentDisConnect,
            tcpTemp, &CTcpSocket::disConTcp,
            Qt::QueuedConnection);//断开信号
    connect(this,&CTcpServer::connectClient,
            tcpTemp, &CTcpSocket::newconnectClient,
            Qt::QueuedConnection); // 客户端新连接信号

    tcpClient.insert(socketDescriptor,tcpTemp);//插入到连接信息中
    emit connectClient(socketDescriptor,ip,port);
    nConnectCount++;
    QLOG_INFO()<<"new connection" <<"socketDescriptor="<<socketDescriptor;
    QLOG_INFO()<<"Current number of connections:"<<nConnectCount;
    qDebug()<<"new connection" <<"socketDescriptor="<<socketDescriptor<<"Current number of connections:"<<nConnectCount;
}

void CTcpServer::sockBinMessageSlot(qint64 socketDescriptor, const QByteArray &data)
{
    emit sockNetMessage(socketDescriptor,data);
}

/// 关闭指定的客户端
void CTcpServer::closeClient(qint64 socketDescriptor, QThread * th)
{
    if(socketDescriptor < 0)
        return;

    if(m_isUsingThread && th)
        ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了

    nConnectCount--;
    QLOG_INFO() << "disconnect" <<"socketDescriptor="<<socketDescriptor;
    QLOG_INFO()<<"Current number of connections:"<<nConnectCount;
    qDebug()<<"new connection" <<"socketDescriptor="<<socketDescriptor<<"Current number of connections:"<<nConnectCount;

    QHash<qint64, CTcpSocket *>::iterator iter = tcpClient.find(socketDescriptor);
    if(iter != tcpClient.end())
    {
        if(!m_isUsingThread)
        {
            disconnect(iter.value(),&CTcpSocket::sockDisConnect,this,&CTcpServer::sockDisConnectSlot);
            disconnect(iter.value(),&CTcpSocket::signalsNetBinary,this,&CTcpServer::sockBinMessageSlot);
        }

        iter.value()->deleteLater();
        tcpClient.erase(iter);
    }
}

void CTcpServer::sockDisConnectSlot(qint64 socketDescriptor, const QString & ip, quint16 port, QThread * th)
{
    closeClient(socketDescriptor,th);

    emit sockDisConnect(socketDescriptor,ip,port);
}

void CTcpServer::clear()
{
    emit this->sentDisConnect(-1);

    if(m_isUsingThread)
        ThreadHandle::getClass().clear();

    QHash<qint64, CTcpSocket *>::iterator iter = tcpClient.begin();
    for(;iter != tcpClient.end();++iter)
    {
        iter.value()->deleteLater();
    }

    tcpClient.clear();
}
