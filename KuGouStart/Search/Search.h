//
// Created by WeiWang on 24-11-18.
//

#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class Search;
}

QT_END_NAMESPACE

class Search : public QWidget {
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);

    ~Search() override;

    void initUi();

private slots:
    void on_recommend_pushButton_clicked();

    void on_rank_pushButton_clicked();

    void on_special_pushButton_clicked();

    void on_channel_pushButton_clicked();
private:
    Ui::Search *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup;
};


#endif //SEARCH_H
