#include "AppController.h"

AppController& AppController::instance()
{
    static AppController instance;
    return instance;
}

AppController::AppController()
    : client(new KuGouClient), server(new KuGouServer)
{
}

AppController::~AppController()
{
    delete client;
    delete server;
}

void AppController::start()
{
    client->show();  // 再启动客户端
}