//
// Created by WeiWang on 25-1-12.
//

#ifndef BASEMENU_H
#define BASEMENU_H

//策略基类

#include "MenuBtn.hpp"
#include <QMenu>

class QTimeLine;
class QGraphicsOpacityEffect;
class QWidgetAction;

#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

class MYMENU_EXPORT BaseMenu: public QMenu {
    Q_OBJECT

public:
    BaseMenu(QWidget *parent = nullptr);

    virtual ~BaseMenu() = default;

    virtual void initMenu(){} // 定义菜单初始化接口

    virtual const BaseMenu *getMenu() const;

    virtual void setShadow(const int& width,const int& radius,const QColor& color){};

    void setCurIndex(const int &idx); //设置当前响应下标

    void setAniDuration(const int& duration);

    QString getStyleSheet() const;

protected:
    void checkHover();

    void checkSelection() const;

    void paintEvent(QPaintEvent *event) override;

    void showEvent(QShowEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void connectAction(const QWidgetAction *widgetAction , MenuBtn* btn);

protected:
    //为了解决QWidgetAction没有与hovered相反的离开事件设置下面两个变量
    QVector<QWidget *> m_lastHover{};
    QVector<QWidget *> m_currentHover{};

    //为了解决顺序图标只能显示一个的问题
    MenuBtn *m_lastSelect{};
    MenuBtn *m_curSelect{};

    //item menu 相关
    int m_curIndex = -1; //当前让menu显示的那item的index

    int m_shadowWidth = 4;
    int m_shadowRadius = 12;
    QColor m_shadowColor = QColor(150,150,150,55);
    //显示动画
    QGraphicsOpacityEffect *m_opacityEffect{};
    QTimeLine *m_timeLine{};
};

#endif //BASEMENU_H
