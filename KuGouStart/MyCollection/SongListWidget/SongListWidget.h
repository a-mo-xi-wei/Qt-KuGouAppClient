//
// Created by WeiWang on 24-11-15.
//

#ifndef SONGLISTWIDGET_H
#define SONGLISTWIDGET_H

#include "SongListBlockWidget.h"

#if defined(COLLECTSONGLIST_LIBRARY)
#define COLLECTSONGLIST_EXPORT Q_DECL_EXPORT
#else
#define COLLECTSONGLIST_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE

namespace Ui {
    class SongListWidget;
}

QT_END_NAMESPACE

class COLLECTSONGLIST_EXPORT SongListWidget : public QWidget {
    Q_OBJECT

public:
    explicit SongListWidget(QWidget *parent = nullptr);

    ~SongListWidget() override;

private:
    void initUi();

    void initBlock() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::SongListWidget *ui;
    QAction* m_searchAction{};
};


#endif //SONGLISTWIDGET_H
