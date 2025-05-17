/**
* @file main.cpp
 * @brief 程序入口文件，初始化日志和应用程序
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#include "KuGouApp.h"
#include "logger.hpp"

#include <QApplication>

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
 * @note 初始化日志、设置日志级别、启动应用程序
 */
int main(int argc, char *argv[])
{
    // 初始化日志
    if (!mylog::logger::get().init("../logs/main.log")) {
        return 1; ///< 日志初始化失败，退出
    }
    mylog::logger::get().set_level(spdlog::level::info); ///< 设置日志级别为 info

    // 三种日志输出方式
    STREAM_INFO() << "STREAM_INFO : 程序开始（info）" << 1; ///< 流式日志
    PRINT_INFO("PRINT_INFO : 程序开始（info）%d", 1);      ///< 格式化日志
    LOG_INFO("LOG_INFO : 程序开始（info）{}", 1);           ///< 模板日志

    // @note 未使用，保留用于调试
    // qInstallMessageHandler(myMessageHandler);

    QApplication a(argc, argv); ///< 创建应用程序
    KuGouApp w;                ///< 创建主窗口
    w.show();                  ///< 显示主窗口
    return a.exec();           ///< 进入事件循环
}