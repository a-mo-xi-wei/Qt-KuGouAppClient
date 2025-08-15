/**
 * @file VideoWidget.h
 * @brief 视频界面的头文件，定义 VideoWidget 类
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QTimer>

#include "VideoPartWidget.h"

class QPushButton;
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

namespace Ui {
class VideoWidget;
}

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
    void initButtonGroup();

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

    void loadSectionBlocks(VideoPartWidget *section, int idx);

private slots:
    /**
     * @brief 处理滚轮值
     * @param value 滚动条值
     * @note 根据滚动位置切换按钮选中状态
     */
    void handleWheelValue(const int &value);

protected:
    void showEvent(QShowEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

public slots:
    void emitInitialized() { QTimer::singleShot(0, this, [this] { emit initialized(); }); }

signals:
    void initialized();

private:
    Ui::VideoWidget *ui;                                      ///< UI 界面指针
    std::unique_ptr<QButtonGroup> m_buttonGroup;              ///< 按钮组
    QPushButton *m_recommend_pushButton;                      ///< 推荐按钮
    QPushButton *m_video_rank_pushButton;                     ///< 视频榜按钮
    QPushButton *m_MV_pushButton;                             ///< MV按钮
    QPushButton *m_site_pushButton;                           ///< 现场按钮
    QPushButton *m_cover_pushButton;                          ///< 翻唱按钮
    QPushButton *m_dance_pushButton;                          ///< 舞蹈按钮
    QPushButton *m_children_pushButton;                       ///< 儿童按钮
    QPushButton *m_live_pushButton;                           ///< TME LIVE按钮
    QPushButton *m_first_concert_pushButton;                  ///< 首唱会按钮
    QPushButton *m_chinese_language_pushButton;               ///< 华语按钮
    QPushButton *m_South_Korea_pushButton;                    ///< 韩国按钮
    QPushButton *m_Japan_pushButton;                          ///< 日本按钮
    QPushButton *m_American_pushButton;                       ///< 欧美按钮
    std::unique_ptr<VideoPartWidget> m_recommendWidget;       ///< 推荐分区
    std::unique_ptr<VideoPartWidget> m_videoRankWidget;       ///< 视频榜分区
    std::unique_ptr<VideoPartWidget> m_MVWidget;              ///< MV 分区
    std::unique_ptr<VideoPartWidget> m_siteWidget;            ///< 现场分区
    std::unique_ptr<VideoPartWidget> m_coverWidget;           ///< 翻唱分区
    std::unique_ptr<VideoPartWidget> m_danceWidget;           ///< 舞蹈分区
    std::unique_ptr<VideoPartWidget> m_childrenWidget;        ///< 儿童分区
    std::unique_ptr<VideoPartWidget> m_liveWidget;            ///< TME LIVE 分区
    std::unique_ptr<VideoPartWidget> m_firstConcertWidget;    ///< 首唱会分区
    std::unique_ptr<VideoPartWidget> m_chineseLanguageWidget; ///< 华语分区
    std::unique_ptr<VideoPartWidget> m_southKoreaWidget;      ///< 韩国分区
    std::unique_ptr<VideoPartWidget> m_japanWidget;           ///< 日本分区
    std::unique_ptr<VideoPartWidget> m_americanWidget;        ///< 欧美分区
    std::unique_ptr<RefreshMask> m_refreshMask;               ///< 刷新遮罩
    QList<QString> m_pixPathVector;                           ///< 封面图片路径列表
    QList<QPair<QString, QString>> m_videoAuthorVector;       ///< 视频名称列表
};

#endif // VIDEOWIDGET_H