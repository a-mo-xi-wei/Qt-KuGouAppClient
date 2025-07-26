/**
 * @file ElaMessageBar.cpp
 * @brief 实现 ElaMessageBar 类，提供消息通知栏功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaMessageBar.h"
#include "ElaIconButton.h"
#include "ElaMessageBarPrivate.h"
#include "ElaTheme.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>

/**
 * @brief 构造函数，初始化消息栏
 * @param policy 位置策略
 * @param messageMode 消息类型
 * @param title 消息标题
 * @param text 消息内容
 * @param displayMsec 显示时长（毫秒）
 * @param parent 父控件指针，默认为 nullptr
 */
ElaMessageBar::ElaMessageBar(ElaMessageBarType::PositionPolicy policy, ElaMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec, QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaMessageBarPrivate())
{
    Q_D(ElaMessageBar);
    d->q_ptr = this;
    d->_borderRadius = 6;
    d->_title = title;
    d->_text = text;
    d->_policy = policy;
    d->_messageMode = messageMode;
    d->_themeMode = eTheme->getThemeMode();
    setFixedHeight(60);
    setMouseTracking(true);
    d->_pOpacity = 1;
    auto font = QFont("Microsoft YaHei");
    font.setWeight(QFont::Bold);
    setFont(font);
    parent->installEventFilter(this);
    d->_closeButton = new ElaIconButton(ElaIconType::Xmark, 17, d->_closeButtonWidth, 30, this);
    switch (d->_messageMode)
    {
    case ElaMessageBarType::Success:
    {
        d->_closeButton->setLightHoverColor(QColor(0xE6, 0xFC, 0xE3));
        d->_closeButton->setDarkHoverColor(QColor(0xE6, 0xFC, 0xE3));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    case ElaMessageBarType::Warning:
    {
        d->_closeButton->setLightHoverColor(QColor(0x5E, 0x4C, 0x22));
        d->_closeButton->setDarkHoverColor(QColor(0x5E, 0x4C, 0x22));
        d->_closeButton->setLightIconColor(Qt::white);
        d->_closeButton->setDarkIconColor(Qt::white);
        break;
    }
    case ElaMessageBarType::Information:
    {
        d->_closeButton->setLightHoverColor(QColor(0xEB, 0xEB, 0xEB));
        d->_closeButton->setDarkHoverColor(QColor(0xEB, 0xEB, 0xEB));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    case ElaMessageBarType::Error:
    {
        d->_closeButton->setLightHoverColor(QColor(0xF7, 0xE1, 0xE4));
        d->_closeButton->setDarkHoverColor(QColor(0xF7, 0xE1, 0xE4));
        d->_closeButton->setDarkIconColor(Qt::black);
        break;
    }
    }
    d->_closeButton->setBorderRadius(5);
    connect(d->_closeButton, &ElaIconButton::clicked, d, &ElaMessageBarPrivate::onCloseButtonClicked);
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 10, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(d->_closeButton);
    setObjectName("ElaMessageBar");
    setStyleSheet("#ElaMessageBar{background-color:transparent;font-weight: bold;}");
    d->_messageBarCreate(displayMsec);
}

/**
 * @brief 析构函数，释放消息栏资源
 */
ElaMessageBar::~ElaMessageBar()
= default;

/**
 * @brief 显示成功消息
 * @param policy 位置策略
 * @param title 消息标题
 * @param text 消息内容
 * @param displayMsec 显示时长（毫秒）
 * @param parent 父控件指针，默认为 nullptr
 */
void ElaMessageBar::success(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
     //qDebug() << QApplication::topLevelWidgets();
    if (!parent)
    {
        /*
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
              parent = widget;
           }
        }
        if (!parent)
        {
            return;
        }
        */
        parent = qApp->topLevelWidgets().at(0);
    }

    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Success, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

/**
 * @brief 显示警告消息
 * @param policy 位置策略
 * @param title 消息标题
 * @param text 消息内容
 * @param displayMsec 显示时长（毫秒）
 * @param parent 父控件指针，默认为 nullptr
 */
void ElaMessageBar::warning(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        /*
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
        */
        parent = qApp->topLevelWidgets().at(0);
    }
    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Warning, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

/**
 * @brief 显示信息消息
 * @param policy 位置策略
 * @param title 消息标题
 * @param text 消息内容
 * @param displayMsec 显示时长（毫秒）
 * @param parent 父控件指针，默认为 nullptr
 */
void ElaMessageBar::information(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        /*
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
        */
        parent = qApp->topLevelWidgets().at(0);
    }
    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Information, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

/**
 * @brief 显示错误消息
 * @param policy 位置策略
 * @param title 消息标题
 * @param text 消息内容
 * @param displayMsec 显示时长（毫秒）
 * @param parent 父控件指针，默认为 nullptr
 */
void ElaMessageBar::error(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent)
{
    if (!parent)
    {
        /*
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for (auto widget : widgetList)
        {
            if (widget->property("ElaBaseClassName").toString() == "ElaWindow")
            {
                parent = widget;
            }
        }
        if (!parent)
        {
            return;
        }
        */
        parent = qApp->topLevelWidgets().at(0);
    }
    ElaMessageBar* bar = new ElaMessageBar(policy, ElaMessageBarType::Error, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

/**
 * @brief 绘制事件，绘制消息栏外观
 * @param event 绘图事件
 */
void ElaMessageBar::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMessageBar);
    QPainter painter(this);
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_borderRadius);

    // 背景和图标绘制
    painter.save();
    painter.setPen(d->_themeMode == ElaThemeType::Light ? QColor(0xBE, 0xBA, 0xBE) : QColor(0x52, 0x50, 0x52));
    switch (d->_messageMode)
    {
    case ElaMessageBarType::Success:
    {
        d->_drawSuccess(&painter);
        break;
    }
    case ElaMessageBarType::Warning:
    {
        d->_drawWarning(&painter);
        break;
    }
    case ElaMessageBarType::Information:
    {
        d->_drawInformation(&painter);
        break;
    }
    case ElaMessageBarType::Error:
    {
        d->_drawError(&painter);
        break;
    }
    }
    // 文字绘制
    // 标题
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(16);
    font.setFamily("JetBrainsMono NF");
    painter.setFont(font);
    int titleTextWidth = painter.fontMetrics().horizontalAdvance(d->_title) + 1;
    if (titleTextWidth > 100)
    {
        titleTextWidth = 100;
    }
    int textFlags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere;
    painter.drawText(QRect(d->_leftPadding + d->_titleLeftSpacing, -1, titleTextWidth, height()), textFlags, d->_title);
    // 正文
    font.setWeight(QFont::Light);
    font.setPixelSize(17);
    font.setFamily("AaSongLiuKaiTi");
    painter.setFont(font);
    painter.drawText(QRect(d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing, 0, width() - (d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing + d->_closeButtonWidth + d->_closeButtonLeftRightMargin / 2), height()), textFlags, d->_text);
    int textHeight = painter.fontMetrics().boundingRect(QRect(d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing, 0, width() - (d->_leftPadding + d->_titleLeftSpacing + titleTextWidth + d->_textLeftSpacing + d->_closeButtonWidth + d->_closeButtonLeftRightMargin), height()), textFlags, d->_text).height();
    if (textHeight >= minimumHeight() - 20)
    {
        setMinimumHeight(textHeight + 20);
    }
    painter.restore();
}

/**
 * @brief 事件过滤器，处理父控件事件
 * @param watched 监控对象
 * @param event 事件对象
 * @return 是否处理事件
 */
bool ElaMessageBar::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaMessageBar);
    if (watched == parentWidget())
    {
        switch (event->type())
        {
        case QEvent::Resize:
        {
            QResizeEvent* resizeEvent = dynamic_cast<QResizeEvent*>(event);
            QSize offsetSize = parentWidget()->size() - resizeEvent->oldSize();
            if (d->_isNormalDisplay)
            {
                switch (d->_policy)
                {
                case ElaMessageBarType::Top:
                {
                    this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->y());
                    break;
                }
                case ElaMessageBarType::Bottom:
                {
                    this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->pos().y() + offsetSize.height());
                    break;
                }
                case ElaMessageBarType::Left:
                case ElaMessageBarType::TopLeft:
                {
                    this->move(d->_messageBarHorizontalMargin, this->pos().y());
                    break;
                }
                case ElaMessageBarType::BottomLeft:
                {
                    this->move(d->_messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
                    break;
                }
                case ElaMessageBarType::Right:
                case ElaMessageBarType::TopRight:
                {
                    this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin, this->y());
                    break;
                }
                case ElaMessageBarType::BottomRight:
                {
                    this->move(parentWidget()->width() - minimumWidth() - d->_messageBarHorizontalMargin, this->pos().y() + offsetSize.height());
                    break;
                }
                }
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return QWidget::eventFilter(watched, event);
}