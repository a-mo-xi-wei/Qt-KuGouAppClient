/**
 * @file LiveBlockWidget.h
 * @brief 定义 LiveBlockWidget 类，提供直播块控件
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#ifndef LIVEBLOCKWIDGET_H
#define LIVEBLOCKWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class LiveBlockWidget;
}
QT_END_NAMESPACE

/**
 * @class LiveBlockWidget
 * @brief 直播块控件类，显示封面、文本和提示
 */
class LiveBlockWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化直播块控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit LiveBlockWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置封面高度扩展
     * @note 调整边距和大小策略以扩展封面高度
     */
    void setCoverHeightExpanding();

    /**
     * @brief 析构函数，清理资源
     */
    ~LiveBlockWidget() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置左下角文本
     * @param text 文本内容
     */
    void setLeftBottomText(const QString &text);

    /**
     * @brief 设置左下角流行按钮字体大小
     * @param size 字体大小
     * @param isBold 是否加粗
     */
    void setLeftPopularBtnFontSize(const int &size, const bool &isBold) const;

    /**
     * @brief 设置提示标签文本
     * @param text 文本内容
     */
    void setTipLabText(const QString &text) const;

    /**
     * @brief 隐藏提示标签
     */
    void setNoTipLab() const;

    /**
     * @brief 设置封面宽高比
     * @param aspectRatio 宽高比
     */
    void setAspectRatio(const float &aspectRatio);

    /**
     * @brief 设置圆形标准值
     * @param stander 标准值
     */
    void setCircleStander(const int &stander) const;

    /**
     * @brief 设置提示样式表
     * @param styleSheet 样式表字符串
     */
    void setTipStyleSheet(const QString &styleSheet) const;

private:
    /**
     * @brief 初始化界面
     * @note 设置提示数组、遮罩和控件属性
     */
    void initUi() const;

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 根据宽高比调整封面高度
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::LiveBlockWidget *ui;              ///< UI 指针
    float         m_aspectRatio = 0.8;    ///< 封面宽高比
};

#endif // LIVEBLOCKWIDGET_H