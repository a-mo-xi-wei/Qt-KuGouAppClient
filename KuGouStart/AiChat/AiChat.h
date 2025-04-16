//
// Created by WeiWang on 25-4-14.
//

#ifndef AICHAT_H
#define AICHAT_H

#include "Chat.h"
#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

class QtMaterialSnackbar;
class QtMaterialFloatingActionButton;
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
    void onSendBtnClicked();

    void getAnswer(const QString &word);

    void onStreamFinished();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::AiChat *const ui;
    Chat m_deepSeek;
    QtMaterialFloatingActionButton *const m_sendBtn;
    std::unique_ptr<QtMaterialSnackbar> m_snackbar{};
    QElapsedTimer m_snackbarTimer; // 用于记录最后一次显示时间
    /*--------------------*/
    TextBubble* m_currentResponseBubble = nullptr; // 新增：当前正在更新的回答气泡
    ChatItemBase* m_currentResponseItem = nullptr; // 对应的聊天项
};


#endif //AICHAT_H
