/**
* @file PopularRightToolButton.cpp
 * @brief 实现 PopularRightToolButton 类，提供热门右侧工具按钮功能
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#include "PopularRightToolButton.h"

#include <QFile>
#include <QPainter>

/**
 * @brief 构造函数，初始化热门右侧工具按钮
 * @param parent 父控件指针，默认为 nullptr
 */
PopularRightToolButton::PopularRightToolButton(QWidget *parent)
    : QToolButton(parent)
{
    this->setCursor(Qt::PointingHandCursor);             ///< 设置手形光标
    this->setStyleSheet(R"(
        QToolButton {
            border-radius: 10px;
        }
        QToolButton:checked {
            border: 3px solid rgb(69, 164, 255);
        }
    )");///< 设置普通和选中样式
}

/**
 * @brief 设置背景图片
 * @param path 图片路径
 */
void PopularRightToolButton::setBackgroundImg(const QString &path)
{
    const QString style = QString(R"(
        QToolButton {
            border-radius: 10px;
            background-image: url(%1);
            background-repeat: no-repeat;
            background-position: center;
        }
        QToolButton:checked{
            border: 3px solid rgb(69, 164, 255);
        }
    )").arg(path);///< 设置背景图片和选中样式
    this->setStyleSheet(style);                          ///< 应用样式表
}

/**
 * @brief 设置左下角文本
 * @param text 文本内容
 */
void PopularRightToolButton::setLeftBottomText(const QString &text)
{
    this->m_leftBottomText = text;                       ///< 设置文本
    update();                                            ///< 刷新界面
}

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 绘制左下角文本
 */
void PopularRightToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);                      ///< 调用父类绘制
    QPainter painter(this);                              ///< 创建画家
    painter.setPen(Qt::white);                           ///< 设置白色画笔
    painter.setFont(QFont(this->fontInfo().family(), 13)); ///< 设置字体
    painter.drawText(10, this->height() - 15, this->m_leftBottomText); ///< 绘制文本
}