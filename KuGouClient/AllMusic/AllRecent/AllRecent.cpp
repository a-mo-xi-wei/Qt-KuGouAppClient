/**
* @file AllRecent.cpp
 * @brief 实现 AllRecent 类，提供最近播放音乐界面功能
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "AllRecent.h"
#include "ui_AllRecent.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近播放音乐界面
 * @param parent 父控件指针，默认为 nullptr
 */
AllRecent::AllRecent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllRecent)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/recent.css")); ///< 加载样式表
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
AllRecent::~AllRecent()
{
    delete ui;
}

/**
 * @brief 处理搜索按钮点击
 */
void AllRecent::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 发射搜索更多音乐信号
}