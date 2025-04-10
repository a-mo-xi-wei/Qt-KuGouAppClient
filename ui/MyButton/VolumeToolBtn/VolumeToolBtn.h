#ifndef VOLUMETOOLBTN_H
#define VOLUMETOOLBTN_H

#include"./SliderWidget/SliderWidget.h"

#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

#include <QToolButton>
#include <QLabel>

class QTimer;
class QVBoxLayout;

class MYBUTTON_EXPORT VolumeToolBtn : public QToolButton {
    Q_OBJECT

public:
    explicit VolumeToolBtn(QWidget *parent = nullptr);

    void initVolumeWidget();

    void initUi();

    void checkMousePosition() const;

    void getVolumeWidgetPosition();

    int getVolumeValue() const;

    void updateIcon(bool isHovered = false);

    void setVolume(const int& value) const;

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void showEvent(QShowEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void onNoVolume(bool flag);

signals:
    void volumeChange(int value);

private:
    bool m_isNoVolume = false;
    std::unique_ptr<MyWidget> m_volumeWidget;
    std::unique_ptr<QLabel> m_volumeLab;
    std::unique_ptr<SliderWidget> m_volumeSlider;
    QVBoxLayout *m_vBoxLayout{};
    QPoint m_volumePosition;
    QWidget *m_volumeParent{};
    QTimer *m_leaveTimer{};
    QTimer *m_positionCheckTimer; // 检查鼠标位置是否在m_volumeWidget上面
};

#endif //VOLUMETOOLBTN_H
