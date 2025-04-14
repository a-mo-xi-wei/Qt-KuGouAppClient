#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include "BubbleFrame.h"

#include <QTextEdit>
#include <QTimer>

class TextBubble : public BubbleFrame {
    Q_OBJECT

public:
    TextBubble(ChatRole role, const QString &text, QWidget *parent = nullptr);

    void startStreaming();

    void appendStreamingContent(const QString &chunk); // 新增分块追加方法

    void finishStreaming();

private:
    void adjustTextHeight();

    void setPlainText(const QString &text);

protected:
    bool eventFilter(QObject *o, QEvent *e) override;

    void resizeEvent(QResizeEvent *event) override;

signals:
    void changeSize();

private:
    QTextEdit *m_pTextEdit;
    // 新增流式显示相关成员
    QString m_streamBuffer;
    QTimer m_streamTimer;
};

#endif // TEXTBUBBLE_H
