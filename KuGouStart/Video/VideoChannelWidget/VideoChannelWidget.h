//
// Created by WeiWang on 24-11-12.
//

#ifndef VIDEOCHANNELWIDGET_H
#define VIDEOCHANNELWIDGET_H

#include"UpToolButton.h"
#include"VideoChannelPartWidget.h"
#include <QWidget>

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoChannelWidget;
}

QT_END_NAMESPACE

class VideoChannelWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoChannelWidget(QWidget *parent = nullptr);

    ~VideoChannelWidget() override;

private:
    void initButtonGroup();

    void initTotalWidget();

    void initUi();

    void initVector();

    private slots:
        void handleWheelValue(const int& value);

    void onUpBtnClicked();

    void onUpBtnShowOrNot();

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::VideoChannelWidget *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    std::unique_ptr<UpToolButton> m_upBtn{};
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
    //专门处理回到最顶部按钮
    QTimer*     m_scrollBarTimer{};  // 定时器
};


#endif //VIDEOCHANNELWIDGET_H
