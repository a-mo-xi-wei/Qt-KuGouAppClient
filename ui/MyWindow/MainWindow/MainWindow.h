#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QWidget>

#include "AboutDialog.h"

#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

class MyTrayIcon;

namespace Area {
    /* 这里我们将一个窗口划分为9个区域，分别为
     左上角（1, 1）、中上（1，2）、右上角（1, 3）
     左中  （2, 1）、 中间（2, 2）、右中  （2, 3）
     左下角（3, 1）、中下（3，2）、 右下角（3, 3）*/
    constexpr int OFFSET = 10;

    constexpr int kMouseRegionLeft = OFFSET;

    constexpr int kMouseRegionTop = OFFSET;

    constexpr int kMouseRegionRight = OFFSET;

    constexpr int kMouseRegionBottom = OFFSET;
}

// 鼠标的 活动范围的 枚举
enum MousePosition {
    // 这个是上面图片划分的区域 1,1 区 就用 11 代表 , 1,2 就用12 代表 以此类推
    kMousePositionLeftTop = 11,

    kMousePositionTop = 12,

    kMousePositionRightTop = 13,

    kMousePositionLeft = 21,

    kMousePositionMid = 22,

    kMousePositionRight = 23,

    kMousePositionLeftBottom = 31,

    kMousePositionBottom = 32,

    kMousePositionRightBottom = 33,
};

class MYWINDOW_EXPORT MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    static void setElaToolTip(QWidget* widget, const QString& tooltip);

private:
    //根据鼠标的设置鼠标样式，用于拉伸
    void setMouseCursor(const int& x, const int& y);

    //判断鼠标的区域，用于拉伸
    int getMouseRegion(const int& x, const int& y) const;

signals:
    void fromTray_noVolume(const bool& flag);

    void exit();

public slots:
    void onShowAboutDialog(bool flag = true);

public:
    MyTrayIcon* m_trayIcon{};//桌面图标
    bool isPress = false;
    QPoint windowsLastPs;
    QPoint mousePs;
    int mouse_press_region = kMousePositionMid;
    QPoint point_offset;
    //关于我的酷狗dialog
    std::unique_ptr<AboutDialog> m_aboutDialog{};
};


#endif //MAINWINDOW_H
