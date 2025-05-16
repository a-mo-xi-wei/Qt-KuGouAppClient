/**
 * @file qtmaterialsnackbar.h
 * @brief 定义 QtMaterialSnackbar 类，提供 Material Design 提示条
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALSNACKBAR_H
#define QTMATERIALSNACKBAR_H

#include "qtmaterialoverlaywidget.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialSnackbarPrivate;

/**
 * @class QtMaterialSnackbar
 * @brief 提示条类，继承自 QtMaterialOverlayWidget，支持 Material Design 风格
 */
class QTMATERIALWIDGET_EXPORT QtMaterialSnackbar : public QtMaterialOverlayWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化提示条
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialSnackbar(QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialSnackbar();

    /**
     * @brief 设置自动隐藏持续时间
     * @param duration 持续时间（毫秒）
     */
    void setAutoHideDuration(int duration);

    /**
     * @brief 获取自动隐藏持续时间
     * @return 持续时间（毫秒）
     */
    int autoHideDuration() const;

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
     * @brief 设置背景颜色
     * @param color 背景颜色
     */
    void setBackgroundColor(const QColor &color);

    /**
     * @brief 获取背景颜色
     * @return 背景颜色
     */
    QColor backgroundColor() const;

    /**
     * @brief 设置背景透明度
     * @param opacity 透明度值
     */
    void setBackgroundOpacity(qreal opacity);

    /**
     * @brief 获取背景透明度
     * @return 透明度值
     */
    qreal backgroundOpacity() const;

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
     * @brief 设置字体大小
     * @param size 字体大小（点）
     */
    void setFontSize(qreal size);

    /**
     * @brief 获取字体大小
     * @return 字体大小（点）
     */
    qreal fontSize() const;

    /**
     * @brief 设置提示条宽度
     * @param width 宽度（像素）
     */
    void setBoxWidth(int width);

    /**
     * @brief 获取提示条宽度
     * @return 宽度（像素）
     */
    int boxWidth() const;

    /**
     * @brief 设置点击隐藏模式
     * @param value 是否启用点击隐藏
     */
    void setClickToDismissMode(bool value);

    /**
     * @brief 获取点击隐藏模式
     * @return 是否启用点击隐藏
     */
    bool clickToDismissMode() const;

public slots:
    /**
     * @brief 添加消息到队列
     * @param message 消息内容
     */
    void addMessage(const QString &message);

    /**
     * @brief 立即添加消息
     * @param message 消息内容
     */
    void addInstantMessage(const QString &message);

protected slots:
    /**
     * @brief 移除并处理队列中的消息
     */
    void dequeue();

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialSnackbarPrivate> d_ptr; ///< 私有类指针

private:
    Q_DISABLE_COPY(QtMaterialSnackbar) ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialSnackbar) ///< 声明私有类
};

#endif // QTMATERIALSNACKBAR_H