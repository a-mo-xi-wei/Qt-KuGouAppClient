#ifndef CHTTPSERVER_H
#define CHTTPSERVER_H

#include "QsLog.h"
#include "networkframemanager.h"
#include "NedAllocatedObject.h"
#include "resourcemanager.h"

#include <QObject>
#include <QMutex>

class chttpserver : public QObject,public NedAllocatedObject
{
    Q_OBJECT

public:
    explicit chttpserver(QObject *parent=nullptr,bool enablessl=false);
    ~chttpserver() override;

    // 设置处理框架管理
    void setNetworkFrameManager(NetworkFrameManager *nfm)
    {
        m_NetworkFrameManager = nfm;
    }
    // 得到处理框架管理
    NetworkFrameManager* getNetworkFrameManager() const { return m_NetworkFrameManager; }

    // 开始侦听
    bool listen(int port=8080,QString server_crt="",QString server_key="");
    // 检测当前服务器是否在运行
    bool isRunning() const { return m_serverIsRunning; }
    // 得到资源管理器
    CResourceManager* getResourceManager() { return &m_ResourceManager; }
    // 设置当前工作目录
    void setCurrentWorkingPath(const QString &path) { m_currentWorkingPath = path; }
    // 得到当前工作目录
    QString getCurrentWorkingPath() { return m_currentWorkingPath; }
    // 处理http传输过来的数据
    void onProcessHttpAccepted(const QPointer<JQHttpServer::Session> &session,QObject *mainObj);
    // 是否启用ssl
    void enableSSL(bool isenable) { m_isEnableSSL = isenable; }
    // 返回是否启用ssl
    bool isenableSSL() { return m_isEnableSSL; }
    // 得到端口
    int getPort()
    {
        int tmpPort = m_htmlServerManage.getListenPort();

#ifndef QT_NO_SSL
        if(m_isEnableSSL)
            tmpPort = m_htmlSslServerManage.getListenPort();
#endif

        return tmpPort;
    }

private:
    // 处理http请求
    static void onHttpAccepted(const QPointer<JQHttpServer::Session> &session,QObject *mainObj);
    // 处理内部文件导入
    bool onProcessLocalFileLoad(const QPointer<JQHttpServer::Session> &session);
    // 打印用户日志
    void printLog(QsLogging::Level type,QString msg);
    // 得到当前时间
    QString getCurrentDate();

private:
    bool m_serverIsRunning;                  /**< 服务器是否在运行 */
    CResourceManager m_ResourceManager;      /**< 资源管理器 */
    QString m_currentWorkingPath;            /**< 当前工作目录 */
    bool m_isEnableSSL;                      /**< 是否启用SSL */
    NetworkFrameManager *m_NetworkFrameManager;
    JQHttpServer::TcpServerManage m_htmlServerManage;
#ifndef QT_NO_SSL
    JQHttpServer::SslServerManage m_htmlSslServerManage;
#endif
    QMutex m_htmlServerMutex;
};

#endif // CHTTPSERVER_H
