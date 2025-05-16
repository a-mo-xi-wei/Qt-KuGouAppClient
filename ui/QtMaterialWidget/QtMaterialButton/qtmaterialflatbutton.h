/**
 * @file qtmaterialflatbutton.h
 * @brief 定义 QtMaterialFlatButton 类，提供 Material Design 扁平按钮
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALFLATBUTTON_H
#define QTMATERIALFLATBUTTON_H

#include <QPushButton>
#include <QScopedPointer>
#include "qtmaterialtheme.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialFlatButtonPrivate;

/**
 * @class QtMaterialFlatButton
 * @brief 扁平按钮类，继承自 QPushButton，支持 Material Design 风格
 */
class QTMATERIALWIDGET_EXPORT QtMaterialFlatButton : public QPushButton {
    Q_OBJECT

    Q_PROPERTY(QColor foregroundColor WRITE setForegroundColor READ foregroundColor) ///< 前景色属性
    Q_PROPERTY(QColor backgroundColor WRITE setBackgroundColor READ backgroundColor) ///< 背景色属性
    Q_PROPERTY(QColor overlayColor WRITE setOverlayColor READ overlayColor)         ///< 覆盖层颜色属性
    Q_PROPERTY(QColor disabledForegroundColor WRITE setDisabledForegroundColor READ disabledForegroundColor) ///< 禁用前景色属性
    Q_PROPERTY(QColor disabledBackgroundColor WRITE setDisabledBackgroundColor READ disabledBackgroundColor) ///< 禁用背景色属性
    Q_PROPERTY(qreal fontSize WRITE setFontSize READ fontSize)                      ///< 字体大小属性

public:
    /**
     * @brief 构造函数，初始化扁平按钮
     * @param parent 父控件指针，默认为 nullptr
     * @param preset 按钮预设样式
     */
    explicit QtMaterialFlatButton(QWidget *parent = 0, Material::ButtonPreset preset = Material::FlatPreset);

    /**
     * @brief 构造函数，初始化扁平按钮
     * @param text 按钮文本
     * @param parent 父控件指针，默认为 nullptr
     * @param preset 按钮预设样式
     */
    explicit QtMaterialFlatButton(const QString &text, QWidget *parent = 0,
                                  Material::ButtonPreset preset = Material::FlatPreset);

    /**
     * @brief 构造函数，初始化扁平按钮
     * @param text 按钮文本
     * @param role 按钮角色
     * @param parent 父控件指针，默认为 nullptr
     * @param preset 按钮预设样式
     */
    QtMaterialFlatButton(const QString &text, Material::Role role, QWidget *parent = 0,
                         Material::ButtonPreset preset = Material::FlatPreset);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialFlatButton() override;

    /**
     * @brief 应用预设样式
     * @param preset 按钮预设样式
     */
    void applyPreset(Material::ButtonPreset preset);

    /**
     * @brief 设置是否使用主题颜色
     * @param value 是否使用主题颜色
     */
    void setUseThemeColors(bool value);

    /**
     * @brief 检查是否使用主题颜色
     * @return 是否使用主题颜色
     */
    bool useThemeColors() const;

    /**
     * @brief 设置按钮角色
     * @param role 按钮角色
     */
    void setRole(Material::Role role);

    /**
     * @brief 获取按钮角色
     * @return 按钮角色
     */
    Material::Role role() const;

    /**
     * @brief 设置前景色
     * @param color 前景色
     */
    void setForegroundColor(const QColor &color);

    /**
     * @brief 获取前景色
     * @return 前景色
     */
    QColor foregroundColor() const;

    /**
     * @brief 设置背景色
     * @param color 背景色
     */
    void setBackgroundColor(const QColor &color);

    /**
     * @brief 获取背景色
     * @return 背景色
     */
    QColor backgroundColor() const;

    /**
     * @brief 设置覆盖层颜色
     * @param color 覆盖层颜色
     */
    void setOverlayColor(const QColor &color);

    /**
     * @brief 获取覆盖层颜色
     * @return 覆盖层颜色
     */
    QColor overlayColor() const;

    /**
     * @brief 设置禁用状态前景色
     * @param color 禁用状态前景色
     */
    void setDisabledForegroundColor(const QColor &color);

    /**
     * @brief 获取禁用状态前景色
     * @return 禁用状态前景色
     */
    QColor disabledForegroundColor() const;

    /**
     * @brief 设置禁用状态背景色
     * @param color 禁用状态背景色
     */
    void setDisabledBackgroundColor(const QColor &color);

    /**
     * @brief 获取禁用状态背景色
     * @return 禁用状态背景色
     */
    QColor disabledBackgroundColor() const;

    /**
     * @brief 设置字体大小
     * @param size 字体大小
     */
    void setFontSize(qreal size);

    /**
     * @brief 获取字体大小
     * @return 字体大小
     */
    qreal fontSize() const;

    /**
     * @brief 设置光晕是否可见
     * @param visible 是否显示光晕
     */
    void setHaloVisible(bool visible);

    /**
     * @brief 检查光晕是否可见
     * @return 是否显示光晕
     */
    bool isHaloVisible() const;

    /**
     * @brief 设置覆盖层样式
     * @param style 覆盖层样式
     */
    void setOverlayStyle(Material::OverlayStyle style);

    /**
     * @brief 获取覆盖层样式
     * @return 覆盖层样式
     */
    Material::OverlayStyle overlayStyle() const;

    /**
     * @brief 设置水波纹样式
     * @param style 水波纹样式
     */
    void setRippleStyle(Material::RippleStyle style);

    /**
     * @brief 获取水波纹样式
     * @return 水波纹样式
     */
    Material::RippleStyle rippleStyle() const;

    /**
     * @brief 设置图标位置
     * @param placement 图标位置
     */
    void setIconPlacement(Material::ButtonIconPlacement placement);

    /**
     * @brief 获取图标位置
     * @return 图标位置
     */
    Material::ButtonIconPlacement iconPlacement() const;

    /**
     * @brief 设置圆角半径
     * @param radius 圆角半径
     */
    void setCornerRadius(qreal radius);

    /**
     * @brief 获取圆角半径
     * @return 圆角半径
     */
    qreal cornerRadius() const;

    /**
     * @brief 设置背景模式
     * @param mode 背景模式
     */
    void setBackgroundMode(Qt::BGMode mode);

    /**
     * @brief 获取背景模式
     * @return 背景模式
     */
    Qt::BGMode backgroundMode() const;

    /**
     * @brief 设置基础透明度
     * @param opacity 基础透明度
     */
    void setBaseOpacity(qreal opacity);

    /**
     * @brief 获取基础透明度
     * @return 基础透明度
     */
    qreal baseOpacity() const;

    /**
     * @brief 设置是否可选中
     * @param value 是否可选中
     */
    void setCheckable(bool value);

    /**
     * @brief 设置是否使用固定水波纹半径
     * @param value 是否固定
     */
    void setHasFixedRippleRadius(bool value);

    /**
     * @brief 检查是否使用固定水波纹半径
     * @return 是否固定
     */
    bool hasFixedRippleRadius() const;

    /**
     * @brief 设置固定水波纹半径
     * @param radius 水波纹半径
     */
    void setFixedRippleRadius(qreal radius);

    /**
     * @brief 设置文本对齐方式
     * @param alignment 文本对齐方式
     */
    void setTextAlignment(Qt::Alignment alignment);

    /**
     * @brief 获取文本对齐方式
     * @return 文本对齐方式
     */
    Qt::Alignment textAlignment() const;

    /**
     * @brief 获取推荐尺寸
     * @return 按钮尺寸
     * @note 重写基类方法
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    /** @brief 图标间距常量 */
    enum {
        IconPadding = 12
    };

    /**
     * @brief 构造函数，初始化扁平按钮（私有）
     * @param d 私有类引用
     * @param parent 父控件指针，默认为 nullptr
     * @param preset 按钮预设样式
     * @note 内部实现
     */
    QtMaterialFlatButton(QtMaterialFlatButtonPrivate &d, QWidget *parent = 0, Material::ButtonPreset preset = Material::FlatPreset);

    /**
     * @brief 检查选中状态
     * @note 重写基类方法
     */
    void checkStateSet() Q_DECL_OVERRIDE;

    /**
     * @brief 处理鼠标按下事件
     * @param event 鼠标事件
     * @note 重写基类方法
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 处理鼠标释放事件
     * @param event 鼠标事件
     * @note 重写基类方法
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 处理调整大小事件
     * @param event 调整大小事件
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 绘制按钮
     * @param event 绘制事件
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 绘制背景
     * @param painter 画笔对象
     */
    virtual void paintBackground(QPainter *painter);

    /**
     * @brief 绘制光晕
     * @param painter 画笔对象
     */
    virtual void paintHalo(QPainter *painter);

    /**
     * @brief 绘制前景
     * @param painter 画笔对象
     */
    virtual void paintForeground(QPainter *painter);

    /**
     * @brief 更新剪裁路径
     */
    virtual void updateClipPath();

    const QScopedPointer<QtMaterialFlatButtonPrivate> d_ptr; ///< 私有实现指针

private:
    Q_DISABLE_COPY(QtMaterialFlatButton) ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialFlatButton) ///< 声明私有类
};

#endif // QTMATERIALFLATBUTTON_H