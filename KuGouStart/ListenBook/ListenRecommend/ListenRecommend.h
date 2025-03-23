//
// Created by WeiWang on 25-2-2.
//

#ifndef LISTENRECOMMEND_H
#define LISTENRECOMMEND_H

#include "ListenOptionMenu/ListenOptionMenu.h"
#include "ListenTableWidget.h"

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ListenRecommend;
}

QT_END_NAMESPACE

class ListenRecommend : public QWidget {
    Q_OBJECT

public:
    explicit ListenRecommend(QWidget *parent = nullptr);

    ~ListenRecommend() override;

private:
    void initUi();

    void initTableWidgets();

    void initDailyRecommendGalleryWidget();

    void initOtherGalleryWidget(const QString& jsonFileName, const ListenTableWidget* gallery);

private slots:
    void on_all_classify_toolButton_clicked();

    void onToolButtonClicked();

    void onRefreshTimeout();

private:
    Ui::ListenRecommend *ui;
    //菜单相关
    ListenOptionMenu* m_menu{};
    //画廊标题vector
    std::vector<std::pair<QString,QString>> m_galleryVector[17];
    //设置定时器，防止刷新次数太多，导致显示问题
    QTimer *m_refreshTimer;
};


#endif //LISTENRECOMMEND_H
