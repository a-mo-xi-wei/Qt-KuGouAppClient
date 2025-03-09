#ifndef CWEBSOCKETSERVER_H
#define CWEBSOCKETSERVER_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QVariant>
#include <QHostAddress>
#include <QtCore/QHash>
#include <QtCore/QByteArray>

#include "singleton.h"
#include "common.h"
#include "networkframemanager.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

struct tagWebClient
{
    tagWebClient()
    {
        m_recvFileState=false;
        memset(&m_tagFileStruct,0,sizeof(m_tagFileStruct));

        m_recvDataState=false;
        m_processrecvData=false;
        memset(&m_tagDataStruct,0,sizeof(m_tagDataStruct));
    }

    void reset(void)
    {
        m_recvFileState=false;
        memset(&m_tagFileStruct,0,sizeof(m_tagFileStruct));

        m_recvDataState=false;
        m_processrecvData=false;
        memset(&m_tagDataStruct,0,sizeof(m_tagDataStruct));

        m_recvFileBytes.clear();
        m_recvDataBytes.clear();
    }

    QByteArray m_recvFileBytes;                                         /**< 用于处理接收到的文件数据 */
    bool m_recvFileState;                                               /**< 接收文件的状态控制 */
    tagFileStruct m_tagFileStruct;                                      /**< 用于文件接收处理 */

    QByteArray m_recvDataBytes;                                         /**< 用于处理接收到的二进制数据 */
    bool m_recvDataState;                                               /**< 接收二进制数据的状态控制 */
    tagDataStruct m_tagDataStruct;                                      /**< 用于二进制数据接收处理 */
    bool m_processrecvData;                                             /**< 是否处理接收二进制数据 */
};

class CWebSocketServer : public QObject/*, public Singleton<CWebSocketServer>*/
{
    Q_OBJECT
public:
    explicit CWebSocketServer(QObject *parent = nullptr);
    ~CWebSocketServer();

    /// 打开一个指定端口的服务器
    Q_INVOKABLE bool OpenServer(int port);
    /// 关闭服务器
    Q_INVOKABLE void CloseServer(void);
    /// 设置服务器最大支持连接数量,默认为30个连接
    Q_INVOKABLE void setMaxPendingConnections(int maxcount);
    /// 关闭指定的客户端
    Q_INVOKABLE void closeClient(QWebSocket *pwebsocket);
    /// 服务器是否启动
    Q_INVOKABLE bool isListening(void);
    /// 得到服务器IP
    QHostAddress getHostAddress(void);
    /// 得到服务器打开的端口
    quint16 getServerPort(void);

    /// 设置网络消息处理框架
    Q_INVOKABLE void SetNetworkFrameManager(NetworkFrameManager *pNetworkFrameManager);
    /// 给指定客户端发送文件(注意点：在网络中使用时一定要设置isExcludeUserInputEvents为false,这个参数是在界面中做防假卡死的)
    Q_INVOKABLE bool sendFile(QWebSocket *pwebsocket,QString filepath,bool isExcludeUserInputEvents=false,QString rootpath="");
    /// 给所有客户端发送文件
    Q_INVOKABLE bool SendAllFile(QString filepath,bool isExcludeUserInputEvents=false,QString rootpath="");
    /// 给指定客户端发送字符数据
    Q_INVOKABLE qint64 Send(QWebSocket *pwebsocket,QString msg);
    /// 给指定客户端发送二进制数据
    Q_INVOKABLE qint64 SendByteArray(QWebSocket *pwebsocket,QByteArray &data,bool isExcludeUserInputEvents=false);
    /// 给所有客户端发送字符数据
    Q_INVOKABLE bool SendAll(QString msg);
    /// 给除了指定客户端的其它客户端发送字符数据
    Q_INVOKABLE bool SendAllOther(QWebSocket *pwebsocket,QString msg);
    /// 给所有客户端发送二进制数据
    Q_INVOKABLE bool SendAllByteArray(QByteArray &data);
    /// 给除了指定客户端的其它客户端发送二进制数据
    Q_INVOKABLE bool SendAllOtherByteArray(QWebSocket *pwebsocket,QByteArray &data);
    /// 是否处理接收文件
    Q_INVOKABLE void setIsProcessRecvFile(bool isProcess,QString recvfiledir="");

    /// 客户端列表锁闭
    Q_INVOKABLE inline void lockClients(void) { m_clientsMutex.lock(); }
    /// 客户端列表解锁
    Q_INVOKABLE inline void unlockClients(void) { m_clientsMutex.unlock(); }
    /// 得到当前所有在线的客户端
    Q_INVOKABLE QList<QWebSocket*> getAllClients(void);
    /// 根据名称得到指定的客户端
    Q_INVOKABLE QWebSocket* getClient(QString objName);
    /// 得到当前在线客户端数量
    Q_INVOKABLE inline int getClientCount(void) { return m_clients.size(); }
    /// 设置是否自动删除socket
    Q_INVOKABLE void setAutoDeleteSocket(bool isDelete) { m_isautoDeleteSocket = isDelete; }
    /// 得到是否自动删除socket
    Q_INVOKABLE bool isAutoDeleteSocket(void) { return m_isautoDeleteSocket; }
    /// 设置指定客户端的关联数据
    Q_INVOKABLE bool setClientVariant(QWebSocket *client,QVariant variant);
    /// 得到指定客户端的关联数据
    Q_INVOKABLE QVariant getClientVariant(QWebSocket *client);
    /// 清除指定客户端的管理数据
    Q_INVOKABLE bool deleteClientVariant(QWebSocket *client);

Q_SIGNALS:
    void closed();

    /// 处理网络字符串消息
    void signalsNetText(QWebSocket *conn,QString mes);
    /// 处理网络二进制消息
    void signalsNetBinary(QWebSocket *conn,QByteArray &data);
    /// 处理一个新的连接到达
    void signalsConnectedNetMes(QWebSocket *conn);
    /// 处理一个连接关闭
    void signalsDisconnectedNetMes(QWebSocket *conn);

private Q_SLOTS:
    /// 一个新的连接到达
    void onNewConnection();
    /// 新的字符串消息到达
    void processTextMessage(QString message);
    /// 新的二进制数据消息到达
    void processBinaryMessage(QByteArray message);
    /// 一个客户端断开连接
    void socketDisconnected();
    /// 心跳处理
    void handleWebSocketHeartTimeOut();

private:
    /// 处理文件接收
    void onPrcessRecvFile(QWebSocket *pwebsocket,const QByteArray &data);
    /// 处理二进制数据接收
    void onPrcessRecvBinaryData(QWebSocket *pwebsocket,const QByteArray &data);

private:
    QWebSocketServer *m_pWebSocketServer;                                  /**< 网络服务器 */
    QMutex m_clientsMutex;
    bool m_isautoDeleteSocket;                                             /**< 是否自动删除socket */
    QHash<QWebSocket *,int> m_clients;                                     /**< 保存所有当前连接上的客户端 */
    QHash<QString,QWebSocket*> m_clientNames;                              /**< 保存所有当前连接上的客户端的名称 */
    QHash<QWebSocket *,QVariant> m_clientVariants;                         /**< 保存当前所有连接的客户端的关联数据 */
    QTimer m_WebSocketHeartTimeOutTimer;                                   /**< 处理心跳的定时器 */
    NetworkFrameManager *m_NetworkFrameManager;                            /**< 网络处理框架 */

    bool m_processrecvFile;                                                /**< 是否处理接收文件 */
    QString m_recvFileSaveDir;                                             /**< 接收文件的保存路径 */
};

#define sCWebSocketServer CWebSocketServer::getSingleton()

#endif // CWEBSOCKETSERVER_H
