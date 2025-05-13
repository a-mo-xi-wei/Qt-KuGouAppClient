/**
* @file ElaThemePrivate.h
 * @brief 定义 ElaThemePrivate 类，管理主题的私有实现
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELATHEMEPRIVATE_H
#define ELATHEMEPRIVATE_H

#include <QColor>
#include <QMap>

#include "Def.h"

class ElaTheme;

/**
 * @class ElaThemePrivate
 * @brief 主题管理的私有实现类，存储主题模式和颜色列表
 */
class ElaThemePrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(ElaTheme)

public:
    /**
     * @brief 构造函数，初始化私有主题对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaThemePrivate(QObject *parent = nullptr);

    /**
     * @brief 析构函数，释放私有主题资源
     */
    ~ElaThemePrivate() override;

private:
    /**
     * @brief 当前主题模式，默认为 Light
     */
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};

    /**
     * @brief 浅色主题颜色列表
     */
    QColor _lightThemeColorList[40];

    /**
     * @brief 深色主题颜色列表
     */
    QColor _darkThemeColorList[40];

    /**
     * @brief 初始化主题颜色列表
     */
    void _initThemeColor();
};

#endif // ELATHEMEPRIVATE_H