//
// Created by WeiWang on 25-2-17.
//

#ifndef POPULARLEFTWIDGET_H
#define POPULARLEFTWIDGET_H

#include <QWidget>

class QGraphicsOpacityEffect;
class QParallelAnimationGroup;
class QLabel;
class QPropertyAnimation;
class QToolButton;

class PopularLeftWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int animatedY READ animatedY WRITE setAnimatedY)
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)
public:
    explicit PopularLeftWidget(QWidget *parent = nullptr);

    int animatedY() const { return m_animatedY; }

    int alpha() const { return m_alpha; }

public slots:
    void setAnimatedY(int y);

    void setAlpha(int alpha);

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
    int m_alpha;
    QPropertyAnimation* m_posAnimation;
    QPropertyAnimation* m_alphaAnimation;
    QGraphicsOpacityEffect* m_opacityEffect{};
    QParallelAnimationGroup* m_aniGroup{};

    //下方三个label
    QLabel *m_stopLab{};
    QLabel *m_refreshLab{};
    QLabel *m_muteLab{};

};


#endif //POPULARLEFTWIDGET_H
