//
// Created by WeiWang on 25-3-21.
//

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>

class QVBoxLayout;
class QtMaterialDialog;

class AboutDialog : public QWidget {
Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    void initDialog(QVBoxLayout* lay);

public slots:
    void onShowDialog();

    void onHideDialog();

private:
    QtMaterialDialog *const m_dialog;
};


#endif //ABOUTDIALOG_H
