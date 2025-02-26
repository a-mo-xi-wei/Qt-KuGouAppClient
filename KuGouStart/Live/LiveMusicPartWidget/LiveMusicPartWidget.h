//
// Created by WeiWang on 25-2-17.
//

#ifndef LIVEMUSICPARTWIDGET_H
#define LIVEMUSICPARTWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LiveMusicPartWidget;
}

QT_END_NAMESPACE

class LiveMusicPartWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveMusicPartWidget(QWidget *parent = nullptr);

    ~LiveMusicPartWidget() override;

    void setTitleName(const QString& name) const;

private:
    void initUi();

    QList<QString> parseJsonFile(const QString &filePath);

    void initBlockWidget();

    int getFileCount(const QString &folderPath);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::LiveMusicPartWidget *ui;
    QList<QString> m_leftBottomTextVec;

};

#endif //LIVEMUSICPARTWIDGET_H
