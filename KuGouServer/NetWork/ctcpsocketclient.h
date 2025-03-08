#ifndef CTCPSOCKETCLIENT_H
#define CTCPSOCKETCLIENT_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <QJsonObject>
#include <QtCore/QHash>
#include <QtCore/QByteArray>
#include <QTcpSocket>
#include <QUrl>
#include <QFile>

#include "../common/singleton.h"
#include "../common/common.h"
#include "networkframemanager.h"
#include "../common/NedAllocatedObject.h"

#define IDD_COMPRESS_BUFFER_SIZE 5120

/**
 * @brief The CTcpSocket class tcpsocket客户端类
 */
class CTcpSocket : public QTcpSocket , public NedAllocatedObject
{
    Q_OBJECT

public:
    explicit CTcpSocket(NetworkFrameManager *pNetworkFrameManager=NULL,
                        QObject *parent = nullptr,
                        bool isReConnecting=false,
                        qintptr socketDescriptor=-1,
                        bool enableencryption=true,
                        bool isencoded=true);
    ~CTcpSocket();

    /// 设置网络消息处理框架
    inline void SetNetworkFrameManager(NetworkFrameManager *pNetworkFrameManager) { m_NetworkFrameManager = pNetworkFrameManager; }
    /// 连接tcp服务器
    Q_INVOKABLE void connectTcpServer(const QString& ip, quint16 port);
    /// 关闭与tcp服务器的连接
    Q_INVOKABLE void close(void);
    /// 发送json数据
    Q_INVOKABLE void sendJson(QJsonObject mes);
     /// 重新连接服务器
    void reconnectTcpServer(void);   
    /// 检测是否连接成功
    bool isConnected(void);
    /// 得到连接的心跳计数
    inline int getHeartCount(void) { return m_HeartCount; }
    /// 设置当前使用线程
    inline void setUsingThread(QThread *thread) { m_curThread = thread; }
    /// 得到当前使用线程
    inline QThread* getUsingThread(void) { return m_curThread; }
    /// 是否自行编码数据
    inline void setEncode(bool encode) { m_isencoded = encode; }
    /// 得到是否自行编码数据
    inline bool isEncode(void) { return m_isencoded; }

signals:
    void dataPacketReady(QByteArray socketData);
    void sockDisConnect(qint64, const QString &, quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    /// 处理网络二进制消息
    void signalsNetBinary(qint64 conn,QByteArray &data);

public slots:
    /// 发送数据
    qint64 send(QByteArray data);
    /// 错误处理
    void error(QAbstractSocket::SocketError socketError);
    /// 断开socket的连接
    void disConTcp(qint64 socketDescriptor);
    /// 发送新用户连接信息
    void newconnectClient(qint64 socketDescriptor, const QString &ip, quint16 port);

private slots:
    /// 处理接收数据
    void OnReadyRead();
    /// 处理连接关闭
    void onDisconnected();
    /// 处理连接成功
    void onConnected();
    /// 处理心跳
    void handleTcpSocketHeartTimeOut();
    /// 处理断开重连
    void handleTcpSocketReconnect();

private:
    /// 解析信息包
    bool parsePacket(void);

private:
    QByteArray m_dataPacket;                                            /**< 用于存放接收的数据 */
    bool m_recvDataState;                                               /**< 用于接收数据处理 */
    tagDatagramHearder m_TcpPacketHearder;                              /**< 用于存放每次接收的包头数据 */
    NetworkFrameManager *m_NetworkFrameManager;                         /**< 网络消息处理框架 */
    QTimer m_TcpSocketHeartTimeOutTimer,m_TcpSocketReconnectTimer;      /**< 用于处理心跳和断开重连的定时器 */
    QString m_ServerIP;                                                 /**< 要连接的服务器的ip */
    int m_ServerPort;                                                   /**< 要连接的服务器的端口 */
    int m_HeartCount;                                                   /**< 心跳计数 */
    bool m_isencoded;                                                   /**< 是否自行编码数据 */
    qintptr socketID;
    QMetaObject::Connection dis;
    QThread *m_curThread;                                               /**< 当前使用线程 */
    bool m_isReConnecting;                                              /**< 是否自动重连 */
    bool m_enableencryption;                                            /**< 是否加密信息  */
    unsigned char uncompressed_buffer[IDD_COMPRESS_BUFFER_SIZE],
                  compressed_buffer[IDD_COMPRESS_BUFFER_SIZE];          /**< 用于存放解压后的数据 */
};

#endif // CWEBSOCKETCLIENT_H
