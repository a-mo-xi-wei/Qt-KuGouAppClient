//
// Created by WeiWang on 25-1-25.
//

#ifndef SONGLISTBLOCKWIDGET_H
#define SONGLISTBLOCKWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SongListBlockWidget;
}

QT_END_NAMESPACE

class SongListBlockWidget : public QWidget {
    Q_OBJECT

public:
    explicit SongListBlockWidget(QWidget *parent = nullptr);

    ~SongListBlockWidget() override;

    void setCoverPix(const QString &pixmapPath) const;

    void setTitleText(const QString &title) const;

private:
    void initUi()const;

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::SongListBlockWidget *ui;
};


#endif //SONGLISTBLOCKWIDGET_H
