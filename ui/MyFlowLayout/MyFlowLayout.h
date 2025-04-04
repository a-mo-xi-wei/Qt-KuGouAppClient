#ifndef MYFLOWLAYOUT_H
#define MYFLOWLAYOUT_H

#include <QLayout>
#include <QStyle>

#if defined(MYFLOWLAYOUT_LIBRARY)
#define MYFLOWLAYOUT_EXPORT Q_DECL_EXPORT
#else
#define MYFLOWLAYOUT_EXPORT Q_DECL_IMPORT
#endif

class MYFLOWLAYOUT_EXPORT MyFlowLayout : public QLayout {
public:
    explicit MyFlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    explicit MyFlowLayout(QWidget *parent, bool home, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    explicit MyFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);

    ~MyFlowLayout() override;

    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;

    int horizontalSpacing() const;

    int verticalSpacing() const;

    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    bool hasHeightForWidth() const Q_DECL_OVERRIDE;

    int heightForWidth(int) const Q_DECL_OVERRIDE;

    int count() const Q_DECL_OVERRIDE;

    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;

    //新增两个函数
    QRect itemGeometry(int index) const;

    QVector<QRect> itemGeometries() const;

    // 添加此方法获取所有块的布局信息
    QVector<QRect> calculateAllItemRects(const QSize& containerSize) const;

    QWidget* widgetAt(int index) const;

    void clear();

    void insertWidget(int index, QWidget* widget);

    QSize minimumSize() const Q_DECL_OVERRIDE;

    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;

    QSize sizeHint() const Q_DECL_OVERRIDE;

    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;

    int fillSpaceX(QWidget *wid) const;

private:
    int doLayout(const QRect &rect, bool testOnly) const;

    int smartSpacing(QStyle::PixelMetric pm) const;

private:
    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
    bool m_home = false;
};

#endif // MYFLOWLAYOUT_H
