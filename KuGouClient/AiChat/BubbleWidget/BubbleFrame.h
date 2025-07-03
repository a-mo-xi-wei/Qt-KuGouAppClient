/**
* @file BubbleFrame.h
 * @brief 定义 BubbleFrame 类，提供聊天气泡框架
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef BUBBLE_H
#define BUBBLE_H

#include "global.h"
#include <QFrame>
#include <QHBoxLayout>

/**
 * @class BubbleFrame
 * @brief 聊天气泡框架类，支持自己、他人和时间角色
 */
class BubbleFrame : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化气泡框架
     * @param role 聊天角色（自己、他人、时间）
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit BubbleFrame(ChatRole role, QWidget *parent = nullptr);

    /**
     * @brief 设置气泡内容控件
     * @param w 内容控件指针
     */
    void setWidget(QWidget *w);

protected:
    /**
     * @brief 绘制事件
     * @param e 绘制事件对象
     * @note 重写基类方法，绘制气泡或时间标签
     */
    void paintEvent(QPaintEvent *e) override;

private:
    QHBoxLayout *m_pHLayout;    ///< 水平布局
    ChatRole     m_role;        ///< 聊天角色
    int          m_margin;      ///< 边距
    QString      m_createTime;  ///< 创建时间
};

#endif // BUBBLE_H