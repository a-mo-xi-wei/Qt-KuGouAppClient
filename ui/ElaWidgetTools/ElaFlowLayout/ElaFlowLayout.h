/**
 * @file ElaFlowLayout.h
 * @brief 定义 ElaFlowLayout 类，提供流式布局功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAFLOWLAYOUT_H
#define ELAFLOWLAYOUT_H

#include <QLayout>
#include <QStyle>

#include "stdafx.h"

class ElaFlowLayoutPrivate;

/**
 * @class ElaFlowLayout
 * @brief 流式布局类，支持动态排列控件并提供动画效果
 */
class ELA_EXPORT ElaFlowLayout : public QLayout
{
    Q_OBJECT
    Q_Q_CREATE(ElaFlowLayout)

public:
    /**
     * @brief 构造函数，初始化流式布局（带父控件）
     * @param parent 父控件指针
     * @param margin 布局边距，默认为 -1（使用默认边距）
     * @param hSpacing 水平间距，默认为 -1（自动计算）
     * @param vSpacing 垂直间距，默认为 -1（自动计算）
     */
    explicit ElaFlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    /**
     * @brief 构造函数，初始化流式布局（无父控件）
     * @param margin 布局边距，默认为 -1（使用默认边距）
     * @param hSpacing 水平间距，默认为 -1（自动计算）
     * @param vSpacing 垂直间距，默认为 -1（自动计算）
     */
    explicit ElaFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);

    /**
     * @brief 析构函数，释放流式布局资源
     */
    ~ElaFlowLayout();

    /**
     * @brief 添加布局项
     * @param item 布局项指针
     */
    void addItem(QLayoutItem* item) override;

    /**
     * @brief 获取水平间距
     * @return 水平间距值
     */
    int horizontalSpacing() const;

    /**
     * @brief 获取垂直间距
     * @return 垂直间距值
     */
    int verticalSpacing() const;

    /**
     * @brief 获取扩展方向
     * @return 扩展方向（Qt::Orientations）
     */
    Qt::Orientations expandingDirections() const override;

    /**
     * @brief 检查是否支持高度依赖宽度
     * @return 是否支持高度依赖宽度
     */
    bool hasHeightForWidth() const override;

    /**
     * @brief 根据宽度计算高度
     * @param width 宽度值
     * @return 对应的高度值
     */
    int heightForWidth(int width) const override;

    /**
     * @brief 获取布局项数量
     * @return 布局项数量
     */
    int count() const override;

    /**
     * @brief 获取指定索引的布局项
     * @param index 布局项索引
     * @return 布局项指针
     */
    QLayoutItem* itemAt(int index) const override;

    /**
     * @brief 获取布局的最小尺寸
     * @return 最小尺寸
     */
    QSize minimumSize() const override;

    /**
     * @brief 设置布局的几何形状
     * @param rect 几何区域
     */
    void setGeometry(const QRect& rect) override;

    /**
     * @brief 获取布局的推荐尺寸
     * @return 推荐尺寸
     */
    QSize sizeHint() const override;

    /**
     * @brief 移除并返回指定索引的布局项
     * @param index 布局项索引
     * @return 布局项指针，若索引无效则返回 nullptr
     */
    QLayoutItem* takeAt(int index) override;

    /**
     * @brief 设置是否启用动画
     * @param isAnimation 是否启用动画
     */
    void setIsAnimation(bool isAnimation);
};

#endif // ELAFLOWLAYOUT_H