/**
 * @file ElaMessageButton.cpp
 * @brief 实现 ElaMessageButton 类，提供消息触发按钮功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaMessageButton.h"
#include "ElaMessageButtonPrivate.h"
#include "ElaMessageBar.h"
#include "ElaTheme.h"
#include "logger.hpp"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

/**
 * @brief 定义 BorderRadius 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, int,                              BorderRadius)

/**
 * @brief 定义 BarTitle 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, QString,                         BarTitle)

/**
 * @brief 定义 BarText 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, QString,                         BarText)

/**
 * @brief 定义 DisplayMsec 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, int,                             DisplayMsec)

/**
 * @brief 定义 MessageTargetWidget 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, QWidget*,                        MessageTargetWidget)

/**
 * @brief 定义 MessageMode 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, ElaMessageBarType::MessageMode,  MessageMode)

/**
 * @brief 定义 PositionPolicy 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaMessageButton, ElaMessageBarType::PositionPolicy, PositionPolicy)

/**
 * @brief 获取当前文件目录
 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化消息按钮
 * @param parent 父控件指针，默认为 nullptr
 */
ElaMessageButton::ElaMessageButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaMessageButtonPrivate())
{
    Q_D(ElaMessageButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setMouseTracking(true);
    setFixedSize(80, 38);
    QFont font("ElaAwesome");
    font.setPixelSize(15);
    setFont(font);
    setText("Message");
    setObjectName("ElaMessageButton");
    setStyleSheet("#ElaMessageButton{background-color:transparent;}");
    d->_pDisplayMsec = 2000;
    d->_pMessageMode = ElaMessageBarType::Success;
    d->_pPositionPolicy = ElaMessageBarType::TopRight;
    d->_themeMode = eTheme->getThemeMode();
    d->_pMessageTargetWidget = nullptr;
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
    connect(this, &ElaMessageButton::clicked, this, [=]() {
        switch(d->_pMessageMode)
        {
        case ElaMessageBarType::Success:
        {
            ElaMessageBar::success(d->_pPositionPolicy, d->_pBarTitle, d->_pBarText, d->_pDisplayMsec, d->_pMessageTargetWidget);
            break;
        }
        case ElaMessageBarType::Warning:
        {
            ElaMessageBar::warning(d->_pPositionPolicy, d->_pBarTitle, d->_pBarText, d->_pDisplayMsec, d->_pMessageTargetWidget);
            break;
        }
        case ElaMessageBarType::Information:
        {
            ElaMessageBar::information(d->_pPositionPolicy, d->_pBarTitle, d->_pBarText, d->_pDisplayMsec, d->_pMessageTargetWidget);
            break;
        }
        case ElaMessageBarType::Error:
        {
            ElaMessageBar::error(d->_pPositionPolicy, d->_pBarTitle, d->_pBarText, d->_pDisplayMsec, d->_pMessageTargetWidget);
            break;
        }
        }
    });
}

/**
 * @brief 构造函数，初始化带文本的消息按钮
 * @param text 按钮文本
 * @param parent 父控件指针，默认为 nullptr
 */
ElaMessageButton::ElaMessageButton(const QString& text, QWidget* parent)
    : ElaMessageButton(parent)
{
    setText(text);
}

/**
 * @brief 析构函数，释放消息按钮资源
 */
ElaMessageButton::~ElaMessageButton()
= default;

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件对象
 */
void ElaMessageButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaMessageButton);
    if (event->button() == Qt::LeftButton)
    {
        d->_isLeftButtonPress = true;
    }
    QPushButton::mousePressEvent(event);
}

/**
 * @brief 鼠标释放事件处理
 * @param event 鼠标事件对象
 */
void ElaMessageButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaMessageButton);
    d->_isLeftButtonPress = false;
    QPushButton::mouseReleaseEvent(event);
}

/**
 * @brief 绘制事件，绘制按钮外观
 * @param event 绘图事件
 */
void ElaMessageButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMessageButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_penBorderWidth + d->_shadowBorderWidth, d->_penBorderWidth + d->_shadowBorderWidth, width() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth), height() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth));
    painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), d->_penBorderWidth));
    painter.setBrush(isEnabled() ? underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase) : ElaThemeColor(d->_themeMode, BasicDisable));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    //文字绘制
    painter.setPen(isEnabled() ? d->_isLeftButtonPress ? ElaThemeColor(d->_themeMode, BasicTextPress) : ElaThemeColor(d->_themeMode, BasicText) : ElaThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(rect(), Qt::AlignCenter, text());
    painter.restore();
}