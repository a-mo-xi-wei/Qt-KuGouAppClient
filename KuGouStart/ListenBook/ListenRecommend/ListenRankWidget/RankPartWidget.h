//
// Created by WeiWang on 25-2-8.
//

#ifndef RANKPARTWIDGET_H
#define RANKPARTWIDGET_H

#include "RankListWidget.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class RankPartWidget;
}

QT_END_NAMESPACE

class RankPartWidget : public QWidget {
    Q_OBJECT

public:
    explicit RankPartWidget(QWidget *parent = nullptr);

    ~RankPartWidget() override;

    void setTitle(const QString& title) const;

    RankListWidget* getRankListWidget(const int& index)const;

private:
    void initRankListArr();

private:
    Ui::RankPartWidget *ui;
    RankListWidget* m_rankListArr[5];
};


#endif //RANKPARTWIDGET_H
