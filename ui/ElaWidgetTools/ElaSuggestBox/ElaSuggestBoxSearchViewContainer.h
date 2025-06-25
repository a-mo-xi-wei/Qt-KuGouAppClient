/**
* @file ElaSuggestBoxSearchViewContainer.h
 * @brief 定义 ElaSuggestBoxSearchViewContainer 类，建议框容器
 * @author WeiWang
 * @date 2025-06-25
 * @version 1.0
 */

#ifndef ELASUGGESTBOXSEARCHVIEWCONTAINER_H
#define ELASUGGESTBOXSEARCHVIEWCONTAINER_H

#include <QWidget>

#include "Def.h"

/**
 * @class ElaSuggestBoxSearchViewContainer
 * @brief 建议框容器，负责绘制阴影和圆角背景
 */
class ElaSuggestBoxSearchViewContainer : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaSuggestBoxSearchViewContainer(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaSuggestBoxSearchViewContainer() override;

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制阴影和圆角矩形背景
     */
    void paintEvent(QPaintEvent *event) override;

private:
    ElaThemeType::ThemeMode _themeMode;                       ///< 当前主题模式
};

#endif // ELASUGGESTBOXSEARCHVIEWCONTAINER_H