//
// Created by WeiWang on 25-2-18.
//

#ifndef POPULARRIGHTTOOLBUTTON_H
#define POPULARRIGHTTOOLBUTTON_H

#include <QToolButton>

QT_BEGIN_NAMESPACE

namespace Ui {
    class PopularRightToolButton;
}

QT_END_NAMESPACE

class PopularRightToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit PopularRightToolButton(QWidget *parent = nullptr);

    ~PopularRightToolButton() override;

    void setBackgroundImg(const QString& path);

    void setLeftBottomText(const QString& text);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::PopularRightToolButton *ui;
    QString m_leftBottomText ;
};

#endif //POPULARRIGHTTOOLBUTTON_H
