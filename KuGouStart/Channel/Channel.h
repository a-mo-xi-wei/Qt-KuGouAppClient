//
// Created by WeiWang on 24-11-12.
//

#ifndef CHANNEL_H
#define CHANNEL_H

#include"PartWidget.h"
#include"UpToolButton.h"

#include <QWidget>

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class Channel;
}

QT_END_NAMESPACE

class Channel : public QWidget {
    Q_OBJECT

public:
    explicit Channel(QWidget *parent = nullptr);

    ~Channel() override;

private:
    void initButtonGroup()const;

    void initTotalWidget()const;

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
    Ui::Channel *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    std::unique_ptr<UpToolButton> m_upBtn{};
    //14个Widget
    std::unique_ptr<PartWidget> m_recommendWidget{};
    std::unique_ptr<PartWidget> m_djWidget{};
    std::unique_ptr<PartWidget> m_languageWidget{};
    std::unique_ptr<PartWidget> m_themeWidget{};
    std::unique_ptr<PartWidget> m_sceneWidget{};
    std::unique_ptr<PartWidget> m_moodWidget{};
    std::unique_ptr<PartWidget> m_styleWidget{};
    std::unique_ptr<PartWidget> m_crowdWidget{};
    std::unique_ptr<PartWidget> m_childrenWidget{};
    std::unique_ptr<PartWidget> m_musicalInstrumentWidget{};
    std::unique_ptr<PartWidget> m_labelWidget{};
    std::unique_ptr<PartWidget> m_varietyWidget{};
    std::unique_ptr<PartWidget> m_nationalCustomsWidget{};
    std::unique_ptr<PartWidget> m_sportsWidget{};
    //配对歌手歌曲
    std::vector<std::pair<QString,QString>> m_songAndsinger{};
    std::vector<QString> m_pixPathVector{};
    //垂直滚动条
    QScrollBar* m_vScrollBar{};
    //专门处理回到最顶部按钮
    QTimer*     m_scrollBarTimer{};  // 定时器
};
#endif //CHANNEL_H
