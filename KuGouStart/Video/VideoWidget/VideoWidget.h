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

};


#endif //VIDEOWIDGET_H
