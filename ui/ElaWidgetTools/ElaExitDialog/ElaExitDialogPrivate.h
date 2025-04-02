#ifndef ELACONTENTDIALOGPRIVATE_H
#define ELACONTENTDIALOGPRIVATE_H

#include "Def.h"
#include "stdafx.h"

class QVBoxLayout;
class ElaPushButton;
class ElaExitDialog;
class ElaMaskWidget;
class ElaExitDialogPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaExitDialog)
public:
    explicit ElaExitDialogPrivate(QObject* parent = nullptr);
    ~ElaExitDialogPrivate();

private:
    qint64 _currentWinID{0};
    ElaThemeType::ThemeMode _themeMode;
    ElaMaskWidget* _maskWidget{nullptr};
    QWidget* _centralWidget{nullptr};
    QWidget* _buttonWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QString _leftButtonText{"cancel"};
    QString _middleButtonText{"minimum"};
    QString _rightButtonText{"exit"};
    ElaPushButton* _leftButton{nullptr};
    ElaPushButton* _middleButton{nullptr};
    ElaPushButton* _rightButton{nullptr};
    void _doCloseAnimation();
};

#endif // ELACONTENTDIALOGPRIVATE_H
