/**
 * @file MyScrollArea.h
 * @brief 定义 MyScrollArea 类，提供带平滑滚动和返回顶部按钮的滚动区域
 * @author WeiWang
 * @date 2024-10-16
 * @version 1.0
 */

#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include "UpToolButton.h"
#include <QScrollArea>
#include <QEasingCurve>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYSCROLLAREA_LIBRARY)
#define MYSCROLLAREA_EXPORT Q_DECL_EXPORT
#else
#define MYSCROLLAREA_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class MyScrollArea
 * @brief 自定义滚动区域类，继承自 QScrollArea，支持平滑滚动和返回顶部功能
 */
class MYSCROLLAREA_EXPORT MyScrollArea : public QScrollArea {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化滚动区域
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MyScrollArea(QWidget *parent = nullptr);

    /**
     * @brief 设置动画状态
     * @param animating 是否正在进行动画
     */
    void setAnimating(const bool &animating);

    /**
     * @brief 设置是否忽略滚轮事件
     * @param ignore 是否忽略
     */
    void setIgnore(const bool &ignore);

    /**
     * @brief 设置缓动曲线
     * @param curve 缓动曲线类型
     */
    void setEasingCurve(const QEasingCurve &curve);

    /**
     * @brief 平滑滚动到指定位置
     * @param targetValue 目标滚动条值
     */
    void smoothScrollTo(const int &targetValue);

protected:
    /**
     * @brief 重写滚轮事件，处理滚动和返回顶部按钮显示逻辑
     * @param event 滚轮事件
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * @brief 重写调整大小事件，更新返回顶部按钮位置
     * @param event 调整大小事件
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 重写隐藏事件，停止定时器
     * @param event 隐藏事件
     */
    void hideEvent(QHideEvent *event) override;

signals:
    /**
     * @brief 滚轮事件触发的信号，传递当前滚动条值
     * @param value 当前滚动条值
     */
    void wheelValue(const int &value);

private slots:
    /**
     * @brief 处理返回顶部按钮点击事件
     */
    void onUpBtnClicked();

    /**
     * @brief 处理返回顶部按钮的显示或隐藏逻辑
     */
    void onUpBtnShowOrNot();

private:
    bool m_isAnimating = false;            ///< 标记动画是否正在进行
    bool m_ignore = false;                 ///< 是否忽略滚轮事件
    std::unique_ptr<UpToolButton> m_upBtn{}; ///< 返回顶部按钮
    QTimer *m_scrollBarTimer{};            ///< 控制返回顶部按钮显示的定时器
    QEasingCurve m_curve = QEasingCurve::OutQuad; ///< 默认缓动曲线
};

#endif // MYSCROLLAREA_H