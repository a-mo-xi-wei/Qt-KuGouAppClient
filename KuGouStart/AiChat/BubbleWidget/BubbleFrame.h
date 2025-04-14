#ifndef BUBBLE_H
#define BUBBLE_H

#include "global.h"
#include <QFrame>
#include <QHBoxLayout>

class BubbleFrame : public QFrame {
    Q_OBJECT

public:
    explicit BubbleFrame(ChatRole role, QWidget *parent = nullptr);

    void setWidget(QWidget *w);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QHBoxLayout *m_pHLayout;
    ChatRole m_role;
    int m_margin;
    QString m_createTime;
};

#endif // BUBBLE_H
