//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLRECENT_H
#define ALLRECENT_H

#include <QWidget>

#if defined(ALLRECENT_LIBRARY)
#define ALLRECENT_EXPORT Q_DECL_EXPORT
#else
#define ALLRECENT_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class AllRecent; }
QT_END_NAMESPACE

class ALLRECENT_EXPORT AllRecent : public QWidget {
Q_OBJECT

public:
    explicit AllRecent(QWidget *parent = nullptr);
    ~AllRecent() override;

private:
    Ui::AllRecent *ui;
};


#endif //ALLRECENT_H
