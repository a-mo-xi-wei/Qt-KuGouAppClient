/**
 * @file VideoWidget.h
 * @brief 视频界面的头文件，定义 VideoWidget 类
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "VideoPartWidget.h"
#include <QWidget>
#include <memory>
#include <vector>

/**
 * @class QScrollBar
 * @brief Qt 滚动条类，用于垂直滚动
 */
class QScrollBar;

/**
 * @class QButtonGroup
 * @brief Qt 按钮组类，用于管理互斥按钮
 */
class QButtonGroup;

/**
 * @namespace Ui
 * @brief UI 命名空间，包含视频界面的 UI 类
 */
QT_BEGIN_NAMESPACE
namespace Ui { class VideoWidget; }
QT_END_NAMESPACE

/**
 * @class VideoWidget
 * @brief 视频界面类，管理多个视频分区和滚动区域
 */
class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit VideoWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     * @note 释放 UI 资源
     */
    ~VideoWidget() override;

private:
    /**
     * @brief 初始化按钮组
     * @note 设置互斥按钮
     */
    void initButtonGroup() const;

    /**
     * @brief 初始化视频分区
     * @note 设置 13 个分区的标题
     */
    void initTotalWidget() const;

    /**
     * @brief 初始化图片路径
     * @note 加载并打乱 120 张封面图片
     */
    void initVector();

    /**
     * @brief 初始化界面
     * @note 设置布局、信号和视频块
     */
    void initUi();

private slots:
    /**
     * @brief 全部按钮切换槽函数
     * @note 切换标题区域显示并调整滚动高度
     */
    void on_all_toolButton_toggled();

    /**
     * @brief 处理滚轮值
     * @param value 滚动条值
     * @note 根据滚动位置切换按钮选中状态
     */
    void handleWheelValue(const int &value);

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整滚动区域高度
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 默认实现
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::VideoWidget *ui;                                     ///< UI 界面指针
    std::unique_ptr<QButtonGroup> m_buttonGroup;             ///< 按钮组
    std::unique_ptr<VideoPartWidget> m_recommendWidget;      ///< 推荐分区
    std::unique_ptr<VideoPartWidget> m_videoRankWidget;      ///< 视频榜分区
    std::unique_ptr<VideoPartWidget> m_MVWidget;             ///< MV 分区
    std::unique_ptr<VideoPartWidget> m_siteWidget;           ///< 现场分区
    std::unique_ptr<VideoPartWidget> m_coverWidget;          ///< 翻唱分区
    std::unique_ptr<VideoPartWidget> m_danceWidget;          ///< 舞蹈分区
    std::unique_ptr<VideoPartWidget> m_childrenWidget;       ///< 儿童分区
    std::unique_ptr<VideoPartWidget> m_liveWidget;           ///< TME LIVE 分区
    std::unique_ptr<VideoPartWidget> m_firstConcertWidget;   ///< 首唱会分区
    std::unique_ptr<VideoPartWidget> m_chineseLanguageWidget;///< 华语分区
    std::unique_ptr<VideoPartWidget> m_southKoreaWidget;     ///< 韩国分区
    std::unique_ptr<VideoPartWidget> m_japanWidget;          ///< 日本分区
    std::unique_ptr<VideoPartWidget> m_americanWidget;       ///< 欧美分区
    std::vector<QString> m_pixPathVector;                    ///< 封面图片路径列表
    QScrollBar *m_vScrollBar{};                              ///< 垂直滚动条
};

#endif // VIDEOWIDGET_H