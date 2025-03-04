//
// Created by WeiWang on 24-11-14.
//

#ifndef DAILYRECOMMEND_H
#define DAILYRECOMMEND_H

#include "MusicItemWidget.h"
#include <QWidget>

#if defined(DAILYRECOMMEND_LIBRARY)
#define DAILYRECOMMEND_EXPORT Q_DECL_EXPORT
#else
#define DAILYRECOMMEND_EXPORT Q_DECL_IMPORT
#endif

class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class DailyRecommend; }
QT_END_NAMESPACE

class DAILYRECOMMEND_EXPORT DailyRecommend : public QWidget {
Q_OBJECT

public:
    explicit DailyRecommend(QWidget *parent = nullptr);
    ~DailyRecommend() override;

private:
    void initUi();

    void initDateLab();

    void initTableWidget();

    void initMusicItem(MusicItemWidget *item);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::DailyRecommend *ui;
    QLabel* m_monthLab{};
    QLabel* m_dayLab{};
};


#endif //DAILYRECOMMEND_H
