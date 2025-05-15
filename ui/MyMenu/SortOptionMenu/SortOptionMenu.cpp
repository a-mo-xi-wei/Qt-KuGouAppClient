/**
 * @file SortOptionMenu.cpp
 * @brief 实现 SortOptionMenu 类，提供歌曲排序选项菜单功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#include "SortOptionMenu.h"

#include <QButtonGroup>
#include <QWidgetAction>

#include "logger.hpp"

/**
 * @brief 构造函数，初始化歌曲排序选项菜单
 * @param parent 父控件指针，默认为 nullptr
 */
SortOptionMenu::SortOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{
    this->setAniDuration(100); ///< 设置动画持续时间为 100 毫秒
}

/**
 * @brief 初始化菜单布局和内容
 */
void SortOptionMenu::initMenu() {
    this->setFixedSize(140, 280);

    // 定义连接悬停事件的 lambda 函数
    auto connectAction = [this](QWidgetAction* actionWidget, MenuBtn* btn) {
        connect(actionWidget, &QWidgetAction::hovered, this, [btn, this] {
            checkHover();
            this->m_currentHover.emplace_back(btn);
            this->m_lastHover = this->m_currentHover;
            btn->setAttribute(Qt::WA_UnderMouse, true); ///< 模拟按钮进入悬停状态
        });
    };

    // 创建按钮组，确保排序选项互斥
    auto buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    // 默认排序
    const auto a_sortByDefaultAction = new QWidgetAction(this);
    {
        auto a_sortByDefaultToolBtn = new MenuBtn(this);
        buttonGroup->addButton(a_sortByDefaultToolBtn);
        a_sortByDefaultToolBtn->setCheckable(true);
        a_sortByDefaultToolBtn->removeFilter(); ///< 移除默认事件过滤器
        a_sortByDefaultToolBtn->setFixedSize(120, 35);
        a_sortByDefaultToolBtn->setIconSize(QSize(18, 18));
        a_sortByDefaultToolBtn->setIcon(QIcon()); ///< 默认无图标
        a_sortByDefaultToolBtn->setText(QStringLiteral("  默认排序"));
        a_sortByDefaultAction->setDefaultWidget(a_sortByDefaultToolBtn);
        connect(a_sortByDefaultToolBtn, &QToolButton::clicked, this, [this, a_sortByDefaultToolBtn] {
            emit deselected(); ///< 发出取消选中信号
            emit defaultSort(); ///< 发出默认排序信号
            this->m_curSelect = a_sortByDefaultToolBtn;
            checkSelection(); ///< 检查并更新选中状态
            this->m_lastSelect = this->m_curSelect;
            this->m_sortBtn = a_sortByDefaultToolBtn; ///< 记录当前排序按钮
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_sortByDefaultAction, &QWidgetAction::hovered, this, [a_sortByDefaultToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByDefaultToolBtn);
            this->m_lastHover = this->m_currentHover;
            a_sortByDefaultToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sortByDefaultAction, a_sortByDefaultToolBtn);
    }

    // 添加时间排序
    const auto a_sortByAddTimeAction = new QWidgetAction(this);
    {
        auto a_sortByAddTimeToolBtn = new MenuBtn(this);
        buttonGroup->addButton(a_sortByAddTimeToolBtn);
        a_sortByAddTimeToolBtn->setCheckable(true);
        a_sortByAddTimeToolBtn->removeFilter();
        a_sortByAddTimeToolBtn->setFixedSize(120, 35);
        a_sortByAddTimeToolBtn->setIconSize(QSize(18, 18));
        a_sortByAddTimeToolBtn->setIcon(QIcon());
        a_sortByAddTimeToolBtn->setText(QStringLiteral("  添加时间"));
        a_sortByAddTimeAction->setDefaultWidget(a_sortByAddTimeToolBtn);
        connect(a_sortByAddTimeToolBtn, &QToolButton::clicked, this, [a_sortByAddTimeToolBtn, this] {
            emit selected(); ///< 发出选中信号
            this->m_curSelect = a_sortByAddTimeToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortByAddTimeToolBtn->icon().isNull()) {
                emit addTimeSort(true); ///< 发出降序排序信号
                a_sortByAddTimeToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
            } else {
                a_sortByAddTimeToolBtn->m_cnt = (a_sortByAddTimeToolBtn->m_cnt + 1) % 2;
                if (a_sortByAddTimeToolBtn->m_cnt & 1) {
                    emit addTimeSort(false); ///< 发出升序排序信号
                    a_sortByAddTimeToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortup.svg")));
                } else {
                    emit addTimeSort(true);
                    a_sortByAddTimeToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
                }
            }
            this->m_sortBtn = a_sortByAddTimeToolBtn;
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_sortByAddTimeAction, &QWidgetAction::hovered, this, [a_sortByAddTimeToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByAddTimeToolBtn);
            this->m_lastHover = this->m_currentHover;
            a_sortByAddTimeToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sortByAddTimeAction, a_sortByAddTimeToolBtn);
    }

    // 歌曲名排序
    const auto a_sortBySongNameAction = new QWidgetAction(this);
    {
        auto a_sortBySongNameToolBtn = new MenuBtn(this);
        buttonGroup->addButton(a_sortBySongNameToolBtn);
        a_sortBySongNameToolBtn->setCheckable(true);
        a_sortBySongNameToolBtn->removeFilter();
        a_sortBySongNameToolBtn->setFixedSize(120, 35);
        a_sortBySongNameToolBtn->setIconSize(QSize(18, 18));
        a_sortBySongNameToolBtn->setIcon(QIcon());
        a_sortBySongNameToolBtn->setText(QStringLiteral("  歌曲名    "));
        a_sortBySongNameAction->setDefaultWidget(a_sortBySongNameToolBtn);
        connect(a_sortBySongNameToolBtn, &QToolButton::clicked, this, [this, a_sortBySongNameToolBtn] {
            emit selected();
            this->m_curSelect = a_sortBySongNameToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortBySongNameToolBtn->icon().isNull()) {
                emit songNameSort(true);
                a_sortBySongNameToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
            } else {
                a_sortBySongNameToolBtn->m_cnt = (a_sortBySongNameToolBtn->m_cnt + 1) % 2;
                if (a_sortBySongNameToolBtn->m_cnt & 1) {
                    emit songNameSort(false);
                    a_sortBySongNameToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortup.svg")));
                } else {
                    emit songNameSort(true);
                    a_sortBySongNameToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
                }
            }
            this->m_sortBtn = a_sortBySongNameToolBtn;
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_sortBySongNameAction, &QWidgetAction::hovered, this, [a_sortBySongNameToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortBySongNameToolBtn);
            this->m_lastHover = this->m_currentHover;
            a_sortBySongNameToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sortBySongNameAction, a_sortBySongNameToolBtn);
    }

    // 歌手排序
    const auto a_sortBySingerAction = new QWidgetAction(this);
    {
        auto a_sortBySingerToolBtn = new MenuBtn(this);
        buttonGroup->addButton(a_sortBySingerToolBtn);
        a_sortBySingerToolBtn->setCheckable(true);
        a_sortBySingerToolBtn->removeFilter();
        a_sortBySingerToolBtn->setFixedSize(120, 35);
        a_sortBySingerToolBtn->setIconSize(QSize(18, 18));
        a_sortBySingerToolBtn->setIcon(QIcon());
        a_sortBySingerToolBtn->setText(QStringLiteral("  歌手       "));
        a_sortBySingerAction->setDefaultWidget(a_sortBySingerToolBtn);
        connect(a_sortBySingerToolBtn, &QToolButton::clicked, this, [this, a_sortBySingerToolBtn] {
            emit selected();
            this->m_curSelect = a_sortBySingerToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortBySingerToolBtn->icon().isNull()) {
                emit singerSort(true);
                a_sortBySingerToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
            } else {
                a_sortBySingerToolBtn->m_cnt = (a_sortBySingerToolBtn->m_cnt + 1) % 2;
                if (a_sortBySingerToolBtn->m_cnt & 1) {
                    emit singerSort(false);
                    a_sortBySingerToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortup.svg")));
                } else {
                    emit singerSort(true);
                    a_sortBySingerToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
                }
            }
            this->m_sortBtn = a_sortBySingerToolBtn;
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_sortBySingerAction, &QWidgetAction::hovered, this, [a_sortBySingerToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortBySingerToolBtn);
            this->m_lastHover = this->m_currentHover;
            a_sortBySingerToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sortBySingerAction, a_sortBySingerToolBtn);
    }

    // 时长排序
    const auto a_sortByDurationAction = new QWidgetAction(this);
    {
        auto a_sortByDurationToolBtn = new MenuBtn(this);
        buttonGroup->addButton(a_sortByDurationToolBtn);
        a_sortByDurationToolBtn->setCheckable(true);
        a_sortByDurationToolBtn->removeFilter();
        a_sortByDurationToolBtn->setFixedSize(120, 35);
        a_sortByDurationToolBtn->setIconSize(QSize(18, 18));
        a_sortByDurationToolBtn->setIcon(QIcon());
        a_sortByDurationToolBtn->setText(QStringLiteral("  时长       "));
        a_sortByDurationAction->setDefaultWidget(a_sortByDurationToolBtn);
        connect(a_sortByDurationToolBtn, &QToolButton::clicked, this, [this, a_sortByDurationToolBtn] {
            emit selected();
            this->m_curSelect = a_sortByDurationToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortByDurationToolBtn->icon().isNull()) {
                emit durationSort(true);
                a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
            } else {
                a_sortByDurationToolBtn->m_cnt = (a_sortByDurationToolBtn->m_cnt + 1) % 2;
                if (a_sortByDurationToolBtn->m_cnt & 1) {
                    emit durationSort(false);
                    a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortup.svg")));
                } else {
                    emit durationSort(true);
                    a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
                }
            }
            this->m_sortBtn = a_sortByDurationToolBtn;
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_sortByDurationAction, &QWidgetAction::hovered, this, [a_sortByDurationToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByDurationToolBtn);
            this->m_lastHover = this->m_currentHover;
            a_sortByDurationToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sortByDurationAction, a_sortByDurationToolBtn);
    }

    // 播放次数排序
    const auto a_sortByPlayCountAction = new QWidgetAction(this);
    {
        auto a_sortByPlayCountToolBtn = new MenuBtn(this);
        buttonGroup->addButton(a_sortByPlayCountToolBtn);
        a_sortByPlayCountToolBtn->setCheckable(true);
        a_sortByPlayCountToolBtn->removeFilter();
        a_sortByPlayCountToolBtn->setFixedSize(120, 35);
        a_sortByPlayCountToolBtn->setIconSize(QSize(18, 18));
        a_sortByPlayCountToolBtn->setIcon(QIcon());
        a_sortByPlayCountToolBtn->setText(QStringLiteral("  播放次数"));
        a_sortByPlayCountAction->setDefaultWidget(a_sortByPlayCountToolBtn);
        connect(a_sortByPlayCountToolBtn, &QToolButton::clicked, this, [this, a_sortByPlayCountToolBtn] {
            emit selected();
            this->m_curSelect = a_sortByPlayCountToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortByPlayCountToolBtn->icon().isNull()) {
                emit playCountSort(true);
                a_sortByPlayCountToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
            } else {
                a_sortByPlayCountToolBtn->m_cnt = (a_sortByPlayCountToolBtn->m_cnt + 1) % 2;
                if (a_sortByPlayCountToolBtn->m_cnt & 1) {
                    emit playCountSort(false);
                    a_sortByPlayCountToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortup.svg")));
                } else {
                    emit playCountSort(true);
                    a_sortByPlayCountToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/sortdown.svg")));
                }
            }
            this->m_sortBtn = a_sortByPlayCountToolBtn;
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_sortByPlayCountAction, &QWidgetAction::hovered, this, [a_sortByPlayCountToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByPlayCountToolBtn);
            this->m_lastHover = this->m_currentHover;
            a_sortByPlayCountToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sortByPlayCountAction, a_sortByPlayCountToolBtn);
    }

    // 随机排序
    const auto a_sortByRandomAction = new QWidgetAction(this);
    {
        auto a_sortByRandomToolBtn = new MenuBtn(this);
        buttonGroup->addButton(a_sortByRandomToolBtn);
        a_sortByRandomToolBtn->setCheckable(true);
        a_sortByRandomToolBtn->removeFilter();
        a_sortByRandomToolBtn->setFixedSize(120, 35);
        a_sortByRandomToolBtn->setIconSize(QSize(18, 18));
        a_sortByRandomToolBtn->setIcon(QIcon());
        a_sortByRandomToolBtn->setText(QStringLiteral("  随机排序"));
        a_sortByRandomAction->setDefaultWidget(a_sortByRandomToolBtn);
        connect(a_sortByRandomToolBtn, &QToolButton::clicked, this, [this, a_sortByRandomToolBtn] {
            emit selected();
            emit randomSort(); ///< 发出随机排序信号
            this->m_curSelect = a_sortByRandomToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            this->m_sortBtn = a_sortByRandomToolBtn;
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_sortByRandomAction, &QWidgetAction::hovered, this, [a_sortByRandomToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByRandomToolBtn);
            this->m_lastHover = this->m_currentHover;
            a_sortByRandomToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sortByRandomAction, a_sortByRandomToolBtn);
    }

    // 添加所有动作到菜单
    this->addAction(a_sortByDefaultAction);
    this->addAction(a_sortByAddTimeAction);
    this->addAction(a_sortBySongNameAction);
    this->addAction(a_sortBySingerAction);
    this->addAction(a_sortByDurationAction);
    this->addAction(a_sortByPlayCountAction);
    this->addAction(a_sortByRandomAction);
    this->hide();
}

/**
 * @brief 获取当前菜单对象
 * @return 当前菜单对象指针
 */
const SortOptionMenu *SortOptionMenu::getMenu() const {
    return this;
}

/**
 * @brief 再次触发当前排序按钮的点击事件
 */
void SortOptionMenu::btnClickAgain() const {
    if (this->m_sortBtn)
        this->m_sortBtn->clicked(); ///< 触发当前排序按钮的点击信号
}