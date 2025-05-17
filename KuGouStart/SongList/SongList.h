/**
* @file SongList.h
 * @brief 歌曲列表界面的头文件，定义 SongList 类
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef SONGLIST_H
#define SONGLIST_H

#include "ListOptionMenu/ListOptionMenu.h"
#include <QWidget>
#include <vector>

/**
 * @namespace Ui
 * @brief UI 命名空间，包含歌曲列表界面的 UI 类
 */
QT_BEGIN_NAMESPACE
namespace Ui { class SongList; }
QT_END_NAMESPACE

/**
 * @class SongList
 * @brief 歌曲列表界面类，管理歌曲块和菜单
 */
class SongList : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SongList(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     * @note 释放 UI 资源
     */
    ~SongList() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置按钮、布局和歌曲块
     */
    void initUi();

    /**
     * @brief 初始化封面库
     * @note 加载封面图片
     */
    void initCoverVector();

    /**
     * @brief 初始化描述库
     * @note 加载描述文本
     */
    void initDescVector();

    /**
     * @brief 打乱数据
     * @note 随机打乱封面和描述列表
     */
    void shuffleVector();

    private slots:
        /**
         * @brief 全部按钮点击槽函数
         * @note 显示菜单并切换图标
         */
        void on_all_toolButton_clicked();

    /**
     * @brief 菜单功能点击槽函数
     * @param funcName 功能名称
     * @note 显示未实现提示
     */
    void onMenuFuncClicked(const QString &funcName);

private:
    Ui::SongList *ui;                   ///< UI 界面指针
    std::vector<QString> m_coverVector; ///< 封面图片列表
    std::vector<QString> m_descVector;  ///< 描述文本列表
    ListOptionMenu *m_menu{};           ///< 选项菜单
};

#endif // SONGLIST_H