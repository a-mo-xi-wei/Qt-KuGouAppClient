#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include <QLabel>
#include "global.h"

class BubbleFrame;

class ChatItemBase : public QWidget {
    Q_OBJECT

public:
    explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);

    void setUserName(const QString &name) const;

    void setUserIcon(const QPixmap &icon) const;

    void setWidget(QWidget *w);

    void startMovie(const bool& flag);

private:
    void initMovie();

private:
    ChatRole m_role;
    QLabel *m_pNameLabel;
    QLabel *m_pIconLabel;
    QWidget *m_pBubble;
    //加载动画
    QLabel* m_loading = Q_NULLPTR;
    QMovie* m_loadingMovie = Q_NULLPTR;
};

#endif // CHATITEMBASE_H
