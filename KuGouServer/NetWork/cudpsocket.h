#ifndef C_UDP_SOCKET_H_INCLUDE_
#define C_UDP_SOCKET_H_INCLUDE_

#include "common.h"
#include "NedAllocatedObject.h"

#include <QObject>
#include <QUdpSocket>
#include <QHash>
#include <QString>
#include <QMutex>

#include "kcp/kcpobj.h"

/**
 * 如果要使用传统的UDP功能，只需要设置isEnableKcp为false就可以了，
 * 如果要使用kcp功能，就设置isEnableKcp为true,默认是使用kcp功能的,采用server/client模式。
 *
 * KCP项目地址 - https://github.com/skywind3000/kcp
 * KCP是一个快速可靠协议，能以比 TCP浪费10%-20%的带宽的代价，换取平均延迟降低 30%-40%，且最大延迟降低三倍的传输效果.
 *
 * 客户端使用例子：
 *
 * CUdpSocket m_udpsocket;
 * connect(&m_udpsocket,SIGNAL(processPendingDatagram(CUdpSocket*,QByteArray)),this,SLOT(onprocesss_processPendingDatagram(CUdpSocket*,QByteArray)));
 * m_udpsocket.SendKcpData(dataarray);
 *
 * 服务器使用例子：
 * CUdpServer m_udpServer;
 * connect(&m_udpServer,SIGNAL(signal_ClientReceiveMsg(CUdpSocket*,QByteArray)),this,SLOT(onprocess_ClientReceiveMsg(CUdpSocket*,QByteArray)));
 * connect(&m_udpServer,SIGNAL(signal_ClientArrive(CUdpSocket*)),this,SLOT(onprocesss_signal_ClientArrive(CUdpSocket*)));
 * connect(&m_udpServer,SIGNAL(signal_ClientLeave(CUdpSocket*)),this,SLOT(onprocesss_signal_ClientLeave(CUdpSocket*)));
 * m_udpServer.open(2000);
 *
 * m_udpServer.SendAll(datagramdata);
 */
class CUdpSocket : public QObject , public NedAllocatedObject
{
    Q_OBJECT

public:
    explicit CUdpSocket(QObject *parent = nullptr,bool isEnableKcp=true,
                        bool isEnableReciver=true,
                        int receivePort=-1,
                        bool isProcessDatagramHearder=true,
                        bool isServerUse=false);
    ~CUdpSocket() override;

    /// 设置要连接的地址和端口
    void Connect(const QHostAddress &address,int port);
    /// 得到socket的名称
    QString getName();
    /// 设置是否处理二进制数据
    inline void setProcessBinaryData(bool isProcessBinaryData) { m_isProcessBinaryData = isProcessBinaryData; }
    /// 是否处理二进制数据
    inline bool isProcessBinaryData() const { return m_isProcessBinaryData; }
    /// 得到一个信息是否处理完成
    inline bool isProcessDatagramHearder() const { return m_isProcessDatagramHearder; }
    /// 设置当前是否使用KCP
    inline void setCurrentUsingKcp(bool isuse) { m_currentUseKcp = isuse; }
    /// 得到当前是否使用kcp
    inline bool isCurrentUsingKcp() const { return m_currentUseKcp; }
    /// 得到socket的地址
    inline QHostAddress getHostAddress() { return m_address; }
    /// 得到socket的端口
    inline int getPort() const { return m_port; }
    /// 得到socket的接收端口号
    inline int getReceivePort() const { return m_receivePort; }
    /// 发送报文(正常)
    qint64 SendByte(const QByteArray& Datagramdata);
    /// 发送报文(KCP使用)
    int SendKcpData(const QByteArray& Datagramdata);
    /// 重置当前心跳
    inline void resetHeartCount() { m_msgHeartCount = 0; }
    /// 得到当前心跳计数
    inline int GetHeartCount() const { return m_msgHeartCount; }
    /// 心跳计数
    inline void heart() { m_msgHeartCount += 1; }
    /// 得到SocketID
    inline QUdpSocket* getSocketID() { return &m_UdpSocket; }
    /// 是否要发送心跳
    inline void setUseHeart(bool isUse) { m_isSendHeart = isUse; }
    /// 得到是否要使用发送心跳
    inline bool isUseHeart() const { return m_isSendHeart; }
    /// 得到是否连接成功
    inline bool isConnected() const { return m_isConnected; }

    /// kcp处理接收到的报文
    void onProcessReciverKcpDatagramdata(QByteArray& Datagramdata,bool isenableKcp);
    /// 处理接收到的报文
    void onProcessReciverDatagramdata();
    /// 处理发送报文
    void onProcessSendData(const char * buf, int len);

private:
    /// 初始Kcp
    void InitKcp();
    /// 释放Kcp
    void ReleaseKcp();
    /// kcp转换后消息处理
    void DoKcpRecv(const char *buf, int len);

signals:
    /// 处理接收到的消息
    void processPendingDatagram(CUdpSocket* socketid,QByteArray datagramdata);
    /// 处理连接成功
    void signal_Connected();
    /// 处理连接断掉
    void signal_Disconnected();

private slots:
    void reciverPendingDatagram();
    void KCPUpdate();

private:
    QUdpSocket m_UdpSocket;
    QByteArray m_datagramData;
    QHostAddress m_address;
    QTimer m_updateTimer;
    int m_port;
    bool m_enableKcp,m_currentUseKcp;
    bool m_isProcessBinaryData;     /**< 是否处理二进制消息 */
    bool m_isSendHeart;             /**< 是否要发送心跳数据，只有在C/S模式下才使用，其它情况下不使用，默认使用心跳 */    
    bool m_isEnableReciver;
    int m_msgHeartCount;             /**< 心跳检测 */
    qint64 m_currentHeartTime;      /**< 用于心跳消息发送 */
    int m_receivePort;              /**< 接收消息的端口号 */
    bool m_isConnected;             /**< 是否连接上 */
    bool m_isServerUsing;           /**< 是否服务器使用 */

    tagUdpDatagramHearder m_DatagramHearder;
    bool m_isProcessDatagramHearder;

    HYKT::KcpObj *m_ikcp;
    QMutex m_kcpMutex;
};

class CUdpServer : public QObject
{
    Q_OBJECT

public:
    explicit CUdpServer(QObject *parent = nullptr,bool isEnableKcp=true);
    ~CUdpServer() override;

    /// 设置要打开的端口
    bool open(int port,bool isenableKcp=true);
    /// 停止服务器
    void stop();
    /// 发送消息给所有人
    void SendAll(const QByteArray& datagramdata);
    /// 发送除了指定socketid的其它人
    void SendOther(const QUdpSocket* socketid,const QByteArray& datagramdata);
    /// 发送给指定socketid
    void SendTo(QUdpSocket* socketid,const QByteArray& datagramdata);
    /// 清除所有的数据
    void Clear();
    /// 关闭指定的socket
    void closeSocket(const CUdpSocket *socketid);
    /// 根据socketid得到相应的socket
    CUdpSocket* getSocket(QUdpSocket *socketid);
    /// 添加一个新的客户端
    CUdpSocket* addSocket(const QHostAddress& address,int port);
    /// 根据名称得到客户端
    CUdpSocket* getSocketByName(const QString& name);
    /// 设置当前是否使用KCP
    inline void enableKcp(bool isuse) { m_enableKcp = isuse; }
    /// 得到当前是否使用kcp
    inline bool isenableKcp() const { return m_enableKcp; }
    /// 检查服务器是否已经启动
    inline bool isListening() const {
        return (m_UdpSocket.state() == QAbstractSocket::ListeningState ||
                m_UdpSocket.state() == QAbstractSocket::BoundState) ? true : false;
    }

signals:
    /// 处理收到的客户端信息
    void signal_ClientReceiveMsg(CUdpSocket* socketid,QByteArray datagramdata);
    /// 处理一个新的客户端到达
    void signal_ClientArrive(CUdpSocket* socketid);
    /// 处理一个客户端离开
    void signal_ClientLeave(CUdpSocket* socketid);

private slots:
    void reciverPendingDatagram();
    void KCPUpdate();
    void on_process_processPendingDatagram(CUdpSocket* socketid,QByteArray datagramdata);

private:
    QUdpSocket m_UdpSocket;
    bool m_enableKcp;
    QTimer m_updateTimer;

    QHash<QString,CUdpSocket*> m_udpClients;
    QHash<QUdpSocket*,CUdpSocket*> m_udpKcpClients;
    QMutex m_ClientMutex;
};

#endif
