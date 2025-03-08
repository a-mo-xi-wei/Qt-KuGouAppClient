#ifndef _C_Q_ENET_H_INCLUDE_
#define _C_Q_ENET_H_INCLUDE_

//#include "../../includes/common/common.h"

#include <QThread>
#include <QTimer>
#include <QHash>
#include <QList>
#include <QString>
#include <QMutex>

typedef struct _ENetPeer ENetPeer;
typedef struct _ENetHost ENetHost;
class NetworkFrameManager;

class CEnetClient : public QThread
{
    Q_OBJECT

public:
    explicit CEnetClient(QObject *parent=nullptr);
    ~CEnetClient();

    /// 设置网络处理框架
    inline void setNetworkFrameManager(NetworkFrameManager *nfm) { m_NetworkFrameManager = nfm; }
    /// 得到网络处理框架
    inline NetworkFrameManager* getNetworkFrameManager(void) { return m_NetworkFrameManager; }

    /// 连接服务器
    bool connectServer(QString serverip,int port=3223);
    /// 断开服务器
    void disconServer(bool islog=true);
    /// 客户端是否启动
    inline bool isRunning(void) { return m_isClientRunning; }
    /// 发送数据
    bool sendData(QByteArray data);

signals:
    /// 日志消息
    void signalSendLog(QString msg);

protected:
    virtual void run();

private:
    bool m_isClientRunning;
    ENetHost *m_client;
    ENetPeer *m_decserver;
    QMutex m_lockclient;
    NetworkFrameManager *m_NetworkFrameManager;
};

class CEnetServer : public QThread
{
    Q_OBJECT

public:
    explicit CEnetServer(QObject *parent=nullptr);
    ~CEnetServer();

    /// 设置网络处理框架
    inline void setNetworkFrameManager(NetworkFrameManager *nfm) { m_NetworkFrameManager = nfm; }
    /// 得到网络处理框架
    inline NetworkFrameManager* getNetworkFrameManager(void) { return m_NetworkFrameManager; }

    /// 打开服务器
    bool openServer(int port=3223);
    /// 关闭服务器
    void closeServer(bool islog=true);
    /// 服务器是否启动
    inline bool isRunning(void) { return m_isServerRunning; }
    /// 得到指定客户端的id
    QString getClientID(ENetPeer *peer);
    /// 发送数据
    bool sendData(ENetPeer *peer,QByteArray data);
    /// 给所有客户端发送数据
    bool sendAll(QByteArray data);

signals:
    /// 日志消息
    void signalSendLog(QString msg);

private:
    /// 添加一个客户端到列表中
    bool addClient(QString uuidStr,ENetPeer *peer);
    /// 从列表中删除一个客户端
    bool delClient(ENetPeer *peer);
    /// 检测指定客户端是否存在
    bool isExistClient(ENetPeer *peer);

protected:
    virtual void run();

private:
    bool m_isServerRunning;
    ENetHost *m_server;
    QMutex m_lockserver;
    QHash<ENetPeer*,QString> m_allsockets;
    NetworkFrameManager *m_NetworkFrameManager;
};

#endif
