/**
 * @file ElaIconButton.cpp
 * @brief 实现 ElaIconButton 类，提供自定义图标按钮功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaIconButton.h"
#include "ElaIconButtonPrivate.h"
#include "ElaTheme.h"

#include <QEvent>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

/**
 * @brief 定义 BorderRadius 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, int,    BorderRadius)

/**
 * @brief 定义 Opacity 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, qreal,  Opacity)

/**
 * @brief 定义 LightHoverColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightHoverColor)

/**
 * @brief 定义 DarkHoverColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkHoverColor)

/**
 * @brief 定义 LightIconColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightIconColor)

/**
 * @brief 定义 DarkIconColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkIconColor)

/**
 * @brief 定义 LightHoverIconColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, LightHoverIconColor)

/**
 * @brief 定义 DarkHoverIconColor 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, QColor, DarkHoverIconColor)

/**
 * @brief 定义 IsSelected 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaIconButton, bool,   IsSelected)

/**
 * @brief 构造函数，初始化带图像的图标按钮
 * @param pix 按钮图像
 * @param parent 父控件指针，默认为 nullptr
 */
ElaIconButton::ElaIconButton(QPixmap pix, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_iconPix = pix.copy();
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

/**
 * @brief 构造函数，初始化带 Ela 图标的按钮
 * @param awesome Ela 图标类型
 * @param parent 父控件指针，默认为 nullptr
 */
ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(15);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

/**
 * @brief 构造函数，初始化带 Ela 图标和像素大小的按钮
 * @param awesome Ela 图标类型
 * @param pixelSize 图标像素大小
 * @param parent 父控件指针，默认为 nullptr
 */
ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

/**
 * @brief 构造函数，初始化带 Ela 图标、像素大小和固定尺寸的按钮
 * @param awesome Ela 图标类型
 * @param pixelSize 图标像素大小
 * @param fixedWidth 按钮固定宽度
 * @param fixedHeight 按钮固定高度
 * @param parent 父控件指针，默认为 nullptr
 */
ElaIconButton::ElaIconButton(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaIconButtonPrivate())
{
    Q_D(ElaIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = eTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    this->setFixedSize(fixedWidth, fixedHeight);
    connect(this, &ElaIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

/**
 * @brief 析构函数，释放图标按钮资源
 */
ElaIconButton::~ElaIconButton()
{
}

/**
 * @brief 设置 Ela 图标
 * @param awesome Ela 图标类型
 */
void ElaIconButton::setAwesome(ElaIconType::IconName awesome)
{
    Q_D(ElaIconButton);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
}

/**
 * @brief 获取当前 Ela 图标
 * @return 当前 Ela 图标类型
 */
ElaIconType::IconName ElaIconButton::getAwesome() const
{
    return this->d_ptr->_pAwesome;
}

/**
 * @brief 设置按钮图像
 * @param pix 按钮图像
 */
void ElaIconButton::setPixmap(QPixmap pix)
{
    Q_D(ElaIconButton);
    d->_iconPix = pix.copy();
}

/**
 * @brief 事件处理
 * @param event 事件对象
 * @return 是否处理事件
 */
bool ElaIconButton::event(QEvent* event)
{
    Q_D(ElaIconButton);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        if (isEnabled() && !d->_pIsSelected)
        {
            d->_isAlphaAnimationFinished = false;
            QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
            connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_isAlphaAnimationFinished = true;
            });
            alphaAnimation->setDuration(175);
            alphaAnimation->setStartValue(d->_pHoverAlpha);
            alphaAnimation->setEndValue(d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor.alpha() : d->_pDarkHoverColor.alpha());
            alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    }
    case QEvent::Leave:
    {
        if (isEnabled() && !d->_pIsSelected)
        {
            d->_isAlphaAnimationFinished = false;
            QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
            connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_isAlphaAnimationFinished = true;
            });
            alphaAnimation->setDuration(175);
            alphaAnimation->setStartValue(d->_pHoverAlpha);
            alphaAnimation->setEndValue(0);
            alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QPushButton::event(event);
}

/**
 * @brief 绘制事件，绘制按钮外观
 * @param event 绘图事件
 */
void ElaIconButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaIconButton);
    QPainter painter(this);
    painter.save();
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    if (d->_isAlphaAnimationFinished || d->_pIsSelected)
    {
        painter.setBrush(d->_pIsSelected ? d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor
                         : isEnabled()   ? underMouse() ? d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor : Qt::transparent
                                         : Qt::transparent);
    }
    else
    {
        QColor hoverColor = d->_themeMode == ElaThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor;
        hoverColor.setAlpha(d->_pHoverAlpha);
        painter.setBrush(hoverColor);
    }
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    // 图标绘制
    if (!d->_iconPix.isNull())
    {
        QPainterPath path;
        path.addEllipse(rect());
        painter.setClipPath(path);
        painter.drawPixmap(rect(), d->_iconPix);
    }
    else
    {
        painter.setPen(isEnabled() ? d->_themeMode == ElaThemeType::Light ? underMouse() ? d->_pLightHoverIconColor : d->_pLightIconColor : underMouse() ? d->_pDarkHoverIconColor : d->_pDarkIconColor
                                  : ElaThemeColor(d->_themeMode, BasicTextDisable));
        painter.drawText(rect(), Qt::AlignCenter, QChar((unsigned short)d->_pAwesome));
    }
    painter.restore();
}