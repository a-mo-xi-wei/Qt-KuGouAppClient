/**
* @file CollectVideo.cpp
 * @brief 实现 CollectVideo 类，管理收藏视频界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "CollectVideo.h"
#include "ui_CollectVideo.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化收藏视频界面
 * @param parent 父控件指针，默认为 nullptr
 */
CollectVideo::CollectVideo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CollectVideo)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css")); ///< 加载样式表
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
CollectVideo::~CollectVideo()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索更多音乐的信号
 */
void CollectVideo::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 发出搜索信号
}