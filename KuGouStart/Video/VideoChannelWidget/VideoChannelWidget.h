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

#include <memory>

class QScrollBar;
class QButtonGroup;

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
    void initTotalWidget() const;

    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 初始化图片路径容器
     */
    void initVector();

private slots:
    /**
     * @brief 处理滚动值变化
     * @param value 滚动条值
     */
    void handleWheelValue(const int &value);

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件对象
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mousePressEvent(QMouseEvent *event) override;

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
    QList<QString>                     m_pixPathVector;       ///< 图片路径容器
    QScrollBar                              *m_vScrollBar;          ///< 垂直滚动条
};

#endif // VIDEOCHANNELWIDGET_H