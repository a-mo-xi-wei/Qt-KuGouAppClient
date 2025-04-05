//
// Created by WeiWang on 25-3-21.
//

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>
#include "libhttp.h"

#if defined(MYDIALOG_LIBRARY)
#define MYDIALOG_EXPORT Q_DECL_EXPORT
#else
#define MYDIALOG_EXPORT Q_DECL_IMPORT
#endif

class ElaText;
class QVBoxLayout;
class QtMaterialDialog;

class MYDIALOG_EXPORT AboutDialog : public QWidget {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    void initDialog(QVBoxLayout *lay);

    void getGiteeProjectStar(const QString &url);

    void initDescText(ElaText* text);

public slots:
    void onShowDialog();

    void onHideDialog();

signals:
    void gotStars();

private:
    QtMaterialDialog *const m_dialog;
    QWidget *m_topWidget{};
    QFont m_font;
    int m_stars = 90;

    //发送网络请求
    CLibhttp m_libHttp;
};


#endif //ABOUTDIALOG_H
