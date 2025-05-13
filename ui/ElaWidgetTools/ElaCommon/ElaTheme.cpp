/**
 * @file ElaTheme.cpp
 * @brief 实现 ElaTheme 类，管理主题模式、颜色和阴影效果
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaTheme.h"
#include "ElaThemePrivate.h"

#include <QPainter>
#include <QPainterPath>

/**
 * @brief 定义 ElaTheme 单例实例
 */
Q_SINGLETON_CREATE_CPP(ElaTheme)

/**
 * @brief 构造函数，初始化主题管理对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaTheme::ElaTheme(QObject* parent)
    : QObject{parent}, d_ptr(new ElaThemePrivate())
{
    Q_D(ElaTheme);
    d->q_ptr = this;
    d->_initThemeColor();
}

/**
 * @brief 析构函数，释放主题管理资源
 */
ElaTheme::~ElaTheme()
{
}

/**
 * @brief 设置主题模式
 * @param themeMode 主题模式 (ElaThemeType::ThemeMode)
 */
void ElaTheme::setThemeMode(ElaThemeType::ThemeMode themeMode)
{
    Q_D(ElaTheme);
    d->_themeMode = themeMode;
    Q_EMIT themeModeChanged(d->_themeMode);
}

/**
 * @brief 获取当前主题模式
 * @return 当前主题模式 (ElaThemeType::ThemeMode)
 */
ElaThemeType::ThemeMode ElaTheme::getThemeMode() const
{
    Q_D(const ElaTheme);
    return d->_themeMode;
}

/**
 * @brief 绘制控件阴影效果
 * @param painter 绘图对象指针
 * @param widgetRect 控件矩形区域
 * @param shadowBorderWidth 阴影边框宽度
 * @param borderRadius 阴影圆角半径
 */
void ElaTheme::drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)
{
    Q_D(ElaTheme);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color = d->_themeMode == ElaThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
    for (int i = 0; i < shadowBorderWidth; i++)
    {
        path.addRoundedRect(shadowBorderWidth - i, shadowBorderWidth - i, widgetRect.width() - (shadowBorderWidth - i) * 2, widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
        int alpha = 1 * (shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter->setPen(color);
        painter->drawPath(path);
    }
    painter->restore();
}

/**
 * @brief 设置指定主题模式和主题颜色的颜色值
 * @param themeMode 主题模式 (ElaThemeType::ThemeMode)
 * @param themeColor 主题颜色类型 (ElaThemeType::ThemeColor)
 * @param newColor 新颜色值
 */
void ElaTheme::setThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor, QColor newColor)
{
    Q_D(ElaTheme);
    if (themeMode == ElaThemeType::Light)
    {
        d->_lightThemeColorList[themeColor] = newColor;
    }
    else
    {
        d->_darkThemeColorList[themeColor] = newColor;
    }
}

/**
 * @brief 获取指定主题模式和主题颜色的颜色值
 * @param themeMode 主题模式 (ElaThemeType::ThemeMode)
 * @param themeColor 主题颜色类型 (ElaThemeType::ThemeColor)
 * @return 对应的颜色值
 */
const QColor& ElaTheme::getThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor)
{
    Q_D(ElaTheme);
    if (themeMode == ElaThemeType::Light)
    {
        return d->_lightThemeColorList[themeColor];
    }
    else
    {
        return d->_darkThemeColorList[themeColor];
    }
}