/**
 * @file ElaFlowLayoutPrivate.cpp
 * @brief 实现 ElaFlowLayoutPrivate 类，管理流式布局的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaFlowLayoutPrivate.h"
#include "ElaFlowLayout.h"

#include <QPropertyAnimation>
#include <QWidget>

/**
 * @brief 构造函数，初始化流式布局私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaFlowLayoutPrivate::ElaFlowLayoutPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放流式布局私有资源
 */
ElaFlowLayoutPrivate::~ElaFlowLayoutPrivate() = default;

/**
 * @brief 执行布局计算
 * @param rect 布局区域
 * @param testOnly 是否仅测试布局（不实际应用）
 * @return 布局高度
 */
int ElaFlowLayoutPrivate::_doLayout(const QRect& rect, bool testOnly) const
{
    Q_Q(const ElaFlowLayout);
    int left, top, right, bottom;
    q->getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    for (QLayoutItem* item : std::as_const(_itemList))
    {
        const QWidget* wid = item->widget();
        int spaceX = q->horizontalSpacing();
        if (spaceX == -1)
        {
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        }
        int spaceY = q->verticalSpacing();
        if (spaceY == -1)
        {
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }
        if (!_lastGeometryMap.contains(item))
        {
            _lastGeometryMap.insert(item, QPoint(x, y));
        }
        if (!testOnly)
        {
            if ((item->geometry().x() == 0 && item->geometry().y() == 0))
            {
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
            }
            else if (item->geometry().x() != x || item->geometry().y() != y)
            {
                if (_isAnimation)
                {
                    // 阻止多重动画
                    if (_lastGeometryMap[item] == QPoint(x, y))
                    {
                        lineHeight = qMax(lineHeight, item->sizeHint().height());
                        return y + lineHeight - rect.y() + bottom;
                    }
                    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(item->widget(), "geometry");
                    geometryAnimation->setStartValue(item->widget()->geometry());
                    geometryAnimation->setEndValue(QRect(QPoint(x, y), item->sizeHint()));
                    geometryAnimation->setDuration(300);
                    geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
                    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                    _lastGeometryMap[item] = QPoint(x, y);
                }
                else
                {
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
                }
            }
        }
        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

/**
 * @brief 智能计算间距
 * @param pm 样式度量值（QStyle::PixelMetric）
 * @return 间距值
 */
int ElaFlowLayoutPrivate::_smartSpacing(QStyle::PixelMetric pm) const
{
    Q_Q(const ElaFlowLayout);
    QObject* parent = q->parent();
    if (!parent)
    {
        return -1;
    }
    else if (parent->isWidgetType())
    {
        QWidget* pw = static_cast<QWidget*>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    else
    {
        return static_cast<QLayout*>(parent)->spacing();
    }
}