//
// Created by WeiWang on 25-3-9.
//

#include "Server.h"

Server::Server() {

}

bool Server::OnProcessHttpAccepted(QObject *obj, const QPointer<JQHttpServer::Session> &session) {
    return NetworkFrameManager::OnProcessHttpAccepted(obj, session);
}
