//
// Created by WeiWang on 24-12-3.
//

#ifndef PARTWIDGET_H
#define PARTWIDGET_H

#include "ChannelBlock.h"

class QLabel;


class PartWidget : public QWidget {
Q_OBJECT

public:
    explicit PartWidget(QWidget *parent = nullptr);

    void setTitleName(const QString &title)const;

    void addBlockWidget(ChannelBlock* block)const;

private:
    void initUi();


private:
    QLabel* m_titleLab{};   //标题
    QLabel* m_titleDeco{};  //标题旁边的 .MHz
    QWidget* m_tabWidget{}; //下面的容器

};


#endif //PARTWIDGET_H
