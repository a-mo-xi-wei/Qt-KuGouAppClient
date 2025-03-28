//
// Created by WeiWang on 25-3-21.
//

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>

#if defined(ABOUTDIALOG_LIBRARY)
#define ABOUTDIALOG_EXPORT Q_DECL_EXPORT
#else
#define ABOUTDIALOG_EXPORT Q_DECL_IMPORT
#endif

class QVBoxLayout;
class QtMaterialDialog;

class ABOUTDIALOG_EXPORT AboutDialog : public QWidget {
Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    void initDialog(QVBoxLayout* lay);

    void initFont();

public slots:
    void onShowDialog();

    void onHideDialog();

private:
    QtMaterialDialog *const m_dialog;
    QWidget* m_topWidget{};
    QFont m_font;
};


#endif //ABOUTDIALOG_H
