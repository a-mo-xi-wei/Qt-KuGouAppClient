#include "ElaSuggestDelegate.h"
#include "ElaSuggestBoxPrivate.h"
#include "ElaSuggestModel.h"
#include "ElaTheme.h"
#include "ElaToolTip.h"
#include "ElaBaseListView.h"

#include <QPainter>
#include <QPainterPath>
#include <qevent.h>

ElaSuggestDelegate::ElaSuggestDelegate(QObject *parent)
    : QStyledItemDelegate{parent} {
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaSuggestDelegate::~ElaSuggestDelegate() = default;

void ElaSuggestDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    int margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    ElaSuggestModel *model = dynamic_cast<ElaSuggestModel *>(const_cast<QAbstractItemModel *>(index.model()));
    ElaSuggestion *suggest = model->getSearchSuggestion(index.row());

    if (option.state.testFlag(QStyle::State_HasFocus)) {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect optionRect = option.rect;
    optionRect.adjust(margin * 2, margin, -margin * 2, -margin);
    path.addRoundedRect(optionRect, 8, 8);
    if (option.state & QStyle::State_Selected) {
        if (option.state & QStyle::State_MouseOver) {
            //选中时覆盖
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicSelectedHoverAlpha));
        } else {
            //选中
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicSelectedAlpha));
        }
    } else {
        if (option.state & QStyle::State_MouseOver) {
            //覆盖时颜色
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicHoverAlpha));
        }
    }
    //文字绘制
    //painter->setPen(ElaThemeColor(_themeMode, BasicText));
    //painter->drawText(option.rect.x() + 30, option.rect.y() + 25, suggest->getSuggestText());
    // 计算可用文本区域（考虑图标空间和边距）
        QRect textRect = option.rect;
    textRect.setLeft(textRect.left() + 30); // 图标区域
    textRect.setRight(textRect.right() - 10); // 右边留出10px边距

    // 获取原始文本
    const QString originalText = suggest->getSuggestText();

    // 使用QFontMetrics计算裁剪文本
    QFontMetrics metrics(painter->font());
    QString clippedText = metrics.elidedText(
        originalText,
        Qt::ElideRight,
        textRect.width()
    );

    // 绘制裁剪后的文本
    painter->setPen(ElaThemeColor(_themeMode, BasicText));
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, clippedText);

    //图标绘制
    if (suggest->getElaIcon() != ElaIconType::None) {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(option.rect.x() + 11, option.rect.y() + 26, QChar((unsigned short) suggest->getElaIcon()));
    }
    painter->restore();
}

QSize ElaSuggestDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}
