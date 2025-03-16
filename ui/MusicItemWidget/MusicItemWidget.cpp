#include "MusicItemWidget.h"
#include "logger.hpp"

#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFile>
#include <QTimer>
#include <QCursor>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QGuiApplication>
#include <QPainterPath>
#include <QtMath>
#include <QPointF>

//图片大小
#define PIX_SIZE 50
//图片圆角
#define PIX_RADIUS 9
// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

QPixmap roundedPix(const QPixmap &src, QSize size, int radius) {
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);

    return dest;
}

MusicItemWidget::MusicItemWidget(SongInfor  info, QWidget *parent)
    :QFrame(parent)
    ,m_information(std::move(info))
    ,timer(new QTimer(this))
{
    this->m_index           = m_information.index;
    this->m_name            = m_information.songName;
    this->m_duration        = m_information.duration;
    this->m_cover           = m_information.cover;
    this->m_singer          = m_information.singer;
    //qDebug()<<"m_index: "<<m_index<<" name: "<<m_name<<" duration: "<<m_duration<<
    //    " cover: "<<m_cover<<"m_singer: "<<m_singer;
    //PRINT_INFO("index: %d , name: %s , duration: %s , singer: %s ",
    //    m_index, m_name.toStdString(), m_duration.toStdString(), m_singer.toStdString());
    this->m_indexLab        = new QLabel(QString("%1").arg(this->m_index + 1, 2, 10, QChar('0')), this);
    this->m_coverLab        = new QLabel(this);
    this->m_coverLab->setAlignment(Qt::AlignCenter);
    this->m_coverLab->setFixedSize(PIX_SIZE,PIX_SIZE);
    this->m_coverLab->setPixmap(roundedPix(this->m_cover,this->m_coverLab->size(),PIX_RADIUS));
    this->m_nameLab         = new QLabel(this->m_name,this);
    this->m_singerLab       = new QLabel(this->m_singer,this);
    this->m_spacerItem1     = new QSpacerItem(40,20,QSizePolicy::Expanding);
    this->m_spacerItem2     = new QSpacerItem(40,20,QSizePolicy::Fixed);
    this->m_durationLab     = new QLabel(this->m_duration,this);
    this->m_playToolBtn     = new QToolButton(this);
    this->m_playNextToolBtn = new QToolButton(this);
    this->m_downloadToolBtn = new QToolButton(this);
    this->m_collectToolBtn  = new QToolButton(this);
    this->m_moreToolBtn     = new QToolButton(this);

    this->setObjectName(QStringLiteral("window"));
    this->m_indexLab->setObjectName(QStringLiteral("indexLab"));
    this->m_coverLab->setObjectName(QStringLiteral("coverLab"));
    this->m_nameLab->setObjectName(QStringLiteral("nameLab"));
    this->m_singerLab->setObjectName(QStringLiteral("singerLab"));
    this->m_durationLab->setObjectName(QStringLiteral("durationLab"));
    this->m_playToolBtn->setObjectName(QStringLiteral("playToolBtn"));
    this->m_playNextToolBtn->setObjectName(QStringLiteral("playNextToolBtn"));
    this->m_downloadToolBtn->setObjectName(QStringLiteral("downloadToolBtn"));
    this->m_collectToolBtn->setObjectName(QStringLiteral("collectToolBtn"));
    this->m_moreToolBtn->setObjectName(QStringLiteral("moreToolBtn"));
    //设置样式
    QFile file(GET_CURRENT_DIR + QStringLiteral("/item.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表 item.css 打开失败QAQ";
        return;
    }

    timer->setInterval(timeInterval); // 设置定时器时间间隔
    max_radius = static_cast<int>(qSqrt(width() * width() + height() * height())); // 计算最大半径

    initUi();
    //smallWidget响应
    connect(this->m_playToolBtn,&QToolButton::clicked,this,&MusicItemWidget::onPlayToolBtnClicked);
    connect(this->m_playNextToolBtn,&QToolButton::clicked,this,&MusicItemWidget::onPlayNextToolBtnClicked);
    connect(this->m_downloadToolBtn,&QToolButton::clicked,this,&MusicItemWidget::onDownloadToolBtnClicked);
    connect(this->m_collectToolBtn,&QToolButton::clicked,this,&MusicItemWidget::onCollectToolBtnClicked);
    connect(this->m_moreToolBtn,&QToolButton::clicked,this,&MusicItemWidget::onMoreToolBtnClicked);
    //menu响应
    auto menu = new MyMenu(MyMenu::MenuKind::SongOption,this);
    m_songOptMenu = menu->getMenu<SongOptionMenu>();
    connect(m_songOptMenu, &SongOptionMenu::play, this, &MusicItemWidget::onPlay);
    connect(m_songOptMenu, &SongOptionMenu::deleteSong, this, &MusicItemWidget::onDeleteSong);
}

void MusicItemWidget::setIndexText(const int &index) const {
    this->m_indexLab->setText(QString("%1").arg(index, 2, 10, QChar('0')));
}

void MusicItemWidget::setInterval(const int &timeInterval) const {
    timer->setInterval(timeInterval);
}

void MusicItemWidget::setFillColor(const QColor &fillcolor) {
    fill_color = fillcolor;
}

void MusicItemWidget::setRadius(const int &radius_) {
    frame_radius = radius_;
}

void MusicItemWidget::setInformation(const SongInfor &info) {
    this->m_index = info.index;
    this->m_name = info.songName;
    this->m_duration = info.duration;
    this->m_cover = info.cover;
    this->m_singer = info.singer;
    this->m_indexLab->setText(QString("%1").arg(this->m_index, 2, 10, QChar('0')));
    this->m_coverLab->setPixmap(roundedPix(this->m_cover, this->m_coverLab->size(), PIX_RADIUS));
    this->m_nameLab->setText(this->m_name);
    this->m_singerLab->setText(this->m_singer);
    this->m_durationLab->setText(this->m_duration);
    update(); // 重绘
}

void MusicItemWidget::setPlayState(const bool &state) {
    this->m_isPlaying = state;
    if (m_isPlaying) {
        // 获取鼠标在控件内的坐标
        QPoint globalPos = QCursor::pos(); // 全局坐标
        QPoint localPos = mapFromGlobal(globalPos); // 转换为控件内坐标

        // 检查鼠标是否在控件区域内（可选）
        if (rect().contains(localPos)) {
            mouse_point = localPos;
        } else {
            // 如果鼠标不在控件内，可以设置默认值（如控件中心）
            mouse_point = rect().center();
        }

        // 启动定时器
        if (!timer->isActive()) {
            connect(timer, &QTimer::timeout, this, [=]{
                radius += radius_var;
                if (radius > max_radius) {
                    timer->stop();
                    return;
                }
                update();
            });
            timer->start();
        }
    } else {
        // 停止时强制更新鼠标位置（不论是否在控件内）
        QPoint globalPos = QCursor::pos();
        QPoint localPos = mapFromGlobal(globalPos);

        // 更新当前鼠标坐标（如果不在区域内使用中心坐标）
        mouse_point = rect().contains(localPos) ? localPos : rect().center();

        // 断开旧连接后建立收缩动画
        timer->disconnect();
        connect(timer, &QTimer::timeout, this, [=]{
            radius -= radius_var;
            if (radius < 0) {
                timer->stop();
                radius = 0; // 归零保持有效值
            }
            update();
        });
        timer->start(); // 立即启动收缩动画
    }
}

void MusicItemWidget::getMenuPosition(const QPoint& pos) {
    this->m_menuPosition = pos;
    // 获取屏幕的尺寸
    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();

    // 计算菜单右侧的全局位置
    //int menuLeftPos = pos.x() - m_menu->width();
    const int menuRightPos  = pos.x() + m_songOptMenu->width();
    const int menuBottomPos = pos.y() + m_songOptMenu->height();
    //int menuTopPos = pos.y() - m_menu->height();
    // 若菜单左侧超出屏幕左侧 (不存在)
    //if(menuLeftPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setX(10);
    //}
    // 如果菜单右侧超出屏幕右侧
    if (menuRightPos > screenGeometry.right()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuRightPos - screenGeometry.right() + 5;
        m_menuPosition.setX(pos.x() - offset);
    }
    // 如果菜单下侧超出屏幕下侧
    if (menuBottomPos > screenGeometry.bottom()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuBottomPos - screenGeometry.bottom() + 5;
        m_menuPosition.setY(pos.y() - offset);
    }
    // 如果菜单下侧超出屏幕下侧（不存在）
    //if(menuTopPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setY(10);
    //}

}

void MusicItemWidget::enterEvent(QEnterEvent *event) {
    QFrame::enterEvent(event);
    mouse_point = event->position(); // 记录鼠标进入坐标
    timer->disconnect(); // 断开可能的timer的所有连接
    connect(timer, &QTimer::timeout, this, [=]{ // 定时器触发，半径增大
        radius += radius_var;
        if (radius > max_radius) {
            timer->stop(); // 达到最大半径，定时器停止
            return;
        }
        update(); // 调用绘制事件
    });
    timer->start(); // 定时器开始
}

void MusicItemWidget::leaveEvent(QEvent *event) {
    QFrame::leaveEvent(event);
    if(!this->m_isPlaying) {
        mouse_point = mapFromGlobal(QCursor::pos());
        timer->disconnect();
        connect(timer, &QTimer::timeout, this, [=]{ // 定时器触发半径减小
            radius -= radius_var;
            if (radius < 0) {
                timer->stop(); // 减小到小于0时定时器停止
                radius = 0; // 确保半径不为负
                return;
            }
            update();
        });
        timer->start();
    }
}

void MusicItemWidget::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    if (!mouse_point.isNull() && radius > 0)
    {
        QPainter painter(this);
        QPainterPath path;
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(fill_color));
        painter.setPen(Qt::NoPen);
        path.addRoundedRect(rect(), frame_radius, frame_radius);
        painter.setClipPath(path);
        painter.drawEllipse(mouse_point, radius, radius); // 画圆
    }
}

void MusicItemWidget::resizeEvent(QResizeEvent *event) {
    QFrame::resizeEvent(event);
    max_radius = static_cast<int>(qSqrt(width() * width() + height() * height())); // 重新计算最大半径
    //设置弹簧宽度
    this->m_spacerItem2->changeSize(this->width()/5,20,QSizePolicy::Fixed);
    //this->setGeometry(this->geometry().x(),this->geometry().y(),
    //    event->size().width(),this->geometry().height());
    //qDebug()<<"当前宽度："<<this->width();
    //qDebug()<<"event->size().width() = "<<event->size().width();
    //qDebug()<<"父对象宽度："<<qobject_cast<QWidget*>(this->parent())->width();
}

void MusicItemWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QFrame::mouseDoubleClickEvent(event);
    this->m_playToolBtn->clicked();
}

void MusicItemWidget::mousePressEvent(QMouseEvent *event) {
    // 判断是否为右键点击
    if (event->button() == Qt::RightButton) {
        getMenuPosition(this->mapToGlobal(event->pos()));
        this->m_songOptMenu->move(this->m_menuPosition);
        this->m_songOptMenu->show();
        this->m_songOptMenu->setCurIndex(this->m_information.index);
    }
    else {
        QFrame::mousePressEvent(event);//要么放else里面，要么注释掉这一行，否则会不显示
    }
}

void MusicItemWidget::onPlayToolBtnClicked() {
    //this->m_information.playCount++;//放这里不明智
    //qDebug()<<"已播放："<<this->m_information.playCount;
    emit play();
}

void MusicItemWidget::onPlayNextToolBtnClicked() {
}

void MusicItemWidget::onDownloadToolBtnClicked() {
}

void MusicItemWidget::onCollectToolBtnClicked() {
}

void MusicItemWidget::onMoreToolBtnClicked() {
    // 获取当前鼠标的全局位置
    getMenuPosition(QCursor::pos());
    this->m_songOptMenu->move(this->m_menuPosition);
    this->m_songOptMenu->show();
    this->m_songOptMenu->setCurIndex(this->m_information.index);

}

void MusicItemWidget::onPlay() {
    emit play();
}

void MusicItemWidget::onNextPlay() {
}

void MusicItemWidget::onAddToPlayQueue() {
}

void MusicItemWidget::onAddToNewSongList() {
}

void MusicItemWidget::onAddToLove() {
}

void MusicItemWidget::onAddToCollect() {
}

void MusicItemWidget::onAddtoPlayList() {
}

void MusicItemWidget::onDownload() {
}

void MusicItemWidget::onShare() {
}

void MusicItemWidget::onComment() {
}

void MusicItemWidget::onSameSong() {
}

void MusicItemWidget::onViewSongInfo() {
}

void MusicItemWidget::onDeleteSong(const int& idx) {
    emit deleteSong(idx);
}

void MusicItemWidget::onOpenInFile() {
}

void MusicItemWidget::onSearch() {
}

void MusicItemWidget::onUpLoad() {
}

void MusicItemWidget::initUi()
{
    this->m_playToolBtn     ->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-gray.svg")));
    this->m_playNextToolBtn ->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/add-music-list-gray.svg")));
    this->m_downloadToolBtn ->setIcon(QIcon(QStringLiteral(":/Res/window/download.svg")));
    this->m_collectToolBtn  ->setIcon(QIcon(QStringLiteral(":/Res/window/collect.svg")));
    this->m_moreToolBtn     ->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/more2-gray.svg")));

    this->m_playToolBtn     ->setCursor(Qt::PointingHandCursor);
    this->m_playNextToolBtn ->setCursor(Qt::PointingHandCursor);
    this->m_downloadToolBtn ->setCursor(Qt::PointingHandCursor);
    this->m_collectToolBtn  ->setCursor(Qt::PointingHandCursor);
    this->m_moreToolBtn     ->setCursor(Qt::PointingHandCursor);

    auto hlayout = new QHBoxLayout(this);
    hlayout->addWidget(this->m_indexLab);
    hlayout->addWidget(this->m_coverLab);
    auto vlayout = new QVBoxLayout;
    vlayout->addWidget(this->m_nameLab);
    vlayout->addWidget(this->m_singerLab);
    hlayout->addLayout(vlayout);
    hlayout->addSpacerItem(this->m_spacerItem1);
    hlayout->addWidget(this->m_durationLab);
    hlayout->addSpacerItem(this->m_spacerItem2);
    hlayout->addWidget(this->m_playToolBtn);
    hlayout->addWidget(this->m_playNextToolBtn);
    hlayout->addWidget(this->m_downloadToolBtn);
    hlayout->addWidget(this->m_collectToolBtn);
    hlayout->addWidget(this->m_moreToolBtn);

    this->m_durationLab->move(this->width()*5/6,(this->height() - this->m_durationLab->height()) / 2);
}
