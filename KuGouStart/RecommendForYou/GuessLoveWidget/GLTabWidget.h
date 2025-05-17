/**
 * @file GLTabWidget.h
 * @brief 定义 GLTabWidget 类，管理“猜你喜欢”推荐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef GLTABWIDGET_H
#define GLTABWIDGET_H

#include <QWidget>

/**
 * @class IconBesideTextToolButton
 * @brief 自定义工具按钮类，图标与文本并排
 */
class IconBesideTextToolButton;

/**
 * @class QLabel
 * @brief 标签类，用于显示文本或图片
 */
class QLabel;

/**
 * @class QToolButton
 * @brief 工具按钮类，用于交互操作
 */
class QToolButton;

/**
 * @class QPaintEvent
 * @brief 绘制事件类
 */
class QPaintEvent;

/**
 * @class GLTabWidget
 * @brief “猜你喜欢”推荐界面类，管理模式选择、歌曲信息和交互按钮
 */
class GLTabWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化推荐界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit GLTabWidget(QWidget *parent = nullptr);

protected:
    /**
     * @brief 绘制事件
     * @param ev 绘制事件
     * @note 绘制控件背景
     */
    void paintEvent(QPaintEvent *ev) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 动态调整控件高度
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化标签、按钮和样式表
     */
    void initUi();

    /**
     * @brief 布局界面
     * @note 使用水平和垂直布局排列控件
     */
    void layoutUi();

private slots:
    /**
     * @brief 模式按钮点击槽函数
     * @note 弹出模式选择菜单
     */
    void onModelBtnClicked();

    /**
     * @brief 处理模式选择
     * @param model 选择的模式
     * @note 更新模式按钮文本
     */
    void onGetModel(const QString &model);

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QLabel                    *m_glLab{};                   ///< “猜你喜欢”标签
    QLabel                    *m_songNameLab{};             ///< 歌曲名标签
    QLabel                    *m_singerLab{};               ///< 歌手标签
    IconBesideTextToolButton  *m_modelBtn{};                ///< 模式选择按钮
    QToolButton               *m_playToolBtn{};             ///< 播放按钮
    QToolButton               *m_nextToolBtn{};             ///< 下一首按钮
    QToolButton               *m_likeToolBtn{};             ///< 喜欢按钮
    QToolButton               *m_dislikeToolBtn{};          ///< 不喜欢按钮
};

#endif // GLTABWIDGET_H