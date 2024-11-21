#ifndef MyFlowLayout_H
#define MyFlowLayout_H

#include <QLayout>
#include <QStyle>

class MyFlowLayout : public QLayout {
public:
    explicit MyFlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    explicit MyFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);

    ~MyFlowLayout();

    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;

    int horizontalSpacing() const;

    int verticalSpacing() const;

    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    bool hasHeightForWidth() const Q_DECL_OVERRIDE;

    int heightForWidth(int) const Q_DECL_OVERRIDE;

    int count() const Q_DECL_OVERRIDE;

    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;

    QSize minimumSize() const Q_DECL_OVERRIDE;

    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;

    QSize sizeHint() const Q_DECL_OVERRIDE;

    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;

private:
    int doLayout(const QRect &rect, bool testOnly) const;

    int smartSpacing(QStyle::PixelMetric pm) const;

private:
    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif // MyFlowLayout_H
