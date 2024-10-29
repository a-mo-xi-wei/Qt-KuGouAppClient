#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QApplication>
#include <QPaintEvent>

#include"MyMenu.h"

class TitleWidget : public QWidget {
    Q_OBJECT

public:
    explicit TitleWidget(QWidget *parent = nullptr);
    void showMenu(const QPoint& pos);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent* ev)override;
private:
    void getMenuPosition(const QPoint& pos);
signals:
    void doubleClicked();  // 自定义的双击信号
private:
    //标题菜单相关
    MyMenu*             m_menu{};
    QPoint              m_menuPosition;
};

#endif // TITLEWIDGET_H
