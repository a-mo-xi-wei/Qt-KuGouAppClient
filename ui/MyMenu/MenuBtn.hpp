/**
* @file MenuBtn.hpp
 * @brief 定义 MenuBtn 类，提供自定义菜单按钮功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#ifndef MENUBTN_H
#define MENUBTN_H

#include <QToolButton>
#include <QEvent>

/**
 * @class MenuBtn
 * @brief 自定义菜单按钮类，继承自 QToolButton，支持图标切换和事件过滤
 */
class MenuBtn : public QToolButton {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化菜单按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MenuBtn(QWidget *parent = nullptr): QToolButton(parent) {
        this->setIconSize(QSize(15, 15));
        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        this->installEventFilter(this);
    }

    /**
     * @brief 初始化按钮的正常和悬停状态图标
     * @param icon1 正常状态图标
     * @param icon2 悬停状态图标
     */
    void initIcon(const QIcon &icon1, const QIcon &icon2) {
        this->m_icon1 = icon1;
        this->m_icon2 = icon2;
    }

    /**
     * @brief 移除事件过滤器
     */
    void removeFilter() {
        this->removeEventFilter(this);
    }

protected:
    /**
     * @brief 重写事件过滤器，处理进入和离开事件以切换图标
     * @param watched 监视的对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::Enter) {
            //qDebug()<<"进入";
            this->setIcon(m_icon2);
        } else if (event->type() == QEvent::Leave) {
            //qDebug()<<"离开";
            this->setIcon(m_icon1);
        }
        return QToolButton::eventFilter(watched, event);
    }

private:
    QIcon m_icon1{}; ///< 正常状态图标
    QIcon m_icon2{}; ///< 悬停状态图标
public:
    int m_cnt = 0;   ///< 记录排序状态的计数器（如在 SortOptionMenu 中用于升降序切换）
};

#endif // MENUBTN_H