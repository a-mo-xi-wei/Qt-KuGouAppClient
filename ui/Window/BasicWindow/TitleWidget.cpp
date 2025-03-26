//
// Created by WeiWang on 25-3-25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TitleWidget.h" resolved

#include "TitleWidget.h"

#include <QFile>

#include "ui_TitleWidget.h"
#include "MyMenu.h"
#include "logger.hpp"

#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
, ui(new Ui::TitleWidget)
{
    ui->setupUi(this);
    initUi();
    {
        //设置样式
        QFile file(GET_CURRENT_DIR + QStringLiteral("/title.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

TitleWidget::~TitleWidget() {
    delete ui;
}

void TitleWidget::initUi() {
    this->setStyleSheet("QWidget#TitleWidget{margin:5px;}");
    const auto menu = new MyMenu(MyMenu::MenuKind::TitleOption,this);
    m_titleOptMenu = menu->getMenu<TitleOptionMenu>();
    connect(m_titleOptMenu,&TitleOptionMenu::about,this,[this]{emit showAboutDialog();});
    connect(m_titleOptMenu,&TitleOptionMenu::exit,this,&TitleWidget::on_close_toolButton_clicked);

    ui->title_index_label1->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    ui->title_index_label2->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    ui->title_index_label3->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    ui->title_index_label4->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    ui->title_index_label2->hide();
    ui->title_index_label3->hide();
    ui->title_index_label4->hide();

    ui->title_line->setPixmap(QPixmap(QStringLiteral(":/Res/tabIcon/line-black.svg")));
    ui->search_lineEdit->addAction(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")),
                                   QLineEdit::LeadingPosition);

    //除非自定义QToolButton否则达不到 CSS 中 border-image 的效果
    //ui->listen_toolButton->setIcon(QIcon(":/Res/titlebar/listen-music-black.svg"));

    QPixmap roundedPix = getRoundedPixmap(QPixmap(QStringLiteral(":/Res/window/portrait.jpg")),
                                       ui->title_portrait_label->size(), ui->title_portrait_label->size().width()/2);
    m_originalCover.load(QStringLiteral(":/Res/window/portrait.jpg"));

    // 设置圆角半径
    ui->title_portrait_label->setPixmap(roundedPix);
    ui->title_portrait_label->setScaledContents(false); // 禁止 QLabel 自动缩放
    ui->title_portrait_label->installEventFilter(this);

    ui->title_gender_label->setPixmap(QPixmap(QStringLiteral(":/Res/window/boy.svg")));

    //设置设置按钮的Frame圆角，填充颜色
    ui->min_toolButton->setRadius(6);
    ui->max_toolButton->setRadius(6);
    ui->close_toolButton->setRadius(6);

    ui->min_toolButton->setFillColor(QColor(QStringLiteral("#969696")));
    ui->max_toolButton->setFillColor(QColor(QStringLiteral("#969696")));
    ui->close_toolButton->setFillColor(QColor(QStringLiteral("#FF0066")));

    ui->min_toolButton->setMyIcon(QIcon(QStringLiteral(":/Res/titlebar/minimize-black.svg")));
    ui->max_toolButton->setMyIcon(QIcon(QStringLiteral(":/Res/titlebar/maximize-black.svg")));
    ui->close_toolButton->setMyIcon(QIcon(QStringLiteral(":/Res/titlebar/close-black.svg")));

    //专门处理标题menu
    connect(ui->menu_toolButton,&QToolButton::clicked,this,&TitleWidget::on_menu_toolButton_clicked);

}

void TitleWidget::showMenu(const QPoint &pos) {
    setMenuPosition(pos);
    //qDebug()<<"当前Menu位置: "<<pos<<" 显示菜单";
    this->m_titleOptMenu->move(this->m_menuPosition);
    this->m_titleOptMenu->show();
    //qDebug()<<"显示菜单成功";
}

RippleButton * TitleWidget::max_toolButton() const {
    return ui->max_toolButton;
}
void TitleWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        on_max_toolButton_clicked();
    }
}

void TitleWidget::mousePressEvent(QMouseEvent *event) {
    // 判断是否为右键点击
    if (event->button() == Qt::RightButton) {
        showMenu(this->mapToGlobal(event->pos()));
    }
    else {
        QWidget::mousePressEvent(event);
    }
}

void TitleWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    // 创建一个线性渐变
    // 先绘制阴影效果
    QRect shadowRect = rect().adjusted(5, 5, -4, 2);
    QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomLeft());
    gradient.setColorAt(0, QColor(QStringLiteral("#87CEFA"))); // 起始颜色
    gradient.setColorAt(1, QColor(QStringLiteral("#eef2ff"))); // 结束颜色
    p.setBrush(gradient);
    //p.drawRoundedRect(shadowRect, 8, 8);
    // 创建一个 QPainterPath，只在左上和右上角有圆角
    QPainterPath path;
    int radius = 8; // 圆角半径

    path.moveTo(shadowRect.topLeft() + QPoint(radius, 0)); // 从左上角的圆角开始
    path.lineTo(shadowRect.topRight() - QPoint(radius, 0)); // 右上角前的直线部分
    path.quadTo(shadowRect.topRight(), shadowRect.topRight() + QPoint(0, radius)); // 右上角的圆角
    path.lineTo(shadowRect.bottomRight()); // 右侧的直线部分到底
    path.lineTo(shadowRect.bottomLeft()); // 底部的直线部分到左侧
    path.lineTo(shadowRect.topLeft() + QPoint(0, radius)); // 左侧的直线部分
    path.quadTo(shadowRect.topLeft(), shadowRect.topLeft() + QPoint(radius, 0)); // 左上角的圆角
    path.closeSubpath();

    p.drawPath(path); // 绘制路径
}

bool TitleWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->title_portrait_label && event->type() == QEvent::Enter) {

        QSize originalSize = ui->title_portrait_label->size();
        // 创建动画组
        auto *group = new QSequentialAnimationGroup(this);

        // 缩小动画
        auto *shrink = new QPropertyAnimation(ui->title_portrait_label, "size");
        shrink->setDuration(300);
        shrink->setEasingCurve(QEasingCurve::InOutQuart);
        shrink->setStartValue(originalSize);
        shrink->setEndValue(originalSize * 0.7);

        // 放大动画
        auto *expand = new QPropertyAnimation(ui->title_portrait_label, "size");
        expand->setDuration(300);
        expand->setEasingCurve(QEasingCurve::InOutQuart);
        expand->setStartValue(originalSize * 0.7);
        expand->setEndValue(originalSize);

        group->addAnimation(shrink);
        group->addAnimation(expand);

        // 连接动画的 valueChanged 信号，动态更新 pixmap
        connect(shrink, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value) {
            QSize newSize = value.toSize();
            ui->title_portrait_label->setPixmap(getRoundedPixmap(m_originalCover, newSize, newSize.width()/2));
        });
        connect(expand, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value) {
            QSize newSize = value.toSize();
            ui->title_portrait_label->setPixmap(getRoundedPixmap(m_originalCover, newSize, newSize.width()/2));
        });

        // 启动动画并自动删除
        group->start(QAbstractAnimation::DeleteWhenStopped);
        //qDebug()<<"开始动画";
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void TitleWidget::on_title_return_toolButton_clicked() {
    qDebug() << "返回，估计要使用堆栈";
    STREAM_INFO() << "返回，估计要使用堆栈";
}

void TitleWidget::on_title_refresh_toolButton_clicked() {
    qDebug() << "刷新界面";
    STREAM_INFO() << "刷新界面";
}

void TitleWidget::on_title_music_pushButton_clicked() {
    ui->title_index_label1->show();
    ui->title_index_label2->hide();
    ui->title_index_label3->hide();
    ui->title_index_label4->hide();
    //显示menu
    emit leftMenuShow(true);
    switch (m_lastType) {
        case RecommendForYou:
            onLeftMenu_recommend_clicked();
            break;
        case MusicRepository:
            onLeftMenu_musicRepository_clicked();
            break;
        case Channel:
            onLeftMenu_channel_clicked();
            break;
        case Video:
            onLeftMenu_video_clicked();
            break;
        case SongList:
            onLeftMenu_songList_clicked();
            break;
        case DailyRecommend:
            onLeftMenu_dailyRecommend_clicked();
            break;
        case Collection:
            onLeftMenu_collection_clicked();
            break;
        case LocalDownload:
            onLeftMenu_localDownload_clicked();
            break;
        case MusicCloudDisk:
            onLeftMenu_musicCloudDisk_clicked();
            break;
        case PurchasedMusic:
            onLeftMenu_purchasedMusic_clicked();
            break;
        case RecentlyPlayed:
            onLeftMenu_recentlyPlayed_clicked();
            break;
        case AllMusic:
            onLeftMenu_allMusic_clicked();
            break;
        default: onLeftMenu_recommend_clicked();
    }
    STREAM_INFO() <<"切换音乐界面";
}

void TitleWidget::on_title_live_pushButton_clicked() {
    emit currentStackChange(StackType::TitleLive);
    emit leftMenuShow(false);
    ui->title_index_label1->hide();
    ui->title_index_label2->show();
    ui->title_index_label3->hide();
    ui->title_index_label4->hide();
    qDebug()<<"直播";
    STREAM_INFO()<<"切换直播界面";
}

void TitleWidget::on_title_listen_book_pushButton_clicked() {
    emit currentStackChange(StackType::ListenBook);
    emit leftMenuShow(false);
    ui->title_index_label1->hide();
    ui->title_index_label2->hide();
    ui->title_index_label3->show();
    ui->title_index_label4->hide();
    qDebug()<<"听书";
    STREAM_INFO()<<"切换听书界面";
}

void TitleWidget::on_title_search_pushButton_clicked() {
    emit currentStackChange(StackType::Search);
    emit leftMenuShow(false);
    ui->title_index_label1->hide();
    ui->title_index_label2->hide();
    ui->title_index_label3->hide();
    ui->title_index_label4->show();
    qDebug()<<"探索";
    STREAM_INFO()<<"切换探索界面";
}

void TitleWidget::on_menu_toolButton_clicked() {
    showMenu(QCursor::pos());
}

void TitleWidget::on_min_toolButton_clicked() {
    //QRect m_startGeometry = this->geometry(); // 获取当前窗口的几何形状(正常状态)
    //QRect m_endGeometry = m_startGeometry;
    ////m_endGeometry.setWidth(0); // 设置目标宽度为0
    //m_endGeometry.setHeight(100); // 设置目标高度为0
    //this->m_animation->setDuration(1000); // 动画时长
    //this->m_animation->setStartValue(m_startGeometry);
    //this->m_animation->setEndValue(m_endGeometry);
    //this->m_animation->setEasingCurve(QEasingCurve::InOutQuad); // 设置动画曲线
    //this->m_animation->start(); // 启动动画
    //// 最小化窗口
    //connect(this->m_animation.get(), &QPropertyAnimation::finished, this, [&]() {this->showMinimized();});
    STREAM_INFO()<<"最小化窗口";
    qobject_cast<QWidget*>(this->parent())->showMinimized();
}

void TitleWidget::on_max_toolButton_clicked(){
    emit maxScreen();
}

void TitleWidget::on_close_toolButton_clicked() {
    STREAM_INFO()<<"退出应用程序";
    QApplication::quit(); // 退出应用程序
}

void TitleWidget::onLeftMenu_recommend_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::RecommendForYou);
    else emit currentStackChange(StackType::RecommendForYou,true);
    this->m_lastType = RecommendForYou;
    qDebug()<<"为你推荐";
    STREAM_INFO()<<"切换为你推荐界面";
}

void TitleWidget::onLeftMenu_musicRepository_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::MusicRepository);
    else emit currentStackChange(StackType::MusicRepository,true);
    this->m_lastType = MusicRepository;
    qDebug()<<"点击乐库";
    STREAM_INFO()<<"切换乐库界面";
}

void TitleWidget::onLeftMenu_channel_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::Channel);
    else emit currentStackChange(StackType::Channel,true);
    this->m_lastType = Channel;
    qDebug()<<"点击频道";
    STREAM_INFO()<<"切换频道界面";
}

void TitleWidget::onLeftMenu_video_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::Video);
    else emit currentStackChange(StackType::Video,true);
    this->m_lastType = Video;
    qDebug()<<"点击视频";
    STREAM_INFO()<<"切换视频界面";
}

void TitleWidget::onLeftMenu_live_clicked() {
    ui->title_live_pushButton->clicked();
    ui->title_live_pushButton->setChecked(true);
    STREAM_INFO()<<"切换直播界面";
}

void TitleWidget::onLeftMenu_songList_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::SongList);
    else emit currentStackChange(StackType::SongList,true);
    this->m_lastType = SongList;
    qDebug()<<"点击歌单";
    STREAM_INFO()<<"切换歌单界面";
}

void TitleWidget::onLeftMenu_dailyRecommend_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::DailyRecommend);
    else emit currentStackChange(StackType::DailyRecommend,false);
    this->m_lastType = DailyRecommend;
    qDebug()<<"点击每日推荐";
    STREAM_INFO()<<"切换每日推荐界面";
}

void TitleWidget::onLeftMenu_collection_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::Collection);
    else emit currentStackChange(StackType::Collection,true);
    this->m_lastType = Collection;
    qDebug()<<"点击我的收藏";
    STREAM_INFO()<<"切换我的收藏界面";
}

void TitleWidget::onLeftMenu_localDownload_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::LocalDownload);
    else emit currentStackChange(StackType::LocalDownload,true);
    this->m_lastType = LocalDownload;
    qDebug()<<"点击本地与下载";
    STREAM_INFO()<<"切换本地与下载界面";
}

void TitleWidget::onLeftMenu_musicCloudDisk_clicked() {
    emit currentStackChange(StackType::MusicCloudDisk,false);
    this->m_lastType = MusicCloudDisk;
    qDebug()<<"点击音乐云盘";
    STREAM_INFO()<<"切换音乐云盘界面";
}

void TitleWidget::onLeftMenu_purchasedMusic_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::PurchasedMusic);
    else emit currentStackChange(StackType::PurchasedMusic,true);
    this->m_lastType = PurchasedMusic;
    qDebug()<<"点击已购音乐";
    STREAM_INFO()<<"切换音乐云盘界面";
}

void TitleWidget::onLeftMenu_recentlyPlayed_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::RecentlyPlayed);
    else emit currentStackChange(StackType::RecentlyPlayed,true);
    this->m_lastType = RecentlyPlayed;
    qDebug()<<"点击最近播放";
    STREAM_INFO()<<"切换最近播放界面";
}

void TitleWidget::onLeftMenu_allMusic_clicked() {
    if (m_lastType == MusicCloudDisk || m_lastType == DailyRecommend) emit currentStackChange(StackType::AllMusic);
    else emit currentStackChange(StackType::AllMusic,true);
    this->m_lastType = AllMusic;
    qDebug()<<"点击全部音乐";
    STREAM_INFO()<<"切换全部音乐界面";
}

void TitleWidget::setMenuPosition(const QPoint &pos) {
    this->m_menuPosition = pos;
    // 获取屏幕的尺寸
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // 计算菜单右侧的全局位置
    //int menuLeftPos = pos.x() - m_menu->width();
    //qDebug() << "Menu width:" << m_titleOptMenu->width();
    int menuRightPos  = pos.x() + m_titleOptMenu->width();
    int menuBottomPos = pos.y() + m_titleOptMenu->height();
    //int menuTopPos = pos.y() - m_menu->height();
    // 若菜单左侧超出屏幕左侧 (不存在)
    //if(menuLeftPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setX(10);
    //}
    // 如果菜单右侧超出屏幕右侧
    if (menuRightPos > screenGeometry.right()) {
        // 动态调整菜单位置，使其在屏幕内显示
        int offset = menuRightPos - screenGeometry.right() + 5;
        m_menuPosition.setX(pos.x() - offset);
    }
    // 如果菜单下侧超出屏幕下侧
    if (menuBottomPos > screenGeometry.bottom()) {
        // 动态调整菜单位置，使其在屏幕内显示
        int offset = menuBottomPos - screenGeometry.bottom() + 5;
        m_menuPosition.setY(pos.y() - offset);
    }
    // 如果菜单下侧超出屏幕下侧（不存在）
    //if(menuTopPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setY(10);
    //}
}

QPixmap TitleWidget::getRoundedPixmap(const QPixmap &src, QSize size, int radius) {
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
