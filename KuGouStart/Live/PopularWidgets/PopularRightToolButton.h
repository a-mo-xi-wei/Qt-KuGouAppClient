//
// Created by WeiWang on 25-2-18.
//

#ifndef POPULARRIGHTTOOLBUTTON_H
#define POPULARRIGHTTOOLBUTTON_H

#include <QToolButton>

class PopularRightToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit PopularRightToolButton(QWidget *parent = nullptr);

    void setBackgroundImg(const QString& path);

    void setLeftBottomText(const QString& text);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_leftBottomText ;
};

#endif //POPULARRIGHTTOOLBUTTON_H
