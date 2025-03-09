#ifndef CSVNVERSIONCLIENT_H
#define CSVNVERSIONCLIENT_H

#include "networkframemanager.h"
#include "cwebsocketclient.h"
#include "common.h"

#include <QObject>
#include <QVector>
#include <utility>

/**
 * @brief The tagFileChangedData struct  存储文件的改变信息
 */
struct tagFileChangedData
{
    tagFileChangedData() = default;
    tagFileChangedData(QString fn,QString pmd5)
        : fileName(std::move(fn)),md5(std::move(pmd5)) {}

    QString fileName;
    QString md5;
};

/**
 * @brief The OperationType enum 操作类型
 */
enum OperationType
{
    OPER_UPLOAD = 0,          // 上传
    OPER_DOWNLOAD,            // 下载
    OPER_COMPRESS,            // 压缩
    OPER_UNCOMPRESS,          // 解压
    OPER_NULL
};

/**
 * @brief The CSVNVersionClient class 版本管理客户端类
 *
 * 使用方法：
 * CSVNVersionClient m_SVNVersionClient;
 * m_SVNVersionClient.startClient("127.0.0.1",1344);
 * m_SVNVersionClient.setCurrentWorkingPath("E:/working/test");
 *
 * 1.设置服务器IP和端口
 * 2.设置工作目录
 */
class CSVNVersionClient : public QObject , public NetworkFrameManager
{
    Q_OBJECT

public:
    /// 构造函数
    explicit CSVNVersionClient(QObject *parent = nullptr);
    /// 析构函数
    ~CSVNVersionClient() override;

    /// 设置当前工作目录
    inline void setCurrentWorkingPath(QString path) { m_currentWorkingPath = std::move(path); }
    /// 得到当前工作目录
    inline QString getCurrentWorkingPath() { return m_currentWorkingPath; }
    /// 开始客户端
    void startClient(const QString& serverip,int port);
    /// 关闭与服务器的连接
    void stopClient();
    /// 检测连接是否建立成功
    bool isConnected();
    /// 将指定文件夹转换成一个包数据
    QByteArray Convertfoldertopackage(const QString& filePath);
    /// 将指定的包数据转换到指定的文件夹
    bool Convertpackagetofolder(const QByteArray& packageData,const QString& filePath);
    /// 得到指定文件夹下所有文件的信息和md5码
    QVector<tagFileChangedData> GetFileListMd5();

    /// 注册用户
    void registerUser(const QString& userName,const QString& userpwd,int type,const QString& projects="",const QString& devices="");
    /// 更改用户数据
    void updateUser(int userid,const QString& userName,const QString& userpwd,int type,const QString& projects="",const QString& devices="");
    /// 删除用户
    void deleteUser(int userid);
    /// 得到用户的信息
    void getUserINfo(int userId);
    /// 登陆用户
    void loginUser(const QString& userName,const QString& userPwd);
    /// 得到当前玩家数据
    tagUserData* getUserData();
    /// 得到相应文件的MD5
    QString getFileMd5(const QString &path);

    /// 添加一个主项目
    void addMainProject(const QString& proName,const QString& proDescribe);
    /// 添加一个子项目
    void addSubProject(int mainId,const QString& proName,const QString& proDescribe);
    /// 添加一个版本
    void addVersion(int mainId,int subId,int type,const QString& log,QVector<tagFileChangedData> &pFileChangedData);
    /// 添加一个版本数据
    void addVersionData(int mainId,int subId,int verid,const QByteArray& data);
    /// 得到版本数据
    void getVersionData(int id);
    /// 得到指定项目最新版本的数据
    void getLastVersionData(int mainId,int subId);
    /// 得到指定ID的项目的信息
    void getProjectInfo(int mainId,const QVector<int>& subIds);
    /// 得到指定ID的项目版本信息
    void getProjectVersionsInfo(int mainId,int usbId);
    /// 删除指定ID的项目信息
    void delProject(int type,int mainId,int usbId,int id);

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
    /// 处理二进制文件发送，接收进度，type:0发送，1接收
    void OnProcessBinaryOperProcess(QWebSocket *conn,int type,qint64 sendsize,qint64 totalsize) override;

signals:
    /// 处理接收到的网络消息
    void on_signalNetText(QWebSocket *conn,QString data);
    /// 处理上传/下载数据的进度
    void on_signalOperProcess(int type,qint64 sendsize,qint64 totalsize);
    /// 处理网络二进制消息
    void on_signalNetBinary(QWebSocket *conn,QByteArray &data);
    /// 处理一个新的连接到达
    void on_signalConnectedNetMes(QWebSocket *conn);
    /// 处理一个连接关闭
    void on_signalDisconnectedNetMes(QWebSocket *conn);
    /// 日志消息处理
    void on_signalemitLogMessage(QString logMsg);

private:
    CWebSocketClient m_WeSocketClient;                   /**< 用于网络操作 */
    QString m_currentWorkingPath;                        /**< 当前工作目录 */
    QVector<tagFileChangedData> m_fileChangedlist;       /**< 文件改变列表 */
};

#endif // CSVNVERSIONCLIENT_H
