//
// Created by WeiWang on 25-2-17.
//

#ifndef LIVEBLOCKWIDGET_H
#define LIVEBLOCKWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LiveBlockWidget;
}

QT_END_NAMESPACE

class LiveBlockWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveBlockWidget(QWidget *parent = nullptr);

    ~LiveBlockWidget() override;

    void setCoverPix(const QString &pixmapPath) const;

    void setLeftBottomText(const QString& text);

    void setLeftPopularBtnFontSize(const int &size,const bool& isBold) const;

    void setTipLabText(const QString& text) const;

    void setNoTipLab() const;

    void setAspectRatio(const float& aspectRatio);

    void setCircleStander(const int& stander) const;

    void setTipStyleSheet(const QString& styleSheet) const;

private:
    void initUi() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::LiveBlockWidget *ui;
    float m_aspectRatio = 0.8;
};


#endif //LIVEBLOCKWIDGET_H
