/**
 * @file Live.h
 * @brief 定义 Live 类，提供直播主界面
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#ifndef LIVE_H
#define LIVE_H

#include <QTimer>

#include "LiveCommonPartWidget/LiveCommonPartWidget.h"
#include "LiveMusicPartWidget/LiveMusicPartWidget.h"
#include "LiveBigLeftWidget/LiveBigLeftWidget.h"

class RefreshMask;
/**
 * @class QScrollBar
 * @brief 滚动条控件类
 */
class QScrollBar;
/**
 * @class QButtonGroup
 * @brief 按钮组类
 */
class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class Live;
}
QT_END_NAMESPACE

/**
 * @class Live
 * @brief 直播主界面类，整合多个直播控件
 */
class Live : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化直播主界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit Live(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~Live() override;

private:
    /**
     * @brief 初始化按钮组
     * @note 设置按钮互斥
     */
    void initButtonGroup() const;

    /**
     * @brief 初始化界面
     * @note 初始化子控件和布局
     */
    void initUi();

    /**
     * @brief 初始化热门控件
     * @note 设置按钮和标签
     */
    void initPopularWidget();

    /**
     * @brief 生成圆角图片
     * @param src 原始图片
     * @param size 目标大小
     * @param radius 圆角半径
     * @return 圆角图片
     */
    QPixmap roundedPixmap(const QPixmap &src, const QSize &size, const int &radius);

    /**
     * @brief 初始化关注控件
     * @note 设置头像和按钮
     */
    void initAttentionWidget();

    /**
     * @brief 初始化推荐控件
     * @note 设置推荐内容
     */
    void initRecommendWidget();

    /**
     * @brief 初始化音乐控件
     * @note 设置音乐内容
     */
    void initMusicWidget();

    /**
     * @brief 初始化新秀控件
     * @note 设置新秀内容
     */
    void initNewStarWidget();

    /**
     * @brief 初始化颜值控件
     * @note 设置颜值内容
     */
    void initAppearanceWidget();

    /**
     * @brief 初始化舞蹈控件
     * @note 设置舞蹈内容
     */
    void initDanceWidget();

    /**
     * @brief 初始化游戏控件
     * @note 设置游戏内容
     */
    void initGameWidget();

private slots:
    /**
     * @brief 处理滚动值
     * @param value 滚动值
     * @note 更新按钮状态
     */
    void handleWheelValue(const int &value);

    /**
     * @brief 全部按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_all_toolButton_clicked();

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 动态调整控件尺寸
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 显示事件
     * @param event 显示事件
     * @note 调整控件尺寸
     */
    void showEvent(QShowEvent *event) override;

public slots:
    void emitInitialized() {QTimer::singleShot(0, this, [this] {emit initialized();});}

signals:
    void initialized();

private:
    Ui::Live                        *ui;                     ///< UI 指针
    std::unique_ptr<QButtonGroup>    m_buttonGroup;          ///< 按钮组
    std::unique_ptr<LiveCommonPartWidget> m_recommendWidget; ///< 推荐控件
    std::unique_ptr<LiveMusicPartWidget>  m_musicWidget;     ///< 音乐控件
    std::unique_ptr<LiveCommonPartWidget> m_newStarWidget;   ///< 新秀控件
    std::unique_ptr<LiveBigLeftWidget>    m_appearanceWidget; ///< 颜值控件
    std::unique_ptr<LiveCommonPartWidget> m_danceWidget;     ///< 舞蹈控件
    std::unique_ptr<LiveBigLeftWidget>    m_gameWidget;      ///< 游戏控件
    std::unique_ptr<RefreshMask>                  m_refreshMask;                 ///< 刷新遮罩
};

#endif // LIVE_H