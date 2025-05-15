/**
* @file WaterDrop.h
 * @brief 定义 WaterDrop 类，提供水滴动画效果
 * @author WeiWang
 * @date 2025-03-13
 * @version 1.0
 */

#ifndef WATERDROP_H
#define WATERDROP_H

#include <QWidget>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QVariantAnimation;

/**
 * @class WaterDrop
 * @brief 自定义水滴动画控件类，继承自 QWidget，支持动态圆形扩展动画
 */
class MYWINDOW_EXPORT WaterDrop : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化水滴动画控件
     * @param parent 父控件指针，默认为 nullptr
     */
    WaterDrop(QWidget *parent = Q_NULLPTR);

    /**
     * @brief 移动水滴到指定位置
     * @param point 目标位置（全局坐标）
     */
    void move(const QPoint &point);

protected:
    /**
     * @brief 重写绘制事件，绘制水滴动画
     * @param event 绘制事件
     */
    void paintEvent(QPaintEvent *event) override;

    public slots:
        /**
         * @brief 处理动画半径变化
         * @param value 动画值
         */
        void onRadiusChanged(QVariant value);

private:
    QVariantAnimation *m_waterDropAnimation; ///< 动画对象
    int m_animationRadius;                   ///< 当前动画半径
};

#endif // WATERDROP_H