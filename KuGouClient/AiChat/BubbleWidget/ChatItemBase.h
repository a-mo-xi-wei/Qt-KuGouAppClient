/**
* @file ChatItemBase.h
 * @brief 定义 ChatItemBase 类，提供聊天项基类
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include <QLabel>
#include "global.h"

class BubbleFrame;

/**
 * @class ChatItemBase
 * @brief 聊天项基类，包含用户名、头像、气泡和加载动画
 */
class ChatItemBase : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化聊天项
     * @param role 聊天角色（自己、他人、时间）
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ChatItemBase() = default;

    /**
     * @brief 设置用户名
     * @param name 用户名
     */
    void setUserName(const QString &name) const;

    /**
     * @brief 设置用户头像
     * @param icon 头像图片
     */
    void setUserIcon(const QPixmap &icon) const;

    /**
     * @brief 设置气泡内容控件
     * @param w 内容控件指针
     */
    void setWidget(QWidget *w);

    /**
     * @brief 启动或停止加载动画
     * @param flag 是否启动
     */
    void startMovie(const bool &flag);

private:
    /**
     * @brief 初始化加载动画
     */
    void initMovie();

private:
    ChatRole                   m_role;           ///< 聊天角色
    QLabel                    *m_pNameLabel{};   ///< 用户名标签
    QLabel                    *m_pIconLabel{};   ///< 头像标签
    std::unique_ptr<QWidget>   m_pBubble;        ///< 气泡控件
    QLabel                    *m_loading{};      ///< 加载动画标签
    QMovie                    *m_loadingMovie{}; ///< 加载动画对象
};

#endif // CHATITEMBASE_H