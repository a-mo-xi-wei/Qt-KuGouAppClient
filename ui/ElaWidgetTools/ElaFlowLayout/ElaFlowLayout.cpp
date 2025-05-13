/**
 * @file ElaFlowLayout.cpp
 * @brief 实现 ElaFlowLayout 类，提供流式布局功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaFlowLayout.h"
#include "ElaFlowLayoutPrivate.h"

#include <QWidget>

/**
 * @brief 构造函数，初始化流式布局（带父控件）
 * @param parent 父控件指针
 * @param margin 布局边距
 * @param hSpacing 水平间距
 * @param vSpacing 垂直间距
 */
ElaFlowLayout::ElaFlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), d_ptr(new ElaFlowLayoutPrivate())
{
    Q_D(ElaFlowLayout);
    d->q_ptr = this;
    d->_hSpacing = hSpacing;
    d->_vSpacing = vSpacing;
    setContentsMargins(margin, margin, margin, margin);
    d->_lastHeightMap.insert(this, 0);
}

/**
 * @brief 构造函数，初始化流式布局（无父控件）
 * @param margin 布局边距
 * @param hSpacing 水平间距
 * @param vSpacing 垂直间距
 */
ElaFlowLayout::ElaFlowLayout(int margin, int hSpacing, int vSpacing)
    : d_ptr(new ElaFlowLayoutPrivate())
{
    Q_D(ElaFlowLayout);
    d->q_ptr = this;
    d->_hSpacing = hSpacing;
    d->_vSpacing = vSpacing;
    setContentsMargins(margin, margin, margin, margin);
    d->_lastHeightMap.insert(this, 0);
}

/**
 * @brief 析构函数，释放流式布局资源
 */
ElaFlowLayout::~ElaFlowLayout()
{
    QLayoutItem* item;
    while ((item = takeAt(0)))
    {
        delete item;
    }
}

/**
 * @brief 添加布局项
 * @param item 布局项指针
 */
void ElaFlowLayout::addItem(QLayoutItem* item)
{
    Q_D(ElaFlowLayout);
    d->_itemList.append(item);
}

/**
 * @brief 获取水平间距
 * @return 水平间距值
 */
int ElaFlowLayout::horizontalSpacing() const
{
    Q_D(const ElaFlowLayout);
    if (d->_hSpacing >= 0)
    {
        return d->_hSpacing;
    }
    else
    {
        return d->_smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

/**
 * @brief 获取垂直间距
 * @return 垂直间距值
 */
int ElaFlowLayout::verticalSpacing() const
{
    Q_D(const ElaFlowLayout);
    if (d->_vSpacing >= 0)
    {
        return d->_vSpacing;
    }
    else
    {
        return d->_smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

/**
 * @brief 获取布局项数量
 * @return 布局项数量
 */
int ElaFlowLayout::count() const
{
    return d_ptr->_itemList.size();
}

/**
 * @brief 获取指定索引的布局项
 * @param index 布局项索引
 * @return 布局项指针
 */
QLayoutItem* ElaFlowLayout::itemAt(int index) const
{
    return d_ptr->_itemList.value(index);
}

/**
 * @brief 移除并返回指定索引的布局项
 * @param index 布局项索引
 * @return 布局项指针，若索引无效则返回 nullptr
 */
QLayoutItem* ElaFlowLayout::takeAt(int index)
{
    Q_D(ElaFlowLayout);
    if (index >= 0 && index < d->_itemList.size())
    {
        return d->_itemList.takeAt(index);
    }
    return nullptr;
}

/**
 * @brief 设置是否启用动画
 * @param isAnimation 是否启用动画
 */
void ElaFlowLayout::setIsAnimation(bool isAnimation)
{
    Q_D(ElaFlowLayout);
    d->_isAnimation = isAnimation;
}

/**
 * @brief 获取扩展方向
 * @return 扩展方向（Qt::Orientations）
 */
Qt::Orientations ElaFlowLayout::expandingDirections() const
{
    return {};
}

/**
 * @brief 检查是否支持高度依赖宽度
 * @return 是否支持高度依赖宽度
 */
bool ElaFlowLayout::hasHeightForWidth() const
{
    return true;
}

/**
 * @brief 根据宽度计算高度
 * @param width 宽度值
 * @return 对应的高度值
 */
int ElaFlowLayout::heightForWidth(int width) const
{
    int height = d_ptr->_doLayout(QRect(0, 0, width, 0), true);
    d_ptr->_lastHeightMap[const_cast<ElaFlowLayout*>(this)] = height;
    return height;
}

/**
 * @brief 设置布局的几何形状
 * @param rect 几何区域
 */
void ElaFlowLayout::setGeometry(const QRect& rect)
{
    Q_D(ElaFlowLayout);
    QLayout::setGeometry(rect);
    d->_doLayout(rect, false);
}

/**
 * @brief 获取布局的推荐尺寸
 * @return 推荐尺寸
 */
QSize ElaFlowLayout::sizeHint() const
{
    return minimumSize();
}

/**
 * @brief 获取布局的最小尺寸
 * @return 最小尺寸
 */
QSize ElaFlowLayout::minimumSize() const
{
    QSize size;
    for (const QLayoutItem* item : std::as_const(d_ptr->_itemList))
    {
        size = size.expandedTo(item->minimumSize());
    }

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}