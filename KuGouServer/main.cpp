/**
 * @file main.cpp
 * @brief 程序入口点，负责初始化并启动单实例服务器。
 *
 * 此文件是服务器应用程序的入口，负责创建并启动单实例服务器，确保同一时刻只有一个实例运行。
 *
 * @author WeiWang
 * @date 2025-05-12
 */

#include "Server.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QLocalSocket>
#include <QLocalServer>

#define TIME_OUT (500) ///< 本地 socket 通信超时时间（毫秒）

/**
 * @brief 单实例核心应用程序类，继承自 QCoreApplication
 * @note 确保程序只有一个实例运行，并支持实例间通信
 */
class QSingleCoreApplication : public QCoreApplication
{
    Q_OBJECT
public:
    QSingleCoreApplication(int &argc, char **argv)
        : QCoreApplication(argc, argv), _isRunning(false), _localServer(NULL)
    {
        _serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
        _initLocalConnection();
    }

    bool isRunning() { return _isRunning; }

private slots:
    /**
     * @brief 处理新连接
     * @note 当新实例尝试运行时触发，可扩展为通信处理
     */
    void _newLocalConnection()
    {
        QLocalSocket *socket = _localServer->nextPendingConnection();
        if (socket)
        {
            socket->waitForReadyRead(2 * TIME_OUT);
            delete socket;
            // 其他处理，如：读取启动参数
        }
    }

private:
    /**
     * @brief 初始化本地连接，检测是否已有实例运行
     * @note 如果连接上服务器，则标记为已有实例；否则创建新服务器
     */
    void _initLocalConnection()
    {
        _isRunning = false;

        QLocalSocket socket;
        socket.connectToServer(_serverName);
        if (socket.waitForConnected(TIME_OUT))
        {
            fprintf(stderr, "%s already running.\n", _serverName.toLocal8Bit().constData());
            _isRunning = true;
            // 其他处理，如：将启动参数发送到服务端
            return;
        }

        // 连接不上服务器，创建新服务器
        _newLocalServer();
    }

    /**
     * @brief 创建本地 socket 服务器
     * @note 用于监听新实例的连接请求
     */
    void _newLocalServer()
    {
        _localServer = new QLocalServer(this);
        connect(_localServer, SIGNAL(newConnection()), this, SLOT(_newLocalConnection()));
        if (!_localServer->listen(_serverName))
        {
            // 监听失败，可能是残留进程服务导致，移除后重试
            if (_localServer->serverError() == QAbstractSocket::AddressInUseError)
            {
                QLocalServer::removeServer(_serverName);
                _localServer->listen(_serverName);
            }
        }
    }

    bool _isRunning;           ///< 是否已有实例运行
    QLocalServer *_localServer; ///< 本地 socket 服务器
    QString _serverName;       ///< 服务器名称（基于应用程序文件名）
};

/**
 * @brief 主函数，程序的入口点。
 *
 * 此函数初始化 Qt 核心应用程序，创建单实例 Server，确保只有一个实例运行，并启动事件循环以处理服务器操作。
 *
 * @param argc 命令行参数的数量。
 * @param argv 命令行参数的数组。
 * @return int 程序退出状态。
 */
int main(int argc, char* argv[])
{
    QSingleCoreApplication a(argc, argv); ///< 创建单实例核心应用程序
    if (a.isRunning())
    {
        fprintf(stderr, "Another instance is already running, exiting.\n");
        return 0; ///< 已有实例运行，退出程序
    }

    Server server; ///< 创建服务器实例
    return a.exec(); ///< 进入事件循环
}
#include "main.moc"
