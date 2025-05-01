//
// Created by WeiWang on 25-2-2.
//

#ifndef LISTENMYDOWNLOAD_H
#define LISTENMYDOWNLOAD_H

#include "DownloadedWidget/DownloadedWidget.h"
#include "DownloadingWidget/DownloadingWidget.h"

#include <QWidget>

class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class ListenMyDownload;
}

QT_END_NAMESPACE

class ListenMyDownload : public QWidget {
    Q_OBJECT

public:
    explicit ListenMyDownload(QWidget *parent = nullptr);

    ~ListenMyDownload() override;

private:
    void initStackedWidget();

    void initUi();

    void initIndexLab();

    void initDownloadedWidget();

    void initDownloadingWidget();

    void enableButton(const bool& flag) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_downloaded_pushButton_clicked();

    void on_downloading_pushButton_clicked();

signals:
    void switch_to_listen_recommend();

private:
    Ui::ListenMyDownload *ui;
    std::unique_ptr<QButtonGroup>   m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<DownloadedWidget>   m_downloaded{};
    std::unique_ptr<DownloadingWidget>  m_downloading{};
};


#endif //LISTENMYDOWNLOAD_H
