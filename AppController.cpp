#include "AppController.h"

AppController& AppController::instance()
{
    static AppController instance;
    return instance;
}

AppController::AppController()
    : login(new LoginRegisterForm)
    , client(new KuGouClient)
    , server(new KuGouServer)
{
    login->hide();
    client->hide();
}

AppController::~AppController()
{
    delete login;
    delete client;
    delete server;
}

void AppController::start()
{
    login->show();
    connect(login,&QDialog::accepted,this,[this] {
        //qDebug()<<"登录界面隐藏,显示客户端";
        client->show();  // 再启动客户端
    });

}