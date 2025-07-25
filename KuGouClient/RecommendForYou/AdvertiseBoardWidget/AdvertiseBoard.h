#ifndef ADVERTISEBOARD_H
#define ADVERTISEBOARD_H

#include <QLabel>

class QPropertyAnimation;

/**
 * @brief 导航按钮的半径常量
 */
static constexpr int DOT_RADIUS = 4;

/**
 * @brief 导航按钮之间的间距常量
 */
static constexpr int DOT_SPACING = 8;

/**
 * @brief 活动状态下导航按钮的额外半径
 */
static constexpr int ACTIVE_DOT_EXTRA = 2;

/**
 * @class NavButton
 * @brief 自定义导航按钮类，用于广告牌的左右导航
 * 
 * 该类继承自 QLabel，用于显示导航按钮的正常和悬停状态图片，并处理鼠标事件。
 */
class NavButton : public QLabel {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param normal 正常状态下的按钮图片路径
     * @param hover 悬停状态下的按钮图片路径
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit NavButton(const QString &normal, const QString &hover, QWidget *parent = nullptr);

    /**
     * @brief 设置按钮的悬停状态
     * @param hover 是否为悬停状态
     */
    void setHoverState(bool hover);

signals:
    /**
     * @brief 信号：当按钮被点击时发出
     */
    void clicked();

protected:
    /**
     * @brief 重写事件处理函数，处理鼠标悬停、点击等事件
     * @param event 事件对象
     * @return 如果事件被处理则返回 true，否则返回 false
     */
    bool event(QEvent *event) override;

private slots:
    /**
     * @brief 检查鼠标是否仍在按钮区域内
     */
    void checkHoverState();

private:
    QPixmap m_normal;       ///< 正常状态下的按钮图片
    QPixmap m_hover;        ///< 悬停状态下的按钮图片
    QTimer* m_checkTimer;   ///< 延迟检查鼠标位置的定时器
};

/**
 * @class AdvertiseBoard
 * @brief 广告牌控件类，用于展示轮播图片
 * 
 * 该类实现了一个支持图片轮播的广告牌控件，包含自动播放、导航按钮和指示圆点。
 */
class AdvertiseBoard : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int slideOffset READ slideOffset WRITE setSlideOffset)

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AdvertiseBoard(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~AdvertiseBoard() override;

    /**
     * @brief 添加一张海报图片
     * @param pixPath 海报图片的路径
     */
    void addPoster(const QString &pixPath);

    /**
     * @brief 设置广告牌的宽高比
     * @param aspectRatio 宽高比，需大于 0
     */
    void setAspectRatio(qreal aspectRatio);

    /**
     * @brief 获取当前滑动偏移量
     * @return 当前的滑动偏移量
     */
    int slideOffset() const { return m_slideOffset; }

    /**
     * @brief 设置滑动偏移量
     * @param offset 滑动偏移量
     */
    void setSlideOffset(int offset);

protected:
    /**
     * @brief 重写绘制事件，绘制海报图片和导航圆点
     * @param event 绘制事件对象
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 重写窗口大小调整事件，更新按钮位置和缩放图片
     * @param event 大小调整事件对象
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 重写鼠标进入事件，显示导航按钮
     * @param event 鼠标进入事件对象
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 重写鼠标离开事件，隐藏导航按钮
     * @param event 鼠标离开事件对象
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 重写鼠标移动事件，处理导航圆点的交互
     * @param event 鼠标移动事件对象
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 重写鼠标释放事件，处理导航圆点的点击
     * @param event 鼠标释放事件对象
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    /**
     * @brief 更新导航按钮的位置
     */
    void updateButtonPosition();

    /**
     * @brief 更新缩放后的海报图片
     */
    void updateScaledPosters();

    /**
     * @brief 计算导航圆点的位置
     * @param centers 输出参数，存储圆点中心点
     * @param totalWidth 输出参数，存储圆点总宽度
     */
    void calculateDotPositions(QList<QPoint> &centers, int &totalWidth);

    /**
     * @brief 切换到下一张海报
     */
    void switchToNext();

    /**
     * @brief 切换到上一张海报
     */
    void switchToPrev();

    /**
     * @brief 切换到指定索引的海报
     * @param index 目标海报的索引
     */
    void switchToIndex(const int &index);

    /**
     * @brief 开始滑动动画
     * @param startValue 动画起始值
     * @param endValue 动画结束值
     */
    void startAnimation(int startValue, int endValue);

    QList<QString> m_postersPath;       ///< 原始海报图片路径列表
    QVector<QPixmap> m_scaledPosters;   ///< 缩放后的海报图片列表
    NavButton *m_leftBtn;               ///< 左侧导航按钮
    NavButton *m_rightBtn;              ///< 右侧导航按钮
    QTimer *m_resizeTimer;              ///< 防抖定时器
    QTimer *m_timer;                    ///< 自动播放定时器
    QPropertyAnimation *m_animation;     ///< 滑动动画对象
    int m_currentIndex = 0;             ///< 当前显示的海报索引
    int m_previousIndex = 0;            ///< 上一次显示的海报索引
    int m_slideOffset = 0;              ///< 当前滑动偏移量
    qreal m_aspectRatio = 2.0;          ///< 广告牌宽高比
    bool m_isAnimating = false;         ///< 是否正在执行动画
    bool m_slidingToNext = true;        ///< 是否向右滑动
    QList<QRect> m_dotRects;            ///< 导航圆点的矩形区域列表
};

#endif // ADVERTISEBOARD_H