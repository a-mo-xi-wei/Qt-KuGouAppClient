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
    void initTotalWidget() const;

    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 初始化歌曲-歌手和封面图片向量
     */
    void initVector();

private slots:
    /**
     * @brief 处理滚动条值变化
     * @param value 滚动条值
     */
    void handleWheelValue(const int &value);

protected:
    /**
     * @brief 调整大小事件，调整滚动区域高度
     * @param event 调整大小事件
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

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
    QList<std::pair<QString, QString>>      m_songAndsinger{};             ///< 歌曲-歌手配对
    QList<QString>                          m_pixPathVector{};             ///< 封面图片路径
    QScrollBar                                   *m_vScrollBar{};                ///< 垂直滚动条
};

#endif // CHANNEL_H