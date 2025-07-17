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
class RefreshMask;

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
    void initTotalWidget();

    /**
     * @brief 初始化界面
     * @note 设置布局、信号和视频块
     */
    void initUi();

    void loadSectionBlocks(VideoPartWidget* section,int idx);

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
    void showEvent(QShowEvent *event) override;

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
    std::unique_ptr<RefreshMask>     m_refreshMask;          ///< 刷新遮罩
    QList<QString> m_pixPathVector;                          ///< 封面图片路径列表
    QList<QPair<QString,QString>> m_videoAuthorVector;       ///< 视频名称列表
};

#endif // VIDEOWIDGET_H