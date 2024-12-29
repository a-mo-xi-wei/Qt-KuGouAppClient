#ifndef SIDEHIDELABEL_H
#define SIDEHIDELABEL_H

#include <QLabel>

class SideHideLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(double sideOffset READ getSideOffset WRITE setSideOffset)
public:
    explicit SideHideLabel(QWidget* parent = nullptr);

    static SideHideLabel* copy(const SideHideLabel* other);

    void setPixmap(const QPixmap& pixmap, const double& sideMaxOffset);

    double getMaxOffset() const;

protected:
    void paintEvent(QPaintEvent *e) override;

public:
    void setSideOffset(const double& of);
    double getSideOffset() const;

    //QPixmap getScaledRoundedPixmap(const QPixmap& pixmap) const;

private:
    double sideOffset = 0;
    double startX = 0;
    int radius = 10;
    QPixmap pixmap;
};

#endif // SIDEHIDELABEL_H
