//
// Created by WeiWang on 24-11-12.
//

#ifndef MVWIDGET_H
#define MVWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MVWidget; }
QT_END_NAMESPACE

class MVWidget : public QWidget {
Q_OBJECT

public:
    explicit MVWidget(QWidget *parent = nullptr);
    ~MVWidget() override;
private:
    void initUi();

protected:
    void resizeEvent(QResizeEvent *event) override;


private:
    Ui::MVWidget *ui;
};


#endif //MVWIDGET_H
