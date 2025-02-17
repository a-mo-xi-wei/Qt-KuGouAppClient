//
// Created by WeiWang on 25-2-17.
//

#ifndef POPULARLEFTWIDGET_H
#define POPULARLEFTWIDGET_H

#include <QWidget>

class QPropertyAnimation;
class QToolButton;

class PopularLeftWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int animatedY READ animatedY WRITE setAnimatedY NOTIFY animatedYChanged)

public:
    explicit PopularLeftWidget(QWidget *parent = nullptr);

    int animatedY() const { return m_animatedY; }

public slots:
    void setAnimatedY(int y);

signals:
    void animatedYChanged(int);

private:
    void initUi();

    void animationUp() const;

    void animationDown() const;

protected:
    void paintEvent(QPaintEvent *event) override;

    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

private:
    QToolButton *m_enterLivBtn{};
    //动画相关
    int m_animatedY;
    QPropertyAnimation* m_posAnimation;

};


#endif //POPULARLEFTWIDGET_H
