//
// Created by WeiWang on 24-10-9.
//

#ifndef RECOMMENDFORYOU_H
#define RECOMMENDFORYOU_H

#include <QWidget>
#include "UpToolButton.h"

class QScrollBar;

QT_BEGIN_NAMESPACE
namespace Ui { class RecommendForYou; }
QT_END_NAMESPACE

class RecommendForYou : public QWidget {
Q_OBJECT

public:
    explicit RecommendForYou(QWidget *parent = nullptr);
    ~RecommendForYou() override;

private:
    void initAdvertiseBoard()const;

    void initClassifyWidget()const;

    void initTabWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleWheelValue(const int& value);

    void onUpBtnClicked();

    void onUpBtnShowOrNot();

private:
    Ui::RecommendForYou *ui;
    std::unique_ptr<UpToolButton> m_upBtn{};
    //垂直滚动条
    QScrollBar* m_vScrollBar{};
    //专门处理回到最顶部按钮
    QTimer*     m_scrollBarTimer{};  // 定时器

};


#endif //RECOMMENDFORYOU_H
