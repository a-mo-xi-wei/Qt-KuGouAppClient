//
// Created by WeiWang on 25-2-7.
//

#ifndef LISTENTABLEWIDGET_H
#define LISTENTABLEWIDGET_H

#include  "GalleryWidget.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class ListenTableWidget;
}

QT_END_NAMESPACE

class ListenTableWidget : public QWidget {
    Q_OBJECT

public:
    explicit ListenTableWidget(QWidget *parent = nullptr);

    ~ListenTableWidget() override;

    QToolButton* getToolBtn() const;

    void setTitle(const QString& title) const;

    GalleryWidget* getGalleryWidget() const;

private slots:
    void on_toolButton_clicked();

signals:
    void toolBtnClicked();

private:
    Ui::ListenTableWidget *ui;
};


#endif //LISTENTABLEWIDGET_H
