#ifndef ELAFLOWLAYOUTPRIVATE_H
#define ELAFLOWLAYOUTPRIVATE_H

#include <QLayout>
#include <QMap>
#include <QObject>
#include <QStyle>

class ElaFlowLayout;

class ElaFlowLayoutPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(ElaFlowLayout) // 声明q_func
public:
    explicit ElaFlowLayoutPrivate(ElaFlowLayout *q); // 必须接收公有类指针
    ~ElaFlowLayoutPrivate();

    ElaFlowLayout *q_ptr; // 显式定义反向指针
private:
    int _doLayout(const QRect &rect, bool testOnly) const;

    int _smartSpacing(QStyle::PixelMetric pm) const;

    mutable QMap<ElaFlowLayout *, int> _lastHeightMap;
    mutable QMap<QLayoutItem *, QPoint> _lastGeometryMap;
    QList<QLayoutItem *> _itemList;
    bool _isAnimation{false};
    int _hSpacing;
    int _vSpacing;
};

#endif // ELAFLOWLAYOUTPRIVATE_H
