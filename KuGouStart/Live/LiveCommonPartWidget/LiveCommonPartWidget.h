//
// Created by WeiWang on 25-2-17.
//

#ifndef LIVECOMMONPARTWIDGET_H
#define LIVECOMMONPARTWIDGET_H

#include <QWidget>

class LiveBlockWidget;

QT_BEGIN_NAMESPACE

namespace Ui {
    class LiveCommonPartWidget;
}

QT_END_NAMESPACE

class LiveCommonPartWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveCommonPartWidget(QWidget *parent = nullptr, int lines = 1);

    ~LiveCommonPartWidget() override;

    void setTitleName(const QString& name) const;

    void setNoTipLab();

private:
    void initUi(const int& lines);

    void initLineOne();

    void initLineTwo();

    int getFileCount(const QString &folderPath);

private slots:
    void on_all_pushButton_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void initOK();

private:
    Ui::LiveCommonPartWidget *ui;
    LiveBlockWidget* m_blockArr[12] = {nullptr};
    QList<QString> m_leftBottomTextVec;
};

#endif //LIVECOMMONPARTWIDGET_H
