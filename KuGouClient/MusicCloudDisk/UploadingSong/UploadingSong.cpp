/**
 * @file UploadingSong.cpp
 * @brief 实现 UploadingSong 类，管理正在上传歌曲界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "UploadingSong.h"
#include "ui_UploadingSong.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化正在上传歌曲界面
 * @param parent 父控件指针，默认为 nullptr
 */
UploadingSong::UploadingSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadingSong)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/uploading.css")); ///< 加载样式表
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
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
UploadingSong::~UploadingSong()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 设置上传、开始、暂停和清除按钮图标
 */
void UploadingSong::initUi()
{
    ui->title_widget->setStyleSheet("font-family: 'TaiwanPearl';font-size: 13px;");
    ui->cloud_upload_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/upload-white.svg"))); ///< 设置上传按钮图标
    ui->cloud_start_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-gray.svg"))); ///< 设置开始按钮图标
    ui->cloud_pause_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/stop-gray.svg"))); ///< 设置暂停按钮图标
    ui->cloud_clear_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-gray.svg"))); ///< 设置清除按钮图标
}

/**
 * @brief 上传按钮点击槽函数
 * @note 显示未实现提示
 */
void UploadingSong::on_cloud_upload_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能暂未实现 敬请期待").arg(ui->cloud_upload_toolButton->text()),
                               1000, this->window());    ///< 显示提示
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索信号
 */
void UploadingSong::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 触发搜索信号
}