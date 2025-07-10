/**
 * @file ChatView.h
 * @brief 定义 ChatView 类，提供聊天视图
 * @author WeiWang
 * @date 2025-04-14
 * @version 1.0
 */

#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QScrollArea>
#include <QVBoxLayout>

class QLabel;

/**
 * @class ChatView
 * @brief 聊天视图类，支持聊天项动态管理、居中初始化部件和动画效果
 */
class ChatView : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化聊天视图
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ChatView(QWidget *parent = nullptr);

    /**
     * @brief 尾部插入聊天项
     * @param item 聊天项控件
     */
    void appendChatItem(QWidget *item);

    /**
     * @brief 头部插入聊天项
     * @param item 聊天项控件
     */
    void prependChatItem(QWidget *item);

    /**
     * @brief 中间插入聊天项
     * @param before 前置控件
     * @param item 聊天项控件
     */
    void insertChatItem(QWidget *before, QWidget *item);

    /**
     * @brief 删除最后一个聊天项
     */
    void removeLastItem();

    /**
     * @brief 删除所有聊天项
     */
    void removeAllItem();

    /**
     * @brief 获取垂直布局
     * @return 垂直布局指针
     */
    QVBoxLayout *getLayout() const;

private:
    /**
     * @brief 更新居中部件位置
     */
    void updateCenterWidgetPosition();

    /**
     * @brief 启动淡出动画
     */
    void startFadeOutAnimation();

    /**
     * @brief 启动淡入动画
     */
    void startFadeInAnimation();

protected:
    /**
     * @brief 事件过滤器
     * @param o 目标对象
     * @param e 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *o, QEvent *e) override;

    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件对象
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief 处理垂直滚动条范围变化
     * @param min 最小值
     * @param max 最大值
     */
    void onVScrollBarMoved(int min, int max);

private:
    QScrollArea *m_pScrollArea;        ///< 滚动区域
    bool         isAppended;           ///< 是否追加聊天项
    QWidget     *m_centerInitWidget{};   ///< 居中初始化部件
};

#endif // CHATVIEW_H