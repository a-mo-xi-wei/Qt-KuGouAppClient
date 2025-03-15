//
// Created by WeiWang on 25-1-12.
//

#ifndef MENUBTN_H
#define MENUBTN_H

#include <QToolButton>
#include <QEvent>

class MenuBtn : public QToolButton {
    Q_OBJECT

public:
    explicit MenuBtn(QWidget *parent = nullptr): QToolButton(parent) {
        this->setIconSize(QSize(15, 15));
        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        this->installEventFilter(this);
    }

    void initIcon(const QIcon &icon1, const QIcon &icon2) {
        this->m_icon1 = icon1;
        this->m_icon2 = icon2;
    }

    void removeFilter() {
        this->removeEventFilter(this);
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::Enter) {
            //qDebug()<<"进入";
            this->setIcon(m_icon2);
        } else if (event->type() == QEvent::Leave) {
            //qDebug()<<"离开";
            this->setIcon(m_icon1);
        }
        return QToolButton::eventFilter(watched, event);
    }

private:
    QIcon m_icon1{}; //leave
    QIcon m_icon2{}; //enter
public:
    int m_cnt = 0; //记录之前的图片
};
#endif //MENUBTN_H
