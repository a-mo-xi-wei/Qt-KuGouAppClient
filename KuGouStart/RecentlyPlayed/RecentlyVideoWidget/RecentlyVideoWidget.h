//
// Created by WeiWang on 24-11-15.
//

#ifndef RECENTLYVIDEOWIDGET_H
#define RECENTLYVIDEOWIDGET_H

#include <QWidget>

#if defined(RECENTLYVIDEOWIDGET_LIBRARY)
#define RECENTLYVIDEOWIDGET_EXPORT Q_DECL_EXPORT
#else
#define RECENTLYVIDEOWIDGET_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class RecentlyVideoWidget; }
QT_END_NAMESPACE

class RECENTLYVIDEOWIDGET_EXPORT RecentlyVideoWidget : public QWidget {
Q_OBJECT

public:
    explicit RecentlyVideoWidget(QWidget *parent = nullptr);
    ~RecentlyVideoWidget() override;

private:
    Ui::RecentlyVideoWidget *ui;
};


#endif //RECENTLYVIDEOWIDGET_H
