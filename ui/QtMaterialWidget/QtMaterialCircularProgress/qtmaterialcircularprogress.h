/**
 * @file qtmaterialcircularprogress.h
 * @brief 定义 QtMaterialCircularProgress 类，提供 Material Design 圆形进度条
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALCIRCULARPROGRESS_H
#define QTMATERIALCIRCULARPROGRESS_H

#include <QProgressBar>
#include "qtmaterialtheme.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialCircularProgressPrivate;

/**
 * @class QtMaterialCircularProgress
 * @brief 圆形进度条类，继承自 QProgressBar，支持 Material Design 风格
 */
class QTMATERIALWIDGET_EXPORT QtMaterialCircularProgress : public QProgressBar
{
    Q_OBJECT

    Q_PROPERTY(qreal lineWidth WRITE setLineWidth READ lineWidth) ///< 进度条线宽属性
    Q_PROPERTY(qreal size WRITE setSize READ size) ///< 进度条尺寸属性
    Q_PROPERTY(QColor color WRITE setColor READ color) ///< 进度条颜色属性

public:
    /**
     * @brief 构造函数，初始化圆形进度条
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialCircularProgress(QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialCircularProgress();

    /**
     * @brief 设置进度条类型
     * @param type 进度条类型（Material::ProgressType）
     */
    void setProgressType(Material::ProgressType type);

    /**
     * @brief 获取进度条类型
     * @return 进度条类型
     */
    Material::ProgressType progressType() const;

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
     * @brief 设置进度条线宽
     * @param width 线宽
     */
    void setLineWidth(qreal width);

    /**
     * @brief 获取进度条线宽
     * @return 线宽
     */
    qreal lineWidth() const;

    /**
     * @brief 设置进度条尺寸
     * @param size 尺寸
     */
    void setSize(int size);

    /**
     * @brief 获取进度条尺寸
     * @return 尺寸
     */
    int size() const;

    /**
     * @brief 设置进度条颜色
     * @param color 颜色
     */
    void setColor(const QColor &color);

    /**
     * @brief 获取进度条颜色
     * @return 颜色
     */
    QColor color() const;

    /**
     * @brief 获取推荐尺寸
     * @return 推荐尺寸
     * @note 重写基类方法
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialCircularProgressPrivate> d_ptr; ///< 私有类指针

private:
    Q_DISABLE_COPY(QtMaterialCircularProgress) ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialCircularProgress) ///< 声明私有类
};

#endif // QTMATERIALCIRCULARPROGRESS_H