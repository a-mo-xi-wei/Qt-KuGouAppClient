//
// Created by WeiWang on 25-4-14.
//

#ifndef AICHAT_H
#define AICHAT_H

#include "Chat.h"
#include <QWidget>
#include <QTimer>

class QLabel;
class TextBubble;
class ChatItemBase;

QT_BEGIN_NAMESPACE

namespace Ui {
    class AiChat;
}

QT_END_NAMESPACE

class AiChat : public QWidget {
    Q_OBJECT

public:
    AiChat(QWidget *parent = nullptr);

    ~AiChat();

private:
    void initUi();

    QPixmap getRoundedPixmap(const QPixmap &src, const QSize &size, const int &radius);

    void dealMessageTime();

private slots:
    void on_send_btn_clicked();

    void getAnswer(const QString &word);

    void onStreamFinished();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::AiChat *ui;
    Chat m_deepSeek;

    /*--------------------*/
    TextBubble* m_currentResponseBubble = nullptr; // 新增：当前正在更新的回答气泡
    ChatItemBase* m_currentResponseItem = nullptr; // 对应的聊天项
};


#endif //AICHAT_H
