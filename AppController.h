#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "KuGouClient.h"  // 客户端模块
#include "KuGouServer.h"    // 服务器模块
#include "login_register_form.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    static AppController& instance();  // 单例模式
    void start();                      // 启动应用程序

private:
    void initFontRes();

private:
    AppController();                   // 私有构造函数
    ~AppController() override;

    LoginRegisterForm   *login;                // 登录界面实例
    KuGouClient         *client;               // 客户端实例
    KuGouServer         *server;               // 服务器实例
};

#endif // APPCONTROLLER_H