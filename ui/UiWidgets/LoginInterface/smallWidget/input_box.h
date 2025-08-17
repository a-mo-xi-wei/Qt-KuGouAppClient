#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include "ElaLineEdit.h"
#include "ElaToolTip.h"

#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

class UIWIDGETS_EXPORT Input_box : public ElaLineEdit {
    Q_OBJECT

public:
    explicit Input_box(const QString& icon, QWidget *parent = nullptr);

    void setIcon(const QString &iconPath);

    void openToolTip();

    void setIconToolTip(const QString& tip) const;

private:
    QRect iconRect() const;

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void iconClicked();

private:
    QPixmap icon;
    ElaToolTip* m_passwordTip = nullptr;
};

#endif // INPUT_BOX_H
