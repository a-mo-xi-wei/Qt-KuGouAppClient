#ifndef CTCP_SOCKETSERVER_H
#define CTCP_SOCKETSERVER_H

#include "../common/singleton.h"
#include "../common/common.h"
#include "networkframemanager.h"
#include "ctcpsocketclient.h"

#include <QThread>
#include <QQueue>
#include <QList>
#include <QHash>
#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
#include <QTcpServer>

//开源项目地址:https://github.com/quan12jiale/QTcpSocket
//线程管理类，类似于一个线程池，单例类
//两种初始化方式，一种是每个线程处理的连接数，一个是一共多少个线程
//********************当前多线程使用还存在问题，暂时不支持多线程**************************
class ThreadHandle
{
public:
    ~ThreadHandle();

    enum ThreadType
    {
        THREADSIZE, //固定线程数
        HANDLESIZE //固定每个线程处理多少连接
    };

    static ThreadHandle & getClass(); //返回对象引用，是单例类

    QThread * getThread(); //取出应该移入的线程
    void initThreadType(ThreadType type = HANDLESIZE,unsigned int max = 1000);//初始化线程管理的方式
    void removeThread(QThread *);//连接断开，线程计数减一
    void clear();//清空计数

protected:
    void initThreadSize();//新建固定线程和启动
    QThread * findThreadSize();//固定线程数的查找
    QThread *findHandleSize();//固定连接数查找

private:
    ThreadHandle();
    ThreadType type;//线程类型
    unsigned int size;//最大值
    QHash<QThread *, unsigned int> threadSize;//保存每个线程的数目
    bool initfist;//是否是第一次初始化，只允许初始化一次。
};

//继承QTCPSERVER以实现多线程TCPscoket的服务器。
//如果socket的信息处理直接处理的话，很多新建的信号和槽是用不到的
//暂不支持多线程
class CTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit CTcpServer(NetworkFrameManager *pNetworkFrameManager=NULL,
                        int numConnections = 10000,
                        bool isencoded=true,
                        QObject *parent = nullptr);
    ~CTcpServer();

    /// 设置网络消息处理框架
    inline void SetNetworkFrameManager(NetworkFrameManager *pNetworkFrameManager) { m_NetworkFrameManager = pNetworkFrameManager; }
    /// 打开一个指定端口的服务器
    Q_INVOKABLE bool OpenServer(int port);
    /// 关闭服务器
    Q_INVOKABLE void CloseServer(void);
    /// 关闭指定的客户端
    Q_INVOKABLE void closeClient(qint64 conn, QThread * th=NULL);

    /// 设置是否使用多线程
    inline void setUsingThread(bool isUse) { m_isUsingThread = isUse; }
    /// 得到是否使用多线程
    inline bool isUsingThread(void) { return m_isUsingThread; }
    /// 重写设置最大连接数函数
    void setMaxPendingConnections(int numConnections);
    /// 覆盖已获取多线程
    void incomingConnection(qintptr socketDescriptor);
    /// 是否自行编码数据
    inline void setEncode(bool encode) { m_isencoded = encode; }
    /// 得到是否自行编码数据
    inline bool isEncode(void) { return m_isencoded; }

    /// 给指定客户端发送二进制数据
    Q_INVOKABLE qint64 SendByteArray(qintptr socketDescriptor,QByteArray data);
    /// 给所有客户端发送二进制数据
    Q_INVOKABLE bool SendAllByteArray(QByteArray data);
    /// 给除了指定客户端的其它客户端发送二进制数据
    Q_INVOKABLE bool SendAllOtherByteArray(qintptr socketDescriptor,QByteArray data);

signals:
    void connectClient(qint64, const QString &, quint16);//发送新用户连接信息
    void sockDisConnect(qint64, const QString &, quint16);//断开连接的用户信息
    void sentDisConnect(qint64); //断开特定连接，并释放资源，-1为断开所有。
    void sockNetMessage(qint64 conn,QByteArray data);

public slots:
    void clear(); //断开所有连接，线程计数器请0
private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void sockDisConnectSlot(qint64 socketDescriptor,const QString &ip, quint16 port, QThread *th);//断开连接的用户信息
    void sockBinMessageSlot(qint64 socketDescriptor,QByteArray data);

private:
    QHash<qint64, CTcpSocket *> tcpClient;                              /**< 管理连接的map */
    NetworkFrameManager *m_NetworkFrameManager;                         /**< 网络消息处理框架 */
    int maxConnections;                                                 /**< 最大连接数 */
    int nConnectCount;                                                  /**< 当前连接数 */
    bool m_isUsingThread;                                               /**< 是否使用多线程处理 */
    bool m_isencoded;                                                   /**< 是否自行编码数据 */
};

#endif // CWEBSOCKETSERVER_H
