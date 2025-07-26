/**
 * @file ElaPlainTextEditStyle.cpp
 * @brief 实现 ElaPlainTextEditStyle 类，提供纯文本编辑框的自定义样式
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaPlainTextEditStyle.h"
#include "ElaTheme.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

/**
 * @brief 构造函数，初始化自定义样式
 * @param style 基础样式，默认为 nullptr
 */
ElaPlainTextEditStyle::ElaPlainTextEditStyle(QStyle* style)
{
    _pExpandMarkWidth = 0;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

/**
 * @brief 析构函数，释放自定义样式资源
 */
ElaPlainTextEditStyle::~ElaPlainTextEditStyle()
= default;

/**
 * @brief 绘制控件元素
 * @param element 控件元素
 * @param option 样式选项
 * @param painter 绘图对象
 * @param widget 目标控件，默认为 nullptr
 */
void ElaPlainTextEditStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            //背景绘制
            QRect editRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            // 边框绘制
            painter->setBrush(ElaThemeColor(_themeMode, BasicBorder));
            painter->drawRoundedRect(editRect, 6, 6);

            painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
            painter->drawRoundedRect(QRectF(editRect.x() + 1.5, editRect.y() + 1.5, editRect.width() - 3, editRect.height() - 3), 6, 6);

            // 底边线绘制
            painter->setBrush(ElaThemeColor(_themeMode, BasicHemline));
            QPainterPath path;
            path.moveTo(6, editRect.height());
            path.lineTo(editRect.width() - 6, editRect.height());
            path.arcTo(QRectF(editRect.width() - 12, editRect.height() - 12, 12, 12), -90, 45);
            path.lineTo(6 - 3 * std::sqrt(2), editRect.height() - (6 - 3 * std::sqrt(2)));
            path.arcTo(QRectF(0, editRect.height() - 12, 12, 12), 270, 45);
            path.closeSubpath();
            painter->drawPath(path);

            //焦点指示器
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
            painter->drawRoundedRect(QRectF(editRect.width() / 2 - _pExpandMarkWidth, editRect.height() - 2.5, _pExpandMarkWidth * 2, 2.5), 2, 2);

            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}