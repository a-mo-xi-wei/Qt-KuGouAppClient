/**
 * @file Video.cpp
 * @brief 实现 Video 类，提供视频界面管理功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "Video.h"
#include "ui_Video.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "Async.h"

#include <QButtonGroup>
#include <QFile>
#include <QTimer>
#include <QtConcurrent/qtconcurrentrun.h>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化视频界面
 * @param parent 父控件指针，默认为 nullptr
 */
Video::Video(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::Video)
      , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css"));
    if (file.open(QIODevice::ReadOnly)) {
        setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
    initUi();
    initStackedWidget();
    connect(ui->stackedWidget,
            &SlidingStackedWidget::animationFinished,
            [this] {
                if (isNumberInitialized)
                    enableButton(true);
            });
    enableButton(false);
}

/**
 * @brief 析构函数，清理资源
 */
Video::~Video()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
*/
QWidget *Video::createPage(const int &id)
{
    QWidget *page = nullptr;
    switch (id) {
    case 0: // Video Channel
        if (!m_videoChannelWidget) {
            m_videoChannelWidget = std::make_unique<VideoChannelWidget>(ui->stackedWidget);
        }
        connect(m_videoChannelWidget.get(),
                &VideoChannelWidget::initialized,
                this,
                [this] {
                    QMetaObject::invokeMethod(this,
                                              "emitInitialized",
                                              Qt::QueuedConnection,
                                              Q_ARG(bool, true));
                    if (ui->stackedWidget->isSlideAnimationFinished()) {
                        enableButton(true);
                    }
                });

        page = m_videoChannelWidget.get();
        break;
    case 1: // MV
        if (!m_MVWidget) {
            m_MVWidget = std::make_unique<MVWidget>(ui->stackedWidget);
        }
        connect(m_MVWidget.get(),
                &MVWidget::initialized,
                this,
                [this] {
                    QMetaObject::invokeMethod(this,
                                              "emitInitialized",
                                              Qt::QueuedConnection,
                                              Q_ARG(bool, true));
                    if (ui->stackedWidget->isSlideAnimationFinished()) {
                        enableButton(true);
                    }
                });
        page = m_MVWidget.get();
        break;
    case 2: // Video
        if (!m_videoWidget) {
            m_videoWidget = std::make_unique<VideoWidget>(ui->stackedWidget);
        }
        connect(m_videoWidget.get(),
                &VideoWidget::initialized,
                this,
                [this] {
                    QMetaObject::invokeMethod(this,
                                              "emitInitialized",
                                              Qt::QueuedConnection,
                                              Q_ARG(bool, true));
                    if (ui->stackedWidget->isSlideAnimationFinished()) {
                        enableButton(true);
                    }
                });
        page = m_videoWidget.get();
        break;
    default:
        qWarning() << "[WARNING] Invalid page ID:" << id;
        return nullptr;
    }
    return page;
}

/**
 * @brief 初始化界面
 */
void Video::initUi()
{
    ElaToolTip *toolTips[] = {
        new ElaToolTip(ui->video_channel_pushButton),
        new ElaToolTip(ui->MV_pushButton),
        new ElaToolTip(ui->video_pushButton)
    };
    const QString toolTipTexts[] = {
        QStringLiteral("视频频道"),
        QStringLiteral("MV"),
        QStringLiteral("视频")
    };

    for (int i = 0; i < 3; ++i) {
        toolTips[i]->setToolTip(toolTipTexts[i]);
    }

    QLabel *idxLabels[] = {ui->index_label1, ui->index_label2, ui->index_label3};
    for (int i = 0; i < 3; ++i) {
        idxLabels[i]->setPixmap(QPixmap(":/Res/window/index_lab.svg"));
        idxLabels[i]->setVisible(i == 0);
    }

    ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
    ui->stackedWidget->setSpeed(400);
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
}

/**
 * @brief 初始化堆栈窗口
 */
void Video::initStackedWidget()
{
    m_buttonGroup->addButton(ui->video_channel_pushButton, 0);
    m_buttonGroup->addButton(ui->MV_pushButton, 1);
    m_buttonGroup->addButton(ui->video_pushButton, 2);
    m_buttonGroup->setExclusive(true);

    for (int i = 0; i < 3; ++i) {
        auto *placeholder = new QWidget;
        auto *layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    m_pages[0]->layout()->addWidget(createPage(0));
    ui->stackedWidget->setCurrentIndex(0);

    connect(m_buttonGroup.get(),
            &QButtonGroup::idClicked,
            this,
            [this](int id) {
                if (m_currentIdx == id)
                    return;

                enableButton(false);
                isNumberInitialized = false;
                QMetaObject::invokeMethod(this,
                                          "emitInitialized",
                                          Qt::QueuedConnection,
                                          Q_ARG(bool, false));

                // 2. 清理旧页面

                QWidget *placeholder = m_pages[m_currentIdx];
                if (!placeholder) {
                    qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
                    enableButton(true);
                    return;
                }

                QLayout *layout = placeholder->layout();
                if (!layout) {
                    layout = new QVBoxLayout(placeholder);
                    layout->setContentsMargins(0, 0, 0, 0);
                    layout->setSpacing(0);
                } else {
                    while (QLayoutItem *item = layout->takeAt(0)) {
                        if (QWidget *widget = item->widget()) {
                            widget->deleteLater();
                        }
                        delete item;
                    }

                    switch (m_currentIdx) {
                    case 0: m_videoChannelWidget.reset();
                        break;
                    case 1: m_MVWidget.reset();
                        break;
                    case 2: m_videoWidget.reset();
                        break;
                    default: break;
                    }
                }

                // 3. 使用单次定时器确保在主线程创建控件
                QTimer::singleShot(0,
                                   this,
                                   [this, id] {
                                       // 1. 创建控件
                                       QWidget *rawWidget = createPage(id);
                                       if (!rawWidget) {
                                           enableButton(true);
                                           return;
                                       }

                                       // 2. 设置父对象并添加到布局
                                       QWidget *placeholder = m_pages[id];
                                       placeholder->layout()->addWidget(rawWidget);

                                       // 4. 执行页面切换
                                       ui->stackedWidget->slideInIdx(id);
                                       m_currentIdx = id;

                                       // 更新索引标签
                                       QLabel *idxLabels[] = {
                                           ui->index_label1, ui->index_label2, ui->index_label3};
                                       for (int i = 0; i < 3; ++i) {
                                           idxLabels[i]->setVisible(i == id);
                                       }

                                       STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().
                                           toStdString() << " 界面";
                                       placeholder->updateGeometry();
                                       placeholder->adjustSize();
                                       ui->stackedWidget->updateGeometry();
                                       ui->stackedWidget->update();
                                   });
            });

    ui->video_channel_pushButton->click();
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void Video::enableButton(bool flag) const
{
    ui->video_channel_pushButton->setEnabled(flag);
    ui->MV_pushButton->setEnabled(flag);
    ui->video_pushButton->setEnabled(flag);
}