/**
 * @file MusicItemWidget.cpp
 * @brief 实现 MusicItemWidget 类，提供音乐条目控件功能
 * @author [WeiWang]
 * @date 2025-05-13
 * @version 1.0
 */

#include "MusicItemWidget.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QApplication>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QFile>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QRandomGenerator>

#define PIX_SIZE 50 ///< 图片大小

#define PIX_RADIUS 9 ///< 图片圆角

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 创建圆角图片
 * @param src 原始图片
 * @param size 目标大小
 * @param radius 圆角半径
 * @return 处理后的图片
 */
QPixmap roundedPix(const QPixmap &src, QSize size, int radius)
{
    if (src.isNull()) {
        return QPixmap(); // 返回空图片而不是尝试处理
    }
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

/**
 * @brief 构造函数，初始化音乐条目控件
 * @param info 歌曲信息
 * @param parent 父控件指针，默认为 nullptr
 */
MusicItemWidget::MusicItemWidget(SongInfor info, QWidget *parent)
    : QFrame(parent), m_information(std::move(info)), timer(new QTimer(this))
{
    this->m_index = m_information.index;
    this->m_name = m_information.songName;
    this->m_duration = m_information.duration;
    this->m_cover = m_information.cover;
    this->m_singer = m_information.singer;
    m_information.album = m_information.album.isEmpty() ? "未知专辑" : m_information.album;
    this->m_album = m_information.album;
    //qDebug()<<"m_index: "<<m_index<<" name: "<<m_name<<" duration: "<<m_duration;
    //    " cover: "<<m_cover<<"m_singer: "<<m_singer;
    //PRINT_INFO("index: %d , name: %s , duration: %s , singer: %s ",
    //    m_index, m_name.toStdString(), m_duration.toStdString(), m_singer.toStdString());
    initUi();
    setInformation(m_information);
    setObjectName(QStringLiteral("MusicItemWidget"));
    m_indexLab->setObjectName(QStringLiteral("indexLab"));
    m_coverLab->setObjectName(QStringLiteral("coverLab"));
    m_nameLab->setObjectName(QStringLiteral("nameLab"));
    m_singerLab->setObjectName(QStringLiteral("singerLab"));
    m_albumLab->setObjectName(QStringLiteral("albumLab"));
    m_durationLab->setObjectName(QStringLiteral("durationLab"));
    m_playToolBtn->setObjectName(QStringLiteral("playToolBtn"));
    m_playNextToolBtn->setObjectName(QStringLiteral("playNextToolBtn"));
    m_downloadToolBtn->setObjectName(QStringLiteral("downloadToolBtn"));
    m_collectToolBtn->setObjectName(QStringLiteral("collectToolBtn"));
    m_moreToolBtn->setObjectName(QStringLiteral("moreToolBtn"));
    // 设置tooltip
    {
        auto playToolBtn_toolTip = new ElaToolTip(m_playToolBtn);
        playToolBtn_toolTip->setToolTip(QStringLiteral("播放"));
        auto playNextToolBtn_toolTip = new ElaToolTip(m_playNextToolBtn);
        playNextToolBtn_toolTip->setToolTip(QStringLiteral("下一首"));
        auto downloadToolBtn_toolTip = new ElaToolTip(m_downloadToolBtn);
        downloadToolBtn_toolTip->setToolTip(QStringLiteral("下载"));
        auto collectToolBtn_toolTip = new ElaToolTip(m_collectToolBtn);
        collectToolBtn_toolTip->setToolTip(QStringLiteral("收藏"));
        auto moreToolBtn_toolTip = new ElaToolTip(m_moreToolBtn);
        moreToolBtn_toolTip->setToolTip(QStringLiteral("更多"));
    }
    //设置样式
    QFile file(GET_CURRENT_DIR + QStringLiteral("/item.css"));
    if (file.open(QIODevice::ReadOnly)) {
        setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表 item.css 打开失败QAQ";
        return;
    }

    timer->setInterval(timeInterval);                                              // 设置定时器时间间隔
    max_radius = static_cast<int>(qSqrt(width() * width() + height() * height())); // 计算最大半径

    //smallWidget响应
    connect(m_playToolBtn, &QToolButton::clicked, this, &MusicItemWidget::onPlayToolBtnClicked);
    connect(m_playNextToolBtn,
            &QToolButton::clicked,
            this,
            &MusicItemWidget::onPlayNextToolBtnClicked);
    connect(m_downloadToolBtn,
            &QToolButton::clicked,
            this,
            &MusicItemWidget::onDownloadToolBtnClicked);
    connect(m_collectToolBtn,
            &QToolButton::clicked,
            this,
            &MusicItemWidget::onCollectToolBtnClicked);
    connect(m_moreToolBtn, &QToolButton::clicked, this, &MusicItemWidget::onMoreToolBtnClicked);
    //menu响应
    auto menu = new MyMenu(MyMenu::MenuKind::SongOption, this);
    m_songOptMenu = menu->getMenu<SongOptionMenu>();
    connect(m_songOptMenu, &SongOptionMenu::play, this, &MusicItemWidget::onPlay);
    connect(m_songOptMenu, &SongOptionMenu::deleteSong, this, &MusicItemWidget::onDeleteSong);

    //跳转选中
    m_blinkTimer = new QTimer(this);
    connect(m_blinkTimer,
            &QTimer::timeout,
            this,
            [this]() {
                // 更新透明度
                m_highlightAlpha += (10 * m_highlightDirection);

                // 反转方向
                if (m_highlightAlpha >= 255) {
                    m_highlightAlpha = 255;
                    m_highlightDirection = -1;
                } else if (m_highlightAlpha <= 0) {
                    m_highlightAlpha = 0;
                    m_highlightDirection = 1;
                }

                update(); // 触发重绘
            });
}

void MusicItemWidget::setCover(const QPixmap &pix)
{
    this->m_cover = roundedPix(pix, m_coverLab->size(), PIX_RADIUS);
    // 更新封面标签的显示
    if (m_coverLab && !pix.isNull()) {
        m_coverLab->setPixmap(this->m_cover);
    }
    this->m_information.cover = m_cover;
    update();
}

void MusicItemWidget::setNetUrl(const QString &netUrl)
{
    this->m_information.netUrl = netUrl;
}

void MusicItemWidget::setLyric(const QString &lyric)
{
    this->m_information.lyric = lyric;
}

void MusicItemWidget::setPopular(const int &popular) const
{
    if (popular < 0)
        this->m_popularLab->setPixmap(QPixmap(
            QString(":/TabIcon/Res/tabIcon/%1-grid-popular.svg").arg(
                QRandomGenerator::global()->bounded(0, 7))));
    else
        this->m_popularLab->setPixmap(
            QPixmap(QString(":/TabIcon/Res/tabIcon/%1-grid-popular.svg").arg(popular)));
    this->m_popularLab->show();
}

/**
 * @brief 设置索引文本
 * @param index 索引值
 */
void MusicItemWidget::setIndexText(const int &index)
{
    this->m_index = index;
    m_indexLab->setText(QString("%1").arg(index, 2, 10, QChar('0')));
}

/**
 * @brief 设置定时器时间间隔，控制填充速度
 * @param timeInterval 时间间隔（毫秒）
 */
void MusicItemWidget::setInterval(const int &timeInterval) const
{
    timer->setInterval(timeInterval);
}

/**
 * @brief 设置涟漪填充颜色
 * @param fillcolor 填充颜色
 */
void MusicItemWidget::setFillColor(const QColor &fillcolor)
{
    fill_color = fillcolor;
}

/**
 * @brief 设置圆角半径
 * @param radius_ 圆角半径
 */
void MusicItemWidget::setRadius(const int &radius_)
{
    frame_radius = radius_;
}

/**
 * @brief 设置歌曲信息
 * @param info 歌曲信息
 */
void MusicItemWidget::setInformation(const SongInfor &info)
{
    this->m_index = info.index;
    this->m_name = info.songName;
    this->m_duration = info.duration;
    this->m_cover = info.cover;
    this->m_singer = info.singer;
    this->m_album = info.album;
    this->m_indexLab->setText(QString("%1").arg(this->m_index + 1, 2, 10, QChar('0')));

    // 确保初始化时设置封面
    if (!info.cover.isNull()) {
        m_coverLab->setPixmap(roundedPix(info.cover, m_coverLab->size(), PIX_RADIUS));
    } else if (!info.coverUrl.isEmpty()) {
        // 如果有封面URL但还没加载，可以显示占位图
        m_coverLab->setPixmap(roundedPix(QPixmap(":/Res/tablisticon/pix4.png"),
                                         m_coverLab->size(),
                                         PIX_RADIUS));
    }

    /*this->m_nameLab->setText(this->m_name);
    this->m_singerLab->setText(this->m_singer);*/
    //qDebug() << "m_nameLab width:" << m_nameLab->width();
    QFontMetrics metrics(this->m_nameLab->font());

    auto nameLab_toolTip = new ElaToolTip(this->m_nameLab);
    nameLab_toolTip->setToolTip(this->m_name);

    QString elidedName = metrics.elidedText(this->m_name, Qt::ElideRight, this->m_nameLab->width());
    this->m_nameLab->setText(elidedName);
    //qDebug() << "m_singerLab width:" << m_singerLab->width();

    auto singerLab_toolTip = new ElaToolTip(this->m_singerLab);
    singerLab_toolTip->setToolTip(this->m_singer);

    QString elidedSinger = metrics.elidedText(this->m_singer,
                                              Qt::ElideRight,
                                              this->m_singerLab->width());
    this->m_singerLab->setText(elidedSinger);

    auto albumLab_toolTip = new ElaToolTip(this->m_albumLab);
    albumLab_toolTip->setToolTip(this->m_album);
    QString elidedAlbum = metrics.elidedText(this->m_album,
                                             Qt::ElideRight,
                                             this->m_albumLab->width());
    this->m_albumLab->setText("<span style='color:gray;'>《" + elidedAlbum + "》&nbsp;</span>");

    this->m_durationLab->setText(this->m_duration);
    update(); // 重绘
}

/**
 * @brief 设置播放状态
 * @param state 是否播放
 */
void MusicItemWidget::setPlayState(const bool &state)
{
    m_isPlaying = state;
    if (m_isPlaying) {
        mouse_point = rect().center();

        // 启动定时器
        timer->disconnect();
        connect(timer,
                &QTimer::timeout,
                this,
                [ = ] {
                    radius += radius_var;
                    if (radius > max_radius) {
                        timer->stop();
                        return;
                    }
                    update();
                });
        timer->start();
    } else {
        mouse_point = rect().center();

        // 断开旧连接后建立收缩动画
        timer->disconnect();
        connect(timer,
                &QTimer::timeout,
                this,
                [ = ] {
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

/**
 * @brief 设置item是否高亮闪烁
 * @param highlight 是否高亮闪烁
 */
void MusicItemWidget::setHighlight(bool highlight)
{
    if (highlight) {
        m_highlightAlpha = 0;     // 从完全透明开始
        m_highlightDirection = 1; // 向不透明变化
        m_blinkTimer->start(30);  // 30ms刷新一次 (约33FPS)
    } else {
        m_blinkTimer->stop();
        m_highlightAlpha = 0; // 重置为透明
        update();             // 清除高亮
    }
}

/**
 * @brief 鼠标进入事件
 * @param event 进入事件对象
 */
void MusicItemWidget::enterEvent(QEnterEvent *event)
{
    //QFrame::enterEvent(event);
    mouse_point = event->position(); // 记录鼠标进入坐标
    timer->disconnect();             // 断开可能的timer的所有连接
    connect(timer,
            &QTimer::timeout,
            this,
            [ = ] // 定时器触发，半径增大
            {
                radius += radius_var;
                if (radius > max_radius) {
                    timer->stop(); // 达到最大半径，定时器停止
                    return;
                }
                update(); // 调用绘制事件
            });
    timer->start(); // 定时器开始
}

/**
 * @brief 鼠标离开事件
 * @param event 事件对象
 */
void MusicItemWidget::leaveEvent(QEvent *event)
{
    //QFrame::leaveEvent(event);
    if (m_forceHover)
        return; // 阻止菜单弹出时的 leaveEvent
    if (!m_isPlaying) {
        mouse_point = mapFromGlobal(QCursor::pos());
        timer->disconnect();
        connect(timer,
                &QTimer::timeout,
                this,
                [ = ] // 定时器触发半径减小
                {
                    radius -= radius_var;
                    if (radius < 0) {
                        timer->stop(); // 减小到小于0时定时器停止
                        radius = 0;    // 确保半径不为负
                        return;
                    }
                    update();
                });
        timer->start();
    }
}

/**
 * @brief 绘制事件
 * @param event 绘图事件对象
 */
void MusicItemWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (!mouse_point.isNull() && radius > 0) {
        QPainterPath path;
        painter.setBrush(QBrush(fill_color));
        painter.setPen(Qt::NoPen);
        path.addRoundedRect(rect(), frame_radius, frame_radius);
        painter.setClipPath(path);
        painter.drawEllipse(mouse_point, radius, radius); // 画圆
        painter.setClipping(false);                       // 禁用剪切路径
    }
    // 添加高亮效果：闪烁动画
    if (m_highlightAlpha > 0) {
        QColor highlightColor(0x8a, 0xbc, 0xd1);   // 原始颜色
        highlightColor.setAlpha(m_highlightAlpha); // 应用当前透明度
        painter.setBrush(highlightColor);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect(), frame_radius, frame_radius);
    }
}

/**
 * @brief 大小调整事件
 * @param event 大小调整事件对象
 */
void MusicItemWidget::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    max_radius = static_cast<int>(qSqrt(width() * width() + height() * height())); // 重新计算最大半径

    //this->setGeometry(this->geometry().x(),this->geometry().y(),
    //    event->size().width(),this->geometry().height());
    //qDebug()<<"当前宽度："<<this->width();
    //qDebug()<<"event->size().width() = "<<event->size().width();
    //qDebug()<<"父对象宽度："<<qobject_cast<QWidget*>(this->parent())->width();
    QFontMetrics metrics(this->m_nameLab->font());
    QString elidedName = metrics.elidedText(this->m_name, Qt::ElideRight, this->m_nameLab->width());
    this->m_nameLab->setText(elidedName);
    //qDebug() << "m_singerLab width:" << m_singerLab->width();

    QString elidedSinger = metrics.elidedText(this->m_singer,
                                              Qt::ElideRight,
                                              this->m_singerLab->width());
    this->m_singerLab->setText(elidedSinger);

    QString elidedAlbum = metrics.elidedText(this->m_album,
                                             Qt::ElideRight,
                                             this->m_albumLab->width());
    this->m_albumLab->setText("<span style='color:gray;'>《" + elidedAlbum + "》</span>");
    if (m_isPlaying)
        setPlayState(true); // 如果正在播放，重新设置涟漪效果
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件对象
 */
void MusicItemWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QFrame::mouseDoubleClickEvent(event);
    m_playToolBtn->clicked();
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件对象
 */
void MusicItemWidget::mousePressEvent(QMouseEvent *event)
{
    // 判断是否为右键点击
    if (event->button() == Qt::RightButton) {
        m_songOptMenu->setCurIndex(m_information.index);
        // 手动标记当前控件保持 hover 状态
        m_forceHover = true;

        // 绑定菜单关闭信号
        connect(m_songOptMenu,
                &QMenu::aboutToHide,
                this,
                [=]() {
                    m_forceHover = false;
                    // 检查鼠标是否真的离开了控件
                    if (!rect().contains(mapFromGlobal(QCursor::pos()))) {
                        QEvent leaveEvent(QEvent::Leave);
                        QApplication::sendEvent(this, &leaveEvent);
                    }
                });
        m_songOptMenu->popup(QCursor::pos());
    } else {
        QFrame::mousePressEvent(event); //要么放else里面，要么注释掉这一行，否则会不显示
    }
}

/**
 * @brief 播放按钮点击处理
 */
void MusicItemWidget::onPlayToolBtnClicked()
{
    //m_information.playCount++;
    //qDebug()<<"已播放："<<m_information.playCount;
    emit play();
}

/**
 * @brief 下一首按钮点击处理
 */
void MusicItemWidget::onPlayNextToolBtnClicked() {}

/**
 * @brief 下载按钮点击处理
 */
void MusicItemWidget::onDownloadToolBtnClicked() {}

/**
 * @brief 收藏按钮点击处理
 */
void MusicItemWidget::onCollectToolBtnClicked() {}

/**
 * @brief 更多按钮点击处理
 */
void MusicItemWidget::onMoreToolBtnClicked()
{
    m_songOptMenu->exec(QCursor::pos());
    m_songOptMenu->setCurIndex(m_information.index);
}

/**
 * @brief 播放菜单项处理
 */
void MusicItemWidget::onPlay()
{
    emit play();
}

/**
 * @brief 下一首播放菜单项处理
 */
void MusicItemWidget::onNextPlay() {}

/**
 * @brief 添加到播放队列菜单项处理
 */
void MusicItemWidget::onAddToPlayQueue() {}

/**
 * @brief 添加到新歌单菜单项处理
 */
void MusicItemWidget::onAddToNewSongList() {}

/**
 * @brief 添加到喜欢菜单项处理
 */
void MusicItemWidget::onAddToLove() {}

/**
 * @brief 添加到收藏菜单项处理
 */
void MusicItemWidget::onAddToCollect() {}

/**
 * @brief 添加到播放列表菜单项处理
 */
void MusicItemWidget::onAddToPlayList() {}

/**
 * @brief 下载菜单项处理
 */
void MusicItemWidget::onDownload() {}

/**
 * @brief 分享菜单项处理
 */
void MusicItemWidget::onShare() {}

/**
 * @brief 评论菜单项处理
 */
void MusicItemWidget::onComment() {}

/**
 * @brief 相似歌曲菜单项处理
 */
void MusicItemWidget::onSameSong() {}

/**
 * @brief 查看歌曲信息菜单项处理
 */
void MusicItemWidget::onViewSongInfo() {}

/**
 * @brief 删除歌曲菜单项处理
 * @param idx 歌曲索引
 */
void MusicItemWidget::onDeleteSong(const int &idx)
{
    emit deleteSong(idx);
}

/**
 * @brief 在文件管理器中打开菜单项处理
 */
void MusicItemWidget::onOpenInFile() {}

/**
 * @brief 搜索菜单项处理
 */
void MusicItemWidget::onSearch() {}

/**
 * @brief 上传菜单项处理
 */
void MusicItemWidget::onUpLoad() {}

/**
 * @brief 初始化用户界面
 */
void MusicItemWidget::initUi()
{
    this->m_indexLab = new QLabel(QString("%1").arg(this->m_index + 1, 2, 10, QChar('0')), this);
    this->m_coverLab = new QLabel(this);
    this->m_coverLab->setAlignment(Qt::AlignCenter);
    this->m_coverLab->setFixedSize(PIX_SIZE, PIX_SIZE);
    this->m_coverLab->setPixmap(roundedPix(this->m_cover, this->m_coverLab->size(), PIX_RADIUS));
    this->m_nameLab = new QLabel(this);
    this->m_singerLab = new QLabel(this);
    this->m_albumLab = new QLabel(this);
    this->m_popularLab = new QLabel(this);
    this->m_durationLab = new QLabel(this);
    this->m_playToolBtn = new QToolButton(this);
    this->m_playNextToolBtn = new QToolButton(this);
    this->m_downloadToolBtn = new QToolButton(this);
    this->m_collectToolBtn = new QToolButton(this);
    this->m_moreToolBtn = new QToolButton(this);

    m_nameLab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_singerLab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_albumLab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->m_nameLab->setFixedWidth(100);
    this->m_singerLab->setFixedWidth(100);
    this->m_albumLab->setFixedWidth(110);

    this->m_popularLab->setPixmap(QPixmap(
        QString(":/TabIcon/Res/tabIcon/%1-grid-popular.svg").arg(
            QRandomGenerator::global()->bounded(0, 7))));
    this->m_popularLab->hide(); ///< 默认不显示
    auto popularLab_toolTip = new ElaToolTip(this->m_popularLab);
    popularLab_toolTip->setToolTip("热度");

    this->m_playToolBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-gray.svg")));
    this->m_playNextToolBtn->setIcon(
        QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/add-music-list-gray.svg")));
    this->m_downloadToolBtn->setIcon(QIcon(QStringLiteral(":/Res/window/download.svg")));
    this->m_collectToolBtn->setIcon(QIcon(QStringLiteral(":/Res/window/collect.svg")));
    this->m_moreToolBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/more2-gray.svg")));

    this->m_playToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_playNextToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_downloadToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_collectToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_moreToolBtn->setCursor(Qt::PointingHandCursor);

    auto hlayout = new QHBoxLayout(this);
    hlayout->addWidget(m_indexLab);
    hlayout->addWidget(m_coverLab);
    auto vlayout = new QVBoxLayout;
    vlayout->addWidget(m_nameLab);
    vlayout->addWidget(m_singerLab);
    hlayout->addLayout(vlayout);
    // 添加第一个弹簧，拉伸系数为 2
    hlayout->addStretch(1);
    hlayout->addWidget(m_albumLab);
    hlayout->addStretch(1);
    hlayout->addWidget(m_popularLab);
    hlayout->addSpacing(10);
    hlayout->addWidget(m_durationLab);
    // 添加第二个弹簧，拉伸系数为 1
    hlayout->addStretch(1);
    hlayout->addWidget(m_playToolBtn);
    hlayout->addWidget(m_playNextToolBtn);
    hlayout->addWidget(m_downloadToolBtn);
    hlayout->addWidget(m_collectToolBtn);
    hlayout->addWidget(m_moreToolBtn);
    //this->m_durationLab->move(this->width()*5/6,(this->height() - this->m_durationLab->height()) / 2);
}