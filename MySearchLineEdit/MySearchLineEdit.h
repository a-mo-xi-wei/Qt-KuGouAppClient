//
// Created by WeiWang on 24-10-10.
//

#ifndef MYSEARCHLINEEDIT_H
#define MYSEARCHLINEEDIT_H

class QPropertyAnimation;

#include <QLineEdit>

class MySearchLineEdit : public QLineEdit {
    Q_OBJECT

public:
    explicit MySearchLineEdit(QWidget *parent = nullptr);

    void setWidth(const int &width);

protected:
    void focusInEvent(QFocusEvent *event) override;

    void focusOutEvent(QFocusEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QPropertyAnimation *m_animation;
    int m_startWidth;
    int m_endWidth;
    int m_maxWidth;
};


#endif //MYSEARCHLINEEDIT_H
