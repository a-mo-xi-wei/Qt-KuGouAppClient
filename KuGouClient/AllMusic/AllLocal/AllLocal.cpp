/**
* @file AllLocal.cpp
 * @brief 实现 AllLocal 类，提供本地音乐界面功能
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "AllLocal.h"
#include "ui_AllLocal.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化本地音乐界面
 * @param parent 父控件指针，默认为 nullptr
 */
AllLocal::AllLocal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllLocal)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/local.css")); ///< 加载样式表
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
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
AllLocal::~AllLocal()
{
    delete ui;
}

/**
 * @brief 初始化界面
 * @note 需要获取本地音乐列表，当前待实现
 */
void AllLocal::initUi()
{
    auto lay = ui->all_local_song_list_widget->layout(); ///< 获取歌曲列表布局
    if (lay->count() == 0)
        ui->scrollArea->hide();                          ///< 隐藏滚动区域
}

/**
 * @brief 处理搜索按钮点击
 */
void AllLocal::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 发射搜索更多音乐信号
}