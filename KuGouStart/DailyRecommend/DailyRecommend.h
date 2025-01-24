//
// Created by WeiWang on 24-11-14.
//

#ifndef DAILYRECOMMEND_H
#define DAILYRECOMMEND_H

#include "MusicItemWidget.h"
#include <QWidget>

class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class DailyRecommend; }
QT_END_NAMESPACE

class DailyRecommend : public QWidget {
Q_OBJECT

public:
    explicit DailyRecommend(QWidget *parent = nullptr);
    ~DailyRecommend() override;

private:
    void initUi();

    void initDateLab();

    void initTableWidget();

    void initMusicItem(MusicItemWidget *item);

private:
    Ui::DailyRecommend *ui;
    QLabel* m_monthLab{};
    QLabel* m_dayLab{};
};


#endif //DAILYRECOMMEND_H
