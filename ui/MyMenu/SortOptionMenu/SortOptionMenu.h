/**
* @file SortOptionMenu.h
 * @brief 定义 SortOptionMenu 类，提供歌曲排序选项菜单功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#ifndef SORTOPTIONMENU_H
#define SORTOPTIONMENU_H

#include <QPointer>

#include "../BaseMenu.h"
#include "../MenuBtn.hpp"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class SortOptionMenu
 * @brief 歌曲排序选项菜单类，继承自 BaseMenu，提供多种排序方式
 */
class MYMENU_EXPORT SortOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化歌曲排序选项菜单
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SortOptionMenu(QWidget *parent = nullptr);

    /**
     * @brief 获取当前菜单对象
     * @return 当前菜单对象指针
     */
    const SortOptionMenu* getMenu() const override;

    /**
     * @brief 初始化菜单布局和内容
     */
    void initMenu() override;

    /**
     * @brief 再次触发当前排序按钮的点击事件
     */
    void btnClickAgain() const;

    signals:
        /**
         * @brief 排序选项被选中信号
         */
        void selected();

    /**
     * @brief 取消选中排序选项信号
     */
    void deselected();

    /**
     * @brief 默认排序信号
     * @param down 是否降序，默认为 true
     */
    void defaultSort(const bool &down = true);

    /**
     * @brief 添加时间排序信号
     * @param down 是否降序，默认为 true
     */
    void addTimeSort(const bool &down = true);

    /**
     * @brief 歌曲名排序信号
     * @param down 是否降序，默认为 true
     */
    void songNameSort(const bool &down = true);

    /**
     * @brief 歌手排序信号
     * @param down 是否降序，默认为 true
     */
    void singerSort(const bool &down = true);

    /**
     * @brief 时长排序信号
     * @param down 是否降序，默认为 true
     */
    void durationSort(const bool &down = true);

    /**
     * @brief 播放次数排序信号
     * @param down 是否降序，默认为 true
     */
    void playCountSort(const bool &down = true);

    /**
     * @brief 随机排序信号
     */
    void randomSort();

private:
    QPointer<MenuBtn> m_sortBtn;; ///< 记录当前选中的排序按钮
};

#endif // SORTOPTIONMENU_H