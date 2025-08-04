/**
 * @file ElaToolTip.cpp
 * @brief 实现 ElaToolTip 类，提供自定义工具提示功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaToolTip.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolTipPrivate.h"

#include <QPainter>
#include <QVBoxLayout>

/**
 * @brief 定义 BorderRadius 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, BorderRadius)

/**
 * @brief 定义 DisplayMsec 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, DisplayMsec)

/**
 * @brief 定义 ShowDelayMsec 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, ShowDelayMsec)

/**
 * @brief 定义 HideDelayMsec 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaToolTip, int, HideDelayMsec)

/**
 * @brief 构造函数，初始化工具提示
 * @param parent 父控件指针，默认为 nullptr
 */
ElaToolTip::ElaToolTip(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaToolTipPrivate())
{
    Q_D(ElaToolTip);
    d->q_ptr           = this;
    d->_pBorderRadius  = 5;
    d->_pDisplayMsec   = 2000;
    d->_pShowDelayMsec = 0;
    d->_pHideDelayMsec = 0;
    d->_pCustomWidget  = nullptr;
    setObjectName("ElaToolTip");
    if (parent)
    {
        parent->installEventFilter(d);
    }

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    d->_toolTipText = new ElaText(this);
    d->_toolTipText->setWordWrap(false);
    d->_toolTipText->setTextPixelSize(12);
    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setContentsMargins(d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2,
                                       d->_shadowBorderWidth * 2);
    d->_mainLayout->addWidget(d->_toolTipText);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [ = ](ElaThemeType::ThemeMode themeMode)
    {
        d->_themeMode = themeMode;
        update();
    });
    hide();
}

/**
 * @brief 析构函数，释放工具提示资源
 */
ElaToolTip::~ElaToolTip() = default;

/**
 * @brief 设置工具提示始终显示
 */
void ElaToolTip::setToolTipShowAlways()
{
    Q_D(ElaToolTip);
    d->_pDisplayMsec = -1;
}

/**
 * @brief 设置工具提示文本
 * @param toolTip 工具提示文本
 */
void ElaToolTip::setToolTip(QString toolTip)
{
    Q_D(ElaToolTip);
    resize(fontMetrics().horizontalAdvance(toolTip), height());
    d->_toolTipText->setText(toolTip);
    Q_EMIT pToolTipChanged();
}

/**
 * @brief 获取工具提示文本
 * @return 工具提示文本
 */
QString ElaToolTip::getToolTip() const
{
    Q_D(const ElaToolTip);
    return d->_toolTipText->text();
}

/**
 * @brief 设置自定义控件
 * @param customWidget 自定义控件指针
 */
void ElaToolTip::setCustomWidget(QWidget* customWidget)
{
    Q_D(ElaToolTip);
    if (!customWidget || customWidget == this)
    {
        return;
    }
    if (d->_pCustomWidget)
    {
        d->_mainLayout->removeWidget(d->_pCustomWidget);
        d->_pCustomWidget->deleteLater();
    }
    d->_toolTipText->hide();
    d->_mainLayout->addWidget(customWidget);
    d->_pCustomWidget = customWidget;
    Q_EMIT pCustomWidgetChanged();
}

/**
 * @brief 获取自定义控件
 * @return 自定义控件指针
 */
QWidget* ElaToolTip::getCustomWidget() const
{
    Q_D(const ElaToolTip);
    return d->_pCustomWidget;
}

/**
 * @brief 更新工具提示位置
 */
void ElaToolTip::updatePos()
{
    Q_D(ElaToolTip);
    d->_updatePos();
}

/**
 * @brief 绘制事件，绘制工具提示外观
 * @param event 绘图事件
 */
void ElaToolTip::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToolTip);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    //阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);
    QRect foregroundRect = rect();
    foregroundRect.adjust(d->_shadowBorderWidth, d->_shadowBorderWidth, -d->_shadowBorderWidth, -d->_shadowBorderWidth);
    painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
    painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
}
