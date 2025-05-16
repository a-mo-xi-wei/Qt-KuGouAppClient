/**
* @file MVBlockWidget.h
 * @brief 定义 MVBlockWidget 类，提供音乐视频块控件
 * @author WeiWang
 * @date 2024-12-27
 * @version 1.0
 */

#ifndef MVBLOCKWIDGET_H
#define MVBLOCKWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MVBlockWidget; }
QT_END_NAMESPACE

/**
 * @class MVBlockWidget
 * @brief 音乐视频块控件类，显示封面、标题和描述
 */
class MVBlockWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐视频块控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MVBlockWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MVBlockWidget() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置标题
     * @param title 标题文本
     */
    void setTitle(const QString &title);

    /**
     * @brief 设置描述
     * @param description 描述文本
     */
    void setDescription(const QString &description);

    /**
     * @brief 隐藏描述标签
     */
    void hideDesc();

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 更新标题文本（省略显示）
     */
    void updateTitleText() const;

    /**
     * @brief 更新描述文本（省略显示）
     */
    void updateDescText() const;

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件对象
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MVBlockWidget *ui;           ///< UI 指针
    QString            m_title;       ///< 标题文本
    QString            m_description; ///< 描述文本
};

#endif // MVBLOCKWIDGET_H