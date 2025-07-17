/**
 * @file Channel.h
 * @brief 定义 Channel 类，提供音乐频道主界面
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include "PartWidget.h"

#include <QWidget>
#include <vector>

class QScrollBar;
class QButtonGroup;
class RefreshMask;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class Channel;
}
QT_END_NAMESPACE

/**
 * @class Channel
 * @brief 音乐频道主界面类，支持分类导航、滚动和音乐块展示
 */
class Channel : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐频道主界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit Channel(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~Channel() override;

private:
    /**
     * @brief 初始化按钮组
     */
    void initButtonGroup() const;

    /**
     * @brief 初始化分区控件
     */
    void initTotalWidget();

    /**
     * @brief 初始化界面
     */
    void initUi();

    void loadSectionBlocks(PartWidget *section, const int &cnt, const int &sum);

private slots:
    /**
     * @brief 处理滚动条值变化
     * @param value 滚动条值
     */
    void handleWheelValue(const int &value);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::Channel                                  *ui;                            ///< UI 指针
    std::unique_ptr<QButtonGroup>                 m_buttonGroup{};               ///< 按钮组
    std::unique_ptr<PartWidget>                   m_recommendWidget{};           ///< 推荐分区控件
    std::unique_ptr<PartWidget>                   m_djWidget{};                  ///< DJ 分区控件
    std::unique_ptr<PartWidget>                   m_languageWidget{};            ///< 语言分区控件
    std::unique_ptr<PartWidget>                   m_themeWidget{};               ///< 主题分区控件
    std::unique_ptr<PartWidget>                   m_sceneWidget{};               ///< 场景分区控件
    std::unique_ptr<PartWidget>                   m_moodWidget{};                ///< 心情分区控件
    std::unique_ptr<PartWidget>                   m_styleWidget{};               ///< 风格分区控件
    std::unique_ptr<PartWidget>                   m_crowdWidget{};               ///< 人群分区控件
    std::unique_ptr<PartWidget>                   m_childrenWidget{};            ///< 儿童分区控件
    std::unique_ptr<PartWidget>                   m_musicalInstrumentWidget{};   ///< 乐器分区控件
    std::unique_ptr<PartWidget>                   m_labelWidget{};               ///< 厂牌分区控件
    std::unique_ptr<PartWidget>                   m_varietyWidget{};             ///< 综艺分区控件
    std::unique_ptr<PartWidget>                   m_nationalCustomsWidget{};     ///< 国风分区控件
    std::unique_ptr<PartWidget>                   m_sportsWidget{};              ///< 运动分区控件
    QList<std::pair<QString, QString>>            m_songAndsinger{};             ///< 歌曲-歌手配对
    QList<QString>                                m_pixPathVector{};             ///< 封面图片路径
    QList<QString>                                m_titleVector{};               ///< 标题
    std::unique_ptr<RefreshMask>                  m_refreshMask;                 ///< 刷新遮罩
};

#endif // CHANNEL_H