/**
 * @file ElaMenuStyle.cpp
 * @brief 实现 ElaMenuStyle 类，提供自定义菜单样式
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaMenuStyle.h"
#include "ElaTheme.h"
#include "ElaMenu.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

/**
 * @brief 构造函数，初始化菜单样式
 * @param style 基础样式指针，默认为 nullptr
 */
ElaMenuStyle::ElaMenuStyle(QStyle* style)
{
    _pMenuItemHeight = 32;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

/**
 * @brief 析构函数，释放菜单样式资源
 */
ElaMenuStyle::~ElaMenuStyle()
= default;

/**
 * @brief 绘制基本图形元素
 * @param element 图形元素
 * @param option 样式选项
 * @param painter 绘制器
 * @param widget 控件指针
 */
void ElaMenuStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelMenu:
    {
        // 高性能阴影
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        eTheme->drawEffectShadow(painter, option->rect, _shadowBorderWidth, 6);
        // 背景绘制
        QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, option->rect.width() - 2 * _shadowBorderWidth, option->rect.height() - 2 * _shadowBorderWidth);
        painter->setPen(ElaThemeColor(_themeMode, PopupBorder));
        painter->setBrush(ElaThemeColor(_themeMode, PopupBase));
        painter->drawRoundedRect(foregroundRect, 6, 6);
        painter->restore();
        return;
    }
    case QStyle::PE_FrameMenu:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

/**
 * @brief 绘制控件元素
 * @param element 控件元素
 * @param option 样式选项
 * @param painter 绘制器
 * @param widget 控件指针
 */
void ElaMenuStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_MenuItem:
    {
        // 提升 menu 变量到外层作用域
        const ElaMenu* menu = widget ? qobject_cast<const ElaMenu*>(widget) : nullptr;

        //内容绘制 区分类型
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator)
            {
                QRect separatorRect = mopt->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                painter->setPen(Qt::NoPen);
                painter->setBrush(ElaThemeColor(_themeMode, BasicBaseLine));
                painter->drawRoundedRect(QRectF(separatorRect.x() + separatorRect.width() * 0.055, separatorRect.center().y(), separatorRect.width() - separatorRect.width() * 0.11, 1.5), 1, 1);
                painter->restore();
                return;
            }
            else
            {
                QRect menuRect = mopt->rect;
                qreal contentPadding = menuRect.width() * 0.055;
                qreal textLeftSpacing = menuRect.width() * 0.082;
                painter->save();
                painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
                //覆盖效果
                if (mopt->state.testFlag(QStyle::State_Enabled) && (mopt->state.testFlag(QStyle::State_MouseOver) || mopt->state.testFlag(QStyle::State_Selected)))
                {
                    QRect hoverRect = menuRect;
                    hoverRect.adjust(0, 2, 0, -2);
                    painter->setPen(Qt::NoPen);
                    if (_pMenuItemHoveredBackgroundColor != QColor() && _pMenuItemHoveredBackgroundColor.isValid())
                        painter->setBrush(_pMenuItemHoveredBackgroundColor);
                    else
                        painter->setBrush(ElaThemeColor(_themeMode, PopupHover));
                    painter->drawRoundedRect(hoverRect, 5, 5);
                }
                // 设置字体颜色
                bool isHovered = false;
                if (_pMenuItemHoveredFontColor != QColor() && _pMenuItemHoveredFontColor.isValid()) {
                    isHovered = mopt->state.testFlag(QStyle::State_MouseOver) || mopt->state.testFlag(QStyle::State_Selected);
                }
               //Icon绘制
                QIcon menuIcon = mopt->icon;
                //check绘制
                if (mopt->menuHasCheckableItems)
                {
                    painter->save();
                    painter->setPen(isHovered ? _pMenuItemHoveredFontColor : (!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == ElaThemeType::Light ? Qt::black : Qt::white));
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(_pMenuItemHeight * 0.57);
                    painter->setFont(iconFont);
                    painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(), _iconWidth, menuRect.height()), Qt::AlignCenter, mopt->checked ? QChar((unsigned short)ElaIconType::Check) : QChar((unsigned short)ElaIconType::None));
                    painter->restore();
                }
                else
                {
                    QString iconText;
                    // const ElaMenu* menu = dynamic_cast<const ElaMenu*>(widget);
                    if (menu)
                    {
                        QAction* action = menu->actionAt(menuRect.center());
                        if (action)
                        {
                            iconText = action->property("ElaIconType").toString();
                        }
                    }
                    if (!iconText.isEmpty())
                    {
                        painter->save();
                        painter->setPen(isHovered ? _pMenuItemHoveredFontColor : (!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == ElaThemeType::Light ? Qt::black : Qt::white));
                        QFont iconFont = QFont("ElaAwesome");
                        iconFont.setPixelSize(_pMenuItemHeight * 0.57);
                        painter->setFont(iconFont);
                        painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(), _iconWidth, menuRect.height()), Qt::AlignCenter, iconText);
                        painter->restore();
                    }
                    else
                    {
                        if (!menuIcon.isNull())
                        {
                            painter->drawPixmap(QRect(menuRect.x() + contentPadding, menuRect.center().y() - _iconWidth / 2, _iconWidth, _iconWidth), menuIcon.pixmap(_iconWidth, _iconWidth));
                        }
                    }
                }
                //文字和快捷键绘制
                if (!mopt->text.isEmpty())
                {
                    QStringList textList = mopt->text.split("\t");
                    painter->setPen(isHovered ? _pMenuItemHoveredFontColor : (!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == ElaThemeType::Light ? Qt::black : Qt::white));
                    painter->drawText(QRectF(menuRect.x() + (_isAnyoneItemHasIcon ? contentPadding + textLeftSpacing : 0) + _iconWidth, menuRect.y(), menuRect.width(), menuRect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, textList[0]);
                    // ====== 新增：绘制红色小圆点 ======
                    bool showRedDot = false;
                    if (menu) {
                        // 通过位置获取对应的 Action
                        QAction* action = menu->actionAt(menuRect.center());
                        if (action) {
                            showRedDot = action->property("showRedDot").toBool();
                        }
                    }

                    if (showRedDot) {
                        painter->save();
                        painter->setRenderHint(QPainter::Antialiasing);
                        painter->setPen(Qt::NoPen);

                        // 使用主题色或固定红色
                        QColor redDotColor = QColor(0xFA4E32);

                        painter->setBrush(redDotColor);

                        // 计算文本宽度
                        QFontMetrics fm(painter->font());
                        int textWidth = fm.horizontalAdvance(textList[0]);

                        // 计算红点位置（文本右侧）
                        int dotSize = 6; // 红点直径
                        int baseX = menuRect.x() + (_isAnyoneItemHasIcon ? contentPadding + textLeftSpacing : 0) + _iconWidth;
                        int dotX = baseX + textWidth + 10; // 文本右侧5像素处
                        int dotY = menuRect.y() + (menuRect.height() - dotSize) / 2;

                        // 绘制红点
                        painter->drawEllipse(dotX, dotY, dotSize, dotSize);
                        painter->restore();
                    }
                    // ====== 结束新增 ======
                    if (textList.count() > 1)
                    {
                        painter->drawText(QRectF(menuRect.x() + contentPadding + _iconWidth + textLeftSpacing, menuRect.y(), menuRect.width() - (contentPadding * 2 + _iconWidth + textLeftSpacing), menuRect.height()), Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine, textList[1]);
                    }
                }
                //展开图标
                if (mopt->menuItemType == QStyleOptionMenuItem::SubMenu)
                {
                    painter->save();
                    painter->setPen(isHovered ? _pMenuItemHoveredFontColor : (!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == ElaThemeType::Light ? Qt::black : Qt::white));
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(18);
                    painter->setFont(iconFont);
                    painter->drawText(QRect(menuRect.right() - 25, menuRect.y(), 25, menuRect.height()), Qt::AlignVCenter, QChar((unsigned short)ElaIconType::AngleRight));
                    painter->restore();
                }
                painter->restore();
            }
        }

        return;
    }
    case QStyle::CE_MenuEmptyArea:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

/**
 * @brief 获取像素度量值
 * @param metric 度量类型
 * @param option 样式选项
 * @param widget 控件指针
 * @return 像素度量值
 */
int ElaMenuStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_SmallIconSize:
    {
        //图标宽度
        return _iconWidth;
    }
    case QStyle::PM_MenuPanelWidth:
    {
        //外围容器宽度
        return _shadowBorderWidth * 1.5;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

/**
 * @brief 计算内容尺寸
 * @param type 内容类型
 * @param option 样式选项
 * @param size 原始尺寸
 * @param widget 控件指针
 * @return 计算后的尺寸
 */
QSize ElaMenuStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_MenuItem:
    {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator)
            {
                break;
            }
            QSize menuItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            const ElaMenu* menu = dynamic_cast<const ElaMenu*>(widget);
            if (menu->isHasIcon() || mopt->menuHasCheckableItems)
            {
                _isAnyoneItemHasIcon = true;
            }
            if (menu->isHasChildMenu())
            {
                return QSize(menuItemSize.width() + 20, _pMenuItemHeight);
            }
            else
            {
                return QSize(menuItemSize.width(), _pMenuItemHeight);
            }
        }
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}