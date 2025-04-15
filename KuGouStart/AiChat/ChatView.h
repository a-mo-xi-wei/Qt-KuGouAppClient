#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QScrollArea>
#include <QVBoxLayout>

class QLabel;

class ChatView : public QWidget {
    Q_OBJECT

public:
    explicit ChatView(QWidget *parent = Q_NULLPTR);

    void appendChatItem(QWidget *item); //尾插

    void prependChatItem(QWidget *item); //头插

    void insertChatItem(QWidget *before, QWidget *item); //中间插

    void removeLastItem();

    void removeAllItem();

    QVBoxLayout *getLayout() const;

private:
    void startFadeOutAnimation();

    void startFadeInAnimation();

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
    QLabel *m_logo;
    QLabel *m_helloText;
    QLabel *m_funcText;
    QWidget *m_centerInitWidget{};
};

#endif // CHATVIEW_H
