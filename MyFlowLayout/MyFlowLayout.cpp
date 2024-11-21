#include <QWidget>
#include "MyFlowLayout.h"

MyFlowLayout::MyFlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent),
      m_hSpace(hSpacing),
      m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}

MyFlowLayout::MyFlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing),
      m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}

MyFlowLayout::~MyFlowLayout() {
    QLayoutItem *item;
    while ((item = MyFlowLayout::takeAt(0)))
        delete item;
}

void MyFlowLayout::addItem(QLayoutItem *item) {
    itemList.append(item);
}

int MyFlowLayout::horizontalSpacing() const {
    if (m_hSpace >= 0) {
        return m_hSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int MyFlowLayout::verticalSpacing() const {
    if (m_vSpace >= 0) {
        return m_vSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int MyFlowLayout::count() const {
    return static_cast<int>(itemList.size());
}

QLayoutItem *MyFlowLayout::itemAt(int index) const {
    return itemList.value(index);
}

QLayoutItem *MyFlowLayout::takeAt(int index) {
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations MyFlowLayout::expandingDirections() const {
    return Qt::Orientations::fromInt(0);
}

bool MyFlowLayout::hasHeightForWidth() const {
    return true;
}

int MyFlowLayout::heightForWidth(int width) const {
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void MyFlowLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize MyFlowLayout::sizeHint() const {
    return minimumSize();
}

QSize MyFlowLayout::minimumSize() const {
    QSize size;
    QLayoutItem *item;
    foreach(item, itemList)
        size = size.expandedTo(item->minimumSize());

    // 使用 contentsMargins() 获取边距值
    auto margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

int MyFlowLayout::doLayout(const QRect &rect, bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    auto effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    QLayoutItem *item;
    foreach(item, itemList) {
        QWidget *wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        int spaceY = verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int MyFlowLayout::smartSpacing(QStyle::PixelMetric pm) const {
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        auto pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}
