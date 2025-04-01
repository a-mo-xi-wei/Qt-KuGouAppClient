//
// Created by WeiWang on 25-3-22.
//

#ifndef REFRESHMASK_H
#define REFRESHMASK_H

#include "qtmaterialcircularprogress.h"

#if defined(MYMASKWIDGET_LIBRARY)
#define MYMASKWIDGET_EXPORT Q_DECL_EXPORT
#else
#define MYMASKWIDGET_EXPORT Q_DECL_IMPORT
#endif

class MYMASKWIDGET_EXPORT RefreshMask : public QWidget {
    Q_OBJECT

public:
    explicit RefreshMask(QWidget *parent = nullptr);

    void showLoading();

    void hideLoading();

private:
    void initUi();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void loadingFinished();

private:
    QWidget *maskWidget{};
    QtMaterialCircularProgress *progress{};
};


#endif //REFRESHMASK_H
