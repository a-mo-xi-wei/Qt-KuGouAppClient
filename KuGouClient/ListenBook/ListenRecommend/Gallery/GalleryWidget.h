/**
* @file GalleryWidget.h
 * @brief 定义 GalleryWidget 类，提供照片画廊控件
 * @author WeiWang
 * @date 2025-05-16
 * @version 1.0
 * @note 参考开源项目：iwxyi 的 Qt-GalleryWidget，项目地址：https://github.com/iwxyi/Qt-GalleryWidget
 */

#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt] 简单易用的轮播图
 *  项目地址为 ： https://github.com/iwxyi/Qt-GalleryWidget
 *  非常实用，感兴趣的可以star
 */

#include "GalleryPhotoWidget.h"

/**
 * @class GalleryWidget
 * @brief 照片画廊控件类，管理照片卡片并支持动态布局和动画
 */
class GalleryWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化照片画廊控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit GalleryWidget(QWidget *parent = nullptr);

    /**
     * @brief 添加照片卡片数据
     * @param it 照片卡片控件指针
     */
    void addData(GalleryPhotoWidget *it);

    /**
     * @brief 调整画廊布局
     * @param emit_pos 动画起始位置，默认为 (0, 0)
     */
    void resizeGallery(QPoint emit_pos = QPoint(0, 0));

    /**
     * @brief 获取照片卡片列表
     * @return 照片卡片列表引用
     */
    QVector<GalleryPhotoWidget *> &getWidgets();

protected:
    /**
     * @brief 调整大小事件，重新布局画廊
     * @param event 调整大小事件
     */
    void resizeEvent(QResizeEvent *event) override;

    signals:
        /**
         * @brief 照片卡片点击信号
         * @param bean 照片卡片控件指针
         */
        void signalPictureClicked(GalleryPhotoWidget *bean);

public:
    static int item_spacing_h; ///< 水平间距
    static int item_spacing_v; ///< 垂直间距

private:
    QVector<GalleryPhotoWidget *> widgets; ///< 照片卡片列表
};

#endif // GALLERYWIDGET_H