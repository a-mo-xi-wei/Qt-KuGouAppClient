#ifndef NETWORK_FRAME_MANAGER_H
#define NETWORK_FRAME_MANAGER_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMutex>
#include <QSemaphore>

#include "../common/common.h"
#include "JQLibrary/jqhttpserver.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QTcpSocket)
QT_FORWARD_DECLARE_CLASS(QSerialPort)

typedef struct _ENetPeer ENetPeer;

class NetworkFrameManager
{
public:
    NetworkFrameManager();
    ~NetworkFrameManager();

    /// 多线程中加锁操作
    inline void lock(void) { m_Mutex.lock(); }
    /// 多线程中解锁操作
    inline void unlock(void) { m_Mutex.unlock(); }

    /// 处理网络字符串消息
    virtual void OnProcessNetText(QWebSocket *conn,QString mes);
    /// 处理网络二进制消息
    virtual void OnProcessNetBinary(QWebSocket *conn,QByteArray &data);
    /// 处理一个新的连接到达
    virtual void OnProcessConnectedNetMes(QWebSocket *conn);
    /// 处理一个连接关闭
    virtual void OnProcessDisconnectedNetMes(QWebSocket *conn);
    /// 处理文件发送
    virtual void OnProcessSendFile(QWebSocket *conn,QString file,qint64 sendsize,qint64 totalsize);
    /// 处理文件接收
    virtual void OnProcessRecvFile(QWebSocket *conn,QString srcfile,QString decfile,FileRecvError pFileRecvError);
    /// 处理二进制文件发送，接收进度，type:0发送，1接收
    virtual void OnProcessBinaryOperProcess(QWebSocket *conn,int type,qint64 sendsize,qint64 totalsize);

    /// 处理网络二进制消息
    virtual void OnProcessNetBinary(QTcpSocket *conn,QByteArray &data);
    /// 处理一个新的连接到达
    virtual void OnProcessConnectedNetMes(QTcpSocket *conn);
    /// 处理一个连接关闭
    virtual void OnProcessDisconnectedNetMes(QTcpSocket *conn);

    /// 处理串口二进制消息
    virtual void OnProcessSerialPortBinary(QSerialPort *serialport,QByteArray &data);

    /// 处理http请求
    virtual bool OnProcessHttpAccepted(QObject *obj,const QPointer< JQHttpServer::Session > &session);

    /// 处理Enet二进制消息
    virtual void OnProcessEnetNetBinary(ENetPeer *conn,QByteArray data);
    /// 处理Enet一个新的连接到达
    virtual void OnProcessEnetConnectedNetMes(ENetPeer *conn);
    /// 处理Enet一个连接关闭
    virtual void OnProcessEnetDisconnectedNetMes(ENetPeer *conn);

protected:
    QMutex m_Mutex;
};

#endif // SERVERFRAMEMANAGER_H
