//
// Created by WeiWang on 24-11-12.
//

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include"VideoPartWidget.h"
#include <QWidget>

#if defined(VIDEOVIDEOWIDGET_LIBRARY)
#define VIDEOVIDEOWIDGET_EXPORT Q_DECL_EXPORT
#else
#define VIDEOVIDEOWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoWidget;
}

QT_END_NAMESPACE

class VIDEOVIDEOWIDGET_EXPORT VideoWidget : public QWidget {
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

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::VideoWidget *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
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
};


#endif //VIDEOWIDGET_H
