//
// Created by WeiWang on 25-3-25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TitleWidget.h" resolved

#include "TitleWidget.h"
#include "ui_TitleWidget.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QShortcut>
#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TitleWidget)
    , m_closeDialog(std::make_unique<ElaExitDialog>(this->window()))
{
    ui->setupUi(this);
    initUi();
    this->setAttribute(Qt::WA_StyledBackground, true);
    qApp->installEventFilter(this);
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
    // 初始化栈，并将初始界面类型压入栈中
    this->m_curType = StackType::RecommendForYou;
    this->m_lastType = StackType::RecommendForYou;
    m_backTypeStack.push(StackType::RecommendForYou);
}

TitleWidget::~TitleWidget() {
    delete ui;
}

void TitleWidget::setEnableChange(const bool &flag) {
    this->m_enableChange = flag;
}

void TitleWidget::initUi() {
    //设置toolTip
    {
    //title_music_pushButton
    auto title_music_pushButton_toolTip = new ElaToolTip(ui->title_music_pushButton);
    title_music_pushButton_toolTip->setToolTip(QStringLiteral("音乐"));

    //title_live_pushButton
    auto title_live_pushButton_toolTip = new ElaToolTip(ui->title_live_pushButton);
    title_live_pushButton_toolTip->setToolTip(QStringLiteral("直播"));

    //title_listen_book_pushButton
    auto title_listen_book_pushButton_toolTip = new ElaToolTip(ui->title_listen_book_pushButton);
    title_listen_book_pushButton_toolTip->setToolTip(QStringLiteral("听书"));

    //title_search_pushButton
    auto title_search_pushButton_toolTip = new ElaToolTip(ui->title_search_pushButton);
    title_search_pushButton_toolTip->setToolTip(QStringLiteral("探索"));

    //listen_toolButton
    auto listen_toolButton_toolTip = new ElaToolTip(ui->listen_toolButton);
    listen_toolButton_toolTip->setToolTip(QStringLiteral("听歌识曲"));

    //title_portrait_label
    auto title_portrait_label_toolTip = new ElaToolTip(ui->title_portrait_label);
    title_portrait_label_toolTip->setToolTip(QStringLiteral("头像"));

    //title_name_label
    auto title_name_label_toolTip = new ElaToolTip(ui->title_name_label);
    title_name_label_toolTip->setToolTip(QStringLiteral("昵称"));

    //title_gender_label
    auto title_gender_label_toolTip = new ElaToolTip(ui->title_gender_label);
    title_gender_label_toolTip->setToolTip(QStringLiteral("性别"));

    //theme_toolButton
    auto theme_toolButton_toolTip = new ElaToolTip(ui->theme_toolButton);
    theme_toolButton_toolTip->setToolTip(QStringLiteral("主题"));

    //message_toolButton
    auto message_toolButton_toolTip = new ElaToolTip(ui->message_toolButton);
    message_toolButton_toolTip->setToolTip(QStringLiteral("消息"));

    //menu_toolButton
    auto menu_toolButton_toolTip = new ElaToolTip(ui->menu_toolButton);
    menu_toolButton_toolTip->setToolTip(QStringLiteral("菜单"));

    //min_toolButton
    auto min_toolButton_toolTip = new ElaToolTip(ui->min_toolButton);
    min_toolButton_toolTip->setToolTip(QStringLiteral("最小化"));

    //max_toolButton
    auto max_toolButton_toolTip = new ElaToolTip(ui->max_toolButton);
    max_toolButton_toolTip->setToolTip(QStringLiteral("最大化"));

    //close_toolButton
    auto close_toolButton_toolTip = new ElaToolTip(ui->close_toolButton);
    close_toolButton_toolTip->setToolTip(QStringLiteral("关闭"));
}

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
    setTitleIndex(1);

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

    m_closeDialog->setParent(this->window());
    m_closeDialog->hide();
    connect(m_closeDialog.get(), &ElaExitDialog::rightButtonClicked, this, []{qApp->quit();});
    connect(m_closeDialog.get(), &ElaExitDialog::middleButtonClicked, this, [=]() {
        m_closeDialog->close();
        on_min_toolButton_clicked();
    });

    //专门处理标题menu
    connect(ui->menu_toolButton,&QToolButton::clicked,this,&TitleWidget::on_menu_toolButton_clicked);

    // 创建快捷键对象，绑定 F5 到 title_refresh_toolButton 的点击事件
    const auto shortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
    connect(shortcut, &QShortcut::activated, ui->title_refresh_toolButton, &QToolButton::click);
}

void TitleWidget::showMenu(const QPoint &pos) {
    setMenuPosition(pos);
    //qDebug()<<"当前Menu位置: "<<pos<<" 显示菜单";
    this->m_titleOptMenu->popup(this->m_menuPosition);
    //this->m_titleOptMenu->show();
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
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::BackButton) {
            if (this->m_enableChange) {
                //qDebug() << "全局监听：鼠标返回键被按下";
                on_title_return_toolButton_clicked();
                return true; // 表示事件已处理，不再继续传播
            }
        }
        if (mouseEvent->button() == Qt::ForwardButton) {
            if (this->m_enableChange) {
                //qDebug() << "全局监听：鼠标前进键被按下";
                if (!m_frontTypeStack.isEmpty()) {
                    StackType nextType = m_frontTypeStack.pop();
                    m_backTypeStack.push(this->m_curType); // 当前状态存入返回栈

                    // 更新界面状态
                    if (nextType == TitleLive) {
                        ui->title_live_pushButton->setChecked(true);
                        emit currentStackChange(StackType::TitleLive);
                        emit leftMenuShow(false);
                        setTitleIndex(2);
                        qDebug() << "[前进] 直播";
                        STREAM_INFO() << "前进到直播界面";
                    }
                    else if (nextType == ListenBook) {
                        ui->title_listen_book_pushButton->setChecked(true);
                        emit currentStackChange(StackType::ListenBook);
                        emit leftMenuShow(false);
                        setTitleIndex(3);
                        qDebug() << "[前进] 听书";
                        STREAM_INFO() << "前进到听书界面";
                    }
                    else if (nextType == Search) {
                        ui->title_search_pushButton->setChecked(true);
                        emit currentStackChange(StackType::Search);
                        emit leftMenuShow(false);
                        setTitleIndex(4);
                        qDebug() << "[前进] 探索";
                        STREAM_INFO() << "前进到探索界面";
                    }
                    else {
                        ui->title_music_pushButton->setChecked(true);
                        setTitleIndex(1);
                        emit leftMenuShow(true);

                        // 统一状态更新
                        this->m_lastType = nextType;
                        this->m_curType = nextType;
                        bool condition = false;
                        if (!m_backTypeStack.isEmpty()) {
                            const StackType top = m_backTypeStack.top();
                            condition = (top == MusicCloudDisk || top == DailyRecommend);
                        }

                        // 触发界面更新（与返回逻辑保持模式一致）
                        switch (nextType) {
                            case RecommendForYou: {
                                if (condition) {
                                    emit currentStackChange(StackType::RecommendForYou);
                                } else {
                                    emit currentStackChange(StackType::RecommendForYou, true);
                                }
                                qDebug() << "[前进] 为你推荐";
                                STREAM_INFO() << "前进到推荐界面";
                                break;
                            }
                            case MusicRepository: {
                                if (condition) {
                                    emit currentStackChange(StackType::MusicRepository);
                                } else {
                                    emit currentStackChange(StackType::MusicRepository, true);
                                }
                                qDebug() << "[前进] 乐库";
                                STREAM_INFO() << "前进到乐库界面";
                                break;
                            }
                            case Channel: {
                                if (condition) {
                                    emit currentStackChange(StackType::Channel);
                                } else {
                                    emit currentStackChange(StackType::Channel, true);
                                }
                                qDebug() << "[前进] 频道";
                                STREAM_INFO() << "前进到频道界面";
                                break;
                            }
                            case Video: {
                                if (condition) {
                                    emit currentStackChange(StackType::Video);
                                } else {
                                    emit currentStackChange(StackType::Video, true);
                                }
                                qDebug() << "[前进] 视频";
                                STREAM_INFO() << "前进到视频界面";
                                break;
                            }
                            case AiChat: {
                                if (condition) {
                                    emit currentStackChange(StackType::AiChat);
                                } else {
                                    emit currentStackChange(StackType::AiChat, true);
                                }
                                qDebug() << "[前进] Ai对话";
                                STREAM_INFO() << "前进到Ai对话界面";
                                break;
                            }
                            case SongList: {
                                if (condition) {
                                    emit currentStackChange(StackType::SongList);
                                } else {
                                    emit currentStackChange(StackType::SongList, true);
                                }
                                qDebug() << "[前进] 歌单";
                                STREAM_INFO() << "前进到歌单界面";
                                break;
                            }
                            case DailyRecommend: {
                                emit currentStackChange(StackType::DailyRecommend, false);
                                qDebug() << "[前进] 每日推荐";
                                STREAM_INFO() << "前进到每日推荐";
                                break;
                            }
                            case Collection: {
                                if (condition) {
                                    emit currentStackChange(StackType::Collection);
                                } else {
                                    emit currentStackChange(StackType::Collection, true);
                                }
                                qDebug() << "[前进] 我的收藏";
                                STREAM_INFO() << "前进到收藏界面";
                                break;
                            }
                            case LocalDownload: {
                                if (condition) {
                                    emit currentStackChange(StackType::LocalDownload);
                                } else {
                                    emit currentStackChange(StackType::LocalDownload, true);
                                }
                                qDebug() << "[前进] 本地下载";
                                STREAM_INFO() << "前进到本地下载";
                                break;
                            }
                            case MusicCloudDisk: {
                                emit currentStackChange(StackType::MusicCloudDisk, false);
                                qDebug() << "[前进] 音乐云盘";
                                STREAM_INFO() << "前进到云盘界面";
                                break;
                            }
                            case PurchasedMusic: {
                                if (condition) {
                                    emit currentStackChange(StackType::PurchasedMusic);
                                } else {
                                    emit currentStackChange(StackType::PurchasedMusic, true);
                                }
                                qDebug() << "[前进] 已购音乐";
                                STREAM_INFO() << "前进到已购音乐";
                                break;
                            }
                            case RecentlyPlayed: {
                                if (condition) {
                                    emit currentStackChange(StackType::RecentlyPlayed);
                                } else {
                                    emit currentStackChange(StackType::RecentlyPlayed, true);
                                }
                                qDebug() << "[前进] 最近播放";
                                STREAM_INFO() << "前进到最近播放";
                                break;
                            }
                            case AllMusic: {
                                if (condition) {
                                    emit currentStackChange(StackType::AllMusic);
                                } else {
                                    emit currentStackChange(StackType::AllMusic, true);
                                }
                                qDebug() << "[前进] 全部音乐";
                                STREAM_INFO() << "前进到全部音乐";
                                break;
                            }
                            default: {
                                emit currentStackChange(StackType::RecommendForYou, true);
                                qDebug() << "[前进] 默认跳转推荐";
                                STREAM_INFO() << "前进到默认推荐";
                            }
                        }
                    }
                    this->m_curType = nextType; // 最终确认当前类型
                }
                return true;
            }
        }
    }
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
    STREAM_INFO() << "返回键被按下";
    if (!m_backTypeStack.isEmpty()) {
        this->m_lastType = m_backTypeStack.pop(); // 先弹出再赋值
        m_frontTypeStack.push(this->m_curType);
        if (this->m_lastType == TitleLive) {
            ui->title_live_pushButton->setChecked(true);
            emit currentStackChange(StackType::TitleLive);
            emit leftMenuShow(false);
            setTitleIndex(2);
            qDebug()<<"[回退] 直播";
            STREAM_INFO()<<"切换直播界面";
        }
        else if (this->m_lastType == ListenBook) {
            ui->title_listen_book_pushButton->setChecked(true);
            emit currentStackChange(StackType::ListenBook);
            emit leftMenuShow(false);
            setTitleIndex(3);
            qDebug()<<"[回退] 听书";
            STREAM_INFO()<<"切换听书界面";
        }
        else if (this->m_lastType == Search) {
            ui->title_search_pushButton->setChecked(true);
            emit currentStackChange(StackType::Search);
            emit leftMenuShow(false);
            setTitleIndex(4);
            qDebug()<<"[回退] 探索";
            STREAM_INFO()<<"切换探索界面";
        }
        else {
            ui->title_music_pushButton->setChecked(true);
            setTitleIndex(1);
            emit leftMenuShow(true);
            switch (this->m_lastType) {
                case RecommendForYou: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::RecommendForYou);
                    } else {
                        emit currentStackChange(StackType::RecommendForYou, true);
                    }
                    qDebug() << "[回退] 为你推荐";
                    STREAM_INFO() << "切换为你推荐界面";
                    break;
                }
                case MusicRepository: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::MusicRepository);
                    } else {
                        emit currentStackChange(StackType::MusicRepository, true);
                    }
                    qDebug() << "[回退] 乐库";
                    STREAM_INFO() << "切换乐库界面";
                    break;
                }
                case Channel: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::Channel);
                    } else {
                        emit currentStackChange(StackType::Channel, true);
                    }
                    qDebug() << "[回退] 频道";
                    STREAM_INFO() << "切换频道界面";
                    break;
                }
                case Video: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::Video);
                    } else {
                        emit currentStackChange(StackType::Video, true);
                    }
                    qDebug() << "[回退] 视频";
                    STREAM_INFO() << "切换视频界面";
                    break;
                }
                case AiChat: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::AiChat);
                    } else {
                        emit currentStackChange(StackType::AiChat, true);
                    }
                    qDebug() << "[回退] Ai对话";
                    STREAM_INFO() << "切换Ai对话界面";
                    break;
                }
                case SongList: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::SongList);
                    } else {
                        emit currentStackChange(StackType::SongList, true);
                    }
                    qDebug() << "[回退] 歌单";
                    STREAM_INFO() << "切换歌单界面";
                    break;
                }
                case DailyRecommend: {
                    emit currentStackChange(StackType::DailyRecommend, false);
                    qDebug() << "[回退] 每日推荐";
                    STREAM_INFO() << "切换每日推荐界面";
                    break;
                }
                case Collection: {
                    //qDebug()<<"this->m_curType："<<this->m_curType;
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::Collection);
                    } else {
                        emit currentStackChange(StackType::Collection, true);
                    }
                    qDebug() << "[回退] 我的收藏";
                    STREAM_INFO() << "切换我的收藏界面";
                    break;
                }
                case LocalDownload: {
                    //qDebug()<<"this->m_curType："<<this->m_curType;
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::LocalDownload);
                    } else {
                        emit currentStackChange(StackType::LocalDownload, true);
                    }
                    qDebug() << "[回退] 本地与下载";
                    STREAM_INFO() << "切换本地与下载界面";
                    break;
                }
                case MusicCloudDisk: {
                    emit currentStackChange(StackType::MusicCloudDisk, false);
                    qDebug() << "[回退] 音乐云盘";
                    STREAM_INFO() << "切换音乐云盘界面";
                    break;
                }
                case PurchasedMusic: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::PurchasedMusic);
                    } else {
                        emit currentStackChange(StackType::PurchasedMusic, true);
                    }
                    qDebug() << "[回退] 已购音乐";
                    STREAM_INFO() << "切换已购音乐界面";
                    break;
                }
                case RecentlyPlayed: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::RecentlyPlayed);
                    } else {
                        emit currentStackChange(StackType::RecentlyPlayed, true);
                    }
                    qDebug() << "[回退] 最近播放";
                    STREAM_INFO() << "切换最近播放界面";
                    break;
                }
                case AllMusic: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::AllMusic);
                    } else {
                        emit currentStackChange(StackType::AllMusic, true);
                    }
                    qDebug() << "[回退] 全部音乐";
                    STREAM_INFO() << "切换全部音乐界面";
                    break;
                }
                default: {
                    if (this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend) {
                        emit currentStackChange(StackType::RecommendForYou);
                    } else {
                        emit currentStackChange(StackType::RecommendForYou, true);
                    }
                    qDebug() << "[回退] 默认为你推荐";
                    STREAM_INFO() << "切换默认推荐界面";
                }
            }
            STREAM_INFO() << "切换音乐界面";
        }
        this->m_curType = this->m_lastType;
    }
}

void TitleWidget::on_title_refresh_toolButton_clicked() {
    emit refresh();
    qDebug() << "刷新界面";
    STREAM_INFO() << "刷新界面";
}

void TitleWidget::on_title_music_pushButton_clicked() {
    ui->title_music_pushButton->setChecked(true);
    setTitleIndex(1);
    //显示menu
    emit leftMenuShow(true);
    switch (this->m_lastType) {
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
        case AiChat:
            onLeftMenu_ai_chat_clicked();
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
    ui->title_live_pushButton->setChecked(true);
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::TitleLive);
    emit leftMenuShow(false);
    setTitleIndex(2);
    this->m_curType = StackType::TitleLive;
    qDebug()<<"直播";
    STREAM_INFO()<<"切换直播界面";
}

void TitleWidget::on_title_listen_book_pushButton_clicked() {
    ui->title_listen_book_pushButton->setChecked(true);
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::ListenBook);
    emit leftMenuShow(false);
    setTitleIndex(3);
    this->m_curType = StackType::ListenBook;
    qDebug()<<"听书";
    STREAM_INFO()<<"切换听书界面";
}

void TitleWidget::on_title_search_pushButton_clicked() {
    ui->title_search_pushButton->setChecked(true);
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::Search);
    emit leftMenuShow(false);
    setTitleIndex(4);
    this->m_curType = StackType::Search;
    qDebug()<<"探索";
    STREAM_INFO()<<"切换探索界面";
}

void TitleWidget::on_listen_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "听歌识曲 功能未实现 敬请期待",
                            1000,this->window());
}

void TitleWidget::on_theme_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "主题 功能未实现 敬请期待",
                            1000,this->window());
}

void TitleWidget::on_message_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "消息 功能未实现 敬请期待",
                            1000,this->window());
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
    STREAM_INFO()<<"显示closeDialog";
    m_closeDialog->show();
}

void TitleWidget::onLeftMenu_recommend_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::RecommendForYou);
    else emit currentStackChange(StackType::RecommendForYou,true);
    this->m_curType = RecommendForYou;
    qDebug()<<"为你推荐";
    STREAM_INFO()<<"切换为你推荐界面";
}

void TitleWidget::onLeftMenu_musicRepository_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::MusicRepository);
    else emit currentStackChange(StackType::MusicRepository,true);
    this->m_curType = MusicRepository;
    qDebug()<<"点击乐库";
    STREAM_INFO()<<"切换乐库界面";
}

void TitleWidget::onLeftMenu_channel_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::Channel);
    else emit currentStackChange(StackType::Channel,true);
    this->m_curType = Channel;
    qDebug()<<"点击频道";
    STREAM_INFO()<<"切换频道界面";
}

void TitleWidget::onLeftMenu_video_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::Video);
    else emit currentStackChange(StackType::Video,true);
    this->m_curType = Video;
    qDebug()<<"点击视频";
    STREAM_INFO()<<"切换视频界面";
}

void TitleWidget::onLeftMenu_live_clicked() {
    ui->title_live_pushButton->clicked();
    ui->title_live_pushButton->setChecked(true);
    STREAM_INFO()<<"切换直播界面";
}

void TitleWidget::onLeftMenu_ai_chat_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::AiChat);
    else emit currentStackChange(StackType::AiChat,true);
    this->m_curType = AiChat;
    qDebug()<<"点击Ai对话";
    STREAM_INFO()<<"切换Ai对话界面";
}

void TitleWidget::onLeftMenu_songList_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::SongList);
    else emit currentStackChange(StackType::SongList,true);
    this->m_curType = SongList;
    qDebug()<<"点击歌单";
    STREAM_INFO()<<"切换歌单界面";
}

void TitleWidget::onLeftMenu_dailyRecommend_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::DailyRecommend);
    else emit currentStackChange(StackType::DailyRecommend,false);
    this->m_curType = DailyRecommend;
    qDebug()<<"点击每日推荐";
    STREAM_INFO()<<"切换每日推荐界面";
}

void TitleWidget::onLeftMenu_collection_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::Collection);
    else emit currentStackChange(StackType::Collection,true);
    this->m_curType = Collection;
    qDebug()<<"点击我的收藏";
    STREAM_INFO()<<"切换我的收藏界面";
}

void TitleWidget::onLeftMenu_localDownload_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::LocalDownload);
    else emit currentStackChange(StackType::LocalDownload,true);
    this->m_curType = LocalDownload;
    qDebug()<<"点击本地与下载";
    STREAM_INFO()<<"切换本地与下载界面";
}

void TitleWidget::onLeftMenu_musicCloudDisk_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::MusicCloudDisk,false);
    this->m_curType = MusicCloudDisk;
    qDebug()<<"点击音乐云盘";
    STREAM_INFO()<<"切换音乐云盘界面";
}

void TitleWidget::onLeftMenu_purchasedMusic_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::PurchasedMusic);
    else emit currentStackChange(StackType::PurchasedMusic,true);
    this->m_curType = PurchasedMusic;
    qDebug()<<"点击已购音乐";
    STREAM_INFO()<<"切换音乐云盘界面";
}

void TitleWidget::onLeftMenu_recentlyPlayed_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::RecentlyPlayed);
    else emit currentStackChange(StackType::RecentlyPlayed,true);
    this->m_curType = RecentlyPlayed;
    qDebug()<<"点击最近播放";
    STREAM_INFO()<<"切换最近播放界面";
}

void TitleWidget::onLeftMenu_allMusic_clicked() {
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::AllMusic);
    else emit currentStackChange(StackType::AllMusic,true);
    this->m_curType = AllMusic;
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

void TitleWidget::setTitleIndex(const int &index) const {
    // 预防非法索引
    if (index < 1 || index > 4) return;

    // 通过索引直接控制显示状态
    ui->title_index_label1->setVisible(index == 1);
    ui->title_index_label2->setVisible(index == 2);
    ui->title_index_label3->setVisible(index == 3);
    ui->title_index_label4->setVisible(index == 4);
}
