/**
 * @file ListenOptionMenu.h
 * @brief 定义 ListenOptionMenu 类，提供听单选项菜单功能
 * @author WeiWang
 * @date 2025-02-03
 * @version 1.0
 */

#ifndef LISTENOPTIONMENU_H
#define LISTENOPTIONMENU_H

#include "../BaseMenu.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

class QFrame;

/**
 * @class ListenOptionMenu
 * @brief 听单选项菜单类，继承自 BaseMenu，提供分类音频内容选择
 */
class UIWIDGETS_EXPORT ListenOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化听单选项菜单
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListenOptionMenu(QWidget *parent = nullptr);

private:
    /**
     * @brief 初始化菜单布局和内容
     */
    void initMenu() override;

    /**
     * @brief 初始化有声小说分类窗口
     * @param widget 目标控件
     */
    void initAudioNovelWidget(QWidget* widget);

    /**
     * @brief 初始化儿童天地分类窗口
     * @param widget 目标控件
     */
    void initChildrenWorldWidget(QWidget* widget);

    /**
     * @brief 初始化评书分类窗口
     * @param widget 目标控件
     */
    void initCommentBookWidget(QWidget* widget);

    /**
     * @brief 初始化助眠解压分类窗口
     * @param widget 目标控件
     */
    void initSleepHelpingWidget(QWidget* widget);

    /**
     * @brief 初始化人文分类窗口
     * @param widget 目标控件
     */
    void initHumanityWidget(QWidget* widget);

    /**
     * @brief 初始化自我充电分类窗口
     * @param widget 目标控件
     */
    void initSelfChargingWidget(QWidget* widget);

    /**
     * @brief 初始化相声曲艺分类窗口
     * @param widget 目标控件
     */
    void initCrosstalkWidget(QWidget* widget);

    /**
     * @brief 初始化情感生活分类窗口
     * @param widget 目标控件
     */
    void initSentimentalityWidget(QWidget* widget);

    /**
     * @brief 初始化广播剧分类窗口
     * @param widget 目标控件
     */
    void initRadioDramaWidget(QWidget* widget);

    /**
     * @brief 初始化娱乐段子分类窗口
     * @param widget 目标控件
     */
    void initEntertainmentJokesWidget(QWidget* widget);

    /**
     * @brief 初始化二次元分类窗口
     * @param widget 目标控件
     */
    void initACGNWidget(QWidget* widget);

    /**
     * @brief 初始化播客分类窗口
     * @param widget 目标控件
     */
    void initPodcastWidget(QWidget* widget);

    /**
     * @brief 初始化粤语分类窗口
     * @param widget 目标控件
     */
    void initCantoneseWidget(QWidget* widget);

    /**
     * @brief 初始化外语分类窗口
     * @param widget 目标控件
     */
    void initForeignLanguageWidget(QWidget* widget);

    /**
     * @brief 初始化创作翻唱分类窗口
     * @param widget 目标控件
     */
    void initCreateCoverWidget(QWidget* widget);

    /**
     * @brief 初始化DJ电音分类窗口
     * @param widget 目标控件
     */
    void initElectronicSoundWidget(QWidget* widget);

    /**
     * @brief 初始化水平分隔线
     * @param frame 分隔线控件
     */
    void initSeparator(QFrame* frame);

    /**
     * @brief 获取当前菜单对象
     * @return 当前菜单对象指针
     */
    const ListenOptionMenu* getMenu() const override;

protected:
    /**
     * @brief 事件过滤器，处理鼠标点击事件
     * @param watched 事件目标对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    /**
     * @brief 菜单项点击信号
     * @param funcName 点击的功能名称
     */
    void clickedFuncName(const QString& funcName);
};

#endif // LISTENOPTIONMENU_H