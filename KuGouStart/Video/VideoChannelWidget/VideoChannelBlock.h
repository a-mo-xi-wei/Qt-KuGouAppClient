/**
 * @file VideoChannelBlock.h
 * @brief 定义 VideoChannelBlock 类，提供视频频道块控件
 * @author WeiWang
 * @date 2024-12-12
 * @version 1.0
 */

#ifndef VIDEOCHANNELBLOCK_H
#define VIDEOCHANNELBLOCK_H

#include <QWidget>
#include <QMouseEvent>

class QLabel;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui { class VideoChannelBlock; }
QT_END_NAMESPACE

/**
 * @class VideoChannelBlock
 * @brief 视频频道块控件类，显示封面、文本和描述
 */
class VideoChannelBlock : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化视频频道块控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit VideoChannelBlock(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~VideoChannelBlock() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置描述
     * @param description 描述文本
     */
    void setDescription(const QString &description);

    /**
     * @brief 设置封面文本
     * @param text 文本内容
     */
    void setCoverText(const QString &text) const;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 初始化封面文本字体
     */
    void initCoverTextFont();

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param watched 目标对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::VideoChannelBlock *ui;               ///< UI 指针
    QLabel                *m_coverTextLab;   ///< 封面文本标签
    QString                m_descriptionText; ///< 描述文本
    QFont                  m_coverTextFont;  ///< 封面文本字体
};

#endif // VIDEOCHANNELBLOCK_H