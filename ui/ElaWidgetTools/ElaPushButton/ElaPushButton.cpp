/**
 * @file ElaPushButton.cpp
 * @brief 实现 ElaPushButton 类，提供自定义按钮功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaPushButton.h"
#include "ElaTheme.h"
#include "ElaPushButtonPrivate.h"

#include <QPainter>
#include <QPainterPath>

/**
 * @brief 定义 BorderRadius 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, int,    BorderRadius)

/**
 * @brief 定义 LightDefaultColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightDefaultColor)

/**
 * @brief 定义 DarkDefaultColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkDefaultColor)

/**
 * @brief 定义 LightHoverColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightHoverColor)

/**
 * @brief 定义 DarkHoverColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkHoverColor)

/**
 * @brief 定义 LightPressColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightPressColor)

/**
 * @brief 定义 DarkPressColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkPressColor)

/**
 * @brief 构造函数，初始化按钮
 * @param parent 父控件指针，默认为 nullptr
 */
ElaPushButton::ElaPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaPushButtonPrivate())
{
    Q_D(ElaPushButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = eTheme->getThemeMode();
    d->_pLightDefaultColor = ElaThemeColor(ElaThemeType::Light, BasicBase);
    d->_pDarkDefaultColor = ElaThemeColor(ElaThemeType::Dark, BasicBase);
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHover);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHover);
    d->_pLightPressColor = ElaThemeColor(ElaThemeType::Light, BasicPress);
    d->_pDarkPressColor = ElaThemeColor(ElaThemeType::Dark, BasicPress);
    d->_lightTextColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_darkTextColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    setMouseTracking(true);
    setFixedHeight(38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

/**
 * @brief 构造函数，初始化带文本的按钮
 * @param text 按钮文本
 * @param parent 父控件指针，默认为 nullptr
 */
ElaPushButton::ElaPushButton(QString text, QWidget* parent)
    : ElaPushButton(parent)
{
    setText(text);
}

/**
 * @brief 析构函数，释放按钮资源
 */
ElaPushButton::~ElaPushButton()
{
}

/**
 * @brief 设置亮色主题文本颜色
 * @param color 文本颜色
 */
void ElaPushButton::setLightTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_lightTextColor = color;
}

/**
 * @brief 获取亮色主题文本颜色
 * @return 文本颜色
 */
QColor ElaPushButton::getLightTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_lightTextColor;
}

/**
 * @brief 设置暗色主题文本颜色
 * @param color 文本颜色
 */
void ElaPushButton::setDarkTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_darkTextColor = color;
}

/**
 * @brief 获取暗色主题文本颜色
 * @return 文本颜色
 */
QColor ElaPushButton::getDarkTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_darkTextColor;
}

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件对象
 */
void ElaPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = true;
    QPushButton::mousePressEvent(event);
}

/**
 * @brief 鼠标释放事件处理
 * @param event 鼠标事件对象
 */
void ElaPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

/**
 * @brief 绘制事件，绘制按钮外观
 * @param event 绘图事件
 */
void ElaPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * (d->_shadowBorderWidth), height() - 2 * d->_shadowBorderWidth);
    if (d->_themeMode == ElaThemeType::Light)
    {
        painter.setPen(ElaThemeColor(ElaThemeType::Light, BasicBorder));
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pLightPressColor : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pDarkPressColor : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor) : ElaThemeColor(d->_themeMode, BasicDisable));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(ElaThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }
    //文字绘制
    painter.setPen(isEnabled() ? d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor : ElaThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}