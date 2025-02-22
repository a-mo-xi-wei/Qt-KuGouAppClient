#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QWidget>

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

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    //根据鼠标的设置鼠标样式，用于拉伸
    void setMouseCursor(const int& x, const int& y);

    //判断鼠标的区域，用于拉伸
    int getMouseRegion(const int& x, const int& y) const;

public:
    bool isPress = false;
    QPoint windowsLastPs;
    QPoint mousePs;
    int mouse_press_region = kMousePositionMid;
    QPoint point_offset;
};


#endif //MAINWINDOW_H
