/**
* @file ElaSuggestDelegate.h
 * @brief 定义 ElaSuggestDelegate 类，建议项委托
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#ifndef ELASUGGESTDELEGATE_H
#define ELASUGGESTDELEGATE_H

#include <QStyledItemDelegate>

#include "Def.h"

/**
 * @class ElaSuggestDelegate
 * @brief 建议项委托，负责绘制建议项
 */
class ElaSuggestDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaSuggestDelegate(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaSuggestDelegate() override;

protected:
    /**
     * @brief 绘制函数
     * @param painter 画家
     * @param option 样式选项
     * @param index 模型索引
     * @note 绘制建议项背景、文本和图标
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /**
     * @brief 大小提示
     * @param option 样式选项
     * @param index 模型索引
     * @return 建议项尺寸
     */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;                       ///< 当前主题模式
};

#endif // ELASUGGESTDELEGATE_H