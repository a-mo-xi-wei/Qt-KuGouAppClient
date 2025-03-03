//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLLOVE_H
#define ALLLOVE_H

#include <QWidget>

#if defined(ALLLOVE_LIBRARY)
#define ALLLOVE_EXPORT Q_DECL_EXPORT
#else
#define ALLLOVE_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class AllLove; }
QT_END_NAMESPACE

class ALLLOVE_EXPORT AllLove : public QWidget {
Q_OBJECT

public:
    explicit AllLove(QWidget *parent = nullptr);
    ~AllLove() override;

private:
    Ui::AllLove *ui;
};


#endif //ALLLOVE_H
