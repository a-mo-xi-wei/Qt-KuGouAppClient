/**
 * @file SlideShow.h
 * @brief 定义 SlideShow 类，提供轮播图控件
 * @author iwxyi
 * @date 2025-05-16
 * @version 1.0
 * @note 参考开源项目：https://github.com/iwxyi/Qt-SlideShow
 */

#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include "SideHideLabel.h"
#include <QVBoxLayout>

class QPushButton;
/**
 * @class SlideShow
 * @brief 轮播图控件类，支持图片滑动、自动轮播和指示器
 */
class SlideShow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化轮播图控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SlideShow(QWidget *parent = nullptr);

    /**
     * @brief 设置图片大小
     * @param size 图片尺寸
     */
    void setPixmapSize(const QSize &size);

    /**
     * @brief 设置图片缩放模式（已废弃）
     * @param scale 是否缩放
     */
    void setPixmapScale(const bool &scale);

    /**
     * @brief 设置自动轮播
     * @param interval 轮播间隔（毫秒），0 表示停止
     */
    void setAutoSlide(const int &interval = 5000) const;

    /**
     * @brief 获取当前图片索引
     * @return 当前索引
     */
    int getCurrentIndex() const;

    /**
     * @brief 调整标签布局
     * @param hidingLabel 即将隐藏的标签，默认为 nullptr
     */
    void adjustLabels(SideHideLabel *hidingLabel = nullptr);

public slots:
    /**
     * @brief 添加图片
     * @param pixmap 图片
     * @param text 关联文本，默认为空
     */
    void addImage(const QPixmap &pixmap, QString text = "");

    /**
     * @brief 插入图片
     * @param index 插入位置
     * @param pixmap 图片
     * @param text 关联文本，默认为空
     */
    void insertImage(const int &index, const QPixmap &pixmap, QString text = "");

    /**
     * @brief 移除图片
     * @param index 移除位置
     */
    void removeImage(const int &index);

    /**
     * @brief 设置当前图片索引
     * @param index 目标索引
     */
    void setCurrentIndex(int index);

    /**
     * @brief 向左滑动
     */
    void slideToLeft();

    /**
     * @brief 向右滑动
     */
    void slideToRight();

private:
    void updateButtonColor(QPushButton *btn, const QColor &normal, const QColor &hover);

    /**
     * @brief 获取缩放和圆角处理的图片
     * @param pixmap 原始图片
     * @return 处理后的图片
     */
    QPixmap getScaledRoundedPixmap(QPixmap pixmap) const;

    /**
     * @brief 移动标签到指定位置
     * @param label 目标标签
     * @param geometry 目标几何
     */
    void moveTo(SideHideLabel *label, QRect geometry) const;

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件对象
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param obj 目标对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    /**
     * @brief 图片被点击信号
     * @param index 点击的图片索引
     */
    void signalImageClicked(int index);

    /**
     * @brief 文本被激活信号
     * @param text 关联文本
     */
    void signalTextActivated(QString text);

private:
    QList<SideHideLabel *>           labels;             ///< 图片标签列表
    QList<QPixmap>                   pixmaps;            ///< 原始图片列表
    QList<QString>                   texts;              ///< 图片关联文本列表
    QList<QPushButton *>   indications;                  ///< 指示器按钮列表
    int                              currentIndex = -1;  ///< 当前图片索引
    QTimer                          *autoSlideTimer{};   ///< 自动轮播定时器
    SideHideLabel                   *hidingLabel{};      ///< 正在隐藏的标签
    QColor                           normalColor = QColor(172, 128, 58);       ///< 指示器默认颜色
    QColor                           selectColor = Qt::yellow;                          ///< 指示器选中颜色
    QSize                            oneSize;                                           ///< 默认图片大小
    double                           backScale = 0.8;                                   ///< 两侧卡片缩放比例
    double                           sideOffside = 0.4;                                 ///< 两侧卡片位置偏移比例
    double                           imgOffside = 0.15;                                 ///< 图片内偏移比例
    bool                             scalePixmap = false;                               ///< 图片缩放标志（已废弃）

    // QRect                            centerRect;         ///< 中心卡片区域
    QRect                            leftRect;           ///< 左侧卡片区域
    QRect                            rightRect;          ///< 右侧卡片区域
    // QRect                            backRect;           ///< 隐藏卡片区域
};

#endif // SLIDESHOW_H