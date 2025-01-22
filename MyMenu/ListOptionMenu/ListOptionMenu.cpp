//
// Created by WeiWang on 25-1-12.
//

#include "ListOptionMenu.h"

#include <QFile>
#include <QWidgetAction>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListOptionMenu::ListOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{
    {
        //设置样式
        QFile file(GET_CURRENT_DIR + QStringLiteral("/listmenu.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}
void ListOptionMenu::initMenu() {
    this->setFixedSize(600,400);
    /*
    //时长
    auto a_sortByDurationAction = new QWidgetAction(this); {
        auto a_sortByDurationToolBtn = new MenuBtn(this);
        a_sortByDurationToolBtn->removeFilter();
        a_sortByDurationToolBtn->setFixedSize(120, 35);
        a_sortByDurationToolBtn->setIconSize(QSize(18, 18));
        a_sortByDurationToolBtn->setIcon(QIcon());
        a_sortByDurationToolBtn->setText(QStringLiteral("  时长       "));
        a_sortByDurationAction->setDefaultWidget(a_sortByDurationToolBtn);
        connect(a_sortByDurationToolBtn, &QToolButton::clicked, this, [this,a_sortByDurationToolBtn] {
            emit selected();
            this->m_curSelect = a_sortByDurationToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortByDurationToolBtn->icon().isNull()) {
                emit durationSort(true);
                a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/sortdown.svg")));
            } else {
                a_sortByDurationToolBtn->m_cnt = (a_sortByDurationToolBtn->m_cnt + 1) % 2;
                if (a_sortByDurationToolBtn->m_cnt & 1) {
                    emit durationSort(false);
                    a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/sortup.svg")));
                } else {
                    emit durationSort(true);
                    a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/sortdown.svg")));
                }
            }
        });
        connect(a_sortByDurationAction, &QWidgetAction::hovered, this, [a_sortByDurationToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByDurationToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortByDurationToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //添加Action
    this->addAction(a_sortByDefaultAction);
    this->addAction(a_sortByAddTimeAction);
    this->addAction(a_sortBySongNameAction);
    this->addAction(a_sortBySingerAction);
    this->addAction(a_sortByDurationAction);
    this->addAction(a_sortByPlayCountAction);
    this->addAction(a_sortByRandomAction);
    this->hide();*/

}

const ListOptionMenu * ListOptionMenu::getMenu() const {
    return this;
}

