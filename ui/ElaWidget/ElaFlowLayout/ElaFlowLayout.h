#ifndef ELAFLOWLAYOUT_H
#define ELAFLOWLAYOUT_H

#include <QLayout>
#include <QMap>
#include <QStyle>

#if defined(ELAFLOWLAYOUT_LIBRARY)
#define ELAFLOWLAYOUT_EXPORT Q_DECL_EXPORT
#else
#define ELAFLOWLAYOUT_EXPORT Q_DECL_IMPORT
#endif

class ElaFlowLayoutPrivate;

class ELAFLOWLAYOUT_EXPORT ElaFlowLayout : public QLayout {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ElaFlowLayout) // 声明d_ptr和d_func()
public:
    explicit ElaFlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    explicit ElaFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);

    ~ElaFlowLayout();

    void addItem(QLayoutItem *item) override;

    int horizontalSpacing() const;

    int verticalSpacing() const;

    Qt::Orientations expandingDirections() const override;

    bool hasHeightForWidth() const override;

    int heightForWidth(int) const override;

    int count() const override;

    QLayoutItem *itemAt(int index) const override;

    QSize minimumSize() const override;

    void setGeometry(const QRect &rect) override;

    QSize sizeHint() const override;

    QLayoutItem *takeAt(int index) override;

    void setIsAnimation(bool isAnimation);

protected:
    QScopedPointer<ElaFlowLayoutPrivate> d_ptr; // 声明私有类指针
};

#endif // ELAFLOWLAYOUT_H
