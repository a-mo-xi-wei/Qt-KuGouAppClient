/**
* @file ElaFlowLayoutPrivate.h
 * @brief 定义 ElaFlowLayoutPrivate 类，管理流式布局的私有实现
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAFLOWLAYOUTPRIVATE_H
#define ELAFLOWLAYOUTPRIVATE_H

#include <QLayout>
#include <QMap>
#include <QObject>
#include <QStyle>

#include "stdafx.h"

class ElaFlowLayout;

/**
 * @class ElaFlowLayoutPrivate
 * @brief 流式布局私有实现类，管理布局项、间距和动画
 */
class ElaFlowLayoutPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaFlowLayout)

public:
    /**
     * @brief 构造函数，初始化流式布局私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaFlowLayoutPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放流式布局私有资源
     */
    ~ElaFlowLayoutPrivate();

private:
    /**
     * @brief 执行布局计算
     * @param rect 布局区域
     * @param testOnly 是否仅测试布局（不实际应用）
     * @return 布局高度
     */
    int _doLayout(const QRect& rect, bool testOnly) const;

    /**
     * @brief 智能计算间距
     * @param pm 样式度量值（QStyle::PixelMetric）
     * @return 间距值
     */
    int _smartSpacing(QStyle::PixelMetric pm) const;

    /**
     * @brief 存储布局的最后高度映射
     */
    mutable QMap<ElaFlowLayout*, int> _lastHeightMap;

    /**
     * @brief 存储布局项的最后几何位置映射
     */
    mutable QMap<QLayoutItem*, QPoint> _lastGeometryMap;

    /**
     * @brief 布局项列表
     */
    QList<QLayoutItem*> _itemList;

    /**
     * @brief 是否启用动画
     */
    bool _isAnimation{false};

    /**
     * @brief 水平间距
     */
    int _hSpacing;

    /**
     * @brief 垂直间距
     */
    int _vSpacing;
};

#endif // ELAFLOWLAYOUTPRIVATE_H