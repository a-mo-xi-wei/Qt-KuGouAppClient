/**
* @file ListenRecentlyPlay.cpp
 * @brief 实现 ListenRecentlyPlay 类，提供最近播放界面功能
 * @author WeiWang
 * @date 2025-02-02
 * @version 1.0
 */

#include "ListenRecentlyPlay.h"
#include "ui_ListenRecentlyPlay.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近播放界面
 * @param parent 父控件指针，默认为 nullptr
 */
ListenRecentlyPlay::ListenRecentlyPlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenRecentlyPlay)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/play.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 应用样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";      ///< 记录错误日志
            return;
        }
    }
}

/**
 * @brief 析构函数，清理资源
 */
ListenRecentlyPlay::~ListenRecentlyPlay()
{
    delete ui;
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发切换到推荐界面的信号
 */
void ListenRecentlyPlay::on_search_pushButton_clicked()
{
    emit switch_to_listen_recommend();                   ///< 发出切换信号
}