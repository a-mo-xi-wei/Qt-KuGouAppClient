/**
 * @file ElaTheme.h
 * @brief 定义 ElaTheme 类，提供主题管理和颜色配置功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELATHEME_H
#define ELATHEME_H

#include <QObject>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"

/**
 * @def eTheme
 * @brief 获取 ElaTheme 单例实例的宏
 */
#define eTheme ElaTheme::getInstance()

/**
 * @def ElaThemeColor(themeMode, themeColor)
 * @brief 获取指定主题模式和主题颜色的宏
 * @param themeMode 主题模式 (ElaThemeType::ThemeMode)
 * @param themeColor 主题颜色类型 (ElaThemeType::ThemeColor)
 */
#define ElaThemeColor(themeMode, themeColor) eTheme->getThemeColor(themeMode, ElaThemeType::themeColor)

class QPainter;
class ElaThemePrivate;

/**
 * @class ElaTheme
 * @brief 主题管理类，负责主题模式的切换、颜色配置和阴影效果绘制
 * @note 该类为单例模式，通过 eTheme 宏访问实例
 */
class ELA_EXPORT ElaTheme : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaTheme)
    Q_SINGLETON_CREATE_H(ElaTheme)

private:
    /**
     * @brief 构造函数，初始化主题管理对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaTheme(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放主题管理资源
     */
    ~ElaTheme();

public:
    /**
     * @brief 设置主题模式
     * @param themeMode 主题模式 (ElaThemeType::ThemeMode)
     */
    void setThemeMode(ElaThemeType::ThemeMode themeMode);

    /**
     * @brief 获取当前主题模式
     * @return 当前主题模式 (ElaThemeType::ThemeMode)
     */
    ElaThemeType::ThemeMode getThemeMode() const;

    /**
     * @brief 绘制控件阴影效果
     * @param painter 绘图对象指针
     * @param widgetRect 控件矩形区域
     * @param shadowBorderWidth 阴影边框宽度
     * @param borderRadius 阴影圆角半径
     */
    void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

    /**
     * @brief 设置指定主题模式和主题颜色的颜色值
     * @param themeMode 主题模式 (ElaThemeType::ThemeMode)
     * @param themeColor 主题颜色类型 (ElaThemeType::ThemeColor)
     * @param newColor 新颜色值
     */
    void setThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor, QColor newColor);

    /**
     * @brief 获取指定主题模式和主题颜色的颜色值
     * @param themeMode 主题模式 (ElaThemeType::ThemeMode)
     * @param themeColor 主题颜色类型 (ElaThemeType::ThemeColor)
     * @return 对应的颜色值
     */
    const QColor& getThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor);

Q_SIGNALS:
    /**
     * @brief 主题模式更改信号
     * @param themeMode 新的主题模式 (ElaThemeType::ThemeMode)
     */
    void themeModeChanged(ElaThemeType::ThemeMode themeMode);
};

#endif // ELATHEME_H