/**
* @file AllPaid.cpp
 * @brief 实现 AllPaid 类，提供付费音乐界面功能
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "AllPaid.h"
#include "ui_AllPaid.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化付费音乐界面
 * @param parent 父控件指针，默认为 nullptr
 */
AllPaid::AllPaid(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllPaid)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/paid.css")); ///< 加载样式表
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
AllPaid::~AllPaid()
{
    delete ui;
}

/**
 * @brief 处理搜索按钮点击
 */
void AllPaid::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 发射搜索更多音乐信号
}