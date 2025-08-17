/**
 * @file RippleButton.h
 * @brief 定义 RippleButton 类，提供带涟漪效果的按钮功能
 * @author [WeiWang]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef RIPPLEBUTTON_H
#define RIPPLEBUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QGraphicsDropShadowEffect>

#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

class QPointF;
class QTimer;
class QIcon;

/**
 * @class RippleButton
 * @brief 带涟漪效果的按钮类，支持阴影和自定义图标
 */
class UIWIDGETS_EXPORT RippleButton : public QToolButton
{
public:
    /**
     * @brief 构造函数，初始化涟漪按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RippleButton(QWidget *parent = nullptr);

    /**
     * @brief 设置涟漪动画速度
     * @param timeInterval 时间间隔（毫秒）
     */
    void setSpeed(const int &timeInterval) const;

    /**
     * @brief 设置涟漪填充颜色
     * @param fillcolor 填充颜色
     */
    void setFillColor(const QColor &fillcolor);

    /**
     * @brief 设置圆角半径
     * @param radius_ 圆角半径
     */
    void setRadius(int radius_);

    /**
     * @brief 设置按钮图标
     * @param ico 图标
     */
    void setMyIcon(const QIcon &ico);

    /**
     * @brief 设置按钮启用状态
     * @param flag 是否启用
     */
    void setEnabled(const bool &flag);

protected:
    /**
     * @brief 鼠标进入事件
     * @param event 进入事件对象
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件
     * @param event 事件对象
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 绘制事件
     * @param event 绘图事件对象
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 大小调整事件
     * @param event 大小调整事件对象
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<QGraphicsDropShadowEffect> m_effect{}; ///< 阴影效果
    int timeInterval = 10;                                 ///< 定时器时间间隔（毫秒）
    QPointF mouse_point;                                   ///< 鼠标位置
    QTimer *timer = nullptr;                               ///< 涟漪定时器
    QTimer *checkTimer = nullptr;                          ///< 鼠标检测定时器
    int max_radius;                                        ///< 最大涟漪半径
    int radius = 0;                                        ///< 当前涟漪半径
    int radius_var = 2;                                    ///< 半径变化量
    QColor fill_color;                                     ///< 填充颜色
    int frame_radius = 0;                                  ///< 圆角半径
    QIcon m_ico;                                           ///< 按钮图标
};

#endif // RIPPLEBUTTON_H