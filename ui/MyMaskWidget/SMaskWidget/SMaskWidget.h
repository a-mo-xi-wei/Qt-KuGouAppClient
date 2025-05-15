/**
 * @file SMaskWidget.h
 * @brief 定义 SMaskWidget 类，提供带动画的遮罩控件
 * @author [WeiWang]
 * @date 2025-05-15
 * @version 1.0
 */

#ifndef SMASKWIDGET_H
#define SMASKWIDGET_H

#include <QWidget>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYMASKWIDGET_LIBRARY)
#define MYMASKWIDGET_EXPORT Q_DECL_EXPORT
#else
#define MYMASKWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QParallelAnimationGroup;
class QPropertyAnimation;

/**
 * @class SMaskWidget
 * @brief 带动画的遮罩控件类，支持圆形和三角形动画效果
 */
class MYMASKWIDGET_EXPORT SMaskWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int animatedY READ animatedY WRITE setAnimatedY)
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)

public:
    /**
     * @brief 构造函数，初始化遮罩控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SMaskWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置默认圆形填充颜色
     * @param color 颜色值
     */
    void setDefaultFillCircleColor(const QColor &color);

    /**
     * @brief 设置默认三角形填充颜色
     * @param color 颜色值
     */
    void setDefaultFillTriangleColor(const QColor &color);

    /**
     * @brief 设置悬浮圆形填充颜色
     * @param color 颜色值
     */
    void setHoverFillCircleColor(const QColor &color);

    /**
     * @brief 设置悬浮三角形填充颜色
     * @param color 颜色值
     */
    void setHoverFillTriangleColor(const QColor &color);

    /**
     * @brief 设置进入控件时是否改变光标
     * @param change 是否改变光标
     */
    void setEnterWidgetChangeCursor(const bool &change);

    /**
     * @brief 设置边框圆角半径
     * @param radius 圆角半径
     */
    void setBorderRadius(const int &radius);

    /**
     * @brief 设置遮罩颜色
     * @param color 颜色值
     */
    void setMaskColor(const QColor &color);

    /**
     * @brief 设置基准尺寸
     * @param stander 基准尺寸
     */
    void setStander(const int &stander);

    /**
     * @brief 设置是否启用动画
     * @param move 是否启用动画
     */
    void setMove(const bool& move);

    /**
     * @brief 获取是否启用动画
     * @return 是否启用动画
     */
    bool getMove();

    /**
     * @brief 获取动画 Y 坐标
     * @return Y 坐标
     */
    int animatedY() const { return m_animatedY; }

    /**
     * @brief 获取透明度
     * @return 透明度值
     */
    int alpha() const { return m_alpha; }

    /**
     * @brief 启动向上动画
     */
    void animationUp();

    /**
     * @brief 启动向下动画
     */
    void animationDown();

public slots:
    /**
     * @brief 设置动画 Y 坐标
     * @param y Y 坐标
     */
    void setAnimatedY(int y);

    /**
     * @brief 设置透明度
     * @param alpha 透明度值
     */
    void setAlpha(int alpha);

private:
    /**
     * @brief 计算几何参数
     */
    void calOnce();

    /**
     * @brief 判断鼠标是否在圆形内
     * @param mouseX 鼠标 X 坐标
     * @param mouseY 鼠标 Y 坐标
     * @return 是否在圆形内
     */
    bool isMouseInCircle(const float &mouseX, const float &mouseY) const;

protected:
    /**
     * @brief 绘制事件，绘制遮罩和动画效果
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标移动事件
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 大小调整事件
     * @param event 大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    float                   m_w;                            ///< 控件宽度
    float                   m_h;                            ///< 控件高度
    float                   m_radius;                       ///< 圆形半径
    float                   m_centerX;                      ///< 圆形中心 X 坐标
    float                   m_centerY;                      ///< 圆形中心 Y 坐标
    QPointF                 m_ap;                           ///< 三角形顶点 A
    QPointF                 m_bp;                           ///< 三角形顶点 B
    QPointF                 m_cp;                           ///< 三角形顶点 C
    QColor                  m_defaultFillCircleColor;       ///< 默认圆形填充颜色
    QColor                  m_hoverFillCircleColor;         ///< 悬浮圆形填充颜色
    QColor                  m_defaultFillTriangleColor;     ///< 默认三角形填充颜色
    QColor                  m_hoverFillTriangleColor;       ///< 悬浮三角形填充颜色
    QColor                  m_maskColor;                    ///< 遮罩颜色
    bool                    m_isEnterWidgetChangeCursor;    ///< 是否进入控件改变光标
    bool                    m_isEnterCircle;                ///< 鼠标是否在圆形内
    int                     m_borderRadius;                 ///< 边框圆角半径
    int                     m_stander;                      ///< 基准尺寸
    int                     m_animatedY;                    ///< 动画 Y 坐标
    int                     m_alpha;                        ///< 透明度
    QPropertyAnimation*     m_posAnimation;                 ///< 位置动画
    QPropertyAnimation*     m_alphaAnimation;               ///< 透明度动画
    QParallelAnimationGroup* m_aniGroup;                    ///< 动画组
    bool                    m_isMove;                       ///< 是否启用动画
};

#endif // SMASKWIDGET_H