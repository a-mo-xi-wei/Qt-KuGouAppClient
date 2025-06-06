#include "ElaLineEditPrivate.h"

#include "ElaLineEdit.h"
#include <QTimer>

ElaLineEditPrivate::ElaLineEditPrivate(QObject *parent)
    : QObject{parent} {
}

ElaLineEditPrivate::~ElaLineEditPrivate() {
}

void ElaLineEditPrivate::onWMWindowClickedEvent(QVariantMap data) {
    Q_Q(ElaLineEdit);
    ElaAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<ElaAppBarType::WMMouseActionType>();
    if (actionType == ElaAppBarType::WMLBUTTONDOWN) {
        if (q->hasSelectedText() && q->hasFocus()) {
            q->clearFocus();
        }
    } else if (actionType == ElaAppBarType::WMLBUTTONUP || actionType == ElaAppBarType::WMNCLBUTTONDOWN) {
        if (containsCursorToItem(q) || (
                actionType == ElaAppBarType::WMLBUTTONUP && q->hasSelectedText())) {
            return;
        }
        if (q->hasFocus()) {
            q->clearFocus();
        }
    }
}

void ElaLineEditPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode) {
    Q_Q(ElaLineEdit);
    _themeMode = themeMode;
    if (q->isVisible()) {
        _changeTheme();
    } else {
        QTimer::singleShot(1, this, [=] {
            _changeTheme();
        });
    }
}

void ElaLineEditPrivate::_changeTheme() {
    Q_Q(ElaLineEdit);
    QPalette palette = q->palette();
    if (_themeMode == ElaThemeType::Light) {
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
    } else {
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
    }
    q->setPalette(palette);
}

bool ElaLineEditPrivate::containsCursorToItem(QWidget* item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}
