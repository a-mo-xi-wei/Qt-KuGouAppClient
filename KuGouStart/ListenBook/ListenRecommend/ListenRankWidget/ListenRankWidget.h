//
// Created by WeiWang on 25-2-8.
//

#ifndef LISTENRANKWIDGET_H
#define LISTENRANKWIDGET_H

#include "RankPartWidget.h"


class ListenRankWidget : public QWidget {
Q_OBJECT

public:
    explicit ListenRankWidget(QWidget *parent = nullptr);

private:
    void initUi();

    void initRankFree();

    void initRankSkyrocket();

    void initRankSell();

    void initRankNew();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    RankPartWidget* rank_free{};
    RankPartWidget* rank_skyrocket{};
    RankPartWidget* rank_sell{};
    RankPartWidget* rank_new{};
};


#endif //LISTENRANKWIDGET_H
