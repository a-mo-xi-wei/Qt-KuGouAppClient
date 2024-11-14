//
// Created by WeiWang on 24-11-14.
//

#ifndef DAILYRECOMMEND_H
#define DAILYRECOMMEND_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class DailyRecommend; }
QT_END_NAMESPACE

class DailyRecommend : public QWidget {
Q_OBJECT

public:
    explicit DailyRecommend(QWidget *parent = nullptr);
    ~DailyRecommend() override;

private:
    Ui::DailyRecommend *ui;
};


#endif //DAILYRECOMMEND_H
