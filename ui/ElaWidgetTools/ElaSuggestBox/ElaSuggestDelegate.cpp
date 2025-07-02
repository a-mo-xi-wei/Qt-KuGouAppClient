/**
 * @file ElaSuggestDelegate.cpp
 * @brief 实现 ElaSuggestDelegate 类，建议项委托
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#include "ElaSuggestDelegate.h"
#include "ElaSuggestBoxPrivate.h"
#include "ElaSuggestModel.h"
#include "ElaTheme.h"
#include "ElaToolTip.h"
#include "ElaBaseListView.h"

#include <QPainter>
#include <QPainterPath>
#include <qevent.h>

/**
 * @brief 构造函数
 * @param parent 父对象指针，默认为 nullptr
 */
ElaSuggestDelegate::ElaSuggestDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = eTheme->getThemeMode();                      ///< 获取主题模式
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;                               ///< 更新主题模式
    });
}

/**
 * @brief 析构函数
 */
ElaSuggestDelegate::~ElaSuggestDelegate() = default;

/**
 * @brief 绘制函数
 * @param painter 画家
 * @param option 样式选项
 * @param index 模型索引
 * @note 绘制建议项背景、文本和图标，包括选中、悬停效果
 */
void ElaSuggestDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    ElaSuggestModel *model = dynamic_cast<ElaSuggestModel *>(const_cast<QAbstractItemModel *>(index.model()));
    if (!model)
    {
        painter->restore();
        return;
    }

    ElaSuggestion *suggest = model->getSearchSuggestion(index.row());
    if (!suggest)
    {
        painter->restore();
        return;
    }
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;          ///< 移除焦点状态
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect optionRect = option.rect;
    optionRect.adjust(margin * 2, margin, -margin * 2, -margin);
    path.addRoundedRect(optionRect, 8, 8);
    if (option.state & QStyle::State_Selected)
    {
        if (option.state & QStyle::State_MouseOver)
        {
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicSelectedHoverAlpha)); ///< 选中悬停背景
        }
        else
        {
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicSelectedAlpha)); ///< 选中背景
        }
    }
    else
    {
        if (option.state & QStyle::State_MouseOver)
        {
            painter->fillPath(path, ElaThemeColor(_themeMode, BasicHoverAlpha)); ///< 悬停背景
        }
    }
    // @note 文本绘制
    //painter->setPen(ElaThemeColor(_themeMode, BasicText));
    //painter->drawText(option.rect.x() + 30, option.rect.y() + 25, suggest->getSuggestText());
    // 计算可用文本区域（考虑图标空间和边距）
    QRect textRect = option.rect;
    textRect.setRight(textRect.right() - 10);                 ///< 右边留出边距
    textRect.setLeft(textRect.left() + 30);                   ///< 留出图标空间
    const QString originalText = suggest->getSuggestText();   ///< 获取原始文本
    QFontMetrics metrics(painter->font());
    QString clippedText = metrics.elidedText(originalText, Qt::ElideRight, textRect.width()); ///< 裁剪文本
    painter->setPen(ElaThemeColor(_themeMode, BasicText));    ///< 设置文本颜色
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, clippedText); ///< 绘制文本

    // @note 图标绘制
    if (suggest->getElaIcon() != ElaIconType::None)
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(option.rect.x() + 11, option.rect.y() + 26, QChar((unsigned short)suggest->getElaIcon())); ///< 绘制图标
    }
    painter->restore();
}

/**
 * @brief 大小提示
 * @param option 样式选项
 * @param index 模型索引
 * @return 建议项尺寸
 */
QSize ElaSuggestDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);                                       ///< 固定高度
    return size;
}