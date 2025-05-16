/**
 * @file IconBesideTextToolButton.h
 * @brief 定义 IconBesideTextToolButton 类，提供图标与文本并排的工具按钮功能
 * @author [WeiWang]
 * @date 2025-05-15
 * @version 1.0
 */

#ifndef ICONBESIDETEXTTOOLBUTTON_H
#define ICONBESIDETEXTTOOLBUTTON_H

#include <QToolButton>

#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class IconBesideTextToolButton
 * @brief 图标与文本并排的工具按钮类，支持悬停效果和大小变化
 */
class MYBUTTON_EXPORT IconBesideTextToolButton : public QToolButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化图标与文本并排的工具按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit IconBesideTextToolButton(QWidget *parent = nullptr);

    /**
     * @brief 设置鼠标进入时的图标大小
     * @param size 图标大小
     */
    void setEnterIconSize(const QSize &size);

    /**
     * @brief 设置鼠标离开时的图标大小
     * @param size 图标大小
     */
    void setLeaveIconSize(const QSize &size);

    /**
     * @brief 设置鼠标进入时的字体大小
     * @param size 字体大小
     */
    void setEnterFontSize(const int &size);

    /**
     * @brief 设置鼠标离开时的字体大小
     * @param size 字体大小
     */
    void setLeaveFontSize(const int &size);

    /**
     * @brief 设置是否在悬停时改变图标或文字大小
     * @param change 是否改变大小
     */
    void setChangeSize(const bool &change);

    /**
     * @brief 设置悬停时的字体颜色
     * @param color 字体颜色
     */
    void setHoverFontColor(const QColor &color);

    /**
     * @brief 设置图标和文字的水平边距
     * @param padding 边距值
     */
    void setPadding(const int &padding);

    /**
     * @brief 设置图标和文字是否靠近显示
     * @param approach 是否靠近
     */
    void setApproach(const bool &approach);

    /**
     * @brief 设置鼠标进入时的图标
     * @param icon 图标
     */
    void setEnterIcon(const QIcon &icon);

    /**
     * @brief 设置鼠标离开时的图标
     * @param icon 图标
     */
    void setLeaveIcon(const QIcon &icon);

protected:
    /**
     * @brief 绘制事件，自定义按钮的图标和文字绘制
     * @param event 绘图事件对象
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 鼠标进入事件，处理悬停效果
     * @param event 进入事件对象
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件，恢复默认状态
     * @param event 事件对象
     */
    void leaveEvent(QEvent *event) override;

private:
    QSize                   m_enterIconSize;        ///< 鼠标进入时的图标大小
    QSize                   m_leaveIconSize;        ///< 鼠标离开时的图标大小
    int                     m_enterFontSize;        ///< 鼠标进入时的字体大小
    int                     m_leaveFontSize;        ///< 鼠标离开时的字体大小
    QColor                  m_hoverFontColor = QColor(255, 255, 255); ///< 悬停时的字体颜色
    bool                    m_changeSize = false;   ///< 是否在悬停时改变大小
    bool                    m_isHover = false;      ///< 是否处于悬停状态
    bool                    m_isApproach = false;   ///< 是否靠近显示图标和文字
    int                     m_padding = 15;         ///< 图标和文字的水平边距
    QIcon                   m_enterIcon;            ///< 鼠标进入时的图标
    QIcon                   m_leaveIcon;            ///< 鼠标离开时的图标
};

#endif // ICONBESIDETEXTTOOLBUTTON_H