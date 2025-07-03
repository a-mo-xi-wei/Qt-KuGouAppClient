/**
* @file SingerWidget.cpp
 * @brief 实现 SingerWidget 类，管理歌手界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "SingerWidget.h"
#include "ui_SingerWidget.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化歌手界面
 * @param parent 父控件指针，默认为 nullptr
 */
SingerWidget::SingerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingerWidget)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/singer.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
}

/**
 * @brief 析构函数，清理资源
 */
SingerWidget::~SingerWidget()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索更多音乐的信号
 */
void SingerWidget::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 发出搜索信号
}