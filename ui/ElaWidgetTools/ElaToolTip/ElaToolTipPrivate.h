/**
* @file ElaToolTipPrivate.h
 * @brief 定义 ElaToolTipPrivate 类，管理工具提示的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELATOOLTIPPRIVATE_H
#define ELATOOLTIPPRIVATE_H

#include "Def.h"

class ElaToolTip;
class QVBoxLayout;
class ElaText;

/**
 * @class ElaToolTipPrivate
 * @brief 工具提示私有实现类，管理事件过滤和动画
 */
class ElaToolTipPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToolTip)
    Q_PROPERTY_CREATE_D(int,      BorderRadius)
    Q_PROPERTY_CREATE_D(int,      DisplayMsec)
    Q_PROPERTY_CREATE_D(int,      ShowDelayMsec)
    Q_PROPERTY_CREATE_D(int,      HideDelayMsec)
    Q_PROPERTY_CREATE_D(QString,  ToolTip)
    Q_PROPERTY_CREATE_D(QWidget*, CustomWidget)
    Q_PROPERTY_CREATE(qreal,      Opacity)

public:
    /**
     * @brief 构造函数，初始化工具提示私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaToolTipPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放工具提示私有资源
     */
    ~ElaToolTipPrivate();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监视对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    /**
     * @brief 工具提示文本控件
     */
    ElaText*                 _toolTipText{nullptr};

    /**
     * @brief 主布局
     */
    QVBoxLayout*             _mainLayout{nullptr};

    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;

    /**
     * @brief 阴影边框宽度
     */
    int                      _shadowBorderWidth{6};

    /**
     * @brief 执行显示动画
     */
    void _doShowAnimation();

    /**
     * @brief 更新工具提示位置
     */
    void _updatePos();
};

#endif // ELATOOLTIPPRIVATE_H