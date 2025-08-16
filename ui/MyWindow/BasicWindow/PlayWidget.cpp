/**
* @file PlayWidget.cpp
 * @brief 实现 PlayWidget 类，提供带圆角阴影和双击信号的播放控件
 * @author WeiWang
 * @date 2024-10-13
 * @version 1.0
 */

#include "PlayWidget.h"

#include <QPropertyAnimation>

#include "ui_PlayWidget.h"
#include "SpeedDialog.h"
#include "ElaMessageBar.h"
#include "ElaToolTip.h"

#include <QSizeGrip>
#include <QTime>

QPixmap roundedPixmap(const QPixmap &src, QSize size, int radius)
{
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    ///< 缩放图片
    QPixmap dest(size);
    dest.fill(Qt::transparent); ///< 设置透明背景

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius); ///< 创建圆角矩形路径
    painter.setClipPath(path);                                              ///< 设置裁剪路径
    painter.drawPixmap(0, 0, scaled);                                       ///< 绘制图片

    return dest;
}

/**
 * @brief 构造函数，初始化播放控件
 * @param parent 父控件指针，默认为 nullptr
 */
PlayWidget::PlayWidget(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::PlayWidget)
      , m_sizeGrip(std::make_unique<QSizeGrip>(this)) ///< 初始化窗口大小调整角标
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    this->setObjectName("PlayWidget");
    this->setStyleSheet("QWidget#PlayWidget{font-family: 'TaiwanPearl';margin:5px;}"); ///< 设置外边距
    initUi();
}

void PlayWidget::setSongName(const QString &name)
{
    ui->song_name_text->setText(name);
    ui->song_name_text->adjustSize(); ///< 适应文本

    m_songNameToolTip->setToolTip(name); ///< 更新歌曲名称提示
    m_songNameToolTip->adjustSize();     ///< 调整提示尺寸
    ui->singer_song_HLayout->update();
}

void PlayWidget::setSingerName(const QString &singer)
{
    ui->singer_text->setText(singer);
    ui->singer_text->adjustSize();

    m_singerToolTip->setToolTip(singer);
    m_songNameToolTip->adjustSize();

    ui->singer_song_HLayout->update(); ///< 更新布局
}

void PlayWidget::setCover(const QPixmap &pix)
{
    if (pix.isNull()) {
        /// 网络歌曲无法解析出图片
        /// if (m_player->getMusicPath().startsWith("http://") || m_player->getMusicPath().
        ///     startsWith("https://"))
        ///     return;
        qDebug() << "图片为空，设置默认图片";
        ui->cover_label->setPixmap(roundedPixmap(
            QPixmap(":/Res/playbar/default-cover.png"),
            ui->cover_label->size(),
            8)); ///< 设置默认封面
    } else {
        qDebug() << "图片不为空，设置图片：" << pix;
        ui->cover_label->setPixmap(roundedPixmap(pix, ui->cover_label->size(), 8)); ///< 设置封面图片
    }
}

void PlayWidget::changeCircleToolButtonState(bool singleCircle)
{
    if (singleCircle) {
        ui->circle_toolButton->setStyleSheet(
            R"(QToolButton{border-image:url(':/Res/playbar/single-list-loop-gray.svg');}
               QToolButton:hover{border-image:url(':/Res/playbar/single-list-loop-blue.svg');})");
    } else {
        ui->circle_toolButton->setStyleSheet(
            R"(QToolButton{border-image:url(':/Res/playbar/list-loop-gray.svg');}
               QToolButton:hover{border-image:url(':/Res/playbar/list-loop-blue.svg');})");
    }
}

/**
 * @brief 更新进度条范围
 * @param duration 总时长（毫秒）
 * @note 设置进度条最大值和时长标签
 */
void PlayWidget::updateSliderRange(const qint64 &duration)
{
    ui->progressSlider->setMaximum(static_cast<int>(duration)); ///< 设置进度条最大值
    // @note 未使用，保留用于调试
    // qDebug() << "改变总时长为：" << duration;
    ui->duration_label->setText(
        QTime::fromMSecsSinceStartOfDay(static_cast<int>(duration)).toString("mm:ss"));
    ///< 更新时长标签
}

void PlayWidget::setNoVolume(const bool &flag)
{
    // @note 未使用，保留用于调试
    // STREAM_INFO() << "KuGouApp 托盘图标点击: " << (flag ? "静音" : "开启声音");
    if ((flag && ui->volume_toolButton->getVolumeValue()) || (
            !flag && !ui->volume_toolButton->getVolumeValue())) {
        QCoreApplication::sendEvent(ui->volume_toolButton, new QEvent(QEvent::Enter)); ///< 触发进入事件
        ui->volume_toolButton->clicked();                                              ///< 模拟点击
    }
}

void PlayWidget::setPlayPauseIcon(bool isPlay)
{
    if (isPlay) {
        onAudioPlay();
    } else {
        onAudioPause();
    }
}

qreal PlayWidget::coverFillRatioValue() const
{
    return coverFillRatio;
}

void PlayWidget::setCoverFillRatioValue(qreal ratio)
{
    coverFillRatio = ratio;
    ui->cover_label->update();
}

void PlayWidget::setTextColor(bool isWhite)
{
    const QString btnColor = QString("QPushButton { color:%1; }").arg(isWhite ? "white" : "black");
    const QString labelColor = QString("QLabel { color:%1; }").arg(isWhite ? "white" : "black");

    ui->speed_pushButton->setStyleSheet(btnColor);
    ui->stander_pushButton->setStyleSheet(btnColor);
    ui->acoustics_pushButton->setStyleSheet(btnColor);

    ui->position_label->setStyleSheet(labelColor);
    ui->duration_label->setStyleSheet(labelColor);
    ui->song_name_text->setStyleSheet(labelColor);
}


void PlayWidget::onSliderPositionChanged(const int &position)
{
    if (ui->progressSlider->isSliderDown())
        return;
    ui->progressSlider->setValue(position); ///< 更新进度条
    ui->position_label->setText(
        QTime::fromMSecsSinceStartOfDay(position).toString("mm:ss")); ///< 更新时间标签
}

void PlayWidget::onCoverChanged(const QPixmap &pix)
{
    setCover(pix);
}

void PlayWidget::onSongNameChanged(const QString &name)
{
    setSongName(name);
}

void PlayWidget::onSingerNameChanged(const QString &name)
{
    setSingerName(name);
}

void PlayWidget::onAudioPlay()
{
    ui->play_or_pause_toolButton->setIcon(
        QIcon(QStringLiteral(":/Res/playbar/play.svg")));
}

void PlayWidget::onAudioPause()
{
    ui->play_or_pause_toolButton->setIcon(
        QIcon(QStringLiteral(":/Res/playbar/pause.svg")));
}

void PlayWidget::initUi()
{
    setStyleSheet("font-family: 'TaiwanPearl';");
    ui->love_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/collect.svg")));
    ///< 设置收藏图标
    ui->download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/download.svg")));
    ///< 设置下载图标
    ui->comment_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/comment.svg")));
    ///< 设置评论图标
    ui->share_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/share.svg"))); ///< 设置分享图标
    ui->more_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/more.svg")));   ///< 设置更多图标
    ui->pre_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/previous-song.svg")));
    ///< 设置上一首图标
    ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg")));
    ///< 设置播放/暂停图标
    ui->next_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/next-song.svg")));
    ///< 设置下一首图标
    ui->erji_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/together.svg")));
    ///< 设置一起听图标
    ui->lyrics_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/song-words.svg")));
    ///< 设置歌词图标
    ui->song_queue_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play-list.svg")));
    ///< 设置播放队列图标

    ui->cover_label->setPixmap(roundedPixmap(QPixmap(":/Res/playbar/default-cover.png"),
                                             ui->cover_label->size(),
                                             8)); ///< 设置默认封面
    ///< @封面动画
    hoverPixmap.load(":/Res/playbar/up-lyric.svg");
    hoverPixmap = hoverPixmap.scaled(ui->cover_label->height() / 2,
                                     ui->cover_label->height() / 2,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation); ///< 缩放封面图片

    coverAnim = new QPropertyAnimation(this, QByteArrayLiteral("coverFillRatio"));
    coverAnim->setDuration(400);
    coverAnim->setEasingCurve(QEasingCurve::OutCubic);

    ui->cover_label->installEventFilter(this);
    ui->cover_label->setAttribute(Qt::WA_Hover); // 确保 hover 事件触发

    // @note 设置工具提示
    setElaToolTip(ui->cover_label, "点击查看歌词写真");             ///< 歌词写真提示
    setElaToolTip(ui->love_toolButton, "我喜欢");              ///< 收藏按钮提示
    setElaToolTip(ui->download_toolButton, "下载");           ///< 下载按钮提示
    setElaToolTip(ui->comment_toolButton, "评论");            ///< 评论按钮提示
    setElaToolTip(ui->share_toolButton, "分享");              ///< 分享按钮提示
    setElaToolTip(ui->more_toolButton, "更多");               ///< 更多按钮提示
    setElaToolTip(ui->circle_toolButton, "循环播放");           ///< 循环播放按钮提示
    setElaToolTip(ui->pre_toolButton, "上一首");               ///< 上一首按钮提示
    setElaToolTip(ui->play_or_pause_toolButton, "播放/暂停");   ///< 播放/暂停按钮提示
    setElaToolTip(ui->next_toolButton, "下一首");              ///< 下一首按钮提示
    setElaToolTip(ui->stander_pushButton, "音质选择");          ///< 音质选择按钮提示
    setElaToolTip(ui->acoustics_pushButton, "音效");          ///< 音效按钮提示
    setElaToolTip(ui->erji_toolButton, "邀请好友一起听");          ///< 一起听按钮提示
    setElaToolTip(ui->lyrics_toolButton, "打开桌面歌词");         ///< 歌词按钮提示
    setElaToolTip(ui->song_queue_toolButton, "播放队列");       ///< 播放队列按钮提示
    m_songNameToolTip = new ElaToolTip(ui->song_name_text); ///< 创建歌曲名称提示
    m_songNameToolTip->setToolTip("网络歌曲");                  ///< 设置默认歌曲提示
    m_singerToolTip = new ElaToolTip(ui->singer_text);      ///< 创建歌手名称提示
    m_singerToolTip->setToolTip("网络歌手");                    ///< 设置默认歌手提示

    // @note 设置大小调整角标
    this->m_sizeGrip->setFixedSize(7, 7);                        ///< 设置角标大小
    this->m_sizeGrip->setObjectName(QStringLiteral("sizegrip")); ///< 设置对象名称

    auto font = QFont("AaSongLiuKaiTi"); ///< 设置字体
    font.setPixelSize(14);
    font.setWeight(QFont::Medium);
    ui->song_name_text->setFont(font); ///< 应用歌曲名称字体
    ui->singer_text->setFont(font);    ///< 应用歌手名称字体
    ui->song_name_text->adjustSize();
    ui->singer_text->adjustSize();

    ui->progressSlider->installEventFilter(this);
    connect(ui->volume_toolButton,
            &VolumeToolBtn::volumeChange,
            this,
            [this](const int value) {
                emit volumeChange(value); ///< 设置播放器音量
            });

    connect(ui->progressSlider,
            &QSlider::sliderReleased,
            this,
            [this] {
                emit sliderReleased(ui->progressSlider->value(),
                                    ui->
                                    progressSlider->maximum()); ///< 发出滑块释放信号
            });                                                 ///< 连接进度条释放信号
}

/**
 * @brief 设置控件的工具提示
 * @param widget 目标控件
 * @param tooltip 提示内容
 */
void PlayWidget::setElaToolTip(QWidget *widget, const QString &tooltip)
{
    auto toolTip = new ElaToolTip(widget);
    toolTip->setToolTip(tooltip);
}

/**
 * @brief 重写鼠标双击事件，触发双击信号
 * @param event 鼠标事件
 */
void PlayWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (!ui->cover_label->rect().contains(ui->cover_label->mapFromParent(event->pos())))
            emit doubleClicked(); ///< 左键双击时发出自定义信号
    }
}

/**
 * @brief 重写绘制事件，绘制带圆角的线性渐变阴影
 * @param ev 绘制事件
 */
void PlayWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setPen(Qt::NoPen); ///< 无边框

    QRect shadowRect = rect().adjusted(5, 0, -4, -5); ///< 调整阴影区域

    // QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomLeft());
    // gradient.setColorAt(0, QColor(QStringLiteral("#eef2ff"))); ///< 起始颜色
    // gradient.setColorAt(1, QColor(QStringLiteral("#87CEFA"))); ///< 结束颜色
    // p.setBrush(gradient);

    QPainterPath path;
    int radius = 8; ///< 圆角半径

    // 构建圆角矩形路径（左下和右下角有圆角）
    path.moveTo(shadowRect.topLeft());
    path.lineTo(shadowRect.topRight());
    path.lineTo(shadowRect.bottomRight() - QPoint(0, radius));
    path.quadTo(shadowRect.bottomRight(), shadowRect.bottomRight() - QPoint(radius, 0));
    path.lineTo(shadowRect.bottomLeft() + QPoint(radius, 0));
    path.quadTo(shadowRect.bottomLeft(), shadowRect.bottomLeft() - QPoint(0, radius));
    path.lineTo(shadowRect.topLeft());
    path.closeSubpath();

    p.setClipPath(path); // 限制绘制范围为圆角区域
    p.drawPath(path);    ///< 绘制路径
}

/**
 * @brief 事件过滤器
 * @param watched 监控对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理进度条和封面标签事件
 */
bool PlayWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->progressSlider) {
        // @note 禁用进度条拖拽
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::MouseButtonDblClick
            || event->type() == QEvent::MouseMove) {
            if (ui->duration_label->text() == "00:00" && ui->position_label->text() ==
                "00:00") {
                return true; ///< 拦截鼠标事件
            }
        }
        if (event->type() == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                ///< 左键按下
                qint64 value = QStyle::sliderValueFromPosition(ui->progressSlider->minimum(),
                                                               ui->progressSlider->maximum(),
                                                               mouseEvent->pos().x(),
                                                               ui->progressSlider->width());
                emit sliderPressed(value * 1000); ///< 发出滑块按下信号
            }
        }
    }
    if (watched == ui->cover_label) {
        if (event->type() == QEvent::Enter) {
            coverAnim->stop();
            coverAnim->setStartValue(coverFillRatio);
            coverAnim->setEndValue(1.0);
            coverAnim->start();
        } else if (event->type() == QEvent::Leave) {
            coverAnim->stop();
            coverAnim->setStartValue(coverFillRatio);
            coverAnim->setEndValue(0.0);
            coverAnim->start();
        } else if (event->type() == QEvent::Paint) {
            QPainter p(ui->cover_label);
            p.setRenderHint(QPainter::Antialiasing, true);
            p.setRenderHint(QPainter::SmoothPixmapTransform, true);

            // 先画默认封面
            QPixmap base = roundedPixmap(ui->cover_label->pixmap(),
                                         ui->cover_label->size(),
                                         8);
            p.drawPixmap(ui->cover_label->rect(), base);

            int coverSquareSide = ui->cover_label->height(); // 左上角正方形区域边长

            // 2) 几何参数
            const int h = ui->cover_label->height();
            const int radius = 11;

            // 3) 圆角阴影遮罩（方向由 m_isLyricWidgetShow 决定）
            if (coverFillRatio > 0.0) {
                const int maskH = qBound(0, int(h * coverFillRatio), h);
                const int y = (m_isLyricWidgetShow ? 0 : (h - maskH)); // 上下方向切换
                QPainterPath path;
                path.addRoundedRect(QRectF(0, y, h, maskH), radius, radius);
                p.fillPath(path, QColor(0, 0, 0, 100));
            }

            // 绘制 hover 图片
            if (!hoverPixmap.isNull() && coverFillRatio > 0.0) {
                // 缩放 hoverPixmap 为左上角正方形 1/3 大小
                int hoverSide = coverSquareSide / 3;
                QPixmap scaledHover = hoverPixmap.scaled(hoverSide,
                                                         hoverSide,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation);

                // 居中在左上角正方形区域
                int x = (coverSquareSide - scaledHover.width()) / 2;
                int y = (coverSquareSide - scaledHover.height()) / 2;

                int visibleHeight = int(scaledHover.height() * coverFillRatio);

                QRectF targetSubRect(x,
                                     y + scaledHover.height() - visibleHeight,
                                     scaledHover.width(),
                                     visibleHeight);

                QRectF sourceRect(0,
                                  scaledHover.height() - visibleHeight,
                                  scaledHover.width(),
                                  visibleHeight);

                p.drawPixmap(targetSubRect, scaledHover, sourceRect);
            }
            return true; // 我们自己绘制，不让 QLabel 默认绘制
        }
    }
    return QWidget::eventFilter(watched, event);
}

void PlayWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // @note 移动角标
    this->m_sizeGrip->move(this->width() - this->m_sizeGrip->width() - 8,
                           this->height() - this->m_sizeGrip->height() - 8);
    this->m_sizeGrip->raise();          ///< 提升角标层级
    this->m_sizeGrip->setVisible(true); ///< 显示角标
}

void PlayWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (ui->cover_label->rect().contains(ui->cover_label->mapFromParent(event->pos()))) {
            emit showLyricWidget();
            // qDebug() << "显示歌词窗口";
            m_isLyricWidgetShow = !m_isLyricWidgetShow;
            if (m_isLyricWidgetShow) {
                hoverPixmap.load(":/Res/playbar/down-lyric.svg");
            } else {
                hoverPixmap.load(":/Res/playbar/up-lyric.svg");
            }
        }
    }
}

/**
 * @brief 播放/暂停按钮点击槽函数
 * @note 切换播放/暂停状态
 */
void PlayWidget::on_play_or_pause_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
    emit clickedPlayPauseBtn(); ///< 发出点击播放/暂停按钮信号
}

/**
 * @brief 收藏按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void PlayWidget::on_love_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 下载按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void PlayWidget::on_download_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 评论按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void PlayWidget::on_comment_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 分享按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void PlayWidget::on_share_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 更多按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void PlayWidget::on_more_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
    }
}

void PlayWidget::on_circle_toolButton_clicked()
{
    emit clickedCircleBtn();
}

void PlayWidget::on_pre_toolButton_clicked()
{
    emit clickedPreBtn(); ///< 发出点击上一首按钮信号
}

void PlayWidget::on_next_toolButton_clicked()
{
    emit clickedNextBtn(); ///< 发出点击下一首按钮信号
}

/**
 * @brief 速度选择按钮点击槽函数
 * @note 显示速度选择界面
 */
void PlayWidget::on_speed_pushButton_clicked()
{
    /// 弹出速度相关界面，并且在隐藏的时候销毁
    auto speedDialog = new SpeedDialog(this->window());
    auto statePtr = std::make_shared<SpeedDialogState>(); ///< 使用智能指针
    // 连接关闭信号以保存状态
    connect(speedDialog,
            &SpeedDialog::aboutToClose,
            [this, speedDialog, statePtr]() {
                // 获取当前状态并保存到文件
                *statePtr = speedDialog->getState();
                statePtr->save();
            });

    // 从文件加载并恢复状态
    speedDialog->setState(*statePtr);
    // @note 未使用，保留用于调试
    /*
        connect(speedDialog, &QObject::destroyed, this, [] {
            qDebug() << "SpeedDialog destroyed";
        });
    */
    auto tmpPos = ui->speed_pushButton->mapToGlobal(
        QPoint(ui->speed_pushButton->width() / 2, ui->speed_pushButton->height() / 2));
    tmpPos.setX(tmpPos.x() - speedDialog->width() / 2);
    tmpPos.setY(tmpPos.y() - speedDialog->height() - 15);
    auto pos = this->window()->mapFromGlobal(tmpPos);
    speedDialog->move(pos);
    speedDialog->show();

    /// 连接槽
    connect(speedDialog,
            &SpeedDialog::btnTextChanged,
            this,
            [this](const QString &text) {
                ui->speed_pushButton->setText(text);
                if (text == "倍速") {
                    if (m_isLyricWidgetShow) {
                        ui->speed_pushButton->setStyleSheet(
                            "QPushButton {background-color: transparent;color:while;}");
                    } else {
                        ui->speed_pushButton->setStyleSheet(
                            "QPushButton {background-color: transparent; color:black;}");
                    }
                } else {
                    if (m_isLyricWidgetShow) {
                        ui->speed_pushButton->setStyleSheet(
                            "QPushButton {background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:1, y2:0,stop:0 "
                            "rgb(105, 225, 255), stop:1 rgba(255, 182, 193, 255)); color:white;}");
                    } else {
                        ui->speed_pushButton->setStyleSheet(
                            "QPushButton {background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:1, y2:0,stop:0 "
                            "rgb(105, 225, 255), stop:1 rgba(255, 182, 193, 255)); color:black;}");
                    }
                }
            });

    connect(speedDialog,
            &SpeedDialog::speedChanged,
            this,
            [this](const float &speed) {
                // @note 调试用 qDebug() << "设置速度为：" << speed;
                //if (this->m_player)
                //    this->m_player->setSpeed(speed);
            });
}

/**
 * @brief 音质选择按钮点击槽函数
 * @note 显示未实现提示
 */
void PlayWidget::on_stander_pushButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::information(ElaMessageBarType::BottomRight,
                                   "Info",
                                   QStringLiteral("音质选择功能 暂未实现 敬请期待"),
                                   1000,
                                   this->window()); ///< 显示未实现提示
        return;
    }
}

/**
 * @brief 音效按钮点击槽函数
 * @note 显示未实现提示
 */
void PlayWidget::on_acoustics_pushButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::information(ElaMessageBarType::BottomRight,
                                   "Info",
                                   QStringLiteral("音效功能 暂未实现 敬请期待"),
                                   1000,
                                   this->window()); ///< 显示未实现提示
        return;
    }
}

/**
 * @brief 一起听按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void PlayWidget::on_erji_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
}

/**
 * @brief 歌词按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void PlayWidget::on_lyrics_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
}

/**
 * @brief 播放队列按钮点击槽函数
 * @note 显示未实现提示
 */
void PlayWidget::on_song_queue_toolButton_clicked()
{
    if (ui->duration_label->text() == "00:00" && ui->position_label->text() == "00:00") {
        ElaMessageBar::information(ElaMessageBarType::BottomRight,
                                   "Info",
                                   QStringLiteral("播放队列功能 暂未实现 敬请期待"),
                                   1000,
                                   this->window()); ///< 显示未实现提示
        return;
    }
}