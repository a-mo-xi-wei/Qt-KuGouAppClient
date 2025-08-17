/**
 * @file MyFlowLayout.h
 * @brief 定义 MyFlowLayout 类，提供自定义流布局功能
 * @author [WeiWang]
 * @date 2025-05-15
 * @version 1.0
 */

#ifndef MYFLOWLAYOUT_H
#define MYFLOWLAYOUT_H

#include <QLayout>
#include <QStyle>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class MyFlowLayout
 * @brief 自定义流布局类，支持动态调整控件间距和换行
 */
class UIWIDGETS_EXPORT MyFlowLayout : public QLayout {
public:
    /**
     * @brief 构造函数，初始化流布局
 * @param parent 父控件指针，默认为 nullptr
     * @param margin 边距，默认为 -1
     * @param hSpacing 水平间距，默认为 -1
     * @param vSpacing 垂直间距，默认为 -1
     */
    explicit MyFlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    /**
     * @brief 构造函数，初始化流布局（支持首页模式）
     * @param parent 父控件指针，默认为 nullptr
     * @param home 是否为首页模式
     * @param margin 边距，默认为 -1
     * @param hSpacing 水平间距，默认为 -1
     * @param vSpacing 垂直间距，默认为 -1
     */
    explicit MyFlowLayout(QWidget *parent, bool home, int margin = -1, int hSpacing = -1, int vSpacing = -1);

    /**
     * @brief 构造函数，初始化流布局（无父控件）
     * @param margin 边距，默认为 -1
     * @param hSpacing 水平间距，默认为 -1
     * @param vSpacing 垂直间距，默认为 -1
     */
    explicit MyFlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);

    /**
     * @brief 析构函数，清理布局项
     */
    ~MyFlowLayout() override;

    /**
     * @brief 添加布局项
     * @param item 布局项指针
     */
    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;

    /**
     * @brief 获取水平间距
     * @return 水平间距值
     */
    int horizontalSpacing() const;

    /**
     * @brief 获取垂直间距
     * @return 垂直间距值
     */
    int verticalSpacing() const;

    /**
     * @brief 获取扩展方向
     * @return 扩展方向
     */
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    /**
     * @brief 判断是否依赖宽度计算高度
     * @return 是否依赖宽度
     */
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;

    /**
     * @brief 根据宽度计算高度
     * @param width 宽度
     * @return 高度
     */
    int heightForWidth(int width) const Q_DECL_OVERRIDE;

    /**
     * @brief 获取布局项数量
     * @return 布局项数量
     */
    int count() const Q_DECL_OVERRIDE;

    /**
     * @brief 获取指定索引的布局项
     * @param index 索引
     * @return 布局项指针
     */
    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;

    /**
     * @brief 获取指定索引的布局项几何区域
     * @param index 索引
     * @return 几何区域
     */
    QRect itemGeometry(int index) const;

    /**
     * @brief 获取所有布局项的几何区域
     * @return 几何区域列表
     */
    QVector<QRect> itemGeometries() const;

    /**
     * @brief 计算所有布局项的几何区域
     * @param containerSize 容器大小
     * @return 几何区域列表
     */
    QVector<QRect> calculateAllItemRects(const QSize& containerSize) const;

    /**
     * @brief 获取指定索引的控件
     * @param index 索引
     * @return 控件指针
     */
    QWidget* widgetAt(int index) const;

    /**
     * @brief 清空布局
     */
    void clear();

    /**
     * @brief 插入控件到指定索引
     * @param index 索引
     * @param widget 控件指针
     */
    void insertWidget(int index, QWidget* widget);

    /**
     * @brief 获取最小尺寸
     * @return 最小尺寸
     */
    QSize minimumSize() const Q_DECL_OVERRIDE;

    /**
     * @brief 设置布局几何区域
     * @param rect 几何区域
     */
    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;

    /**
     * @brief 获取建议尺寸
     * @return 建议尺寸
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /**
     * @brief 移除指定索引的布局项
     * @param index 索引
     * @return 布局项指针
     */
    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;

    /**
     * @brief 计算水平填充间距
     * @param wid 参考控件指针
     * @return 间距值
     */
    int fillSpaceX(QWidget *wid) const;

private:
    /**
     * @brief 执行布局计算
     * @param rect 布局区域
     * @param testOnly 是否仅测试
     * @return 布局高度
     */
    int doLayout(const QRect &rect, bool testOnly) const;

    /**
     * @brief 获取智能间距
     * @param pm 样式指标
     * @return 间距值
     */
    int smartSpacing(QStyle::PixelMetric pm) const;

private:
    QList<QLayoutItem*> itemList;   ///< 布局项列表
    int                 m_hSpace;   ///< 水平间距
    int                 m_vSpace;   ///< 垂直间距
    bool                m_home;     ///< 是否为首页模式
};

#endif // MYFLOWLAYOUT_H