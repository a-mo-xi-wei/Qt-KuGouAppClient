/**
* @file SideHideLabel.h
 * @brief 定义 SideHideLabel 类，提供支持偏移和圆角裁剪的标签控件
 * @author iwxyi
 * @date 2025-05-16
 * @version 1.0
 * @note 参考开源项目：https://github.com/iwxyi/Qt-SlideShow
 */

#ifndef SIDEHIDELABEL_H
#define SIDEHIDELABEL_H

#include <QLabel>

/**
 * @class SideHideLabel
 * @brief 标签控件类，支持图片偏移和圆角裁剪
 */
class SideHideLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(double sideOffset READ getSideOffset WRITE setSideOffset) ///< 侧边偏移属性

public:
    /**
     * @brief 构造函数，初始化标签控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SideHideLabel(QWidget *parent = nullptr);

    /**
     * @brief 复制标签控件
     * @param other 源标签控件指针
     * @return 新创建的标签控件指针
     */
    static SideHideLabel *copy(const SideHideLabel *other);

    /**
     * @brief 设置图片和最大偏移量
     * @param pixmap 图片
     * @param sideMaxOffset 最大偏移量
     */
    void setPixmap(const QPixmap &pixmap, const double &sideMaxOffset);

    /**
     * @brief 获取最大偏移量
     * @return 最大偏移量
     */
    double getMaxOffset() const;

protected:
    /**
     * @brief 绘制事件
     * @param e 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *e) override;

public:
    /**
     * @brief 设置侧边偏移量
     * @param of 偏移量
     */
    void setSideOffset(const double &of);

    /**
     * @brief 获取侧边偏移量
     * @return 偏移量
     */
    double getSideOffset() const;

private:
    double sideOffset = 0;  ///< 侧边偏移量
    double startX = 0;      ///< 起始 X 位置
    int radius = 10;        ///< 圆角半径
    QPixmap pixmap;         ///< 图片
};

#endif // SIDEHIDELABEL_H