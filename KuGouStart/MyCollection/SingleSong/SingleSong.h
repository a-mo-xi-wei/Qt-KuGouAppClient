//
// Created by WeiWang on 24-11-14.
//

#ifndef SINGLESONG_H
#define SINGLESONG_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SingleSong; }
QT_END_NAMESPACE

class SingleSong : public QWidget {
Q_OBJECT

public:
    explicit SingleSong(QWidget *parent = nullptr);
    ~SingleSong() override;

private:
    void initUi();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::SingleSong *ui;
    QAction *m_searchAction; //专门为了设置图片
};


#endif //SINGLESONG_H
