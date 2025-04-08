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

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_history_recommend_toolButton_clicked();

    void on_play_toolButton_clicked();

    void on_vip_toolButton_clicked();

    void on_collect_toolButton_clicked();

    void on_download_toolButton_clicked();

    void on_batch_toolButton_clicked();

private:
    Ui::DailyRecommend *ui;
    QLabel* m_monthLab{};
    QLabel* m_dayLab{};
};


#endif //DAILYRECOMMEND_H
