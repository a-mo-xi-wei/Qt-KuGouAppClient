//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLLOCAL_H
#define ALLLOCAL_H

#include <QWidget>

#if defined(ALLLOCAL_LIBRARY)
#define ALLLOCAL_EXPORT Q_DECL_EXPORT
#else
#define ALLLOCAL_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE

namespace Ui {
    class AllLocal;
}

QT_END_NAMESPACE

class ALLLOCAL_EXPORT AllLocal : public QWidget {
    Q_OBJECT

public:
    explicit AllLocal(QWidget *parent = nullptr);

    ~AllLocal() override;

private:
    void initUi();

private:
    Ui::AllLocal *ui;
};


#endif //ALLLOCAL_H
