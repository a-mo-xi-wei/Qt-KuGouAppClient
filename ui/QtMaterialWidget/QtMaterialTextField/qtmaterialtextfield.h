/**
 * @file qtmaterialtextfield.h
 * @brief 定义 QtMaterialTextField 类，提供 Material Design 文本输入框
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALTEXTFIELD_H
#define QTMATERIALTEXTFIELD_H

#include <QLineEdit>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialTextFieldPrivate;

/**
 * @class QtMaterialTextField
 * @brief 文本输入框类，继承自 QLineEdit，支持 Material Design 风格
 */
class QTMATERIALWIDGET_EXPORT QtMaterialTextField : public QLineEdit
{
    Q_OBJECT

    Q_PROPERTY(QColor textColor WRITE setTextColor READ textColor)         ///< 文本颜色属性
    Q_PROPERTY(QColor inkColor WRITE setInkColor READ inkColor)           ///< 墨水颜色属性
    Q_PROPERTY(QColor inputLineColor WRITE setInputLineColor READ inputLineColor) ///< 输入线颜色属性

public:
    /**
     * @brief 构造函数，初始化文本输入框
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialTextField(QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialTextField();

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
     * @brief 设置是否显示标签
     * @param value 是否显示标签
     */
    void setShowLabel(bool value);

    /**
     * @brief 获取是否显示标签
     * @return 是否显示标签
     */
    bool hasLabel() const;

    /**
     * @brief 设置标签字体大小
     * @param size 字体大小（点）
     */
    void setLabelFontSize(qreal size);

    /**
     * @brief 获取标签字体大小
     * @return 字体大小（点）
     */
    qreal labelFontSize() const;

    /**
     * @brief 设置标签文本
     * @param label 标签文本
     */
    void setLabel(const QString &label);

    /**
     * @brief 获取标签文本
     * @return 标签文本
     */
    QString label() const;

    /**
     * @brief 设置文本颜色
     * @param color 文本颜色
     */
    void setTextColor(const QColor &color);

    /**
     * @brief 获取文本颜色
     * @return 文本颜色
     */
    QColor textColor() const;

    /**
     * @brief 设置标签颜色
     * @param color 标签颜色
     */
    void setLabelColor(const QColor &color);

    /**
     * @brief 获取标签颜色
     * @return 标签颜色
     */
    QColor labelColor() const;

    /**
     * @brief 设置墨水颜色
     * @param color 墨水颜色
     */
    void setInkColor(const QColor &color);

    /**
     * @brief 获取墨水颜色
     * @return 墨水颜色
     */
    QColor inkColor() const;

    /**
     * @brief 设置输入线颜色
     * @param color 输入线颜色
     */
    void setInputLineColor(const QColor &color);

    /**
     * @brief 获取输入线颜色
     * @return 输入线颜色
     */
    QColor inputLineColor() const;

    /**
     * @brief 设置是否显示输入线
     * @param value 是否显示输入线
     */
    void setShowInputLine(bool value);

    /**
     * @brief 获取是否显示输入线
     * @return 是否显示输入线
     */
    bool hasInputLine() const;

protected:
    /**
     * @brief 私有构造函数，初始化文本输入框
     * @param d 私有类指针
     * @param parent 父控件指针，默认为 nullptr
     */
    QtMaterialTextField(QtMaterialTextFieldPrivate &d, QWidget *parent = 0);

    /**
     * @brief 处理事件
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialTextFieldPrivate> d_ptr; ///< 私有类指针

private:
    Q_DISABLE_COPY(QtMaterialTextField) ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialTextField) ///< 声明私有类
};

#endif // QTMATERIALTEXTFIELD_H