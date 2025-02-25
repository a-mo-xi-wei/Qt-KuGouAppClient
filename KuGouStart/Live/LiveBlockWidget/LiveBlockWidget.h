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

    void setTipLabText(const QString& text) const;

private:
    void initUi() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::LiveBlockWidget *ui;
};


#endif //LIVEBLOCKWIDGET_H
