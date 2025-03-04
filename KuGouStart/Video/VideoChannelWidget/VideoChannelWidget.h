//
// Created by WeiWang on 24-11-12.
//

#ifndef VIDEOCHANNELWIDGET_H
#define VIDEOCHANNELWIDGET_H

#include "VideoChannelPartWidget.h"

#if defined(VIDEOCHANNELWIDGET_LIBRARY)
#define VIDEOCHANNELWIDGET_EXPORT Q_DECL_EXPORT
#else
#define VIDEOCHANNELWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoChannelWidget;
}

QT_END_NAMESPACE

class VIDEOCHANNELWIDGET_EXPORT VideoChannelWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoChannelWidget(QWidget *parent = nullptr);

    ~VideoChannelWidget() override;

private:
    void initButtonGroup()const;

    void initTotalWidget()const;

    void initUi();

    void initVector();

private slots:
    void handleWheelValue(const int& value);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::VideoChannelWidget *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    QWidget* m_parent{};
    //11个Widget
    std::unique_ptr<VideoChannelPartWidget> m_popularWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_childrenWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_themeWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_filmWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_varietyWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_ACGNWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_sceneWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_languageWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_danceWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_siteWidget{};
    std::unique_ptr<VideoChannelPartWidget> m_singerWidget{};
    //图片路径
    std::vector<QString> m_pixPathVector{};
    //垂直滚动条
    QScrollBar* m_vScrollBar{};
};


#endif //VIDEOCHANNELWIDGET_H
