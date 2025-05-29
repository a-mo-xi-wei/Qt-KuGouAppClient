/**
 * @file qtmaterialslider_p.h
 * @brief 定义 QtMaterialSliderPrivate 类，滑块控件的私有实现
 * @author laserpants
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef QTMATERIALSLIDER_P_H
#define QTMATERIALSLIDER_P_H

#include <QtGlobal>
#include <QColor>
#include <QRectF>

class QtMaterialSlider;
class QtMaterialSliderThumb;
class QtMaterialSliderTrack;
class QtMaterialSliderStateMachine;

/**
 * @class QtMaterialSliderPrivate
 * @brief 滑块控件的私有实现
 * @note 内部使用
 */
class QtMaterialSliderPrivate
{
    Q_DISABLE_COPY(QtMaterialSliderPrivate)                    ///< 禁用拷贝

    Q_DECLARE_PUBLIC(QtMaterialSlider)                        ///< 声明控件指针

public:
    /**
     * @brief 构造函数
     * @param q 滑块控件指针
     */
    QtMaterialSliderPrivate(QtMaterialSlider *q);

    /**
     * @brief 析构函数
     */
    ~QtMaterialSliderPrivate();

    /**
     * @brief 初始化滑块控件
     */
    void init();

    /**
     * @brief 获取轨道边界矩形
     * @return 轨道边界矩形
     */
    QRectF trackBoundingRect() const;

    /**
     * @brief 获取滑块边界矩形
     * @return 滑块边界矩形
     */
    QRectF thumbBoundingRect() const;

    /**
     * @brief 根据位置计算值
     * @param pos 鼠标位置
     * @return 对应的滑块值
     */
    int valueFromPosition(const QPoint &pos) const;

    /**
     * @brief 设置悬停状态
     * @param status 是否悬停
     */
    void setHovered(bool status);

    QtMaterialSlider *const q_ptr;                            ///< 控件指针
    QtMaterialSliderThumb *thumb;                             ///< 滑块对象
    QtMaterialSliderTrack *track;                             ///< 轨道对象
    QtMaterialSliderStateMachine *stateMachine;               ///< 状态机对象
    QColor                thumbColor;                         ///< 滑块颜色
    QColor                trackColor;                         ///< 轨道颜色
    QColor                disabledColor;                      ///< 禁用状态颜色
    int                   stepTo;                             ///< 步进目标值
    int                   oldValue;                           ///< 上次值
    int                   trackWidth;                         ///< 轨道宽度
    bool                  hoverTrack;                         ///< 轨道悬停状态
    bool                  hoverThumb;                         ///< 滑块悬停状态
    bool                  hover;                              ///< 整体悬停状态
    bool                  step;                               ///< 步进状态
    bool                  pageStepMode;                       ///< 页面步进模式
    bool                  useThemeColors;                     ///< 是否使用主题颜色
};

#endif // QTMATERIALSLIDER_P_H