//
// Created by WeiWang on 24-11-14.
//

#ifndef LIVE_H
#define LIVE_H

#include "LiveCommonPartWidget/LiveCommonPartWidget.h"
#include "LiveMusicPartWidget/LiveMusicPartWidget.h"
#include "LiveBigLeftWidget/LiveBigLeftWidget.h"

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class Live;
}

QT_END_NAMESPACE

class Live : public QWidget {
    Q_OBJECT

public:
    explicit Live(QWidget *parent = nullptr);

    ~Live() override;

private:
    void initButtonGroup()const;

    void initUi();

    void initPopularWidget();

    QPixmap roundedPixmap(const QPixmap &src,const QSize& size,const int& radius);

    void initAttentionWidget();

    void initRecommendWidget();

    void initMusicWidget();

    void initNewStarWidget();

    void initAppearanceWidget();

    void initDanceWidget();

    void initGameWidget();

private slots:
    void handleWheelValue(const int& value);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void showEvent(QShowEvent *event) override;

private:
    Ui::Live *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    std::unique_ptr<LiveCommonPartWidget> m_recommendWidget{};
    std::unique_ptr<LiveMusicPartWidget> m_musicWidget{};
    std::unique_ptr<LiveCommonPartWidget> m_newStarWidget{};
    std::unique_ptr<LiveBigLeftWidget> m_appearanceWidget{};
    std::unique_ptr<LiveCommonPartWidget> m_danceWidget{};
    std::unique_ptr<LiveBigLeftWidget> m_gameWidget{};

    //垂直滚动条
    QScrollBar* m_vScrollBar{};
};

#endif //LIVE_H
