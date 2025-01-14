#include "VolumeToolBtn.h"

#include<QTimer>
#include<QHBoxLayout>
#include<QLabel>

VolumeToolBtn::VolumeToolBtn(QWidget *parent)
    : QToolButton(parent)
      , m_volumeWidget(std::make_unique<MyWidget>())
      , m_volumeLab(std::make_unique<QLabel>())
      , m_volumeSlider(std::make_unique<SliderWidget>())
      , m_leaveTimer(new QTimer(this))
      , m_positionCheckTimer(new QTimer(this)) {
    initUi();
    // 设置定时器为单次触发
    m_leaveTimer->setSingleShot(true);
    connect(m_leaveTimer, &QTimer::timeout, this, [this]() { this->m_volumeWidget->hide(); });
    connect(m_positionCheckTimer, &QTimer::timeout, this, &VolumeToolBtn::checkMousePosition);
    m_positionCheckTimer->setInterval(100); // 每100毫秒检查一次
    connect(this, &QToolButton::clicked, this, [this] {
        m_isNoVolume = !m_isNoVolume;
        if (m_isNoVolume) {
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-blue.svg")));
            emit m_volumeWidget->noVolume(true);
        }
        else {
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-blue.svg")));
            emit m_volumeWidget->noVolume(false);
        }
    });
    connect(this->m_volumeSlider.get(), &QSlider::valueChanged, [this] {
        this->m_volumeLab->setText(" "+QString::number(this->m_volumeSlider->getValue()) + "%");
        emit volumeChange(this->m_volumeSlider->getValue());
    });
    connect(this->m_volumeSlider.get(), &SliderWidget::noVolume,this,[this](bool flag){onNoVolume(flag);});
}

void VolumeToolBtn::initVolumeWidget() {
    this->m_volumeParent = this->window();
    if(this->m_volumeParent)this->m_volumeWidget->setParent(m_volumeParent);
    else qDebug()<<"m_volumeParent is nullptr";
    this->m_volumeWidget->hide();
    this->m_volumeLab->setParent(this->m_volumeWidget.get());
    this->m_volumeSlider->setParent(this->m_volumeWidget.get());
    this->m_volumeSlider->setOrientation(Qt::Vertical);
    this->m_volumeSlider->setFixedSize(40,135);
    this->m_volumeSlider->setMaximum(100);
    this->m_volumeSlider->setValue(20);
    this->m_volumeSlider->setToolTip(QStringLiteral("调节音量"));
    this->m_volumeSlider->setContentsMargins(0,20,0,10);
    this->m_volumeLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    this->m_volumeLab->setContentsMargins(0,0,0,0);
    this->m_volumeLab->setText(" "+QString::number(this->m_volumeSlider->getValue()) + "%");
    auto hBoxLayout = new QHBoxLayout;
    hBoxLayout->setAlignment(Qt::AlignCenter);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->addWidget(this->m_volumeSlider.get());
    this->m_vBoxLayout = new QVBoxLayout(this->m_volumeWidget.get());
    this->m_vBoxLayout->setAlignment(Qt::AlignCenter);
    this->m_vBoxLayout->setSpacing(5);
    this->m_vBoxLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    this->m_vBoxLayout->addLayout(hBoxLayout);
    this->m_vBoxLayout->addWidget(this->m_volumeLab.get());
    this->m_vBoxLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding));
}

void VolumeToolBtn::initUi() {
    this->setStyleSheet(QStringLiteral("border:none;"));
    this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")));
    initVolumeWidget();
}

void VolumeToolBtn::checkMousePosition() const {
    QPoint globalMousePos = QCursor::pos(); // 获取全局鼠标位置
    // 检查鼠标是否在 m_volumeWidget 内
    if (!this->m_volumeWidget->isHidden()) {
        if (this->m_volumeWidget->geometry().contains(this->m_volumeParent->mapFromGlobal(globalMousePos))) {
            this->m_volumeWidget->show();
            // 鼠标进入时取消定时器
            if (m_leaveTimer->isActive())
                m_leaveTimer->stop();
        } else {
            //正在显示，但是鼠标不在范围内
            if (!m_leaveTimer->isActive())
                this->m_leaveTimer->start();
        }
    } else {
        // 如果 m_volumeWidget 已经隐藏
        if (this->m_positionCheckTimer->isActive())
            this->m_positionCheckTimer->stop();
    }
}

void VolumeToolBtn::getVolumeWidgetPosition() {
    this->m_volumePosition = this->m_volumeParent->mapFromGlobal(this->mapToGlobal(QPoint(0, 0)));
    this->m_volumePosition -= QPoint(20, this->m_volumeWidget->height() + 10);
    //qDebug() << "VolumeWidgetPosition : "<<this->m_volumePosition;
}

void VolumeToolBtn::enterEvent(QEnterEvent *event) {
    QToolButton::enterEvent(event);
    if (m_isNoVolume)this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-blue.svg")));
    else this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-blue.svg")));
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

void VolumeToolBtn::leaveEvent(QEvent *event) {
    QToolButton::leaveEvent(event);
    if (m_isNoVolume)this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-gray.svg")));
    else this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")));
    // 鼠标离开时启动1秒的延迟定时器
    m_leaveTimer->start(500);
    // 启动定时器开始检查鼠标位置
    m_positionCheckTimer->start();
}

void VolumeToolBtn::showEvent(QShowEvent *event) {
    QToolButton::showEvent(event);
    getVolumeWidgetPosition();
    //qDebug()<<this->m_volumePosition;
    this->m_volumeWidget->move(this->m_volumePosition);
}

void VolumeToolBtn::onNoVolume(bool flag) {
    if(flag) {

        if(!this->m_isNoVolume) {
            this->m_isNoVolume = true;
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-gray.svg")));
        }
    }
    else {
        //有声音
        if(this->m_isNoVolume) {//之前无声
            this->m_isNoVolume = false;
            this->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")));
        }
    }
}
