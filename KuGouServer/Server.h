//
// Created by WeiWang on 25-3-9.
//

#ifndef SERVER_H
#define SERVER_H

#include "chttpserver.h"

class Server : public NetworkFrameManager {
public:
    Server();

    /// 处理http请求
    bool OnProcessHttpAccepted(QObject *obj, const QPointer<JQHttpServer::Session> &session) override;

private:
    chttpserver *httpserver;
};


#endif //SERVER_H
