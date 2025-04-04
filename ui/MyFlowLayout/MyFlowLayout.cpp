#include <QWidget>
#include "MyFlowLayout.h"

#define DEFAULTSPACE 10
#define DEFAULTHOMESPACE 10

MyFlowLayout::MyFlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent),
      m_hSpace(hSpacing),
      m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}

MyFlowLayout::MyFlowLayout(QWidget *parent, bool home, int margin, int hSpacing, int vSpacing): QLayout(parent),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing),
    m_home(home) {
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
    if (m_hSpace >= 0 || m_hSpace == -1) {
        return m_hSpace;
    }
    return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int MyFlowLayout::verticalSpacing() const {
    if (m_vSpace >= 0 || m_vSpace == -1) {
        return m_vSpace;
    }
    return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int MyFlowLayout::count() const {
    return static_cast<int>(itemList.size());
}

QLayoutItem *MyFlowLayout::itemAt(int index) const {
    return itemList.value(index);
}

QRect MyFlowLayout::itemGeometry(int index) const {
    if(index >= 0 && index < count()) {
        return itemAt(index)->geometry();
    }
    return {};
}

QVector<QRect> MyFlowLayout::itemGeometries() const {
    QVector<QRect> rects;
    for(int i=0; i<count(); ++i) {
        if(QLayoutItem* item = itemAt(i)) {
            rects.append(item->geometry());
        }
    }
    return rects;
}

QVector<QRect> MyFlowLayout::calculateAllItemRects(const QSize& containerSize) const {
    QVector<QRect> rects;
    if (containerSize.width() <= 0) return rects;

    int x = 0;
    int y = 0;
    int lineHeight = 0;
    const int spacing = this->spacing();

    for (int i = 0; i < count(); ++i) {
        QLayoutItem* item = itemAt(i);
        if (!item) continue;

        QSize itemSize = item->sizeHint().expandedTo(item->minimumSize());
        if (x + itemSize.width() > containerSize.width() && x > 0) { // 换行
            y += lineHeight + spacing;
            x = 0;
            lineHeight = 0;
        }

        rects.append(QRect(x, y, itemSize.width(), itemSize.height()));

        x += itemSize.width() + spacing;
        lineHeight = qMax(lineHeight, itemSize.height());
    }

    return rects;
}

QWidget * MyFlowLayout::widgetAt(int index) const {
    if(index >= 0 && index < count()) {
        return itemAt(index)->widget();
    }
    return nullptr;
}

void MyFlowLayout::clear() {
    while(QLayoutItem* item = takeAt(0)) {
        delete item;
    }
}

// MyFlowLayout.cpp
void MyFlowLayout::insertWidget(int index, QWidget* widget) {
    // 参数校验
    if (index < 0 || index > itemList.size()) {
        index = itemList.size();
    }

    // 创建布局项并插入到指定位置
    QLayoutItem* item = new QWidgetItem(widget);
    if (index >= itemList.size()) {
        itemList.append(item);
    } else {
        itemList.insert(index, item);
    }

    // 通知布局需要重新计算
    invalidate();

    // 如果已经设置父控件，需要添加到父控件
    if (QWidget* parent = parentWidget()) {
        widget->setParent(parent);
    }
}

QLayoutItem *MyFlowLayout::takeAt(int index) {
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

int MyFlowLayout::fillSpaceX(QWidget *wid) const {
    int num = 0;
    int x = 0;
    int numH = 0;
    int space = 4;
    if (m_home) {
        space = DEFAULTHOMESPACE;
    }
    int len = this->parentWidget()->width() - this->contentsMargins().left() - this->contentsMargins().right();
    while (true) {
        num++;
        if (num * (wid->width() + space) - space >= len) {
            //最小间距space
            break;
        }
    }

    num = num - 1;
    if (num <= 1) {
        //numH = static_cast<int>(itemList.size());
        return DEFAULTSPACE;
    }
    int height = wid->height();
    numH = ceil(static_cast<double>(itemList.size()) / num);
    x = len + space - num * (wid->width() + space);
    x = ceil(static_cast<double>(x) / (num - 1)) + space;
    x = x - 1; //考虑边框等因素影响

    int maxY = numH * (height + x) + DEFAULTSPACE - x;
    if (m_home) {
        maxY = numH * (height + DEFAULTHOMESPACE) + DEFAULTSPACE;
    }
    this->parentWidget()->setFixedHeight(maxY);
    return x;
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

    int fillX = 0;
    bool bFillX = false;

    QLayoutItem *item;
    foreach(item, itemList) {
        QWidget *wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1) {
            if (!bFillX) {
                bFillX = true;
                fillX = fillSpaceX(wid);
            }
            spaceX = fillX;
        }
        int spaceY = verticalSpacing();
        if (spaceY == -1 && fillX >= 0) {
            spaceY = fillX;
        } else {
            spaceY = wid->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }
        if (m_home) {
            spaceY = DEFAULTSPACE;
        }
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
    }
    // 当父是QWidget=>顶层布局的默认间距为pm样式。
    if (parent->isWidgetType()) {
        auto pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    // 当父为QLayout=>子布局的默认间距由父布局的间距来确定。
    return static_cast<QLayout *>(parent)->spacing();
}
