/**
 * @file InteractiveButtonBase.h
 * @brief 定义 InteractiveButtonBase 类，提供交互式按钮基类
 * @author [iwxyi]
 * @date 2025-05-15
 * @version 1.0
 * @note 参考开源项目：iwxyi 的 Qt-InteractiveButtons，地址：https://github.com/iwxyi/Qt-InteractiveButtons
 */

#ifndef INTERACTIVEBUTTONBASE_H
#define INTERACTIVEBUTTONBASE_H

/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt] 简单易用的轮播图
 *  项目地址为 ： https://github.com/iwxyi/Qt-InteractiveButtons
 *  非常实用，感兴趣的可以star
 */

#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

#include <QPushButton>

/**
 * @class InteractiveButtonBase
 * @brief 交互式按钮基类，支持水波纹、抖动和前景动画效果
 */
class UIWIDGETS_EXPORT InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool self_enabled READ getSelfEnabled WRITE setSelfEnabled) ///< 是否启用自定义的按钮（true）
    Q_PROPERTY(bool parent_enabled READ getParentEnabled WRITE setParentEnabled)
    ///< 是否启用父类按钮（false）
    Q_PROPERTY(bool fore_enabled READ getForeEnabled WRITE setForeEnabled) ///< 是否绘制自定义按钮前景色（true）
    Q_PROPERTY(QString text READ getText WRITE setText) ///< 前景文字
    Q_PROPERTY(QString icon_path READ getIconPath WRITE setIconPath) ///< 前景图标
    Q_PROPERTY(QString pixmap_path READ getPixmapPath WRITE setPixmapPath) ///< 前景图标
    Q_PROPERTY(QColor icon_color READ getIconColor WRITE setIconColor) ///< 前景图标帅色
    Q_PROPERTY(QColor text_color READ getTextColor WRITE setTextColor) ///< 前景文字颜色
    Q_PROPERTY(QColor background_color READ getNormalColor WRITE setNormalColor) ///< 背景颜色
    Q_PROPERTY(QColor border_color READ getBorderColor WRITE setBorderColor) ///< 边界颜色
    Q_PROPERTY(QColor hover_color READ getHoverColor WRITE setHoverColor) ///< 鼠标悬浮背景颜色
    Q_PROPERTY(QColor press_color READ getPressColor WRITE setPressColor) ///< 鼠标按下背景颜色
    Q_PROPERTY(int hover_duration READ getHoverAniDuration WRITE setHoverAniDuration) ///< 鼠标悬浮动画周期
    Q_PROPERTY(int press_duration READ getPressAniDuration WRITE setPressAniDuration) ///< 鼠标按下动画周期
    Q_PROPERTY(int click_duration READ getClickAniDuration WRITE setClickAniDuration) ///< 鼠标点击动画周期
    Q_PROPERTY(double icon_padding_proper READ getIconPaddingProper WRITE setIconPaddingProper)
    ///< 图标四边空白处大小比例
    Q_PROPERTY(int radius READ getRadius WRITE setRadius)                      ///< 边框圆角半径
    Q_PROPERTY(int border_width READ getBorderWidth WRITE setBorderWidth)      ///< 边框线条粗细
    Q_PROPERTY(bool fixed_fore_pos READ getFixedTextPos WRITE setFixedTextPos) ///< 是否固定前景位置（false）
    Q_PROPERTY(bool text_dynamic_size READ getTextDynamicSize WRITE setTextDynamicSize)
    ///< 修改字体大小时调整按钮最小尺寸（false）
    Q_PROPERTY(bool leave_after_clicked READ getLeaveAfterClick WRITE setLeaveAfterClick)
    ///< 鼠标单击松开后取消悬浮效果（针对菜单、弹窗）
    Q_PROPERTY(bool show_animation READ getShowAni WRITE setShowAni) ///< 是否启用出现动画（鼠标移开则消失）（false）
    Q_PROPERTY(bool water_animation READ getWaterRipple WRITE setWaterRipple)
    ///< 是否启用点击水波纹动画（否则使用渐变）（true）
    Q_PROPERTY(int font_size READ getFontSizeT WRITE setFontSizeT) ///< 动：按钮字体动画效果（自动，不应该设置）
public:
    /**
     * @brief 构造函数，初始化交互式按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit InteractiveButtonBase(QWidget *parent = nullptr);

    /**
     * @brief 构造函数，初始化带文本的交互式按钮
     * @param text 按钮文本
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit InteractiveButtonBase(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief 构造函数，初始化带图标的交互式按钮
     * @param icon 按钮图标
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit InteractiveButtonBase(const QIcon &icon, QWidget *parent = nullptr);

    /**
     * @brief 构造函数，初始化带可变色图标的交互式按钮
     * @param pixmap 按钮图标
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit InteractiveButtonBase(const QPixmap &pixmap, QWidget *parent = nullptr);

    /**
     * @brief 构造函数，初始化带图标和文本的交互式按钮
     * @param icon 按钮图标
     * @param text 按钮文本
     * @param parent 父控件指针，默认为 nullptr
     */
    InteractiveButtonBase(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    /**
     * @brief 构造函数，初始化带可变色图标和文本的交互式按钮
     * @param pixmap 按钮图标
     * @param text 按钮文本
     * @param parent 父控件指针，默认为 nullptr
     */
    InteractiveButtonBase(const QPixmap &pixmap, const QString &text, QWidget *parent = nullptr);

    /**
     * @enum PaintModel
     * @brief 前景绘制模式
     */
    enum PaintModel
    {
        None,       ///< 无前景，仅绘制背景
        Text,       ///< 纯文字
        Icon,       ///< 纯图标
        PixmapMask, ///< 可变色图标（通过 pixmap 和遮罩实现）
        IconText,   ///< 图标+文字（左对齐）
        PixmapText  ///< 可变色图标+文字（左对齐）
    };

    /**
     * @struct PaintAddin
     * @brief 前景额外图标配置
     */
    struct PaintAddin
    {
        /**
         * @brief 默认构造函数
         */
        PaintAddin()
            : enable(false) {}

        /**
         * @brief 带参数构造函数
         * @param p 图标
         * @param a 对齐方式
         * @param s 图标尺寸
         */
        PaintAddin(QPixmap p, Qt::Alignment a, QSize s)
            : enable(true), pixmap(p), align(a), size(s) {}

        bool enable;         ///< 是否启用额外图标
        QPixmap pixmap;      ///< 可变色图标
        Qt::Alignment align; ///< 对齐方式
        QSize size;          ///< 固定大小
    };

    /**
     * @struct Jitter
     * @brief 鼠标松开时的抖动动画
     */
    struct Jitter
    {
        /**
         * @brief 构造函数
         * @param p 目标坐标
         * @param t 时间戳
         */
        Jitter(QPoint p, qint64 t)
            : point(p), timestamp(t) {}

        QPoint point;     ///< 目标坐标
        qint64 timestamp; ///< 运动到目标坐标的时间戳
    };

    /**
     * @struct Water
     * @brief 鼠标按下/松开的水波纹动画
     */
    struct Water
    {
        /**
         * @brief 构造函数
         * @param p 水波纹中心点
         * @param t 按下时间戳
         */
        Water(QPoint p, qint64 t)
            : point(p), progress(0), press_timestamp(t),
              release_timestamp(0), finish_timestamp(0), finished(false) {}

        QPoint point;             ///< 水波纹中心点
        int progress;             ///< 水波纹进度（已弃用，使用时间戳）
        qint64 press_timestamp;   ///< 按下时间戳
        qint64 release_timestamp; ///< 松开时间戳
        qint64 finish_timestamp;  ///< 结束时间戳
        bool finished;            ///< 是否结束
    };

    /**
     * @struct EdgeVal
     * @brief 四周边界 padding 配置
     */
    struct EdgeVal
    {
        /**
         * @brief 默认构造函数
         */
        EdgeVal() {}

        /**
         * @brief 带参数构造函数
         * @param l 左边距
         * @param t 上边距
         * @param r 右边距
         * @param b 下边距
         */
        EdgeVal(int l, int t, int r, int b)
            : left(l), top(t), right(r), bottom(b) {}

        int left;   ///< 左边距
        int top;    ///< 上边距
        int right;  ///< 右边距
        int bottom; ///< 下边距
    };

    /**
     * @enum NolinearType
     * @brief 非线性动画类型
     */
    enum NolinearType
    {
        Linear,         ///< 线性动画
        SlowFaster,     ///< 慢-快动画
        FastSlower,     ///< 快-慢动画
        SlowFastSlower, ///< 慢-快-慢动画
        SpringBack20,   ///< 20% 回弹动画
        SpringBack50    ///< 50% 回弹动画
    };

    /**
     * @brief 设置按钮文本
     * @param text 按钮文本
     */
    virtual void setText(const QString &text);

    /**
     * @brief 设置图标路径
     * @param path 图标路径
     */
    virtual void setIconPath(const QString &path);

    /**
     * @brief 设置图标
     * @param icon 图标
     */
    virtual void setIcon(const QIcon &icon);

    /**
     * @brief 设置可变色图标路径
     * @param path 图标路径
     */
    virtual void setPixmapPath(const QString &path);

    /**
     * @brief 设置可变色图标
     * @param pixmap 图标
     */
    virtual void setPixmap(const QPixmap &pixmap);

    /**
     * @brief 设置额外图标（如角标）
     * @param pixmap 图标
     * @param align 对齐方式，默认为 Qt::AlignRight
     * @param size 图标尺寸，默认为 QSize(0, 0)
     */
    void setPaintAddIn(QPixmap pixmap,
                       Qt::Alignment align = Qt::AlignRight,
                       const QSize &size = QSize(0, 0));

    /**
     * @brief 设置子类功能是否启用
     * @param e 是否启用，默认为 true
     */
    void setSelfEnabled(const bool &e = true);

    /**
     * @brief 设置父类功能是否启用
     * @param e 是否启用，默认为 false
     */
    void setParentEnabled(const bool &e = false);

    /**
     * @brief 设置前景绘制是否启用
     * @param e 是否启用，默认为 true
     */
    void setForeEnabled(const bool &e = true);

    /**
     * @brief 设置悬浮动画时长
     * @param d 时长（毫秒）
     */
    void setHoverAniDuration(const int &d);

    /**
     * @brief 设置按下动画时长
     * @param d 时长（毫秒）
     */
    void setPressAniDuration(const int &d);

    /**
     * @brief 设置点击动画时长
     * @param d 时长（毫秒）
     */
    void setClickAniDuration(const int &d);

    /**
     * @brief 设置水波纹动画时长
     * @param press 按下时长（毫秒）
     * @param release 松开时长（毫秒）
     * @param finish 渐变消失时长（毫秒）
     */
    void setWaterAniDuration(const int &press, const int &release, const int &finish);

    /**
     * @brief 设置水波纹动画是否启用
     * @param enable 是否启用，默认为 true
     */
    void setWaterRipple(const bool &enable = true);

    /**
     * @brief 设置抖动动画是否启用
     * @param enable 是否启用，默认为 true
     */
    void setJitterAni(const bool &enable = true);

    /**
     * @brief 设置统一绘制区域是否启用
     * @param enable 是否启用，默认为 true
     */
    void setUnifyGeometry(const bool &enable = true);

    /**
     * @brief 设置背景颜色
     * @param bg 背景颜色
     */
    void setBgColor(const QColor &bg);

    /**
     * @brief 设置悬浮和按下背景颜色
     * @param hover 悬浮背景颜色
     * @param press 按下背景颜色
     */
    void setBgColor(const QColor &hover, const QColor &press);

    /**
     * @brief 设置默认背景颜色
     * @param color 背景颜色
     */
    void setNormalColor(const QColor &color);

    /**
     * @brief 设置边框颜色
     * @param color 边框颜色
     */
    void setBorderColor(const QColor &color);

    /**
     * @brief 设置悬浮背景颜色
     * @param color 背景颜色
     */
    void setHoverColor(const QColor &color);

    /**
     * @brief 设置按下背景颜色
     * @param color 背景颜色
     */
    void setPressColor(const QColor &color);

    /**
     * @brief 设置图标颜色
     * @param color 图标颜色，默认为黑色
     */
    void setIconColor(const QColor &color = QColor(0, 0, 0));

    /**
     * @brief 设置文本颜色
     * @param color 文本颜色，默认为黑色
     */
    void setTextColor(const QColor &color = QColor(0, 0, 0));

    /**
     * @brief 设置焦点背景颜色
     * @param color 背景颜色
     */
    void setFocusBg(const QColor &color);

    /**
     * @brief 设置焦点边框颜色
     * @param color 边框颜色
     */
    void setFocusBorder(const QColor &color);

    /**
     * @brief 设置字体大小
     * @param f 字体大小
     */
    void setFontSize(const int &f);

    /**
     * @brief 模拟悬浮状态
     */
    void setHover();

    /**
     * @brief 设置对齐方式
     * @param a 对齐方式
     */
    void setAlign(Qt::Alignment a);

    /**
     * @brief 设置圆角半径
     * @param r 半径
     */
    void setRadius(const int &r);

    /**
     * @brief 设置圆角半径（分别设置 x 和 y）
     * @param rx x 半径
     * @param ry y 半径
     */
    void setRadius(const int &rx, const int &ry);

    /**
     * @brief 设置边框宽度
     * @param x 宽度
     */
    void setBorderWidth(const int &x);

    /**
     * @brief 设置禁用状态
     * @param dis 是否禁用，默认为 true
     */
    void setDisabled(const bool &dis = true);

    /**
     * @brief 设置四边距
     * @param l 左边距
     * @param r 右边距
     * @param t 上边距
     * @param b 下边距
     */
    void setPaddings(const int &l, const int &r, const int &t, const int &b);

    /**
     * @brief 设置水平和垂直边距
     * @param h 水平边距
     * @param v 垂直边距
     */
    void setPaddings(const int &h, const int &v);

    /**
     * @brief 设置统一边距
     * @param x 边距值
     */
    void setPaddings(const int &x);

    /**
     * @brief 设置图标边距比例
     * @param x 比例（0~1）
     */
    void setIconPaddingProper(const double &x);

    /**
     * @brief 设置前景是否固定位置
     * @param f 是否固定，默认为 true
     */
    void setFixedForePos(const bool &f = true);

    /**
     * @brief 设置前景是否固定大小
     * @param f 是否固定，默认为 true
     * @param addin 额外边距
     */
    void setFixedForeSize(const bool &f = true, const int &addin = 0);

    /**
     * @brief 设置按钮为正方形
     */
    void setSquareSize();

    /**
     * @brief 设置文本动态调整大小
     * @param d 是否启用，默认为 true
     */
    void setTextDynamicSize(const bool &d = true);

    /**
     * @brief 设置点击后是否取消悬浮效果
     * @param l 是否启用，默认为 true
     */
    void setLeaveAfterClick(const bool &l = true);

    /**
     * @brief 设置是否启用双击事件
     * @param e 是否启用，默认为 true
     */
    void setDoubleClicked(const bool &e = true);

    /**
     * @brief 设置是否自动调整文本颜色
     * @param a 是否启用，默认为 true
     */
    void setAutoTextColor(const bool &a = true);

    /**
     * @brief 设置是否模拟焦点状态
     * @param f 是否启用，默认为 true
     */
    void setPretendFocus(const bool &f = true);

    /**
     * @brief 设置是否屏蔽悬浮效果
     * @param b 是否启用，默认为 true
     */
    void setBlockHover(const bool &b = true);

    /**
     * @brief 设置前景出现动画
     * @param enable 是否启用，默认为 true
     */
    void setShowAni(const bool &enable = true);

    /**
     * @brief 触发前景出现动画
     */
    void showForeground();

    /**
     * @brief 触发前景出现动画（指定方向）
     * @param point 出现方向
     */
    void showForeground2(QPoint point = QPoint(0, 0));

    /**
     * @brief 隐藏前景
     */
    void hideForeground();

    /**
     * @brief 延迟触发前景出现动画
     * @param time 延迟时间（毫秒）
     * @param point 出现方向
     */
    void delayShowed(int time, QPoint point = QPoint(0, 0));

    /**
     * @brief 获取按钮文本
     * @return 按钮文本
     */
    QString getText();

    /**
     * @brief 设置菜单
     * @param menu 菜单对象
     */
    void setMenu(QMenu *menu);

    /**
     * @brief 设置按钮状态
     * @param s 状态，默认为 true
     */
    void setState(const bool &s = true);

    /**
     * @brief 获取按钮状态
     * @return 按钮状态
     */
    bool getState() const;

    /**
     * @brief 模拟按下状态
     * @param s 目标状态，默认为 true
     * @param a 是否避免重复点击
     */
    virtual void simulateStatePress(bool s = true, bool a = false);

    /**
     * @brief 检查是否处于悬浮状态
     * @return 是否悬浮
     */
    bool isHovering() const { return hovering; }

    /**
     * @brief 检查是否处于按下状态
     * @return 是否按下
     */
    bool isPressing() const { return pressing; }

    /**
     * @brief 模拟悬浮状态
     */
    void simulateHover();

    /**
     * @brief 取消悬浮和按下状态
     * @param force 是否强制取消
     */
    void discardHoverPress(bool force = false);

    /**
     * @brief 获取子类功能启用状态
     * @return 是否启用
     */
    bool getSelfEnabled() const { return self_enabled; }

    /**
     * @brief 获取父类功能启用状态
     * @return 是否启用
     */
    bool getParentEnabled() const { return parent_enabled; }

    /**
     * @brief 获取前景绘制启用状态
     * @return 是否启用
     */
    bool getForeEnabled() const { return fore_enabled; }

    /**
     * @brief 获取图标颜色
     * @return 图标颜色
     */
    QColor getIconColor() const { return icon_color; }

    /**
     * @brief 获取文本颜色
     * @return 文本颜色
     */
    QColor getTextColor() const { return text_color; }

    /**
     * @brief 获取默认背景颜色
     * @return 背景颜色
     */
    QColor getNormalColor() const { return normal_bg; }

    /**
     * @brief 获取边框颜色
     * @return 边框颜色
     */
    QColor getBorderColor() const { return border_bg; }

    /**
     * @brief 获取悬浮背景颜色
     * @return 背景颜色
     */
    QColor getHoverColor() const { return hover_bg; }

    /**
     * @brief 获取按下背景颜色
     * @return 背景颜色
     */
    QColor getPressColor() const { return press_bg; }

    /**
     * @brief 获取图标路径
     * @return 图标路径
     */
    QString getIconPath() const { return ""; }

    /**
     * @brief 获取可变色图标路径
     * @return 图标路径
     */
    QString getPixmapPath() const { return ""; }

    /**
     * @brief 获取悬浮动画时长
     * @return 时长（毫秒）
     */
    int getHoverAniDuration() const { return hover_bg_duration; }

    /**
     * @brief 获取按下动画时长
     * @return 时长（毫秒）
     */
    int getPressAniDuration() const { return press_bg_duration; }

    /**
     * @brief 获取点击动画时长
     * @return 时长（毫秒）
     */
    int getClickAniDuration() const { return click_ani_duration; }

    /**
     * @brief 获取图标边距比例
     * @return 比例
     */
    double getIconPaddingProper() const { return icon_padding_proper; }

    /**
     * @brief 获取圆角半径
     * @return 半径
     */
    int getRadius() const { return qMax(radius_x, radius_y); }

    /**
     * @brief 获取边框宽度
     * @return 宽度
     */
    int getBorderWidth() const { return border_width; }

    /**
     * @brief 获取前景固定位置状态
     * @return 是否固定
     */
    bool getFixedTextPos() const { return fixed_fore_pos; }

    /**
     * @brief 获取文本动态调整大小状态
     * @return 是否启用
     */
    bool getTextDynamicSize() const { return text_dynamic_size; }

    /**
     * @brief 获取点击后取消悬浮状态
     * @return 是否启用
     */
    bool getLeaveAfterClick() const { return leave_after_clicked; }

    /**
     * @brief 获取前景出现动画状态
     * @return 是否启用
     */
    bool getShowAni() const { return show_animation; }

    /**
     * @brief 获取水波纹动画状态
     * @return 是否启用
     */
    bool getWaterRipple() const { return water_animation; }

#if QT_DEPRECATED_SINCE(5, 11)
    /**
     * @brief 设置前景固定位置（已废弃）
     * @param f 是否固定，默认为 true
     * @deprecated 使用 setFixedForePos 代替
     */
    QT_DEPRECATED_X("Use InteractiveButtonBase::setFixedForePos(bool fixed = true)")
    void setFixedTextPos(const bool &f = true);
#endif

protected:
    /**
     * @brief 鼠标进入事件
     * @param event 进入事件
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件
     * @param event 事件
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标松开事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标移动事件
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 大小调整事件
     * @param event 大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 获得焦点事件
     * @param event 焦点事件
     */
    void focusInEvent(QFocusEvent *event) override;

    /**
     * @brief 失去焦点事件
     * @param event 焦点事件
     */
    void focusOutEvent(QFocusEvent *event) override;

    /**
     * @brief 状态变更事件
     * @param event 事件
     */
    void changeEvent(QEvent *event) override;

    /**
     * @brief 绘制事件
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 判断点是否在按钮区域内
     * @param point 坐标点
     * @return 是否在区域内
     */
    virtual bool inArea(QPoint point);

    /**
     * @brief 获取背景绘制路径
     * @return 绘制路径
     */
    virtual QPainterPath getBgPainterPath();

    /**
     * @brief 获取水波纹绘制路径
     * @param water 水波纹对象
     * @return 绘制路径
     */
    virtual QPainterPath getWaterPainterPath(Water water);

    /**
     * @brief 绘制图标（在文本前）
     * @param painter 绘图对象
     * @param icon_rect 图标区域
     */
    virtual void drawIconBeforeText(QPainter &painter, QRect icon_rect);

    /**
     * @brief 获取统一绘制区域
     * @return 绘制区域
     */
    QRect getUnifiedGeometry();

    /**
     * @brief 更新统一绘制区域
     */
    void updateUnifiedGeometry();

    /**
     * @brief 绘制水波纹动画
     * @param painter 绘图对象
     */
    void paintWaterRipple(QPainter &painter);

    /**
     * @brief 设置抖动动画
     */
    void setJitter();

    /**
     * @brief 获取字体大小（动画用）
     * @return 字体大小
     */
    int getFontSizeT() const;

    /**
     * @brief 设置字体大小（动画用）
     * @param f 字体大小
     */
    void setFontSizeT(const int &f);

    /**
     * @brief 获取最大值
     * @param a 第一个值
     * @param b 第二个值
     * @return 最大值
     */
    int max(int a, int b) const;

    /**
     * @brief 获取最小值
     * @param a 第一个值
     * @param b 第二个值
     * @return 最小值
     */
    int min(int a, int b) const;

    /**
     * @brief 快速计算平方根
     * @param X 输入值
     * @return 平方根
     */
    int quick_sqrt(long X) const;

    /**
     * 获取现行时间戳，13位，精确到毫秒
     * @return 时间戳
     */
    qint64 getTimestamp() const;

    /**
     * @brief 判断是否为亮色
     * @param color 颜色
     * @return 是否为亮色
     */
    bool isLightColor(QColor color);

    /**
     * @brief 获取回弹动画进度
     * @param x 当前进度
     * @param max 最大进度
     * @return 动画进度
     */
    int getSpringBackProgress(int x, int max);

    /**
     * @brief 获取透明颜色
     * @param color 原始颜色
     * @param level 透明度
     * @return 透明颜色
     */
    QColor getOpacityColor(QColor color, double level = 0.5);

    /**
     * @brief 获取遮罩图标
     * @param p 原始图标
     * @param c 颜色
     * @return 遮罩图标
     */
    QPixmap getMaskPixmap(QPixmap p, QColor c);

    /**
     * @brief 获取非线性动画进度
     * @param p 当前进度
     * @param type 动画类型
     * @return 动画进度
     */
    double getNolinearProg(int p, NolinearType type);

    /**
     * @brief 获取图标模式
     * @return 图标模式
     */
    QIcon::Mode getIconMode() const;

signals:
    /** @brief 前景出现动画完成 */
    void showAniFinished();

    /** @brief 前景消失动画完成 */
    void hideAniFinished();

    /** @brief 按下出现动画完成 */
    void pressAppearAniFinished();

    /** @brief 按下消失动画完成 */
    void pressDisappearAniFinished();

    /** @brief 抖动动画完成 */
    void jitterAniFinished();

    /** @brief 双击事件 */
    void doubleClicked();

    /** @brief 右键点击事件 */
    void rightClicked();

    /** @brief 获得焦点 */
    void signalFocusIn();

    /** @brief 失去焦点 */
    void signalFocusOut();

    /** @brief 鼠标进入 */
    void signalMouseEnter();

    /** @brief 鼠标进入延迟信号 */
    void signalMouseEnterLater();

    /** @brief 鼠标离开 */
    void signalMouseLeave();

    /** @brief 鼠标离开延迟信号 */
    void signalMouseLeaveLater();

    /** @brief 鼠标按下 */
    void signalMousePress(QMouseEvent *event);

    /** @brief 鼠标按下延迟信号 */
    void signalMousePressLater(QMouseEvent *event);

    /** @brief 鼠标松开 */
    void signalMouseRelease(QMouseEvent *event);

    /** @brief 鼠标松开延迟信号 */
    void signalMouseReleaseLater(QMouseEvent *event);

public slots:
    /**
     * @brief 锚点定时器超时
     */
    virtual void anchorTimeOut();

    /**
     * @brief 处理点击事件
     */
    virtual void slotClicked();

    /**
     * @brief 关闭按钮状态
     */
    void slotCloseState();

protected:
    PaintModel model;       ///< 前景绘制模式
    QIcon icon;             ///< 按钮图标
    QString text;           ///< 按钮文本
    QPixmap pixmap;         ///< 可变色图标
    PaintAddin paint_addin; ///< 额外图标配置
    EdgeVal fore_paddings;  ///< 前景四边距

    bool self_enabled;          ///< 子类功能是否启用
    bool parent_enabled;        ///< 父类功能是否启用
    bool fore_enabled;          ///< 前景绘制是否启用
    bool show_animation;        ///< 前景出现动画是否启用
    bool show_foreground;       ///< 是否显示前景
    bool show_ani_appearing;    ///< 前景出现动画进行中
    bool show_ani_disappearing; ///< 前景消失动画进行中
    int show_duration;          ///< 出现动画时长
    qint64 show_timestamp;      ///< 出现动画时间戳
    qint64 hide_timestamp;      ///< 消失动画时间戳
    int show_ani_progress;      ///< 出现动画进度
    QPoint show_ani_point;      ///< 出现动画起始点
    QRect paint_rect;           ///< 绘制区域

    QPoint enter_pos;         ///< 鼠标进入坐标
    QPoint press_pos;         ///< 鼠标按下坐标
    QPoint release_pos;       ///< 鼠标松开坐标
    QPoint mouse_pos;         ///< 当前鼠标坐标
    QPoint anchor_pos;        ///< 锚点坐标
    QPoint offset_pos;        ///< 当前偏移量
    QPoint effect_pos;        ///< 效果坐标
    QPoint release_offset;    ///< 松开时偏移
    bool hovering;            ///< 是否悬浮
    bool pressing;            ///< 是否按下
    qint64 hover_timestamp;   ///< 悬浮时间戳
    qint64 leave_timestamp;   ///< 离开时间戳
    qint64 press_timestamp;   ///< 按下时间戳
    qint64 release_timestamp; ///< 松开时间戳
    int hover_bg_duration;    ///< 悬浮动画时长
    int press_bg_duration;    ///< 按下动画时长
    int click_ani_duration;   ///< 点击动画时长
    QTimer *anchor_timer;     ///< 锚点定时器
    int move_speed;           ///< 移动速度

    QColor icon_color;          ///< 图标颜色
    QColor text_color;          ///< 文本颜色
    QColor normal_bg;           ///< 默认背景颜色
    QColor hover_bg;            ///< 悬浮背景颜色
    QColor press_bg;            ///< 按下背景颜色
    QColor border_bg;           ///< 边框颜色
    QColor focus_bg;            ///< 焦点背景颜色
    QColor focus_border;        ///< 焦点边框颜色
    int hover_speed;            ///< 悬浮渐变速度
    int press_start;            ///< 按下渐变起始值
    int press_speed;            ///< 按下渐变速度
    int hover_progress;         ///< 悬浮渐变进度
    int press_progress;         ///< 按下渐变进度
    double icon_padding_proper; ///< 图标边距比例
    int icon_text_padding;      ///< 图标与文本间距
    int icon_text_size;         ///< 图标与文本模式图标大小
    int border_width;           ///< 边框宽度
    int radius_x;               ///< x 圆角半径
    int radius_y;               ///< y 圆角半径
    int font_size;              ///< 字体大小
    bool fixed_fore_pos;        ///< 前景是否固定位置
    bool fixed_fore_size;       ///< 前景是否固定大小
    bool text_dynamic_size;     ///< 文本是否动态调整大小
    bool auto_text_color;       ///< 是否自动调整文本颜色
    bool focusing;              ///< 是否获得焦点

    bool click_ani_appearing;         ///< 点击动画出现中
    bool click_ani_disappearing;      ///< 点击动画消失中
    int click_ani_progress;           ///< 点击动画进度
    QMouseEvent *mouse_press_event;   ///< 按下事件
    QMouseEvent *mouse_release_event; ///< 松开事件

    bool unified_geometry; ///< 是否使用统一绘制区域
    int _l;                ///< 统一区域左坐标
    int _t;                ///< 统一区域上坐标
    int _w;                ///< 统一区域宽度
    int _h;                ///< 统一区域高度

    bool jitter_animation;      ///< 抖动动画是否启用
    double elastic_coefficient; ///< 弹性系数
    QList<Jitter> jitters;      ///< 抖动动画队列
    int jitter_duration;        ///< 抖动动画时长

    bool water_animation;       ///< 水波纹动画是否启用
    QList<Water> waters;        ///< 水波纹动画队列
    int water_press_duration;   ///< 水波纹按下时长
    int water_release_duration; ///< 水波纹松开时长
    int water_finish_duration;  ///< 水波纹结束时长
    int water_radius;           ///< 水波纹半径

    Qt::Alignment align;      ///< 对齐方式
    bool _state;              ///< 按钮状态
    bool leave_after_clicked; ///< 点击后是否取消悬浮
    bool _block_hover;        ///< 是否屏蔽悬浮效果

    bool double_clicked;  ///< 是否启用双击
    QTimer *double_timer; ///< 双击定时器
    bool double_prevent;  ///< 阻止双击触发单击
};

#endif // INTERACTIVEBUTTONBASE_H