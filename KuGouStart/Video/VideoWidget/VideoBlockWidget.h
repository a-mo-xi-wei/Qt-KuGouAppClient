/**
 * @file VideoBlockWidget.h
 * @brief 视频块界面的头文件，定义 VideoBlockWidget 类
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef VIDEOBLOCKWIDGET_H
#define VIDEOBLOCKWIDGET_H

#include <QWidget>
#include <QMouseEvent>

/**
 * @namespace Ui
 * @brief UI 命名空间，包含视频块界面的 UI 类
 */
QT_BEGIN_NAMESPACE
namespace Ui { class VideoBlockWidget; }
QT_END_NAMESPACE

/**
 * @class VideoBlockWidget
 * @brief 视频块界面类，显示视频封面、名称、作者和图标
 */
class VideoBlockWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit VideoBlockWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     * @note 释放 UI 资源
     */
    ~VideoBlockWidget() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     * @note 更新封面显示
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置视频名称
     * @param name 视频名称
     * @note 更新名称标签并添加工具提示
     */
    void setVideoName(const QString &name);

    /**
     * @brief 设置图标图片
     * @param pix 图片路径
     * @note 更新图标显示，应用圆角效果
     */
    void setIconPix(const QString &pix) const;

    /**
     * @brief 设置作者
     * @param author 作者名称
     * @note 更新作者标签并添加工具提示
     */
    void setAuthor(const QString &author);

    /**
     * @brief 显示提示标签
     * @note 启用封面的提示标签
     */
    void setShowTip() const;

    /**
     * @brief 设置提示文本
     * @param text 提示文本
     * @note 更新封面的提示标签内容
     */
    void setTipText(const QString &text) const;

private:
    /**
     * @brief 初始化界面
     * @note 设置遮罩、播放按钮、时长和随机播放量
     */
    void initUi() const;

    /**
     * @brief 更新视频名称文本
     * @note 处理名称的省略显示
     */
    void updateVideoNameText() const;

    /**
     * @brief 更新作者文本
     * @note 处理作者的省略显示
     */
    void updateVideoAuthorText() const;

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
     * @brief 事件过滤器
     * @param watched 监控对象
     * @param event 事件
     * @return 是否处理事件
     * @note 默认实现
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 更新封面高度和文本省略
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::VideoBlockWidget *ui;         ///< UI 界面指针
    QString m_videoName;              ///< 视频名称
    QString m_videoAuthor;            ///< 视频作者
};

#endif // VIDEOBLOCKWIDGET_H