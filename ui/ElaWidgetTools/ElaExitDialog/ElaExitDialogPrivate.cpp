#include "ElaExitDialogPrivate.h"
#include "ElaExitDialog.h"
#include "ElaMaskWidget.h"

ElaExitDialogPrivate::ElaExitDialogPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaExitDialogPrivate::~ElaExitDialogPrivate()
{
}

void ElaExitDialogPrivate::_doCloseAnimation()
{
    Q_Q(ElaExitDialog);
    _maskWidget->doMaskAnimation(0);
    q->QDialog::close();
}
