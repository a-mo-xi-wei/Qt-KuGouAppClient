#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include "TitleOptionMenu/TitleOptionMenu.h"

#include <QWidget>

class TitleWidget : public QWidget {
    Q_OBJECT

public:
    explicit TitleWidget(QWidget *parent = nullptr);

    void showMenu(const QPoint &pos);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *ev) override;

private:
    void setMenuPosition(const QPoint &pos);

signals:
    void doubleClicked(); // 自定义的双击信号

    void showAboutDialog();

    void exit();

private:
    //标题菜单相关
    TitleOptionMenu *m_titleOptMenu{};
    QPoint m_menuPosition;
};

#endif // TITLEWIDGET_H
