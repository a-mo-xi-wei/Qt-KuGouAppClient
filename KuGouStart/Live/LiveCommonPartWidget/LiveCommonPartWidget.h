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

    void setTitleName(const QString& name);

private:
    void initUi(const int& lines);

    QList<QString> parseJsonFile(const QString &filePath);

    void initLineOne();

    void initLineTwo();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::LiveCommonPartWidget *ui;
    LiveBlockWidget* m_blockArr[12] = {nullptr};
    QList<QString> m_leftBottomTextVec;
};

#endif //LIVECOMMONPARTWIDGET_H
