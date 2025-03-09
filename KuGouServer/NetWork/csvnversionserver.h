#ifndef CSVNVERSIONSERVER_H
#define CSVNVERSIONSERVER_H

#include "sqlitedataprovider.h"
#include "networkframemanager.h"
#include "cwebsocketserver.h"
#include "common.h"

#include <QObject>

/**
 * @brief The CSVNVersionServer class 版本管理服务器类
 *
 * 使用方法：
 * CSVNVersionServer m_SVNVersionServer;
 * m_SVNVersionServer.setDataBase("E:/working/libqtcore/extends/svnversion_data.db");
 * m_SVNVersionServer.startServer(1344);
 *
 * 1.首先设置我们要使用数据库文件的路径；
 * 2.设置网络处理端口
 */
class CSVNVersionServer : public QObject , public NetworkFrameManager
{
    Q_OBJECT

public:
    /// 构造函数
    explicit CSVNVersionServer(QObject *parent = nullptr);
    /// 析构函数
    ~CSVNVersionServer() override;

    /// 设置版本库所使用数据库
    void setDataBase(const QString& dbPath);

    /// 启动服务器
    void startServer(int serverPort,int maxClients=30);
    /// 停止服务器
    void stopServer();
    /// 服务器是否启动
    inline bool isListening() { return m_webSocketServer.isListening(); }

    /// 处理网络字符串消息
    void OnProcessNetText(QWebSocket *conn,QString mes) override;
    /// 处理网络二进制消息
    void OnProcessNetBinary(QWebSocket *conn,QByteArray &data) override;
    /// 处理一个新的连接到达
    void OnProcessConnectedNetMes(QWebSocket *conn) override;
    /// 处理一个连接关闭
    void OnProcessDisconnectedNetMes(QWebSocket *conn) override;
    /// 处理文件发送
    void OnProcessSendFile(QWebSocket *conn,QString file,qint64 sendsize,qint64 totalsize) override;
    /// 处理文件接收
    void OnProcessRecvFile(QWebSocket *conn,QString srcfile,QString decfile,FileRecvError pFileRecvError) override;

signals:
    /// 日志消息处理
    void on_signalemitLogMessage(QString logMsg);

private:
    /// 处理用户注册消息
    void onProcessNetRegiterUser(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理用户更新消息
    void onProcessNetUpdateUser(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理用户删除消息
    void onProcessNetDeleteUser(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理用户登陆消息
    void onProcessNetLoginUser(QWebSocket *conn,QJsonObject &mesObj);

    /// 处理项目添加消息
    void onProcessNetProjectAdd(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理项目删除消息
    void onProcessNetProjectDel(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理项目添加版本
    void onProcessNetVersionAdd(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理项目得到版本数据
    void onProcessNetVersionGetData(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理项目得到项目信息
    void onProcessNetProjectGetInfo(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理项目得到版本信息
    void onProcessNetProjectGetVersionsInfo(QWebSocket *conn,QJsonObject &mesObj);
    /// 处理项目得到用户信息
    void onProcessNetProjectGetUserInfo(QWebSocket *conn,QJsonObject &mesObj);

    /// 添加指定ID保存的版本数据
    bool addVersionData(int id,const QByteArray& data);
    /// 得到指定ID保存的版本数据
    QByteArray getVersionData(int id);
    /// 检测指定用户ID和密码的用户是否存在
    bool isExistUser(QWebSocket *conn,int id, const QString &pwd);
    /// 插入一条用户操作日志信息
    void insertuseroperatorlog(int userid,int type,int grade,const QString& logmsg);

private:
    SqliteDataProvider m_sqliteDataProvider;                 /**< 用于数据库操作  */
    CWebSocketServer   m_webSocketServer;                    /**< websocket用于操作网络服务  */
};

#endif // CSVNVERSIONSERVER_H
