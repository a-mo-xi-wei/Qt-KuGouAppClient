//
// Created by WeiWang on 24-11-12.
//

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include"UpToolButton.h"
#include"VideoPartWidget.h"
#include <QWidget>

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoWidget;
}

QT_END_NAMESPACE

class VideoWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = nullptr);

    ~VideoWidget() override;

private:
    void initButtonGroup()const;

    void initTotalWidget()const;

    void initUi();

    void initVector();

private slots:
    void on_all_toolButton_toggled();

    void handleWheelValue(const int& value);

    void onUpBtnClicked();

    void onUpBtnShowOrNot();

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::VideoWidget *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    std::unique_ptr<UpToolButton> m_upBtn{};
    //13个widget
    std::unique_ptr<VideoPartWidget> m_recommendWidget      {};
    std::unique_ptr<VideoPartWidget> m_videoRankWidget      {};
    std::unique_ptr<VideoPartWidget> m_MVWidget             {};
    std::unique_ptr<VideoPartWidget> m_siteWidget           {};
    std::unique_ptr<VideoPartWidget> m_coverWidget          {};
    std::unique_ptr<VideoPartWidget> m_danceWidget          {};
    std::unique_ptr<VideoPartWidget> m_childrenWidget       {};
    std::unique_ptr<VideoPartWidget> m_liveWidget           {};
    std::unique_ptr<VideoPartWidget> m_firstConcertWidget   {};
    std::unique_ptr<VideoPartWidget> m_chineseLanguageWidget{};
    std::unique_ptr<VideoPartWidget> m_southKoreaWidget     {};
    std::unique_ptr<VideoPartWidget> m_japanWidget          {};
    std::unique_ptr<VideoPartWidget> m_americanWidget       {};
    //图片路径
    std::vector<QString> m_pixPathVector{};
    //垂直滚动条
    QScrollBar* m_vScrollBar{};
    //专门处理回到最顶部按钮
    QTimer*     m_scrollBarTimer{};  // 定时器
};


#endif //VIDEOWIDGET_H
