/**
 * @file AdvertiseBoard.h
 * @brief 广告轮播界面的头文件，定义 NavButton 和 AdvertiseBoard 类
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef ADVERTISEBOARD_H
#define ADVERTISEBOARD_H

#include <QLabel>
#include <QList>

/** @brief 导航圆点半径常量 */
static constexpr int DOT_RADIUS = 4;

/** @brief 导航圆点间距常量 */
static constexpr int DOT_SPACING = 8;

/** @brief 活动圆点额外大小常量 */
static constexpr int ACTIVE_DOT_EXTRA = 2;

/**
 * @class NavButton
 * @brief 导航按钮类，用于广告轮播的左右切换
 */
class NavButton : public QLabel
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param normal 普通状态图片
     * @param hover 悬停状态图片
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit NavButton(const QString &normal, const QString &hover, QWidget *parent = nullptr);

signals:
    void clicked();

protected:
    /**
     * @brief 鼠标进入事件
     * @param event 进入事件
     * @note 显示悬停状态图片
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件
     * @param event 离开事件
     * @note 恢复普通状态图片
     */
    void leaveEvent(QEvent *event) override;
    
    /**
     * @brief 鼠标按下事件
     * @param event 按下事件
     * @note 发送按下信号
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPixmap m_normal; ///< 普通状态图片
    QPixmap m_hover;  ///< 悬停状态图片
};

/**
 * @class AdvertiseBoard
 * @brief 广告轮播界面类，管理海报展示和导航
 */
class AdvertiseBoard : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AdvertiseBoard(QWidget *parent = nullptr);

    /**
     * @brief 添加海报
     * @param pixmap 海报图片
     * @note 仅当图片有效时添加
     */
    void addPoster(const QPixmap &pixmap);

    /**
     * @brief 设置宽高比
     * @param aspectRatio 宽高比
     * @note 仅当值大于 0 时生效，默认 2.0
     */
    void setAspectRatio(qreal aspectRatio);

    /**
     * @brief 设置自动播放间隔
     * @param interval 间隔时间（毫秒）
     * @note 仅当值大于 0 时生效
     */
    void setAutoPlayInterval(int interval);

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制当前海报和导航圆点
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整按钮和圆点位置
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 鼠标进入事件
     * @param event 进入事件
     * @note 显示导航按钮并暂停自动播放
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件
     * @param event 离开事件
     * @note 隐藏导航按钮并恢复自动播放
     */
    void leaveEvent(QEvent *event) override;

private:
    /**
     * @brief 更新按钮位置
     * @note 根据控件大小调整左右按钮位置
     */
    void updateButtonPosition();

    /**
     * @brief 计算导航圆点位置
     * @return 圆点位置列表
     * @note 根据海报数量和控件大小计算
     */
    void calculateDotPositions(QList<QPoint> &centers, int &totalWidth);

    QList<QPixmap> m_posters;           ///< 海报图片列表
    NavButton *m_leftBtn{};             ///< 左导航按钮
    NavButton *m_rightBtn{};            ///< 右导航按钮
    QTimer *m_timer{};                  ///< 自动播放定时器
    int m_currentIndex = 0;             ///< 当前海报索引
    qreal m_aspectRatio = 2.0;          ///< 宽高比，默认 2.0
};

#endif // ADVERTISEBOARD_H