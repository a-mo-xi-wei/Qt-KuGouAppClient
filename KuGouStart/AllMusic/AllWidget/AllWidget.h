//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLWIDGET_H
#define ALLWIDGET_H

#include <QWidget>

// 添加导出宏定义
#if defined(ALLWIDGET_LIBRARY)
#define ALLWIDGET_EXPORT Q_DECL_EXPORT
#else
#define ALLWIDGET_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
    class AllWidget;
}
QT_END_NAMESPACE

class ALLWIDGET_EXPORT AllWidget : public QWidget {
    Q_OBJECT

public:
    explicit AllWidget(QWidget *parent = nullptr);

    ~AllWidget() override;

private:
    void initUi();

private:
    Ui::AllWidget *ui;
};


#endif //ALLWIDGET_H
