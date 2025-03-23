//
// Created by WeiWang on 24-11-15.
//

#ifndef PAIDSINGLE_H
#define PAIDSINGLE_H

#include "SortOptionMenu.h"

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class PaidSingle;
}

QT_END_NAMESPACE

class PaidSingle : public QWidget {
    Q_OBJECT

public:
    explicit PaidSingle(QWidget *parent = nullptr);

    ~PaidSingle() override;

private:
    void initUi();

    void getMenuPosition(const QPoint &pos);

private slots:
    void on_single_sort_toolButton_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::PaidSingle *ui;
    QAction *m_searchAction; //专门为了设置图片

    //菜单相关
    SortOptionMenu *m_sortOptMenu{};
    QPoint m_menuPosition;
};


#endif //PAIDSINGLE_H
