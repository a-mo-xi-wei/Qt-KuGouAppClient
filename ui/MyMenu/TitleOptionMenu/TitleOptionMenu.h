/**
 * @file TitleOptionMenu.h
 * @brief 定义 TitleOptionMenu 类，提供标题栏选项菜单功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#ifndef TITLEOPTIONMENU_H
#define TITLEOPTIONMENU_H

#include "../BaseMenu.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class TitleOptionMenu
 * @brief 标题栏选项菜单类，继承自 BaseMenu，提供壁纸、设置、退出等功能
 */
class MYMENU_EXPORT TitleOptionMenu : public BaseMenu
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化标题栏选项菜单
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit TitleOptionMenu(QWidget* parent = nullptr);

    /**
     * @brief 初始化菜单布局和内容
     */
    void initMenu() override;

    /**
     * @brief 获取当前菜单对象
     * @return 当前菜单对象指针
     */
    const TitleOptionMenu* getMenu() const override;

signals:
    /**
     * @brief 设置动态壁纸信号
     */
    void wallpaper();

    /**
     * @brief 手机播放信号
     */
    void phonePlay();

    /**
     * @brief 传歌到设备信号
     */
    void uploadToDevice();

    /**
     * @brief 听歌赚金币信号
     */
    void earnCoin();

    /**
     * @brief 音乐遥控器信号
     */
    void controller();

    /**
     * @brief 均衡器信号
     */
    void balance();

    /**
     * @brief AI帮你唱信号
     */
    void aiHelpYou();

    /**
     * @brief 音效插件信号
     */
    void soundPlugin();

    /**
     * @brief 定时设置信号
     */
    void timeSetting();

    /**
     * @brief 应用工具信号
     */
    void appTool();

    /**
     * @brief 恢复窗口信号
     */
    void restoreWindow();

    /**
     * @brief 更新信息信号
     */
    void updateInfo();

    /**
     * @brief 关于应用信号
     */
    void about();

    /**
     * @brief 设置信号
     */
    void settings();

    /**
     * @brief 退出登录信号
     */
    void logOut();

    /**
     * @brief 退出应用信号
     */
    void exit();
};

#endif // TITLEOPTIONMENU_H
