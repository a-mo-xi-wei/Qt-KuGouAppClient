/**
 * @file MainWindow.h
 * @brief 定义 MainWindow 类，提供主窗口功能和交互
 * @author WeiWang
 * @date 2025-03-13
 * @version 1.0
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AboutDialog.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif


/**
 * @brief 窗口区域划分命名空间
 */
namespace Area {
    /* 这里我们将一个窗口划分为9个区域，分别为
     左上角（1, 1）、中上（1，2）、右上角（1, 3）
     左中  （2, 1）、 中间（2, 2）、右中  （2, 3）
     左下角（3, 1）、中下（3，2）、 右下角（3, 3）*/

    /**
     * @brief 窗口边缘偏移量
     */
    constexpr int OFFSET = 10;

    /**
     * @brief 鼠标区域左边界
     */
    constexpr int kMouseRegionLeft = OFFSET;

    /**
     * @brief 鼠标区域上边界
     */
    constexpr int kMouseRegionTop = OFFSET;

    /**
     * @brief 鼠标区域右边界
     */
    constexpr int kMouseRegionRight = OFFSET;

    /**
     * @brief 鼠标区域下边界
     */
    constexpr int kMouseRegionBottom = OFFSET;
}

/**
 * @brief 鼠标位置区域枚举
 */
enum MousePosition {
    kMousePositionLeftTop = 11,    ///< 左上角 (1,1)
    kMousePositionTop = 12,        ///< 中上 (1,2)
    kMousePositionRightTop = 13,   ///< 右上角 (1,3)
    kMousePositionLeft = 21,       ///< 左中 (2,1)
    kMousePositionMid = 22,        ///< 中间 (2,2)
    kMousePositionRight = 23,      ///< 右中 (2,3)
    kMousePositionLeftBottom = 31, ///< 左下角 (3,1)
    kMousePositionBottom = 32,     ///< 中下 (3,2)
    kMousePositionRightBottom = 33 ///< 右下角 (3,3)
};

/**
 * @class MainWindow
 * @brief 自定义主窗口类，继承自 QWidget，支持窗口拉伸、托盘交互和关闭动画
 */
class MYWINDOW_EXPORT MainWindow : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化主窗口
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    /**
     * @brief 重写绘制事件，绘制窗口阴影
     * @param event 绘制事件
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 重写窗口大小调整事件
     * @param event 大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 重写鼠标按下事件，处理窗口拖动和拉伸
     * @param ev 鼠标事件
     */
    void mousePressEvent(QMouseEvent *ev) override;

    /**
     * @brief 重写鼠标释放事件，结束拖动或拉伸
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 重写鼠标移动事件，处理窗口拖动和拉伸
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 重写显示事件，执行开场动画
     * @param event 显示事件
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief 重写关闭事件，执行渐变关闭动画
     * @param event 关闭事件
     */
    void closeEvent(QCloseEvent *event) override;

    /**
     * @brief 设置控件的工具提示
     * @param widget 目标控件
     * @param tooltip 提示内容
     */
    static void setElaToolTip(QWidget *widget, const QString &tooltip);

private:
    /**
     * @brief 设置鼠标光标形状
     * @param x 鼠标 X 坐标
     * @param y 鼠标 Y 坐标
     */
    void setMouseCursor(const int &x, const int &y);

    /**
     * @brief 获取鼠标所在区域
     * @param x 鼠标 X 坐标
     * @param y 鼠标 Y 坐标
     * @return 鼠标区域编号
     */
    int getMouseRegion(const int &x, const int &y) const;

public slots:
    /**
     * @brief 显示或隐藏关于对话框
     * @param flag 是否显示
     */
    void onShowAboutDialog(bool flag = true);

public:
    bool isPress = false;                        ///< 鼠标按下标志
    QPoint windowsLastPs;                        ///< 窗口上次位置
    QPoint mousePs;                              ///< 鼠标按下位置
    int mouse_press_region = kMousePositionMid;  ///< 鼠标按下区域
    QPoint point_offset;                         ///< 鼠标移动偏移
    std::unique_ptr<AboutDialog> m_aboutDialog{};///< 关于对话框
    bool m_showDialog = false;                   ///< 关于对话框显示标志
};

#endif // MAINWINDOW_H