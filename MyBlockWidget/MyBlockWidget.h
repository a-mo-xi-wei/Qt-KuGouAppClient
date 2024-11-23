//
// Created by WeiWang on 24-11-23.
//

#ifndef MYBLOCKWIDGET_H
#define MYBLOCKWIDGET_H

#include"SMaskWidget.h"

#include <QWidget>

class QLabel;
class QToolButton;

class MyBlockWidget : public QWidget {
Q_OBJECT

public:
    explicit MyBlockWidget(const QString &path,QWidget *parent = nullptr);

    void setTipLabText(const QString& text);

    void setPopularDirection(const int& direction);

    void setPopularBtnText(const QString& text);

    SMaskWidget& getMask();
private:
    void initUi();

    void initTipArr();

protected:
    void paintEvent(QPaintEvent* ev)override;

    void enterEvent(QEnterEvent* ev)override;

    void leaveEvent(QEvent* ev)override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent* event) override;

private:
    QWidget* m_bacWidget{};
    std::unique_ptr<SMaskWidget>m_mask;
    QLabel* m_tipLab{};
    int m_popularDirection = 0;//0 没有 ， 1 左边 ， 2 右边
    QToolButton* m_rightPopularBtn{};
    QToolButton* m_leftPopularBtn{};
    bool m_isHoverCoverLab = false;
    std::vector<QString> m_tipArr;
};


#endif //MYBLOCKWIDGET_H
