//
// Created by WeiWang on 24-12-3.
//

#ifndef SSCROLLAREA_H
#define SSCROLLAREA_H

#include <QScrollArea>

class SScrollArea : public QScrollArea {
    Q_OBJECT

public:
    explicit SScrollArea(QWidget *parent = nullptr);

    void setAnimating(const bool &animating);

    void setIgnore(const bool &ignore);

protected:
    void wheelEvent(QWheelEvent *event) override;

signals:
    void wheelValue(const int &value);

private:
    bool m_isAnimating = false; // 标记动画是否正在进行
    bool m_ignore = false;
};


#endif //SSCROLLAREA_H
