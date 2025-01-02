//
// Created by WeiWang on 24-10-16.
//

#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QScrollArea>

class MyScrollArea : public QScrollArea {
    Q_OBJECT

public:
    enum ScrollAreaKind {
        Outside,
        Inside
    };

    explicit MyScrollArea(QWidget *parent = nullptr);

    void setAnimating(const bool &animating);

    void setIgnore(const bool &ignore);

    void setScrollAreaKind(const ScrollAreaKind &kind);

protected:
    void wheelEvent(QWheelEvent *event) override;

signals:
    void wheelValue(const int &value);

private:
    bool m_isAnimating = false; // 标记动画是否正在进行
    bool m_ignore = false;
    ScrollAreaKind m_scrollAreaKind = ScrollAreaKind::Outside;
};


#endif //MYSCROLLAREA_H
