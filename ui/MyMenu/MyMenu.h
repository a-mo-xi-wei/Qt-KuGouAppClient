/**
* @file MyMenu.h
 * @brief 定义 MyMenu 类，使用策略模式管理不同类型的菜单
 * @author WeiWang
 * @date 2024-10-19
 * @version 1.0
 */

#ifndef MYWIDGETMENU_H
#define MYWIDGETMENU_H

#include "BaseMenu.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

#define REGISTER_MENU(menuKind, menuClass)        \
namespace {                                    \
const bool menuClass##_registered = [](){      \
MyMenu::registerMenu(menuKind,             \
[](QWidget* parent){ return new menuClass(parent); }); \
return true;                               \
}();                                           \
}


/**
 * @class MyMenu
 * @brief 菜单管理类，使用策略模式动态创建和初始化不同类型的菜单
 */
class MYMENU_EXPORT MyMenu
{

public:
    /**
     * @brief 菜单类型枚举
     */
    enum class MenuKind
    {
        SongOption, ///< 歌曲选项菜单
        SortOption, ///< 排序选项菜单
        TitleOption, ///< 标题栏选项菜单
        ListOption, ///< 列表选项菜单
        ListenOption, ///< 收听选项菜单
        None ///< 无类型
    };

    ///< 菜单创建函数
    using MenuCreator = std::function<BaseMenu*(QWidget*)>;

    ///< 菜单注册函数
    static void registerMenu(MenuKind kind, const MenuCreator& creator);

    /**
     * @brief 构造函数，根据菜单类型初始化菜单
     * @param kind 菜单类型
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MyMenu(const MenuKind& kind, QWidget* parent = nullptr);

    ~MyMenu();

    /**
     * @brief 模板方法，获取具体类型的菜单对象
     * @tparam T 菜单类型
     * @return 指向具体菜单对象的指针
     */
    template <typename T>
    T* getMenu() const
    {
        return qobject_cast<T*>(this->m_menu);
    }

private:
    BaseMenu* m_menu{}; ///< 指向具体菜单的指针
    MenuKind m_kind; ///< 菜单类型

    static auto registry() -> QMap<MenuKind, MenuCreator>&; ///< 获取菜单注册表
};

#endif // MYWIDGETMENU_H
