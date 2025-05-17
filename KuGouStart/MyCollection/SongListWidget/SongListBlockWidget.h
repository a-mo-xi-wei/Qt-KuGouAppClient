/**
* @file SongListBlockWidget.h
 * @brief 定义 SongListBlockWidget 类，显示歌单块
 * @author WeiWang
 * @date 2025-01-25
 * @version 1.0
 */

#ifndef SONGLISTBLOCKWIDGET_H
#define SONGLISTBLOCKWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class SongListBlockWidget;
}
QT_END_NAMESPACE

/**
 * @class SongListBlockWidget
 * @brief 歌单块类，显示歌单封面和标题
 */
class SongListBlockWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化歌单块
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SongListBlockWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~SongListBlockWidget() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置标题文本
     * @param title 标题文本
     */
    void setTitleText(const QString &title) const;

private:
    /**
     * @brief 初始化界面
     * @note 设置标题字体、封面控件和遮罩样式
     */
    void initUi() const;

protected:
    /**
     * @brief 进入事件
     * @param event 进入事件
     * @note 遮罩显示（已注释）
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 离开事件
     * @param event 离开事件
     * @note 遮罩隐藏（已注释）
     */
    void leaveEvent(QEvent *event) override;

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

private:
    Ui::SongListBlockWidget *ui; ///< UI 指针
};

#endif // SONGLISTBLOCKWIDGET_H