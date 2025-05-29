/**
 * @file qtmaterialslider.h
 * @brief 定义 QtMaterialSlider 类，Material 风格滑块控件
 * @author laserpants
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef QTMATERIALSLIDER_H
#define QTMATERIALSLIDER_H

#include <QAbstractSlider>
#include <QScopedPointer>

/**
 * @def QT_MATERIAL_SLIDER_MARGIN
 * @brief 滑块控件边距，固定为 30 像素
 */
#define QT_MATERIAL_SLIDER_MARGIN 30

/**
 * @def QTMATERIALWIDGET_EXPORT
 * @brief 动态库导出宏，定义库的导出/导入行为
 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QtMaterialSliderPrivate;

/**
 * @class QtMaterialSlider
 * @brief Material 风格滑块控件，支持主题颜色和动画
 */
class QTMATERIALWIDGET_EXPORT QtMaterialSlider : public QAbstractSlider {
    Q_OBJECT

    Q_PROPERTY(QColor thumbColor WRITE setThumbColor READ thumbColor) ///< 滑块颜色属性
    Q_PROPERTY(QColor trackColor WRITE setTrackColor READ trackColor) ///< 轨道颜色属性
    Q_PROPERTY(QColor disabledColor WRITE setDisabledColor READ disabledColor) ///< 禁用状态颜色属性

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialSlider(QWidget *parent = 0);

    /**
     * @brief 析构函数
     */
    ~QtMaterialSlider();

    /**
     * @brief 设置是否使用主题颜色
     * @param value 是否使用主题颜色
     */
    void setUseThemeColors(bool value);

    /**
     * @brief 获取是否使用主题颜色
     * @return 是否使用主题颜色
     */
    bool useThemeColors() const;

    /**
     * @brief 设置滑块颜色
     * @param color 滑块颜色
     */
    void setThumbColor(const QColor &color);

    /**
     * @brief 获取滑块颜色
     * @return 滑块颜色
     */
    QColor thumbColor() const;

    /**
     * @brief 设置轨道颜色
     * @param color 轨道颜色
     */
    void setTrackColor(const QColor &color);

    /**
     * @brief 获取轨道颜色
     * @return 轨道颜色
     */
    QColor trackColor() const;

    /**
     * @brief 设置禁用状态颜色
     * @param color 禁用状态颜色
     */
    void setDisabledColor(const QColor &color);

    /**
     * @brief 获取禁用状态颜色
     * @return 禁用状态颜色
     */
    QColor disabledColor() const;

    /**
     * @brief 设置页面步进模式
     * @param pageStep 是否启用页面步进
     */
    void setPageStepMode(bool pageStep);

    /**
     * @brief 获取页面步进模式
     * @return 是否启用页面步进
     */
    bool pageStepMode() const;

    /**
     * @brief 获取最小尺寸提示
     * @return 最小尺寸
     */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    /**
     * @brief 设置反向外观
     * @param value 是否反向
     */
    void setInvertedAppearance(bool value);

protected:
    /**
     * @brief 滑块变化处理
     * @param change 变化类型
     * @note 处理方向、值变化等
     */
    void sliderChange(SliderChange change) Q_DECL_OVERRIDE;

    /**
     * @brief 鼠标移动事件
     * @param event 鼠标事件
     * @note 处理滑块拖动和悬停
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 启动滑块拖动或步进
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 结束滑块拖动或步进
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 鼠标离开事件
     * @param event 事件
     * @note 更新悬停状态
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 更新滑块偏移
     */
    void updateThumbOffset();

    const QScopedPointer<QtMaterialSliderPrivate> d_ptr; ///< 私有类指针

private:
    Q_DISABLE_COPY(QtMaterialSlider) ///< 禁用拷贝

    Q_DECLARE_PRIVATE(QtMaterialSlider) ///< 声明私有类
};

#endif // QTMATERIALSLIDER_H