/**
* @file TextBubble.h
 * @brief 定义 TextBubble 类，提供文本气泡功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include "BubbleFrame.h"

#include <QTextEdit>
#include <QTimer>

/**
 * @class TextBubble
 * @brief 文本气泡类，支持流式显示和动态尺寸调整
 */
class TextBubble : public BubbleFrame
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化文本气泡
     * @param role 聊天角色（自己、他人、时间）
     * @param text 初始文本
     * @param parent 父控件指针，默认为 nullptr
     */
    TextBubble(ChatRole role, const QString &text, QWidget *parent = nullptr);

    /**
     * @brief 启动流式显示
     */
    void startStreaming();

    /**
     * @brief 追加流式内容
     * @param chunk 分块内容
     */
    void appendStreamingContent(const QString &chunk);

    /**
     * @brief 结束流式显示
     */
    void finishStreaming();

private:
    /**
     * @brief 调整文本高度
     */
    void adjustTextHeight();

    /**
     * @brief 设置纯文本内容
     * @param text 文本内容
     */
    void setPlainText(const QString &text);

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
     * @brief 调整大小事件
     * @param event 调整大小事件对象
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) override;

    signals:
        /**
         * @brief 尺寸变化信号
         */
        void changeSize();

private:
    QTextEdit *m_pTextEdit{};     ///< 文本编辑器
    QString    m_streamBuffer;    ///< 流式缓冲区
    QTimer     m_streamTimer;     ///< 流式定时器
};

#endif // TEXTBUBBLE_H