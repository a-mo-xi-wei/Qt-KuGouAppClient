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

    void setTitle(const QString& title) const;

    GalleryWidget* getGalleryWidget() const;

    void setCnt(const int& cnt) ;

    int getCnt() const;

private:
    void initUi() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_toolButton_clicked();

signals:
    void toolBtnClicked(const int& cnt);

private:
    Ui::ListenTableWidget *ui;
    int m_cnt = 0;
};


#endif //LISTENTABLEWIDGET_H
