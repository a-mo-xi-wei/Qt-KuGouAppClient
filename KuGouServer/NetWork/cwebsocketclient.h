#ifndef CWEBSOCKETCLIENT_H
#define CWEBSOCKETCLIENT_H

#include <QObject>
#include <QTimer>
#include <QtCore/QHash>
#include <QtCore/QByteArray>
#include <QWebSocket>
#include <QTimer>
#include <QUrl>
#include <QFile>

#include "singleton.h"
#include "common.h"
#include "networkframemanager.h"

QT_FORWARD_DECLARE_CLASS(QWebSocket)

class CWebSocketClient : public QObject
{
    Q_OBJECT

public:
    explicit CWebSocketClient(NetworkFrameManager *pNetworkFrameManager=NULL,QObject *parent = nullptr);
    ~CWebSocketClient();

    /// 打开指定地址的网络连接
    Q_INVOKABLE void Open(QUrl url);
    /// 打开指定地址的网络连接
    Q_INVOKABLE void Open2(QString serverip,int serverport);
    /// 关闭连接
    Q_INVOKABLE void Close(bool isClosed=true);
    /// 得到要连接的服务器IP
    Q_INVOKABLE QString getServerIP(void) { return m_serverIP; }
    /// 得到要连接的服务器端口
    Q_INVOKABLE int getServerPort(void) { return m_serverPort; }
    /// 检测连接是否建立成功
    Q_INVOKABLE bool isConnected(void);
    /// 设置是否断线重连
    Q_INVOKABLE void setDisReconnect(bool isconnect) { m_isDisreconnected = isconnect; }
    /// 是否断线重连
    Q_INVOKABLE bool isDisReconnect(void) { return m_isDisreconnected; }
    /// 得到网络客户端
    Q_INVOKABLE inline QWebSocket* getWebSocket(void) { return &m_webSocket; }
    /// 发送字符串
    Q_INVOKABLE qint64 Send(QString msg);
    /// 发送文件(注意点：在网络中使用时一定要设置isExcludeUserInputEvents为false,这个参数是在界面中做防假卡死的)
    Q_INVOKABLE bool sendFile(QString filepath,bool isExcludeUserInputEvents=true,QString rootpath="");
    /// 发送二进制数据
    Q_INVOKABLE qint64 sendBinaryMessage(const QByteArray &data,bool isExcludeUserInputEvents=false);
    /// 设置网络消息处理框架
    Q_INVOKABLE void SetNetworkFrameManager(NetworkFrameManager *pNetworkFrameManager);
    /// 是否处理接收文件
    Q_INVOKABLE void setIsProcessRecvFile(bool isProcess,QString recvfiledir="");

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
    /// 处理字符串消息
    void onTextMessageReceived(QString message);
    /// 处理二进制消息
    void onBinaryReceived(QByteArray message);
    /// 处理连接关闭
    void onDisconnected();
    /// 处理连接成功
    void onConnected();
    /// 处理心跳
    void handleWebSocketHeartTimeOut();
    /// 处理断开重连
    void handleWebSocketReconnect();

private:
    /// 处理文件接收
    void onPrcessRecvFile(QWebSocket *pwebsocket,const QByteArray &data);
    /// 处理二进制数据接收
    void onPrcessRecvBinaryData(QWebSocket *pwebsocket,const QByteArray &data);

private:
    QWebSocket m_webSocket;                                             /**< 网络客户端 */
    QString m_serverIP;                                                 /**< 要连接的服务器IP */
    int m_serverPort;                                                   /**< 要连接的服务器端口 */
    QUrl m_websocketurl;                                                /**< 要连接的服务器地址 */
    QTimer m_WebSocketHeartTimeOutTimer,m_WebSocketReconnectTimer;      /**< 用于处理心跳和断开重连的定时器 */
    bool m_isCloseSocket;
    bool m_isDisreconnected;                                            /**< 是否开启断线重连 */

    NetworkFrameManager *m_NetworkFrameManager;                         /**< 网络消息处理框架 */

    QByteArray m_recvFileBytes;                                         /**< 用于处理接收到的文件数据 */
    bool m_recvFileState;                                               /**< 接收文件的状态控制 */
    tagFileStruct m_tagFileStruct;                                      /**< 用于文件接收处理 */
    bool m_processrecvFile;                                             /**< 是否处理接收文件 */
    QString m_recvFileSaveDir;                                          /**< 接收文件的保存路径 */

    QByteArray m_recvDataBytes;                                         /**< 用于处理接收到的二进制数据 */
    bool m_recvDataState;                                               /**< 接收二进制数据的状态控制 */
    tagDataStruct m_tagDataStruct;                                      /**< 用于二进制数据接收处理 */
    bool m_processrecvData;                                             /**< 是否处理接收二进制数据 */
};

class CWebSocketClientManager : public Singleton<CWebSocketClientManager>
{
public:
     CWebSocketClientManager();
     ~CWebSocketClientManager();

     /// 添加一个客户端
     bool addClient(QString clientName,CWebSocketClient* pClient);
     /// 得到一个客户端
     CWebSocketClient* getClient(QString clientName);
     /// 清除所有的客户端
     void clearAllClients(void);
     /// 清除指定的客户端
     bool deleteClient(QString clientName);

private:
    QHash<QString,CWebSocketClient*> m_WebSocketClients;
};

#define sCWebSocketClientManager CWebSocketClientManager::getSingleton()

#endif // CWEBSOCKETCLIENT_H
