/**
 * @file main.cpp
 * @brief 程序入口文件，初始化日志和单实例应用程序
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#include "KuGouApp.h"
#include "logger.hpp"

#include <QApplication>
#include <QFileInfo>
#include <QLocalSocket>
#include <QLocalServer>

#define TIME_OUT (500) ///< 本地 socket 通信超时时间（毫秒）

/**
 * @brief 单实例应用程序类，继承自 QApplication
 * @note 确保程序只有一个实例运行，并支持实例间通信和窗口激活
 */
class QSingleApplication : public QApplication
{
    Q_OBJECT
public:
    QSingleApplication(int &argc, char **argv)
        : QApplication(argc, argv), w(nullptr), _isRunning(false), _localServer(nullptr)
    {
        _serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
        _initLocalConnection();
    }

    bool isRunning() const { return _isRunning; }

    QWidget *w; ///< 主窗口指针

private slots:
    /**
     * @brief 处理新连接，激活主窗口
     * @note 当新实例尝试运行时触发
     */
    void _newLocalConnection()
    {
        if (QLocalSocket *socket = _localServer->nextPendingConnection())
        {
            socket->waitForReadyRead(2 * TIME_OUT);
            delete socket;
            // 其他处理，如：读取启动参数
            _activateWindow();
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

    /**
     * @brief 激活主窗口
     * @note 将主窗口显示并置于前台
     */
    void _activateWindow() const
    {
        if (w)
        {
            w->showNormal();
            w->activateWindow();
            w->raise();
        }
    }

    bool _isRunning;           ///< 是否已有实例运行
    QLocalServer *_localServer; ///< 本地 socket 服务器
    QString _serverName;       ///< 服务器名称（基于应用程序文件名）
};

/**
 * @brief 自定义 Qt 消息处理函数
 * @param type 消息类型
 * @param context 消息上下文
 * @param msg 消息内容
 * @note 未使用，保留用于调试
 */
/*
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "[Qt Message] %s (File: %s, Line: %d, Function: %s)\n",
            localMsg.constData(), context.file, context.line, context.function);
}
*/

/**
 * @brief 主函数，程序入口
 * @param argc 参数数量
 * @param argv 参数数组
 * @return 程序退出码
 * @note 初始化日志、设置日志级别、启动单实例应用程序
 */
int main(int argc, char *argv[])
{
    // 初始化日志
    if (!mylog::logger::get().init("../logs/main.log"))
    {
        return 1; ///< 日志初始化失败，退出
    }
    mylog::logger::get().set_level(spdlog::level::info); ///< 设置日志级别为 info

    // 三种日志输出方式
    STREAM_INFO() << "STREAM_INFO : 程序开始（info）" << 1; ///< 流式日志
    PRINT_INFO("PRINT_INFO : 程序开始（info）%d", 1);       ///< 格式化日志
    LOG_INFO("LOG_INFO : 程序开始（info）{}", 1);           ///< 模板日志

    // @note 未使用，保留用于调试
    // qInstallMessageHandler(myMessageHandler);

    QSingleApplication a(argc, argv); ///< 创建单实例应用程序
    if (a.isRunning())
    {
        LOG_INFO("另一个实例已在运行，退出");
        return 0; ///< 已有实例运行，退出程序
    }

    KuGouApp w; ///< 创建主窗口
    a.w = &w;   ///< 将主窗口指针赋值给 QSingleApplication
    w.show();   ///< 显示主窗口
    return a.exec(); ///< 进入事件循环
}
#include "main.moc"