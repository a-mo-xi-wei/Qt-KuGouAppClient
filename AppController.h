#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include "KuGouClient.h"  // 客户端模块
#include "KuGouServer.h"    // 服务器模块

class AppController : public QObject
{
    Q_OBJECT
public:
    static AppController& instance();  // 单例模式
    void start();                      // 启动应用程序

private:
    AppController();                   // 私有构造函数
    ~AppController() override;

    KuGouClient *client;               // 客户端实例
    KuGouServer *server;               // 服务器实例
};

#endif // APPCONTROLLER_H