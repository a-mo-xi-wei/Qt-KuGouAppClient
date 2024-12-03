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
protected:
    void wheelEvent(QWheelEvent *event) override;
signals:
    void wheelValue(const int& value);
};


#endif //SSCROLLAREA_H
