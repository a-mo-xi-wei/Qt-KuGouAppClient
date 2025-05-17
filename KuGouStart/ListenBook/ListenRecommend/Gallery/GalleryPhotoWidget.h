/**
 * @file GalleryPhotoWidget.h
 * @brief 定义 GalleryPhotoWidget 类，提供照片卡片控件
 * @author WeiWang
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef GALLERYPHOTOWIDGET_H
#define GALLERYPHOTOWIDGET_H

#include "MyBlockWidget.h"
#include "WaterZoomButton.h"

class QGraphicsDropShadowEffect;

/**
 * @class GalleryPhotoWidget
 * @brief 照片卡片控件类，显示封面、标题、描述、流行度，支持水波放大效果
 */
class GalleryPhotoWidget : public WaterZoomButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化照片卡片控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit GalleryPhotoWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置标题文本
     * @param title 标题文本
     */
    void setTitleText(const QString &title);

    /**
     * @brief 设置描述文本
     * @param desc 描述文本
     */
    void setDescribeText(const QString &desc);

    /**
     * @brief 设置流行度文本
     * @param text 流行度文本
     */
    void setPopularText(const QString &text) const;

private:
    /**
     * @brief 初始化界面
     * @note 设置阴影效果、背景色、遮罩和标签
     */
    void initUi();

    /**
     * @brief 更新标题文本
     * @note 处理换行和省略显示
     */
    void updateTitleText() const;

    /**
     * @brief 更新描述文本
     * @note 处理换行和省略显示
     */
    void updateDescText() const;

protected:
    /**
     * @brief 绘制事件，调整封面和标签位置
     * @param event 绘制事件
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 鼠标移动事件，处理标题颜色变化
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标离开事件，恢复标题颜色和阴影
     * @param event 事件
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 鼠标进入事件，启用阴影效果
     * @param event 进入事件
     */
    void enterEvent(QEnterEvent *event) override;

public:
    static int fixed_width;      ///< 控件固定宽度
    static int fixed_height;     ///< 控件固定高度
    static int content_width;    ///< 内容区域宽度
    static int content_height;   ///< 内容区域高度
    static int pixmap_width;     ///< 图片宽度
    static int pixmap_height;    ///< 图片高度

private:
    MyBlockWidget             *m_coverWidget{};    ///< 封面控件
    QLabel                    *m_titleLab{};       ///< 标题标签
    QLabel                    *m_descLab{};        ///< 描述标签
    QString                    m_titleText;        ///< 标题文本
    QString                    m_describeText;     ///< 描述文本
    QGraphicsDropShadowEffect *m_shadowEffect{};   ///< 阴影效果
};

#endif // GALLERYPHOTOWIDGET_H