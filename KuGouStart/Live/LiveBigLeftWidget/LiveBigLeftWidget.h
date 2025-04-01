//
// Created by WeiWang on 25-2-17.
//

#ifndef LIVEBIGLEFTWIDGET_H
#define LIVEBIGLEFTWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LiveBigLeftWidget;
}

QT_END_NAMESPACE

class LiveBigLeftWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveBigLeftWidget(QWidget *parent = nullptr);

    ~LiveBigLeftWidget() override;

    void setTitleName(const QString& name) const;

    void setNoTipLab() const;

private:
    void initUi();

    QList<QString> parseJsonFile(const QString &filePath);

    void initBlockWidget();

    int getFileCount(const QString &folderPath);

protected:
    void resizeEvent(QResizeEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::LiveBigLeftWidget *ui;
    QList<QString> m_leftBottomTextVec;

};


#endif //LIVEBIGLEFTWIDGET_H
