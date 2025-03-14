﻿#ifndef SUBANDPUB_SERVER_INCLUDE_H
#define SUBANDPUB_SERVER_INCLUDE_H

#include "networkframemanager.h"
#include "cwebsocketserver.h"
#include "cwebsocketclient.h"
#include "QsLog.h"

class CWebSocketServer;
class CWebSocketClient;

class subandpubserver : public QObject , public NetworkFrameManager
{
    Q_OBJECT

public:
    explicit subandpubserver(QObject *parent = nullptr);
    ~subandpubserver() override;

    /// 设置websocket服务器启动端口
    inline void setWebsocketServerPort(int port=1101) { m_websocketServerPort = port; }
    /// 得到websocket服务器启动端口
    inline int getWebsocketServerPort() const { return m_websocketServerPort; }
    /// 得到服务器唯一ID
    inline QString getServerUUID() { return m_serverUUID; }
    /// 设置服务器唯一ID
    inline void setServerUUID(const QString &uuid) { m_serverUUID = uuid; }
    /// 生成一个随机的服务器ID
    QString RandomGenerateServerUUID();

    /// 启动服务器
    bool startServer();
    /// 停止服务器
    void stopServer(bool isShowlog=true);
    /// 检测服务器是否已经启动
    bool isRunning();

    /// 添加一个兄弟服务器
    bool addBrotherServer(const QString& ip,int port);
    /// 通过配置文件导入兄弟服务器
    bool loadBrotherServers(const QString& filepath);
    /// 保存配置文件
    bool savaBrotherServers(const QString& filepath="");
    /// 关闭所有的兄弟服务器
    void closeAllBrotherServers();
    /// 得到所有的兄弟服务器
    inline QHash<QWebSocket*,CWebSocketClient*>& getAllBrotherServers() { return m_brotherServers; }
    /// 得到所有兄弟服务器列表
    inline QList<tagBrotherServerItem>& getallBrotherServerList() { return m_allBrotherServerList; }

private:
    /// 打印用户日志
    void printLog(QsLogging::Level type,const QString& msg);
    /// 得到当前时间
    QString getCurrentDate();
    /// 给所有兄弟结点发送消息
    void sendMessagetoBrother(const QString& data, const QWebSocket* client=nullptr);
    /// 检测指定的消息是否存在
    bool isExistSubscribeMessage(const QString& message);
    /// 检测指定消息的密码是否一致
    bool isSubscribeMessagePWDSame(const QString& message,const QString& pwd);
    /// 检测指定消息是否有密码
    bool isExistPWDsubMessage(const QString& message);
    /// 检测指定连接是否是兄弟结点
    CWebSocketClient* isExistClientInBrotherServers(QWebSocket* client);
    /// 检测指定的客户端是否订阅了指定的消息
    bool isExistClientSubMessage(const QWebSocket* client,const QString& message);
    /// 从指定订阅消息中删除指定的客户端
    bool deleteclientFromSubMessage(const QWebSocket* client,const QString& message);
    /// 向订阅了指定消息的客户端发送指定消息
    bool sendMessageToClient(const QString& message,const QString& content);
    /// 根据用户发送消息生成一个消息ID
    QString generateMessageID(const QWebSocket* client,const QString& msg);
    /// 得到客户端的UUID
    tagSendFailClient* getSendFailClient(const QWebSocket *client);
    tagSendFailClient* getSendFailClient2(const QString& uuid);
    /// 重置掉线客户端
    void ResetSendFailClient(const QWebSocket *client);

signals:
    /// 日志消息
    void showlog(QString msg);

protected:
    /// 处理网络字符串消息
    void OnProcessNetText(QWebSocket *conn,QString mes) override;
    /// 处理一个新的连接到达
    void OnProcessConnectedNetMes(QWebSocket *conn) override;
    /// 处理一个连接关闭
    void OnProcessDisconnectedNetMes(QWebSocket *conn) override;

private slots:
    /// 系统更新定时器
    void timer_system_update();

private:
    CWebSocketServer m_MainWebSocketServer;                               /**< websocket服务器 */
    int m_websocketServerPort;                                            /**< 服务器启动端口 */
    QString m_serverUUID;                                                 /**< 服务器唯一标识 */
    QTimer m_updateTimer;                                                 /**< 系统更新定时器 */
    QString m_currentConfigFile;                                          /**< 当前操作的配置文件 */
    QHash<QWebSocket*,CWebSocketClient*> m_brotherServers;                /**< 兄弟服务器 */
    QHash<QString,QList<QWebSocket*>> m_allsubscribes;                    /**< 管理所有的订阅消息 */
    QHash<QString,QString> m_allsubscribePWDs;                            /**< 管理所有的订阅消息的密码 */
    QList<QJsonObject> m_allsubscribeMessageList;                         /**< 存储所有的订阅消息 */
    QHash<QString,tagSubMsgItem> m_allsubscribeMessageHash;               /**< 用hash存储所有的订阅消息 */
    QHash<QString,QList<tagSendFailMsgItem>> m_allSendFailMessageList;    /**< 消息发送失败列表 */
    QHash<QString,tagSendFailClient> m_allSendFailClientList;             /**< 消息发送失败客户端列表 */
    QList<tagBrotherServerItem> m_allBrotherServerList;                   /**< 兄弟服务器列表 */
    QHash<QWebSocket*,ClientIdentity> m_clientidentityinfos;              /**< 客户端的身份认证信息 */
};

#endif // SUBANDPUB_SERVER_INCLUDE_H
