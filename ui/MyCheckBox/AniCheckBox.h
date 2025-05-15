/**
 * @file AniCheckBox.h
 * @brief 定义 AniCheckBox 类，提供带动画的复选框基类
 * @author [iwxyi]
 * @date 2025-05-15
 * @version 1.0
 * @note 此处参考开源项目：iwxyi 大神的 [Qt控件] 带动画的 CheckBox，项目地址为：https://github.com/iwxyi/Qt-AniCheckBox，非常实用，有兴趣的可以 star
 */

#ifndef ANICHECKBOX_H
#define ANICHECKBOX_H
/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt控件] 带动画的 CheckBox
 *  项目地址为 ： https://github.com/iwxyi/Qt-AniCheckBox
 *  非常实用，有兴趣的可以star
 */
#include <QCheckBox>
#include <QEasingCurve>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYCHECKBOX_LIBRARY)
#define MYCHECKBOX_EXPORT Q_DECL_EXPORT
#else
#define MYCHECKBOX_EXPORT Q_DECL_IMPORT
#endif

class QPropertyAnimation;

/**
 * @class AniCheckBox
 * @brief 带动画的复选框基类，继承自 QCheckBox
 */
class MYCHECKBOX_EXPORT AniCheckBox : public QCheckBox {
    Q_OBJECT
    Q_PROPERTY(double hover_prog READ getHoverProg WRITE setHoverProg)
    Q_PROPERTY(double part_prog READ getPartProg WRITE setPartProg)
    Q_PROPERTY(double check_prog READ getCheckProg WRITE setCheckProg)

public:
    /**
     * @brief 构造函数，初始化复选框
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AniCheckBox(QWidget *parent = nullptr);

    /**
     * @brief 设置前景颜色
     * @param c 颜色值
     */
    void setForeColor(QColor c);

protected:
    /**
     * @brief 绘制事件，绘制复选框和文字
     * @param e 绘图事件
     */
    void paintEvent(QPaintEvent *e) override;

    /**
     * @brief 鼠标进入事件，触发悬浮动画
     * @param e 进入事件
     */
    void enterEvent(QEnterEvent *e) override;

    /**
     * @brief 鼠标离开事件，触发离开动画
     * @param e 事件
     */
    void leaveEvent(QEvent *e) override;

    /**
     * @brief 判断点击是否在按钮区域
     * @param pos 点击位置
     * @return 是否在按钮区域
     */
    bool hitButton(const QPoint &pos) const override;

    /**
     * @brief 复选框状态变化处理
     * @param state 复选框状态
     */
    virtual void checkStateChanged(const int& state);

    /**
     * @brief 绘制复选框
     * @param painter 绘图对象
     * @param rect 绘制区域
     */
    virtual void drawBox(QPainter &painter, QRectF rect);

    /**
     * @brief 启动动画
     * @param property 属性名称
     * @param begin 起始值
     * @param end 结束值
     * @param duration 动画时长，默认为 500 毫秒
     * @param curve 缓动曲线，默认为 OutQuad
     * @return 动画对象
     */
    QPropertyAnimation *startAnimation(const QByteArray &property, double begin, double end, int duration = 500,
                                       QEasingCurve curve = QEasingCurve::OutQuad);

protected:
    /**
     * @brief 获取悬浮进度
     * @return 悬浮进度值
     */
    double getHoverProg() const;

    /**
     * @brief 设置悬浮进度
     * @param prog 进度值
     */
    void setHoverProg(double prog);

    /**
     * @brief 获取部分选中进度
     * @return 部分选中进度值
     */
    double getPartProg() const;

    /**
     * @brief 设置部分选中进度
     * @param prog 进度值
     */
    void setPartProg(double prog);

    /**
     * @brief 获取选中进度
     * @return 选中进度值
     */
    double getCheckProg() const;

    /**
     * @brief 设置选中进度
     * @param prog 进度值
     */
    void setCheckProg(double prog);

protected:
    int     boxSide = 0;                    ///< 选择框边长，0 为自适应
    QColor  foreColor = QColor(0x2753ff);   ///< 前景颜色
    double  hoverProg = 0;                  ///< 鼠标悬浮进度
    double  partyProg = 0;                  ///< 部分选中进度
    double  checkProg = 0;                  ///< 选中进度
};

#endif // ANICHECKBOX_H