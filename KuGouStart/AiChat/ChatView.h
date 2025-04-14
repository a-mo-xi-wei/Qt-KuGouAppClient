#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QScrollArea>
#include <QVBoxLayout>

class ChatView : public QWidget {
    Q_OBJECT

public:
    explicit ChatView(QWidget *parent = Q_NULLPTR);

    void appendChatItem(QWidget *item); //尾插

    void prependChatItem(QWidget *item); //头插

    void insertChatItem(QWidget *before, QWidget *item); //中间插

    void removeAllItem() const;

    QVBoxLayout *getLayout() const;

protected:
    bool eventFilter(QObject *o, QEvent *e) override;

    void paintEvent(QPaintEvent *event) override;

private slots:
    void onVScrollBarMoved(int min, int max);

private:
    //QWidget *m_pCenterWidget;
    QVBoxLayout *m_pVl;
    QScrollArea *m_pScrollArea;
    bool isAppended;
};

#endif // CHATVIEW_H
