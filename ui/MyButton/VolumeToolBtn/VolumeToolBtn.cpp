/**
 * @file VolumeToolBtn.cpp
 * @brief 实现 VolumeToolBtn 类，提供音量调节按钮功能
 * @author [WeiWang]
 * @date 2025-05-15
 * @version 1.0
 */

#include "VolumeToolBtn.h"
#include "ElaToolTip.h"
#include <QCoreApplication>
#include <QTimer>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

/**
 * @brief 构造函数，初始化音量调节按钮
 * @param parent 父控件指针，默认为 nullptr
 */
VolumeToolBtn::VolumeToolBtn(QWidget *parent)
    : QToolButton(parent)
    , m_volumeWidget(std::make_unique<MyWidget>())
    , m_volumeLab(std::make_unique<QLabel>())
    , m_volumeSlider(std::make_unique<SliderWidget>())
    , m_leaveTimer(new QTimer(this))
    , m_positionCheckTimer(new QTimer(this))
    , m_isNoVolume(false)
{
    initUi();

    // 设置离开定时器为单次触发
    m_leaveTimer->setSingleShot(true);
    connect(m_leaveTimer, &QTimer::timeout, this, [this] { this->m_volumeWidget->hide(); });

    // 设置鼠标位置检查定时器
    m_positionCheckTimer->setInterval(100);
    connect(m_positionCheckTimer, &QTimer::timeout, this, &VolumeToolBtn::checkMousePosition);

    // 连接点击事件，切换静音状态
    connect(this, &QToolButton::clicked, this, [this] {
        m_isNoVolume = !m_isNoVolume;
        if (m_isNoVolume) {
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-blue.svg")));
            emit m_volumeWidget->noVolume(true);
        } else {
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-blue.svg")));
            emit m_volumeWidget->noVolume(false);
        }
    });

    // 连接滑块值变化事件，更新音量标签和信号
    connect(this->m_volumeSlider.get(), &QSlider::valueChanged, [this] {
        this->m_volumeLab->setText(" " + QString::number(this->m_volumeSlider->getValue()) + "%");
        emit volumeChange(this->m_volumeSlider->getValue());
    });

    // 连接滑块静音信号
    connect(this->m_volumeSlider.get(), &SliderWidget::noVolume, this, &VolumeToolBtn::onNoVolume);
}

/**
 * @brief 初始化音量调节控件
 */
void VolumeToolBtn::initVolumeWidget() {
    // 设置音量控件的父窗口
    this->m_volumeParent = this->window();
    if (this->m_volumeParent) {
        this->m_volumeWidget->setParent(m_volumeParent);
        m_volumeParent->installEventFilter(this);
    } else {
        qDebug() << "m_volumeParent is nullptr";
    }

    // 初始隐藏音量控件
    this->m_volumeWidget->hide();

    // 设置音量标签和滑块的父控件
    this->m_volumeLab->setParent(this->m_volumeWidget.get());
    this->m_volumeSlider->setParent(this->m_volumeWidget.get());

    // 配置滑块
    this->m_volumeSlider->setOrientation(Qt::Vertical);
    this->m_volumeSlider->setFixedSize(40, 135);
    this->m_volumeSlider->setMaximum(100);
    this->m_volumeSlider->setValue(30);
    this->m_volumeSlider->setContentsMargins(0, 20, 0, 10);

    // 配置音量标签
    this->m_volumeLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    this->m_volumeLab->setContentsMargins(0, 0, 0, 0);
    this->m_volumeLab->setText(" " + QString::number(this->m_volumeSlider->getValue()) + "%");

    // 设置布局
    auto hBoxLayout = new QHBoxLayout;
    hBoxLayout->setAlignment(Qt::AlignCenter);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->addWidget(this->m_volumeSlider.get());

    this->m_vBoxLayout = new QVBoxLayout(this->m_volumeWidget.get());
    this->m_vBoxLayout->setAlignment(Qt::AlignCenter);
    this->m_vBoxLayout->setSpacing(5);
    this->m_vBoxLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    this->m_vBoxLayout->addLayout(hBoxLayout);
    this->m_vBoxLayout->addWidget(this->m_volumeLab.get());
    this->m_vBoxLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

/**
 * @brief 初始化按钮界面
 */
void VolumeToolBtn::initUi() {
    this->setStyleSheet(QStringLiteral("border:none;"));
    this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")));
    initVolumeWidget();
}

/**
 * @brief 检查鼠标位置，控制音量控件显示
 */
void VolumeToolBtn::checkMousePosition() const {
    QPoint globalMousePos = QCursor::pos();
    if (!this->m_volumeWidget->isHidden()) {
        if (this->m_volumeWidget->geometry().contains(this->m_volumeParent->mapFromGlobal(globalMousePos))) {
            this->m_volumeWidget->show();
            if (m_leaveTimer->isActive()) {
                m_leaveTimer->stop();
            }
        } else {
            if (!m_leaveTimer->isActive()) {
                this->m_leaveTimer->start(500);
            }
        }
    } else {
        if (this->m_positionCheckTimer->isActive()) {
            this->m_positionCheckTimer->stop();
        }
    }
}

/**
 * @brief 获取音量控件位置
 */
void VolumeToolBtn::getVolumeWidgetPosition() {
    this->m_volumePosition = this->m_volumeParent->mapFromGlobal(this->mapToGlobal(QPoint(0, 0)));
    this->m_volumePosition -= QPoint(20, this->m_volumeWidget->height() + 10);
}

/**
 * @brief 获取当前音量值
 * @return 音量值
 */
int VolumeToolBtn::getVolumeValue() const {
    return this->m_volumeSlider->getValue();
}

/**
 * @brief 更新按钮图标
 * @param isHovered 是否悬浮，默认为 false
 */
void VolumeToolBtn::updateIcon(bool isHovered) {
    if (m_isNoVolume) {
        setIcon(QIcon(isHovered ? ":/Res/playbar/volume-off-blue.svg"
                                : ":/Res/playbar/volume-off-gray.svg"));
    } else {
        setIcon(QIcon(isHovered ? ":/Res/playbar/volume-on-blue.svg"
                                : ":/Res/playbar/volume-on-gray.svg"));
    }
}

/**
 * @brief 设置音量值
 * @param value 音量值
 */
void VolumeToolBtn::setVolume(const int &value) const {
    this->m_volumeSlider->setValue(value);
}

/**
 * @brief 鼠标进入事件，显示音量控件
 * @param event 进入事件
 */
void VolumeToolBtn::enterEvent(QEnterEvent *event) {
    QToolButton::enterEvent(event);
    updateIcon(true);
    getVolumeWidgetPosition();
    this->m_volumeWidget->move(this->m_volumePosition);
    this->m_volumeWidget->show();
    // 鼠标进入时取消定时器
    if (m_leaveTimer->isActive()) {
        m_leaveTimer->stop();
    }
    // 鼠标在 m_volumeWidget 内，取消定时器
    if (this->m_positionCheckTimer->isActive()) {
        this->m_positionCheckTimer->stop();
    }
}

/**
 * @brief 鼠标离开事件，延迟隐藏音量控件
 * @param event 事件
 */
void VolumeToolBtn::leaveEvent(QEvent *event) {
    QToolButton::leaveEvent(event);
    updateIcon(false);
    // 鼠标离开时启动1秒的延迟定时器
    m_leaveTimer->start(500);
    // 启动定时器开始检查鼠标位置
    m_positionCheckTimer->start();
}

/**
 * @brief 控件显示事件，更新音量控件位置
 * @param event 显示事件
 */
void VolumeToolBtn::showEvent(QShowEvent *event) {
    QToolButton::showEvent(event);
    getVolumeWidgetPosition();
    this->m_volumeWidget->move(this->m_volumePosition);
}

/**
 * @brief 事件过滤器，处理主窗口事件
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 */
bool VolumeToolBtn::eventFilter(QObject *watched, QEvent *event) {
    if (watched == m_volumeParent) {
        if (event->type() == QEvent::Resize || event->type() == QEvent::Move) {
            // 主窗口调整大小或移动时更新音量部件位置
            if (m_volumeWidget->isVisible()) {
                getVolumeWidgetPosition();
                m_volumeWidget->move(m_volumePosition);
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            /*// 鼠标点击主窗口时，检查是否在音量控件或按钮外
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint globalClickPos = mouseEvent->globalPosition().toPoint();
            QPoint localClickPos = m_volumeParent->mapFromGlobal(globalClickPos);

            // 判断点击位置是否在 m_volumeWidget 或 VolumeToolBtn 内
            bool isInsideVolumeWidget = m_volumeWidget->geometry().contains(localClickPos);
            bool isInsideButton = this->geometry().contains(this->mapFromGlobal(globalClickPos));

            if (!isInsideVolumeWidget && !isInsideButton) {
                // 点击外部区域，立即隐藏音量控件
                m_volumeWidget->hide();
                m_leaveTimer->stop(); // 停止可能正在运行的计时器
            }*/
            QCoreApplication::sendEvent(this, new QEvent(QEvent::Leave));
        }
    }
    return QToolButton::eventFilter(watched, event);
}

/**
 * @brief 处理静音状态变化
 * @param flag 是否静音
 */
void VolumeToolBtn::onNoVolume(bool flag) {
    if (flag) {
        if (!this->m_isNoVolume) {
            this->m_isNoVolume = true;
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-gray.svg")));
        }
    } else {
        if (this->m_isNoVolume) {
            this->m_isNoVolume = false;
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")));
        }
    }
}