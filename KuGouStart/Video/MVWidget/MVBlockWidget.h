//
// Created by WeiWang on 24-12-27.
//

#ifndef MVBLOCKWIDGET_H
#define MVBLOCKWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class MVBlockWidget;
}

QT_END_NAMESPACE

class MVBlockWidget : public QWidget {
    Q_OBJECT

public:
    explicit MVBlockWidget(QWidget *parent = nullptr);

    ~MVBlockWidget() override;

    void setCoverPix(const QString &pixmapPath) const;

    void setTitle(const QString &title);

    void setDescription(const QString &description);

private:
    void initUi();

    void updateTitleText() const;

    void updateDescText() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MVBlockWidget *ui;
    //title ,desc
    QString m_title;
    QString m_description;
};


#endif //MVBLOCKWIDGET_H
