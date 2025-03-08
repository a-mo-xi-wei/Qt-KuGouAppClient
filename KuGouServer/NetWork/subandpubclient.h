#ifndef _SUBANDPUB_CLIENT_H_INCLUDE_
#define _SUBANDPUB_CLIENT_H_INCLUDE_

#include <QObject>
#include <QHash>
#include <QList>
#include <QTimer>

#include "networkframemanager.h"
#include "cwebsocketclient.h"
#include "../common/common.h"
#include "../QsLog/QsLog.h"

class subandpubclient : public QObject , public NetworkFrameManager
{
    Q_OBJECT

public:
    explicit subandpubclient(QObject *parent = nullptr);
    ~subandpubclient();

    /// 打开指定地址的网络连接
    void Open2(QString ip,int port);
    /// 关闭连接
    void Close(void);
    /// 检测连接是否建立成功
    bool isConnected(void);
    /// 是否断线重连后自动订阅消息
    inline bool isAutoSubMessage(void) { return m_isAutosubMessage; }
    /// 设置是否断线重连后自动订阅消息
    inline void setAutoSubMessage(bool isenable) { m_isAutosubMessage = isenable; }
    /// 得到订阅的消息列表
    inline QHash<QString,QString>& getsubMessageList(void) { return m_subMessageList; }
    /// 清除订阅的消息列表
    inline void clearSubMessages(void) { m_subMessageList.clear(); }
    /// 得到客户端唯一ID
    inline QString getClientUUID(void) { return m_clientUUID; }
    /// 设置客户端唯一ID
    inline void setClientUUID(QString uuid) { m_clientUUID = uuid; }
    /// 得到连接的服务器IP
    inline QString getServerIP(void) { return m_serverip; }
    /// 设置连接的服务器IP
    inline void setServerIP(QString ip) { m_serverip = ip; }
    /// 得到连接的服务器端口
    inline int getServerPort(void) { return m_serverport; }
    /// 设置连接的服务器端口
    inline void setServerPort(int port) { m_serverport = port; }
    /// 生成一个随机的ID
    QString RandomGenerateClientUUID(void);
    /// 通过配置文件
    bool loadConfigFile(QString filepath);
    /// 保存配置文件
    bool savaConfigFile(QString filepath="");

    /// 订阅消息
    void subMessage(QString message,QString password="");
    /// 退订消息
    void unSubMessage(QString message);
    /// 发布消息
    bool pubMessage(QString message,QString content,
                    QString password="",QString time="",
                    bool isAddofflinelist=true);

private:
    /// 清除指定的订阅消息
    void deleteSubMessage(QString message);
    void Open(QUrl url);

signals:
    /// 日志消息
    void signalshowlog(QString msg);
    /// 处理收到的消息
    void signalprocessmessage(QJsonObject& msgobj);

private:
    /// 打印用户日志
    void printLog(QsLogging::Level type,QString msg);
    /// 得到当前时间
    QString getCurrentDate(void);

protected:
    /// 处理网络字符串消息
    virtual void OnProcessNetText(QWebSocket *conn,QString mes);
    /// 处理一个新的连接到达
    virtual void OnProcessConnectedNetMes(QWebSocket *conn);
    /// 处理一个连接关闭
    virtual void OnProcessDisconnectedNetMes(QWebSocket *conn);

private slots:
    /// 系统更新定时器
    void timer_system_update(void);

private:
    CWebSocketClient m_mainWebSocketClient;
    QString m_currentConfigPath;                        /**< 当前操作的配置文件 */
    QString m_serverip;                                 /**< 服务器IP */
    QTimer m_updateTimer;                               /**< 系统更新定时器 */
    int m_serverport;                                   /**< 服务器端口 */
    QHash<QString,QString> m_subMessageList;            /**< 当前订阅的消息列表 */
    QString m_clientUUID;                               /**< 客户端唯一标识 */
    bool m_isAutosubMessage;                            /**< 断线重连后自动订阅消息 */
    bool m_isAutoSubMsgSuccesed;                        /**< 断线重连后是否自动订阅消息成功 */
    QList<tagPubMessage> m_pubMessageList;              /**< 发送不成功消息列表 */
};

#endif // SUBANDPUBCLIENT_H
