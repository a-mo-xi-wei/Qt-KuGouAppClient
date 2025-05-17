/**
* @file PopularRightToolButton.h
 * @brief 定义 PopularRightToolButton 类，提供热门右侧工具按钮
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#ifndef POPULARRIGHTTOOLBUTTON_H
#define POPULARRIGHTTOOLBUTTON_H

#include <QToolButton>

/**
 * @class PopularRightToolButton
 * @brief 热门右侧工具按钮类，显示背景图和左下角文本
 */
class PopularRightToolButton : public QToolButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化热门右侧工具按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PopularRightToolButton(QWidget *parent = nullptr);

    /**
     * @brief 设置背景图片
     * @param path 图片路径
     */
    void setBackgroundImg(const QString &path);

    /**
     * @brief 设置左下角文本
     * @param text 文本内容
     */
    void setLeftBottomText(const QString &text);

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制左下角文本
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_leftBottomText;    ///< 左下角文本
};

#endif // POPULARRIGHTTOOLBUTTON_H