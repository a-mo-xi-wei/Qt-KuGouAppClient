/**
 * @file MyBlockWidget.h
 * @brief 定义 MyBlockWidget 类，提供块状控件功能
 * @author WeiWang
 * @date 24-11-23
 * @version 1.0
 */
/*遵循一个原则：需要接口就自己添加*/

#ifndef MYBLOCKWIDGET_H
#define MYBLOCKWIDGET_H

#include "SMaskWidget.h"

#if defined(MYBLOCKWIDGET_LIBRARY)
#define MYBLOCKWIDGET_EXPORT Q_DECL_EXPORT
#else
#define MYBLOCKWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QLabel;
class QToolButton;

/**
 * @class MyBlockWidget
 * @brief 块状控件类，支持遮罩、提示标签和流行度按钮
 */
class MYBLOCKWIDGET_EXPORT MyBlockWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化块状控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MyBlockWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置边框图片
     * @param path 图片路径
     * @param border 圆角半径，默认为 8
     */
    void setBorderImage(const QString &path, const int &border = 8) const;

    /**
     * @brief 设置提示标签文本
     * @param text 文本内容
     */
    void setTipLabText(const QString &text) const;

    /**
     * @brief 设置流行度按钮方向
     * @param direction 方向（0:无，1:左，2:右）
     */
    void setPopularDirection(const int &direction);

    /**
     * @brief 设置流行度按钮文本
     * @param text 文本内容
     */
    void setPopularBtnText(const QString &text) const;

    /**
     * @brief 设置是否显示提示标签
     * @param show 是否显示，默认为 true
     */
    void setShowTip(bool show = true) const;

    /**
     * @brief 设置是否扩展响应范围
     * @param expandRespond 是否扩展
     */
    void setExpandRespond(const bool &expandRespond);

    /**
     * @brief 设置右侧流行度按钮图标
     * @param icon 图标路径
     */
    void setRightPopularBtnIcon(const QString &icon) const;

    /**
     * @brief 设置左侧流行度按钮图标
     * @param icon 图标路径
     */
    void setLeftPopularBtnIcon(const QString &icon);

    /**
     * @brief 设置右侧流行度按钮宽度
     * @param width 宽度
     */
    void setRightPopularBtnWidth(const int &width) const;

    /**
     * @brief 设置左侧流行度按钮宽度
     * @param width 宽度
     */
    void setLeftPopularBtnWidth(const int &width) const;

    /**
     * @brief 设置右侧流行度按钮字体大小
     * @param size 字体大小
     */
    void setRightPopularBtnFontSize(const int &size) const;

    /**
     * @brief 设置左侧流行度按钮字体大小和加粗
     * @param size 字体大小
     * @param isBold 是否加粗
     */
    void setLeftPopularBtnFontSize(const int &size, const bool &isBold) const;

    /**
     * @brief 设置宽高比
     * @param aspectRatio 宽高比
     */
    void setAspectRatio(const float &aspectRatio);

    /**
     * @brief 设置流行度按钮左边距
     * @param leftPadding 左边距
     */
    void setPopularBtnLeftPadding(const int &leftPadding);

    /**
     * @brief 设置是否带单位（万）
     * @param haveNumberUnit 是否带单位
     */
    void setHaveNumberUnit(const bool &haveNumberUnit);

    /**
     * @brief 设置提示标签数组
     * @param tipArr 提示标签数组
     */
    void setTipArr(const std::vector<QString> &tipArr);

    /**
     * @brief 设置提示标签样式
     * @param style 样式表
     */
    void setTipStyleSheet(const QString &style) const;

    /**
     * @brief 显示时长按钮
     */
    void setDurationBtnShow() const;

    /**
     * @brief 设置时长按钮文本
     * @param text 文本内容
     */
    void setDurationBtnText(const QString &text) const;

    /**
     * @brief 获取遮罩控件
     * @return 遮罩控件引用
     */
    SMaskWidget &getMask() const;

public slots:
    /**
     * @brief 显示遮罩
     */
    void onShowMask();

    /**
     * @brief 隐藏遮罩
     */
    void onHideMask();

private:
    /**
     * @brief 初始化用户界面
     */
    void initUi();

    /**
     * @brief 初始化提示标签数组
     */
    void initTipArr();

protected:
    /**
     * @brief 绘制事件
     * @param ev 绘图事件对象
     */
    void paintEvent(QPaintEvent *ev) override;

    /**
     * @brief 鼠标进入事件
     * @param ev 进入事件对象
     */
    void enterEvent(QEnterEvent *ev) override;

    /**
     * @brief 鼠标离开事件
     * @param ev 事件对象
     */
    void leaveEvent(QEvent *ev) override;

    /**
     * @brief 大小调整事件
     * @param event 大小调整事件对象
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件对象
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件对象
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件对象
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QWidget*                m_bacWidget{};          ///< 背景控件
    std::unique_ptr<SMaskWidget> m_mask;           ///< 遮罩控件
    QLabel*                 m_tipLab{};             ///< 提示标签
    int                     m_popularDirection = 0; ///< 流行度按钮方向（0:无，1:左，2:右）
    QToolButton*            m_rightPopularBtn{};    ///< 右侧流行度按钮
    QToolButton*            m_leftPopularBtn{};     ///< 左侧流行度按钮
    QToolButton*            m_durationBtn{};        ///< 时长按钮
    QString                 m_rightPopularBtnStyle; ///< 右侧流行度按钮样式
    QString                 m_leftPopularBtnStyle;  ///< 左侧流行度按钮样式
    bool                    m_isHoverCover = false; ///< 是否悬停在封面上
    std::vector<QString>    m_tipArr;              ///< 提示标签数组
    bool                    m_isExpandRespond = false; ///< 是否扩展响应范围
    float                   m_aspectRatio = 1;     ///< 宽高比
    bool                    m_haveUnit = true;     ///< 是否带单位（万）
};

#endif // MYBLOCKWIDGET_H