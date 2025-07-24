/**
 * @file ElaMenu.cpp
 * @brief 实现 ElaMenu 类，提供自定义菜单功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaMenu.h"
#include "ElaMenuStyle.h"
#include "ElaMenuPrivate.h"

#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

/**
 * @brief 构造函数，初始化菜单
 * @param parent 父控件指针，默认为 nullptr
 */
ElaMenu::ElaMenu(QWidget* parent)
    : QMenu(parent), d_ptr(new ElaMenuPrivate())
{
    Q_D(ElaMenu);
    d->q_ptr = this;
    auto opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.95);
    setGraphicsEffect(opacityEffect);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("ElaMenu");
    setAttribute(Qt::WA_StyledBackground, true);
    d->_menuStyle = new ElaMenuStyle(style());
    setStyle(d->_menuStyle);
    d->_pAnimationImagePosY = 0;
}

/**
 * @brief 构造函数，初始化带标题的菜单
 * @param title 菜单标题
 * @param parent 父控件指针，默认为 nullptr
 */
ElaMenu::ElaMenu(const QString& title, QWidget* parent)
    : ElaMenu(parent)
{
    setTitle(title);
}

/**
 * @brief 析构函数，释放菜单资源
 */
ElaMenu::~ElaMenu()
{
}

/**
 * @brief 设置菜单透明度
 * @param opacity 透明度值（0.0 到 1.0）
 */
void ElaMenu::setOpacity(qreal opacity)
{
    auto opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(opacity);
    setGraphicsEffect(opacityEffect);
}

/**
 * @brief 设置是否阻止菜单隐藏
 * @param prevent 是否阻止隐藏
 */
void ElaMenu::setPreventHide(bool prevent)
{
    Q_D(ElaMenu);
    d->m_preventHide = prevent;
}

/**
 * @brief 设置菜单项高度
 * @param menuItemHeight 菜单项高度
 */
void ElaMenu::setMenuItemHeight(int menuItemHeight)
{
    Q_D(ElaMenu);
    d->_menuStyle->setMenuItemHeight(menuItemHeight);
}

/**
 * @brief 获取菜单项高度
 * @return 菜单项高度
 */
int ElaMenu::getMenuItemHeight() const
{
    Q_D(const ElaMenu);
    return d->_menuStyle->getMenuItemHeight();
}

void ElaMenu::setMenuItemHoveredBackgroundColor(const QColor &color) {
    Q_D(ElaMenu);
    d->_menuStyle->setMenuItemHoveredBackgroundColor(color);
}

void ElaMenu::setMenuItemHoveredFontColor(const QColor &color) {
    Q_D(ElaMenu);
    d->_menuStyle->setMenuItemHoveredFontColor(color);
}

/**
 * @brief 添加现有菜单
 * @param menu 菜单指针
 * @return 菜单对应的动作
 */
QAction* ElaMenu::addMenu(QMenu* menu)
{
    return QMenu::addMenu(menu);
}

/**
 * @brief 添加新菜单（仅标题）
 * @param title 菜单标题
 * @return 新创建的菜单指针
 */
ElaMenu* ElaMenu::addMenu(const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    QMenu::addAction(menu->menuAction());
    return menu;
}

/**
 * @brief 添加新菜单（带图标）
 * @param icon 菜单图标
 * @param title 菜单标题
 * @return 新创建的菜单指针
 */
ElaMenu* ElaMenu::addMenu(const QIcon& icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setIcon(icon);
    QMenu::addAction(menu->menuAction());
    return menu;
}

/**
 * @brief 添加新菜单（带 Ela 图标）
 * @param icon Ela 图标类型
 * @param title 菜单标题
 * @return 新创建的菜单指针
 */
ElaMenu* ElaMenu::addMenu(ElaIconType::IconName icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    QMenu::addAction(menu->menuAction());
    menu->menuAction()->setProperty("ElaIconType", QChar((unsigned short)icon));
    return menu;
}

/**
 * @brief 添加带 Ela 图标的动作
 * @param icon Ela 图标类型
 * @param text 动作文本
 * @return 新创建的动作指针
 */
QAction* ElaMenu::addElaIconAction(ElaIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    QMenu::addAction(action);
    return action;
}

/**
 * @brief 添加带 Ela 图标和快捷键的动作
 * @param icon Ela 图标类型
 * @param text 动作文本
 * @param shortcut 快捷键
 * @return 新创建的动作指针
 */
QAction* ElaMenu::addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut)
{
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    QMenu::addAction(action);
    return action;
}

/**
 * @brief 检查菜单是否包含子菜单
 * @return 是否包含子菜单
 */
bool ElaMenu::isHasChildMenu() const
{
    QList<QAction*> actionList = this->actions();
    for (auto action : actionList)
    {
        if (action->isSeparator())
        {
            continue;
        }
        if (action->menu())
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief 检查菜单是否包含图标
 * @return 是否包含图标
 */
bool ElaMenu::isHasIcon() const
{
    QList<QAction*> actionList = this->actions();
    for (auto action : actionList)
    {
        if (action->isSeparator())
        {
            continue;
        }
        QMenu* menu = action->menu();
        if (menu && (!menu->icon().isNull() || !menu->property("ElaIconType").toString().isEmpty()))
        {
            return true;
        }
        if (!action->icon().isNull() || !action->property("ElaIconType").toString().isEmpty())
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief 显示事件处理
 * @param event 显示事件
 */
void ElaMenu::showEvent(QShowEvent* event)
{
    Q_EMIT menuShow();
    Q_D(ElaMenu);
    ///< 消除阴影偏移
    move(this->pos().x() - 6, this->pos().y());
    if (!d->_animationPix.isNull())
    {
        d->_animationPix = QPixmap();
    }
    d->_animationPix = this->grab(this->rect());
    QPropertyAnimation* posAnimation = new QPropertyAnimation(d, "pAnimationImagePosY");
    connect(posAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_animationPix = QPixmap();
        update();
    });
    connect(posAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    posAnimation->setEasingCurve(QEasingCurve::OutCubic);
    posAnimation->setDuration(400);
    int targetPosY = height();
    if (targetPosY > 160)
    {
        if (targetPosY < 320)
        {
            targetPosY = 160;
        }
        else
        {
            targetPosY /= 2;
        }
    }

    if (pos().y() + d->_menuStyle->getMenuItemHeight() + 9 >= QCursor::pos().y())
    {
        posAnimation->setStartValue(-targetPosY);
    }
    else
    {
        posAnimation->setStartValue(targetPosY);
    }

    posAnimation->setEndValue(0);
    posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QMenu::showEvent(event);
}

/**
 * @brief 鼠标释放事件处理
 * @param event 鼠标事件
 */
void ElaMenu::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaMenu);
    QAction* action = this->actionAt(event->pos());
    if (action && d->m_preventHide)
    {
        action->activate(QAction::Trigger);
        d->m_preventHide = false; //重置回去
        return;
    }
    QMenu::mouseReleaseEvent(event);
}

/**
 * @brief 重绘事件，绘制菜单动画
 * @param event 绘图事件
 */
void ElaMenu::paintEvent(QPaintEvent* event)
{
    Q_D(ElaMenu);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    if (!d->_animationPix.isNull())
    {
        painter.drawPixmap(QRect(0, d->_pAnimationImagePosY, width(), height()), d->_animationPix);
    }
    else
    {
        QMenu::paintEvent(event);
    }
}