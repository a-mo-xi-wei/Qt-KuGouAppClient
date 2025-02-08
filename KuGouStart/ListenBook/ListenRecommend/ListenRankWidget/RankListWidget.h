//
// Created by WeiWang on 25-2-8.
//

#ifndef RANKLISTWIDGET_H
#define RANKLISTWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class RankListWidget;
}

QT_END_NAMESPACE

class RankListWidget : public QWidget {
    Q_OBJECT

public:
    explicit RankListWidget(QWidget *parent = nullptr);

    ~RankListWidget() override;

    void setDescText(const QString& text) const;

    void setInfoText(const QString& text) const;

    void setCoverImg(const QString& path) const;

    void setRankMedal(const QString& path) const;

    void setRankNumber(const QString& number) const;

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

private:
    Ui::RankListWidget *ui;
};


#endif //RANKLISTWIDGET_H
