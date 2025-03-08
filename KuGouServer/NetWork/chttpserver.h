#ifndef CHTTPSERVER_H
#define CHTTPSERVER_H

#include "../common/common.h"
#include "../../includes/QsLog/QsLog.h"
#include "networkframemanager.h"
#include "../common/NedAllocatedObject.h"
#include "../common/resourcemanager.h"
#include "JQLibrary/jqhttpserver.h"

#include <QObject>
#include <QMutex>

class chttpserver : public QObject,public NedAllocatedObject
{
    Q_OBJECT

public:    
    explicit chttpserver(QObject *parent=NULL,bool enablessl=false);
    ~chttpserver();

    /// 设置处理框架管理
    inline void setNetworkFrameManager(NetworkFrameManager *nfm)
    {
        m_NetworkFrameManager = nfm;
    }
    /// 得到处理框架管理
    inline NetworkFrameManager* getNetworkFrameManager(void) { return m_NetworkFrameManager; }

    /// 开始侦听
    bool listen(int port=8080,QString server_crt="",QString server_key="");
    /// 检测当前服务器是否在运行
    inline bool isRunning() { return m_serverisRunning; }
    /// 得到资源管理器
    inline CResourceManager* getResourceManager() { return &m_ResourceManager; }
    /// 设置当前工作目录
    inline void setCurrentWorkingPath(QString path) { m_currentWorkingPath = path; }
    /// 得到当前工作目录
    inline QString getCurrentWorkingPath(void) { return m_currentWorkingPath; }
    /// 处理http传输过来的数据
    void onProcessHttpAccepted(const QPointer< JQHttpServer::Session > &session,QObject *mainObj);
    /// 是否启用ssl
    inline void enableSSL(bool isenable) { m_isEnableSSL = isenable; }
    /// 返回是否启用ssl
    inline bool isenableSSL(void) { return m_isEnableSSL; }
    /// 得到端口
    inline int getPort(void)
    {
        int tmpPort = m_htmlServerManage.getListenPort();

#ifndef QT_NO_SSL
        if(m_isEnableSSL)
            tmpPort = m_htmlSslServerManage.getListenPort();
#endif

        return tmpPort;
    }

signals:
    /// 日志消息
    void signalshowlog(QString msg);

private:
    /// 处理http请求
    static void onHttpAccepted(const QPointer< JQHttpServer::Session > &session,QObject *mainObj);
    /// 处理内部文件导入
    bool onprocesslocalfileload(const QPointer< JQHttpServer::Session > &session);
    /// 打印用户日志
    void printLog(QsLogging::Level type,QString msg);
    /// 得到当前时间
    QString getCurrentDate(void);

private:
    bool m_serverisRunning;                  /**< 服务器是否在运行 */
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
