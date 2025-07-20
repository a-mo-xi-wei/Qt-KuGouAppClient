/**
 * @file AiChat.h
 * @brief 定义 AiChat 类，提供 AI 聊天界面
 * @author WeiWang
 * @date 2025-04-14
 * @version 1.0
 */

#ifndef AICHAT_H
#define AICHAT_H

#include "Chat.h"
#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>

class QtMaterialSnackbar;
class QtMaterialFloatingActionButton;
class QLabel;
class TextBubble;
class ChatItemBase;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class AiChat;
}
QT_END_NAMESPACE

/**
 * @class AiChat
 * @brief AI 聊天界面类，集成问题输入、聊天视图和流式回答
 */
class AiChat : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化 AI 聊天界面
     * @param parent 父控件指针，默认为 nullptr
     */
    AiChat(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~AiChat();

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 生成圆角图片
     * @param src 源图片
     * @param size 目标尺寸
     * @param radius 圆角半径
     * @return 圆角图片
     */
    QPixmap getRoundedPixmap(const QPixmap &src, const QSize &size, const int &radius);

    /**
     * @brief 处理消息时间气泡
     */
    void dealMessageTime();

private slots:
    /**
     * @brief 处理发送按钮点击
     */
    void onSendBtnClicked();

    /**
     * @brief 接收回答内容
     * @param word 回答片段
     */
    void getAnswer(const QString &word);

    /**
     * @brief 处理流式响应结束
     */
    void onStreamFinished();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 目标对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void emitInitialized() {QTimer::singleShot(0, this, [this] {emit initialized();});}

signals:
    void initialized();

private:
    Ui::AiChat *const                   ui;                     ///< UI 指针
    Chat                                m_deepSeek;             ///< 聊天对象
    QtMaterialFloatingActionButton *const m_sendBtn;            ///< 发送按钮
    std::unique_ptr<QtMaterialSnackbar> m_snackbar;             ///< 消息条
    QElapsedTimer                       m_snackbarTimer;        ///< 消息条计时器
    TextBubble                         *m_currentResponseBubble; ///< 当前回答气泡
    ChatItemBase                       *m_currentResponseItem;   ///< 当前回答聊天项
};

#endif // AICHAT_H