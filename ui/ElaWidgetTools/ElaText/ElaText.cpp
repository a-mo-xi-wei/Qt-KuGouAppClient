/**
 * @file ElaText.cpp
 * @brief 实现 ElaText 类，提供自定义文本标签功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaTextPrivate.h"

#include <QContextMenuEvent>
#include <QPainter>

/**
 * @brief 构造函数，初始化文本标签
 * @param parent 父控件指针，默认为 nullptr
 */
ElaText::ElaText(QWidget* parent)
    : QLabel(parent), d_ptr(new ElaTextPrivate())
{
    Q_D(ElaText);
    d->q_ptr = this;
    d->_pTextStyle = ElaTextType::NoStyle;
    d->_pElaIcon = ElaIconType::None;
    setObjectName("ElaText");
    setStyleSheet("#ElaText{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
    textFont.setPixelSize(28);
    setFont(textFont);
    setWordWrap(true);
    d->_themeMode = eTheme->getThemeMode();
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaTextPrivate::onThemeChanged);
}

/**
 * @brief 构造函数，初始化带文本的文本标签
 * @param text 文本内容
 * @param parent 父控件指针，默认为 nullptr
 */
ElaText::ElaText(QString text, QWidget* parent)
    : ElaText(parent)
{
    setText(text);
}

/**
 * @brief 构造函数，初始化带文本和像素大小的文本标签
 * @param text 文本内容
 * @param pixelSize 字体像素大小
 * @param parent 父控件指针，默认为 nullptr
 */
ElaText::ElaText(QString text, int pixelSize, QWidget* parent)
    : ElaText(text, parent)
{
    QFont font = this->font();
    font.setPixelSize(pixelSize);
    setFont(font);
}

/**
 * @brief 析构函数，释放文本标签资源
 */
ElaText::~ElaText()
= default;

/**
 * @brief 设置是否任意换行
 * @param isWrapAnywhere 是否任意换行
 */
void ElaText::setIsWrapAnywhere(bool isWrapAnywhere)
{
    Q_D(ElaText);
    setWordWrap(isWrapAnywhere);
    d->_isWrapAnywhere = isWrapAnywhere;
}

/**
 * @brief 获取是否任意换行
 * @return 是否任意换行
 */
bool ElaText::getIsWrapAnywhere() const
{
    Q_D(const ElaText);
    return d->_isWrapAnywhere;
}

/**
 * @brief 设置字体像素大小
 * @param size 像素大小
 */
void ElaText::setTextPixelSize(int size)
{
    QFont font = this->font();
    font.setPixelSize(size);
    setFont(font);
}

/**
 * @brief 获取字体像素大小
 * @return 像素大小
 */
int ElaText::getTextPixelSize() const
{
    return this->font().pixelSize();
}

/**
 * @brief 设置字体点大小
 * @param size 点大小
 */
void ElaText::setTextPointSize(int size)
{
    QFont font = this->font();
    font.setPointSize(size);
    setFont(font);
}

/**
 * @brief 获取字体点大小
 * @return 点大小
 */
int ElaText::getTextPointSize() const
{
    return this->font().pointSize();
}

/**
 * @brief 设置文本样式
 * @param textStyle 文本样式
 */
void ElaText::setTextStyle(ElaTextType::TextStyle textStyle)
{
    Q_D(ElaText);
    QFont textFont = font();
    d->_pTextStyle = textStyle;
    switch (textStyle)
    {
    case ElaTextType::NoStyle:
    {
        break;
    }
    case ElaTextType::Caption:
    {
        textFont.setPixelSize(12);
        break;
    }
    case ElaTextType::Body:
    {
        textFont.setPixelSize(13);
        break;
    }
    case ElaTextType::BodyStrong:
    {
        textFont.setPixelSize(13);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Subtitle:
    {
        textFont.setPixelSize(20);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Title:
    {
        textFont.setPixelSize(28);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::TitleLarge:
    {
        textFont.setPixelSize(40);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    case ElaTextType::Display:
    {
        textFont.setPixelSize(48);
        textFont.setWeight(QFont::DemiBold);
        break;
    }
    }
    setFont(textFont);
}

/**
 * @brief 获取文本样式
 * @return 文本样式
 */
ElaTextType::TextStyle ElaText::getTextStyle() const
{
    Q_D(const ElaText);
    return d->_pTextStyle;
}

/**
 * @brief 设置图标
 * @param elaIcon 图标名称
 */
void ElaText::setElaIcon(ElaIconType::IconName elaIcon)
{
    Q_D(ElaText);
    d->_pElaIcon = elaIcon;
    update();
    Q_EMIT pElaIconChanged();
}

/**
 * @brief 获取图标
 * @return 图标名称
 */
ElaIconType::IconName ElaText::getElaIcon() const
{
    Q_D(const ElaText);
    return d->_pElaIcon;
}

/**
 * @brief 绘制事件，绘制文本或图标
 * @param event 绘图事件
 */
void ElaText::paintEvent(QPaintEvent* event)
{
    Q_D(ElaText);
    if (d->_pElaIcon != ElaIconType::None)
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(this->font().pixelSize());
        painter.setFont(iconFont);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        painter.drawText(rect(), Qt::AlignCenter, QChar((unsigned short)d->_pElaIcon));
        painter.restore();
    }
    else
    {
        if (wordWrap() && d->_isWrapAnywhere)
        {
            QPainter painter(this);
            painter.save();
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
            painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere, text());
            painter.restore();
        }
        else
        {
            QLabel::paintEvent(event);
        }
    }
}