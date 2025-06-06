/**
 * @file TitleWidget.cpp
 * @brief 实现 TitleWidget 类，提供标题栏功能和界面导航管理
 * @author WeiWang
 * @date 2025-03-25
 * @version 1.0
 */

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

#include "MySearchLineEdit.h"

/**
 * @brief 获取当前文件所在目录路径
 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化标题栏控件
 * @param parent 父控件指针，默认为 nullptr
 */
TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TitleWidget)
    , m_closeDialog(std::make_unique<ElaExitDialog>(this->window()))
{
    ui->setupUi(this);
    initUi();
    qApp->installEventFilter(this); ///< 安装应用程序级事件过滤器

    // 加载样式表
        QFile file(GET_CURRENT_DIR + QStringLiteral("/title.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
        // qDebug() << "样式表打开失败QAQ"; ///< 调试用，记录样式表加载失败
            return;
        }

    // 初始化导航栈
    this->m_curType = StackType::RecommendForYou;
    this->m_lastType = StackType::RecommendForYou;
    m_backTypeStack.push(StackType::RecommendForYou);
}

/**
 * @brief 析构函数，释放资源
 */
TitleWidget::~TitleWidget()
{
    delete ui;
}

/**
 * @brief 设置是否允许界面切换
 * @param flag 是否允许切换
 */
void TitleWidget::setEnableChange(const bool &flag)
{
    this->m_enableChange = flag;
}

/**
 * @brief 初始化界面元素
 */
void TitleWidget::initUi()
{
    // 设置工具提示
    {
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

    this->setStyleSheet("QWidget#TitleWidget{margin:5px;}"); ///< 设置外边距

    // 初始化标题选项菜单
    const auto menu = new MyMenu(MyMenu::MenuKind::TitleOption, this);
    m_titleOptMenu = menu->getMenu<TitleOptionMenu>();
    connect(m_titleOptMenu, &TitleOptionMenu::about, this, [this] { emit showAboutDialog(); });
    connect(m_titleOptMenu, &TitleOptionMenu::exit, this, &TitleWidget::on_close_toolButton_clicked);

    // 设置标题索引指示器
    ui->title_index_label1->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    ui->title_index_label2->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    ui->title_index_label3->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    ui->title_index_label4->setPixmap(
        QPixmap(QStringLiteral(":/Res/titlebar/h-line.png")).scaled(30, 15, Qt::KeepAspectRatio));
    setTitleIndex(1);

    // 设置搜索框和图标
    ui->title_line->setPixmap(QPixmap(QStringLiteral(":/Res/tabIcon/line-black.svg")));

    auto searchLineEdit = new MySearchLineEdit();
    searchLineEdit->setObjectName("searchLineEdit");
    searchLineEdit->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    searchLineEdit->setMinimumSize(100,40);
    searchLineEdit->setMaximumHeight(40);
    searchLineEdit->setBorderRadius(8);
    searchLineEdit->setClearButtonEnabled(true);
    searchLineEdit->setStyleSheet("padding-left: 10px;");
    auto font = QFont("AaSongLiuKaiTi");
    font.setWeight(QFont::Bold);
    font.setPointSize(12);
    searchLineEdit->setFont(font);
    ui->search_song_suggest_box->setMinimumWidth(0);
    ui->search_song_suggest_box->setLineEdit(searchLineEdit);
    searchLineEdit->setPlaceholderText("搜索歌曲");


    //除非自定义QToolButton否则达不到 CSS 中 border-image 的效果
    //ui->listen_toolButton->setIcon(QIcon(":/Res/titlebar/listen-music-black.svg"));

    QPixmap roundedPix = getRoundedPixmap(QPixmap(QStringLiteral(":/Res/window/portrait.jpg")),
                                       ui->title_portrait_label->size(), ui->title_portrait_label->size().width()/2);
    m_originalCover.load(QStringLiteral(":/Res/window/portrait.jpg"));

    // 设置圆角半径
    ui->title_portrait_label->setPixmap(roundedPix);
    ui->title_portrait_label->setScaledContents(false); // 禁止 QLabel 自动缩放
    ui->title_portrait_label->installEventFilter(this);

    // 设置性别图标
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

    // 初始化退出对话框
    m_closeDialog->setParent(this->window());
    m_closeDialog->hide();
    connect(m_closeDialog.get(), &ElaExitDialog::rightButtonClicked, this, []{qApp->quit();});
    connect(m_closeDialog.get(), &ElaExitDialog::middleButtonClicked, this, [=]() {
        m_closeDialog->close();
        on_min_toolButton_clicked();
    });

    // 连接菜单按钮
    connect(ui->menu_toolButton, &QToolButton::clicked, this, &TitleWidget::on_menu_toolButton_clicked);

    // 创建快捷键对象，绑定 F5 到 title_refresh_toolButton 的点击事件
    const auto shortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
    connect(shortcut, &QShortcut::activated, ui->title_refresh_toolButton, &QToolButton::click);
}

/**
 * @brief 显示标题选项菜单
 * @param pos 菜单显示的全局位置
 */
void TitleWidget::showMenu(const QPoint &pos)
{
    setMenuPosition(pos);
    // qDebug() << "当前Menu位置: " << pos << " 显示菜单"; ///< 调试用，记录菜单位置
    this->m_titleOptMenu->popup(this->m_menuPosition); ///< 显示菜单
    // qDebug() << "显示菜单成功"; ///< 调试用，记录菜单显示
}

/**
 * @brief 获取最大化按钮
 * @return 最大化按钮指针
 */
RippleButton *TitleWidget::max_toolButton() const
{
    return ui->max_toolButton;
}

/**
 * @brief 重写鼠标双击事件，触发最大化
 * @param event 鼠标事件
 */
void TitleWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        on_max_toolButton_clicked(); ///< 左键双击触发最大化
    }
}

/**
 * @brief 重写鼠标按下事件，处理右键菜单
 * @param event 鼠标事件
 */
void TitleWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        showMenu(this->mapToGlobal(event->pos())); ///< 右键显示菜单
    } else {
        QWidget::mousePressEvent(event);
    }
}

/**
 * @brief 重写绘制事件，绘制带圆角的线性渐变阴影
 * @param ev 绘制事件
 */
void TitleWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿
    p.setPen(Qt::NoPen); ///< 无边框

    QRect shadowRect = rect().adjusted(5, 5, -4, 2); ///< 调整阴影区域
    QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomLeft());
    gradient.setColorAt(0, QColor(QStringLiteral("#87CEFA"))); ///< 起始颜色
    gradient.setColorAt(1, QColor(QStringLiteral("#eef2ff"))); ///< 结束颜色
    p.setBrush(gradient);

    // 创建一个 QPainterPath，只在左上和右上角有圆角
    QPainterPath path;
    int radius = 8; ///< 圆角半径
    path.moveTo(shadowRect.topLeft() + QPoint(radius, 0));
    path.lineTo(shadowRect.topRight() - QPoint(radius, 0));
    path.quadTo(shadowRect.topRight(), shadowRect.topRight() + QPoint(0, radius));
    path.lineTo(shadowRect.bottomRight());
    path.lineTo(shadowRect.bottomLeft());
    path.lineTo(shadowRect.topLeft() + QPoint(0, radius));
    path.quadTo(shadowRect.topLeft(), shadowRect.topLeft() + QPoint(radius, 0));
    path.closeSubpath();

    p.drawPath(path); ///< 绘制路径
}

/**
 * @brief 重写事件过滤器，处理鼠标返回/前进键和头像动画
 * @param watched 监视的对象
 * @param event 事件对象
 * @return 是否处理事件
 */
bool TitleWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::BackButton) {
            if (this->m_enableChange) {
                //qDebug() << "全局监听：鼠标返回键被按下"; ///< 调试用，记录返回键
                on_title_return_toolButton_clicked();
                return true; // 表示事件已处理，不再继续传播
            }
        }
        if (mouseEvent->button() == Qt::ForwardButton) {
            if (this->m_enableChange) {
                //qDebug() << "全局监听：鼠标前进键被按下"; ///< 调试用，记录前进键
                if (!m_frontTypeStack.isEmpty()) {
                    StackType nextType = m_frontTypeStack.pop();
                    m_backTypeStack.push(this->m_curType); ///< 当前状态存入返回栈

                    // 更新界面状态
                    if (nextType == TitleLive) {
                        ui->title_live_pushButton->setChecked(true);
                        emit currentStackChange(StackType::TitleLive);
                        emit leftMenuShow(false);
                        setTitleIndex(2);
                    // qDebug() << "[前进] 直播"; ///< 调试用
                        STREAM_INFO() << "前进到直播界面";
                    }
                    else if (nextType == ListenBook) {
                        ui->title_listen_book_pushButton->setChecked(true);
                        emit currentStackChange(StackType::ListenBook);
                        emit leftMenuShow(false);
                        setTitleIndex(3);
                    // qDebug() << "[前进] 听书"; ///< 调试用
                        STREAM_INFO() << "前进到听书界面";
                    }
                    else if (nextType == Search) {
                        ui->title_search_pushButton->setChecked(true);
                        emit currentStackChange(StackType::Search);
                        emit leftMenuShow(false);
                        setTitleIndex(4);
                    // qDebug() << "[前进] 探索"; ///< 调试用
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
                    this->m_curType = nextType; ///< 更新当前类型
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

/**
 * @brief 返回按钮点击事件，触发界面回退
 */
void TitleWidget::on_title_return_toolButton_clicked()
{
    STREAM_INFO() << "返回键被按下";
    if (!m_backTypeStack.isEmpty()) {
        this->m_lastType = m_backTypeStack.pop();
        m_frontTypeStack.push(this->m_curType);
        if (this->m_lastType == TitleLive) {
            ui->title_live_pushButton->setChecked(true);
            emit currentStackChange(StackType::TitleLive);
            emit leftMenuShow(false);
            setTitleIndex(2);
            // qDebug() << "[回退] 直播"; ///< 调试用
            STREAM_INFO() << "切换直播界面";
        } else if (this->m_lastType == ListenBook) {
            ui->title_listen_book_pushButton->setChecked(true);
            emit currentStackChange(StackType::ListenBook);
            emit leftMenuShow(false);
            setTitleIndex(3);
            // qDebug() << "[回退] 听书"; ///< 调试用
            STREAM_INFO() << "切换听书界面";
        } else if (this->m_lastType == Search) {
            ui->title_search_pushButton->setChecked(true);
            emit currentStackChange(StackType::Search);
            emit leftMenuShow(false);
            setTitleIndex(4);
            // qDebug() << "[回退] 探索"; ///< 调试用
            STREAM_INFO() << "切换探索界面";
        } else {
            ui->title_music_pushButton->setChecked(true);
            setTitleIndex(1);
            emit leftMenuShow(true);
            switch (this->m_lastType) {
                case RecommendForYou:
                    emit currentStackChange(StackType::RecommendForYou,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 为你推荐"; ///< 调试用
                    STREAM_INFO() << "切换为你推荐界面";
                    break;
                case MusicRepository:
                    emit currentStackChange(StackType::MusicRepository,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 乐库"; ///< 调试用
                    STREAM_INFO() << "切换乐库界面";
                    break;
                case Channel:
                    emit currentStackChange(StackType::Channel,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 频道"; ///< 调试用
                    STREAM_INFO() << "切换频道界面";
                    break;
                case Video:
                    emit currentStackChange(StackType::Video,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 视频"; ///< 调试用
                    STREAM_INFO() << "切换视频界面";
                    break;
                case AiChat:
                    emit currentStackChange(StackType::AiChat,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] Ai对话"; ///< 调试用
                    STREAM_INFO() << "切换Ai对话界面";
                    break;
                case SongList:
                    emit currentStackChange(StackType::SongList,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 歌单"; ///< 调试用
                    STREAM_INFO() << "切换歌单界面";
                    break;
                case DailyRecommend:
                    emit currentStackChange(StackType::DailyRecommend, false);
                    // qDebug() << "[回退] 每日推荐"; ///< 调试用
                    STREAM_INFO() << "切换每日推荐界面";
                    break;
                case Collection:
                    emit currentStackChange(StackType::Collection,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 我的收藏"; ///< 调试用
                    STREAM_INFO() << "切换我的收藏界面";
                    break;
                case LocalDownload:
                    emit currentStackChange(StackType::LocalDownload,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 本地与下载"; ///< 调试用
                    STREAM_INFO() << "切换本地与下载界面";
                    break;
                case MusicCloudDisk:
                    emit currentStackChange(StackType::MusicCloudDisk, false);
                    // qDebug() << "[回退] 音乐云盘"; ///< 调试用
                    STREAM_INFO() << "切换音乐云盘界面";
                    break;
                case PurchasedMusic:
                    emit currentStackChange(StackType::PurchasedMusic,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 已购音乐"; ///< 调试用
                    STREAM_INFO() << "切换已购音乐界面";
                    break;
                case RecentlyPlayed:
                    emit currentStackChange(StackType::RecentlyPlayed,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 最近播放"; ///< 调试用
                    STREAM_INFO() << "切换最近播放界面";
                    break;
                case AllMusic:
                    emit currentStackChange(StackType::AllMusic,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 全部音乐"; ///< 调试用
                    STREAM_INFO() << "切换全部音乐界面";
                    break;
                default:
                    emit currentStackChange(StackType::RecommendForYou,
                                            !(this->m_curType == MusicCloudDisk || this->m_curType == DailyRecommend));
                    // qDebug() << "[回退] 默认为你推荐"; ///< 调试用
                    STREAM_INFO() << "切换默认推荐界面";
            }
            STREAM_INFO() << "切换音乐界面";
        }
        this->m_curType = this->m_lastType; ///< 更新当前类型
    }
}

/**
 * @brief 刷新按钮点击事件，触发界面刷新
 */
void TitleWidget::on_title_refresh_toolButton_clicked()
{
    emit refresh();
    // qDebug() << "刷新界面"; ///< 调试用
    STREAM_INFO() << "刷新界面";
}

/**
 * @brief 音乐按钮点击事件，切换到音乐界面
 */
void TitleWidget::on_title_music_pushButton_clicked()
{
    ui->title_music_pushButton->setChecked(true);
    setTitleIndex(1);
    emit leftMenuShow(true); ///< 显示左侧菜单
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
            break;
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
        default:
            onLeftMenu_recommend_clicked();
    }
    STREAM_INFO() << "切换音乐界面";
}

/**
 * @brief 直播按钮点击事件，切换到直播界面
 */
void TitleWidget::on_title_live_pushButton_clicked()
{
    ui->title_live_pushButton->setChecked(true);
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::TitleLive);
    emit leftMenuShow(false);
    setTitleIndex(2);
    this->m_curType = StackType::TitleLive;
    // qDebug() << "直播"; ///< 调试用
    STREAM_INFO() << "切换直播界面";
}

/**
 * @brief 听书按钮点击事件，切换到听书界面
 */
void TitleWidget::on_title_listen_book_pushButton_clicked()
{
    ui->title_listen_book_pushButton->setChecked(true);
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::ListenBook);
    emit leftMenuShow(false);
    setTitleIndex(3);
    this->m_curType = StackType::ListenBook;
    // qDebug() << "听书"; ///< 调试用
    STREAM_INFO() << "切换听书界面";
}

/**
 * @brief 探索按钮点击事件，切换到探索界面
 */
void TitleWidget::on_title_search_pushButton_clicked()
{
    ui->title_search_pushButton->setChecked(true);
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::Search);
    emit leftMenuShow(false);
    setTitleIndex(4);
    this->m_curType = StackType::Search;
    // qDebug() << "探索"; ///< 调试用
    STREAM_INFO() << "切换探索界面";
}

/**
 * @brief 听歌识曲按钮点击事件，显示未实现提示
 */
void TitleWidget::on_listen_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "听歌识曲 功能未实现 敬请期待", 1000, this->window());
}

/**
 * @brief 主题按钮点击事件，显示未实现提示
 */
void TitleWidget::on_theme_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "主题 功能未实现 敬请期待", 1000, this->window());
}

/**
 * @brief 消息按钮点击事件，显示未实现提示
 */
void TitleWidget::on_message_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "消息 功能未实现 敬请期待", 1000, this->window());
}

/**
 * @brief 菜单按钮点击事件，显示标题选项菜单
 */
void TitleWidget::on_menu_toolButton_clicked()
{
    showMenu(QCursor::pos());
}

/**
 * @brief 最小化按钮点击事件，最小化窗口
 */
void TitleWidget::on_min_toolButton_clicked()
{
    // 以下为调试用动画代码，当前未启用
    // QRect m_startGeometry = this->geometry();
    // QRect m_endGeometry = m_startGeometry;
    // m_endGeometry.setHeight(100);
    // this->m_animation->setDuration(1000);
    // this->m_animation->setStartValue(m_startGeometry);
    // this->m_animation->setEndValue(m_endGeometry);
    // this->m_animation->setEasingCurve(QEasingCurve::InOutQuad);
    // this->m_animation->start();
    // connect(this->m_animation.get(), &QPropertyAnimation::finished, this, [&]() {this->showMinimized();});

    STREAM_INFO() << "最小化窗口";
    qobject_cast<QWidget *>(this->parent())->showMinimized(); ///< 最小化父窗口
}

/**
 * @brief 最大化按钮点击事件，触发最大化信号
 */
void TitleWidget::on_max_toolButton_clicked()
{
    emit maxScreen();
}

/**
 * @brief 关闭按钮点击事件，显示退出对话框
 */
void TitleWidget::on_close_toolButton_clicked()
{
    STREAM_INFO() << "显示closeDialog";
    m_closeDialog->show(); ///< 显示退出对话框
}

/**
 * @brief 左侧菜单推荐项点击事件，切换到推荐界面
 */
void TitleWidget::onLeftMenu_recommend_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::RecommendForYou);
    else emit currentStackChange(StackType::RecommendForYou,true);
    this->m_curType = RecommendForYou;
    qDebug()<<"为你推荐";
    STREAM_INFO()<<"切换为你推荐界面";
}

/**
 * @brief 左侧菜单乐库项点击事件，切换到乐库界面
 */
void TitleWidget::onLeftMenu_musicRepository_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::MusicRepository);
    else emit currentStackChange(StackType::MusicRepository,true);
    this->m_curType = MusicRepository;
    qDebug()<<"点击乐库";
    STREAM_INFO()<<"切换乐库界面";
}

/**
 * @brief 左侧菜单频道项点击事件，切换到频道界面
 */
void TitleWidget::onLeftMenu_channel_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::Channel);
    else emit currentStackChange(StackType::Channel,true);
    this->m_curType = Channel;
    qDebug()<<"点击频道";
    STREAM_INFO()<<"切换频道界面";
}

/**
 * @brief 左侧菜单视频项点击事件，切换到视频界面
 */
void TitleWidget::onLeftMenu_video_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::Video);
    else emit currentStackChange(StackType::Video,true);
    this->m_curType = Video;
    qDebug()<<"点击视频";
    STREAM_INFO()<<"切换视频界面";
}

/**
 * @brief 左侧菜单直播项点击事件，切换到直播界面
 */
void TitleWidget::onLeftMenu_live_clicked()
{
    ui->title_live_pushButton->clicked();
    ui->title_live_pushButton->setChecked(true);
    STREAM_INFO()<<"切换直播界面";
}

/**
 * @brief 左侧菜单AI对话项点击事件，切换到AI对话界面
 */
void TitleWidget::onLeftMenu_ai_chat_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::AiChat);
    else emit currentStackChange(StackType::AiChat,true);
    this->m_curType = AiChat;
    qDebug()<<"点击Ai对话";
    STREAM_INFO()<<"切换Ai对话界面";
}

/**
 * @brief 左侧菜单歌单项点击事件，切换到歌单界面
 */
void TitleWidget::onLeftMenu_songList_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::SongList);
    else emit currentStackChange(StackType::SongList,true);
    this->m_curType = SongList;
    qDebug()<<"点击歌单";
    STREAM_INFO()<<"切换歌单界面";
}

/**
 * @brief 左侧菜单每日推荐项点击事件，切换到每日推荐界面
 */
void TitleWidget::onLeftMenu_dailyRecommend_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::DailyRecommend);
    else emit currentStackChange(StackType::DailyRecommend,false);
    this->m_curType = DailyRecommend;
    qDebug()<<"点击每日推荐";
    STREAM_INFO()<<"切换每日推荐界面";
}

/**
 * @brief 左侧菜单收藏项点击事件，切换到收藏界面
 */
void TitleWidget::onLeftMenu_collection_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::Collection);
    else emit currentStackChange(StackType::Collection,true);
    this->m_curType = Collection;
    qDebug()<<"点击我的收藏";
    STREAM_INFO()<<"切换我的收藏界面";
}

/**
 * @brief 左侧菜单本地下载项点击事件，切换到本地下载界面
 */
void TitleWidget::onLeftMenu_localDownload_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::LocalDownload);
    else emit currentStackChange(StackType::LocalDownload,true);
    this->m_curType = LocalDownload;
    qDebug()<<"点击本地与下载";
    STREAM_INFO()<<"切换本地与下载界面";
}

/**
 * @brief 左侧菜单音乐云盘项点击事件，切换到音乐云盘界面
 */
void TitleWidget::onLeftMenu_musicCloudDisk_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    emit currentStackChange(StackType::MusicCloudDisk,false);
    this->m_curType = MusicCloudDisk;
    qDebug()<<"点击音乐云盘";
    STREAM_INFO()<<"切换音乐云盘界面";
}

/**
 * @brief 左侧菜单已购音乐项点击事件，切换到已购音乐界面
 */
void TitleWidget::onLeftMenu_purchasedMusic_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::PurchasedMusic);
    else emit currentStackChange(StackType::PurchasedMusic,true);
    this->m_curType = PurchasedMusic;
    qDebug()<<"点击已购音乐";
    STREAM_INFO()<<"切换音乐云盘界面";
}

/**
 * @brief 左侧菜单最近播放项点击事件，切换到最近播放界面
 */
void TitleWidget::onLeftMenu_recentlyPlayed_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::RecentlyPlayed);
    else emit currentStackChange(StackType::RecentlyPlayed,true);
    this->m_curType = RecentlyPlayed;
    qDebug()<<"点击最近播放";
    STREAM_INFO()<<"切换最近播放界面";
}

/**
 * @brief 左侧菜单全部音乐项点击事件，切换到全部音乐界面
 */
void TitleWidget::onLeftMenu_allMusic_clicked()
{
    this->m_lastType = this->m_curType;
    this->m_backTypeStack.push(m_lastType);
    if (m_backTypeStack.top() == MusicCloudDisk || m_backTypeStack.top() == DailyRecommend) emit currentStackChange(StackType::AllMusic);
    else emit currentStackChange(StackType::AllMusic,true);
    this->m_curType = AllMusic;
    qDebug()<<"点击全部音乐";
    STREAM_INFO()<<"切换全部音乐界面";
}

/**
 * @brief 设置菜单显示位置，确保在屏幕范围内
 * @param pos 菜单的全局位置
 */
void TitleWidget::setMenuPosition(const QPoint &pos)
{
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

/**
 * @brief 生成圆角图片
 * @param src 原始图片
 * @param size 目标尺寸
 * @param radius 圆角半径
 * @return 圆角图片
 */
QPixmap TitleWidget::getRoundedPixmap(const QPixmap &src, QSize size, int radius)
{
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
 * @brief 设置标题索引，控制界面指示器显示
 * @param index 标题索引（1-4）
 */
void TitleWidget::setTitleIndex(const int &index) const
{
    if (index < 1 || index > 4) return; ///< 预防非法索引

    ui->title_index_label1->setVisible(index == 1);
    ui->title_index_label2->setVisible(index == 2);
    ui->title_index_label3->setVisible(index == 3);
    ui->title_index_label4->setVisible(index == 4);
}