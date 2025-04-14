#ifndef QTMATERIALSTATETRANSITION_H
#define QTMATERIALSTATETRANSITION_H

#include <QAbstractTransition>
#include "qtmaterialstatetransitionevent.h"

#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QTMATERIALWIDGET_EXPORT QtMaterialStateTransition : public QAbstractTransition
{
    Q_OBJECT

public:
    QtMaterialStateTransition(QtMaterialStateTransitionType type);

protected:
    virtual bool eventTest(QEvent *event);
    virtual void onTransition(QEvent *);

private:
    QtMaterialStateTransitionType m_type;
};

#endif // QTMATERIALSTATETRANSITION_H
