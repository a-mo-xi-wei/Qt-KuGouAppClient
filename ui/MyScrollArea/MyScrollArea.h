//
// Created by WeiWang on 24-10-16.
//

#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include"UpToolButton.h"

#include <QScrollArea>
#include <QEasingCurve>

#if defined(MYSCROLLAREA_LIBRARY)
#define MYSCROLLAREA_EXPORT Q_DECL_EXPORT
#else
#define MYSCROLLAREA_EXPORT Q_DECL_IMPORT
#endif

class MYSCROLLAREA_EXPORT MyScrollArea : public QScrollArea {
    Q_OBJECT

public:
    explicit MyScrollArea(QWidget *parent = nullptr);

    void setAnimating(const bool &animating);

    void setIgnore(const bool &ignore);

    void setEasingCurve(const QEasingCurve &curve);

protected:
    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void hideEvent(QHideEvent *event) override;

signals:
    void wheelValue(const int &value);

private slots:
    void onUpBtnClicked();

    void onUpBtnShowOrNot();

private:
    bool m_isAnimating = false; // 标记动画是否正在进行
    bool m_ignore = false;

    QWidget* m_parent{};

    std::unique_ptr<UpToolButton> m_upBtn{};
    //按钮位置
    //QPos
    //专门处理回到最顶部按钮
    QTimer*     m_scrollBarTimer{};  // 定时器
    //设置回到最顶部的特效
    QEasingCurve m_curve = QEasingCurve::OutBounce;
};
#endif //MYSCROLLAREA_H
