/**
 * @file PopularLeftWidget.h
 * @brief 定义 PopularLeftWidget 类，提供热门左侧控件
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#ifndef POPULARLEFTWIDGET_H
#define POPULARLEFTWIDGET_H

#include <QWidget>

/**
 * @class QGraphicsOpacityEffect
 * @brief 图形透明效果类
 */
class QGraphicsOpacityEffect;
/**
 * @class QParallelAnimationGroup
 * @brief 并行动画组类
 */
class QParallelAnimationGroup;
/**
 * @class QLabel
 * @brief 标签控件类
 */
class QLabel;
/**
 * @class QPropertyAnimation
 * @brief 属性动画类
 */
class QPropertyAnimation;
/**
 * @class QToolButton
 * @brief 工具按钮类
 */
class QToolButton;

/**
 * @class PopularLeftWidget
 * @brief 热门左侧控件类，支持动画和交互按钮
 */
class PopularLeftWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int animatedY READ animatedY WRITE setAnimatedY)
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)

public:
    /**
     * @brief 构造函数，初始化热门左侧控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PopularLeftWidget(QWidget *parent = nullptr);

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
     * @brief 初始化界面
     * @note 初始化直播按钮和标签
     */
    void initUi();

    /**
     * @brief 执行向上动画
     * @note 调整位置和透明度
     */
    void animationUp() const;

    /**
     * @brief 执行向下动画
     * @note 调整位置和透明度
     */
    void animationDown() const;

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制背景遮罩和控件位置
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 进入事件
     * @param event 进入事件
     * @note 显示控件并触发向上动画
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 离开事件
     * @param event 离开事件
     * @note 隐藏控件并触发向下动画
     */
    void leaveEvent(QEvent *event) override;

private:
    QToolButton             *m_enterLivBtn{};        ///< 进入直播间按钮
    int                      m_animatedY;            ///< 动画 Y 坐标
    int                      m_alpha;                ///< 透明度值
    QPropertyAnimation      *m_posAnimation{};       ///< 位置动画
    QPropertyAnimation      *m_alphaAnimation{};     ///< 透明度动画
    QGraphicsOpacityEffect  *m_opacityEffect{};      ///< 透明效果
    QParallelAnimationGroup *m_aniGroup{};           ///< 动画组
    QLabel                  *m_stopLab{};            ///< 停止标签
    QLabel                  *m_refreshLab{};         ///< 刷新标签
    QLabel                  *m_muteLab{};            ///< 静音标签
};

#endif // POPULARLEFTWIDGET_H