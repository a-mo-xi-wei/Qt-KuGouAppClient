#ifndef WATERDROP_H
#define WATERDROP_H

#include <QWidget>
class QVariantAnimation;

#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

class MYWINDOW_EXPORT WaterDrop : public QWidget {
    Q_OBJECT

public:
    WaterDrop(QWidget *parent = Q_NULLPTR);

    void move(const QPoint &point);

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void onRadiusChanged(QVariant value);

private:
    QVariantAnimation *m_waterDropAnimation;

    int m_animationRadius;
};
#endif // WATERDROP_H
