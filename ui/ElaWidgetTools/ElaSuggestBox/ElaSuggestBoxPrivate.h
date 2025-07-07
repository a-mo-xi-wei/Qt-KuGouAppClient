/**
 * @file ElaSuggestBoxPrivate.h
 * @brief 定义 ElaSuggestBoxPrivate 类，搜索建议框的私有实现
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#ifndef ELASUGGESTBOXPRIVATE_H
#define ELASUGGESTBOXPRIVATE_H

#include <QAction>

#include "Def.h"
#include "stdafx.h"
#include "libhttp.h"

class ElaSuggestion : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(ElaIconType::IconName, ElaIcon)        ///< 建议项图标
    Q_PROPERTY_CREATE(QString, SuggestText)                  ///< 建议项文本
    Q_PROPERTY_CREATE(QString, SuggestKey)                   ///< 建议项唯一键
    Q_PROPERTY_CREATE(QVariantMap, SuggestData)              ///< 建议项数据

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaSuggestion(QObject *parent = nullptr);

};

class QVBoxLayout;
class ElaLineEdit;
class ElaNavigationNode;
class ElaSuggestModel;
class ElaBaseListView;
class ElaSuggestDelegate;
class ElaSuggestBox;
class ElaSuggestBoxSearchViewContainer;

/**
 * @class ElaSuggestBoxPrivate
 * @brief 搜索建议框的私有实现，管理建议项和界面
 */
class ElaSuggestBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSuggestBox)                                ///< 声明控件指针

    Q_PROPERTY_CREATE_D(int, BorderRadius)                   ///< 边框圆角半径
    Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)///< 搜索大小写敏感性

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaSuggestBoxPrivate(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaSuggestBoxPrivate() override;

    /**
     * @brief 主题模式变化槽函数
     * @param themeMode 主题模式
     * @note 更新搜索图标
     */
    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);

    /**
     * @brief 搜索编辑框文本编辑槽函数
     * @param searchText 搜索文本
     * @note 根据文本获取建议项并显示
     */
    Q_SLOT void onSearchEditTextEdit(const QString &searchText);

    /**
     * @brief 搜索视图点击槽函数
     * @param index 模型索引
     * @note 处理建议项点击
     */
    Q_SLOT void onSearchViewClicked(const QModelIndex &index);

private slots:
    /**
     * @brief 搜索编辑框宽度变化槽函数
     * @note 更新建议框位置和尺寸
     */
    void onSearchEditWidthChanged();

private:
    ElaThemeType::ThemeMode _themeMode;                      ///< 当前主题模式
    QAction *_lightSearchAction{nullptr};                    ///< 亮色搜索图标动作
    QAction *_darkSearchAction{nullptr};                     ///< 暗色搜索图标动作
    QVector<QSharedPointer<ElaSuggestion>> _suggestionVector;         ///< 建议项列表
    ElaSuggestBoxSearchViewContainer *_searchViewBaseWidget{nullptr}; ///< 建议框容器
    ElaLineEdit *_searchEdit{nullptr};                       ///< 搜索编辑框
    ElaSuggestModel *_searchModel{nullptr};                  ///< 建议项模型
    ElaBaseListView *_searchView{nullptr};                   ///< 建议项视图
    ElaSuggestDelegate *_searchDelegate{nullptr};            ///< 建议项委托
    QVBoxLayout *_shadowLayout{nullptr};                     ///< 阴影布局
    QSize _lastSize;                                         ///< 上次尺寸
    bool _isExpandAnimationFinished{true};                   ///< 展开动画完成标志
    bool _isCloseAnimationFinished{true};                    ///< 关闭动画完成标志
    bool m_ignoreTextChanges = false;                        ///< 忽略文本变化标志

    /**
     * @brief 启动尺寸动画
     * @param oldSize 旧尺寸
     * @param newSize 新尺寸
     */
    void _startSizeAnimation(QSize oldSize, QSize newSize);

    /**
     * @brief 启动展开动画
     */
    void _startExpandAnimation();

    /**
     * @brief 启动关闭动画
     */
    void _startCloseAnimation();

    /**
     * @brief 执行尺寸动画
     */
    void _doStartSizeAnimation();

    /*---------------*/
    QSize _pendingSize;                                      ///< 待处理的目标尺寸
    QTimer* _animationTimer;                                 ///< 延迟启动动画的定时器

    CLibhttp                             m_libHttp;                 ///< HTTP 请求库
};

#endif // ELASUGGESTBOXPRIVATE_H