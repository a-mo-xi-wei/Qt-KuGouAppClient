/**
 * @file VideoChannelWidget.h
 * @brief 定义 VideoChannelWidget 类，提供视频频道分类界面
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef VIDEOCHANNELWIDGET_H
#define VIDEOCHANNELWIDGET_H

#include "VideoChannelPartWidget.h"

class QScrollBar;
class QButtonGroup;
class RefreshMask;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui { class VideoChannelWidget; }
QT_END_NAMESPACE

/**
 * @class VideoChannelWidget
 * @brief 视频频道分类界面类，支持热门、儿童、主题等分类
 */
class VideoChannelWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化视频频道界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit VideoChannelWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~VideoChannelWidget() override;

private:
    /**
     * @brief 初始化按钮组
     */
    void initButtonGroup() const;

    /**
     * @brief 初始化分类部件
     */
    void initTotalWidget();

    /**
     * @brief 初始化界面
     */
    void initUi();

    void loadSectionBlocks(VideoChannelPartWidget* section,const int& cnt , const int& sum);

private slots:
    /**
     * @brief 处理滚动值变化
     * @param value 滚动条值
     */
    void handleWheelValue(const int &value);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::VideoChannelWidget                  *ui;                    ///< UI 指针
    std::unique_ptr<QButtonGroup>            m_buttonGroup;         ///< 按钮组
    std::unique_ptr<VideoChannelPartWidget>  m_popularWidget;       ///< 热门分类
    std::unique_ptr<VideoChannelPartWidget>  m_childrenWidget;      ///< 儿童分类
    std::unique_ptr<VideoChannelPartWidget>  m_themeWidget;         ///< 主题分类
    std::unique_ptr<VideoChannelPartWidget>  m_filmWidget;          ///< 影视分类
    std::unique_ptr<VideoChannelPartWidget>  m_varietyWidget;       ///< 综艺分类
    std::unique_ptr<VideoChannelPartWidget>  m_ACGNWidget;          ///< 二次元分类
    std::unique_ptr<VideoChannelPartWidget>  m_sceneWidget;         ///< 场景分类
    std::unique_ptr<VideoChannelPartWidget>  m_languageWidget;      ///< 语言分类
    std::unique_ptr<VideoChannelPartWidget>  m_danceWidget;         ///< 舞蹈分类
    std::unique_ptr<VideoChannelPartWidget>  m_siteWidget;          ///< 现场分类
    std::unique_ptr<VideoChannelPartWidget>  m_singerWidget;        ///< 歌手分类
    QList<QString>                           m_pixPathVector;       ///< 图片路径容器
    QList<QString>                           m_coverTextVector;     ///< 图片文字容器
    std::unique_ptr<RefreshMask>             m_refreshMask;         ///< 刷新遮罩
};

#endif // VIDEOCHANNELWIDGET_H