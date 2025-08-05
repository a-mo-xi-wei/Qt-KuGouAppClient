#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "KuGouClient.h"  // 客户端模块
#include "KuGouServer.h"    // 服务器模块
#include "login_register_form.h"

///< 前向声明
class MyTrayIcon;

class AppController : public QObject
{
    Q_OBJECT

public:
    static AppController& instance(); ///< 单例模式
    void start();                     ///< 启动应用程序

private:
    void initFontRes();

    AppController(); ///< 私有构造函数
    ~AppController() override;

    void showSystemLoginInfo();

    MyTrayIcon* m_trayIcon;         ///< 系统托盘图标
    LoginRegisterForm* m_login;     ///< 登录界面实例
    KuGouClient* m_client;          ///< 客户端实例
    KuGouServer* m_server;          ///< 服务器实例
    bool m_isLoginAccepted = false; ///< 是否已经成功登录
};

#endif // APPCONTROLLER_H
