#include "networkframemanager.h"
//#include "cwebsocketserver.h"

NetworkFrameManager::NetworkFrameManager()
= default;

NetworkFrameManager::~NetworkFrameManager()
= default;

/**
 * @brief NetworkFrameManager::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn 到达的新的连接
 */
void NetworkFrameManager::OnProcessConnectedNetMes(QWebSocket *conn)
{    

}

/**
 * @brief NetworkFrameManager::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn 要断开的连接
 */
void NetworkFrameManager::OnProcessDisconnectedNetMes(QWebSocket *conn)
{

}

/**
 * @brief NetworkFrameManager::OnProcessNetBinary 处理网络二进制消息
 * @param conn 要处理的客户端
 * @param data 到达的二进制消息
 */
void NetworkFrameManager::OnProcessNetBinary(QWebSocket *conn,QByteArray &data)
{

}

/**
 * @brief NetworkFrameManager::OnProcessNetText 处理网络字符串消息
 * @param conn 要处理的客户端
 * @param mes 到达的字符串消息
 */
void NetworkFrameManager::OnProcessNetText(QWebSocket *conn,QString mes)
{

}

/**
 * @brief NetworkFrameManager::OnProcessSendFile 处理文件发送（客户端使用）
 * @param conn 要处理的客户端
 * @param file 要处理的文件的绝对路径
 * @param sendsize 当前发送的文件数据大小
 * @param totalsize 当前文件总的大小
 */
void NetworkFrameManager::OnProcessSendFile(QWebSocket *conn,QString file,qint64 sendsize,qint64 totalsize)
{

}

/**
 * @brief NetworkFrameManager::OnProcessRecvFile 处理文件接收
 * @param conn 要处理的客户端 
 * @param srcfile 原始文件名称
 * @param decfile 保存的文件
 * @param pFileRecvError 状态码
 */
void NetworkFrameManager::OnProcessRecvFile(QWebSocket *conn,QString srcfile,QString decfile,FileRecvError pFileRecvError)
{

}

/**
 * @brief NetworkFrameManager::OnProcessBinaryOperProcess 处理二进制文件发送，接收进度，type:0发送，1接收
 * @param conn 要处理的客户端
 * @param type 0 - 上传数据； 1 - 下载数据
 * @param sendsize 实际处理的数据大小
 * @param totalsize 总的要处理的数据大小
 */
void NetworkFrameManager::OnProcessBinaryOperProcess(QWebSocket *conn,int type,qint64 sendsize,qint64 totalsize)
{

}

/**
 * @brief NetworkFrameManager::OnProcessNetBinary 处理网络二进制消息
 * @param conn
 * @param data
 */
void NetworkFrameManager::OnProcessNetBinary(QTcpSocket *conn,QByteArray &data)
{

}

/**
 * @brief NetworkFrameManager::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn
 */
void NetworkFrameManager::OnProcessConnectedNetMes(QTcpSocket *conn)
{

}

/**
 * @brief NetworkFrameManager::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn
 */
void NetworkFrameManager::OnProcessDisconnectedNetMes(QTcpSocket *conn)
{

}

/**
 * @brief NetworkFrameManager::OnProcessSerialPortBinary 处理串口二进制消息
 * @param data
 */
void NetworkFrameManager::OnProcessSerialPortBinary(QSerialPort *serialport,QByteArray &data)
{

}

/**
 * @brief NetworkFrameManager::OnProcessHttpAccepted 处理http请求
 * @param session
 */
bool NetworkFrameManager::OnProcessHttpAccepted(QObject *obj,const QPointer< JQHttpServer::Session > &session)
{
    return false;
}
