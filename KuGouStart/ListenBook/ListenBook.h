//
// Created by WeiWang on 24-11-18.
//

#ifndef LISTENBOOK_H
#define LISTENBOOK_H

#include <QWidget>
class QButtonGroup;


QT_BEGIN_NAMESPACE
namespace Ui { class ListenBook; }
QT_END_NAMESPACE

class ListenBook : public QWidget {
Q_OBJECT

public:
    explicit ListenBook(QWidget *parent = nullptr);
    ~ListenBook() override;

    void initUi();

private:
    Ui::ListenBook *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};

};


#endif //LISTENBOOK_H
