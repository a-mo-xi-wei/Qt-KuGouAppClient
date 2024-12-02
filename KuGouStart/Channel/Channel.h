//
// Created by WeiWang on 24-11-12.
//

#ifndef CHANNEL_H
#define CHANNEL_H

#include <QWidget>

class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class Channel;
}

QT_END_NAMESPACE

class Channel : public QWidget {
    Q_OBJECT

public:
    explicit Channel(QWidget *parent = nullptr);

    ~Channel() override;

private:
    void initButtonGroup();

private:
    Ui::Channel *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
};


#endif //CHANNEL_H
