/**
* @file main.cpp
 * @brief 程序入口点，负责初始化并启动服务器。
 *
 * 此文件是服务器应用程序的入口，负责创建并启动服务器实例。
 *
 * @author WeiWang
 * @date 2025-05-12
 */

#include "Server.h"

/**
 * @brief 主函数，程序的入口点。
 *
 * 此函数初始化 Qt 核心应用程序，创建 Server 实例，并启动事件循环以处理服务器操作。
 *
 * @param argc 命令行参数的数量。
 * @param argv 命令行参数的数组。
 * @return int 程序退出状态。
 */
int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    Server server;
    return a.exec();
}