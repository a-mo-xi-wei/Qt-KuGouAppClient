//
// Created by WeiWang on 24-10-19.
//

#ifndef MYWIDGETMENU_H
#define MYWIDGETMENU_H

#include <QEvent>
#include <QMenu>

#include <QToolButton>
class MenuBtn : public QToolButton
{
    Q_OBJECT
public:
    explicit MenuBtn( QWidget *parent = nullptr):QToolButton(parent) {
        this->setIconSize(QSize(15, 15));
        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        this->installEventFilter(this);
    }
    void initIcon(const QIcon& icon1,const QIcon& icon2) {
        this->m_icon1 = icon1;
        this->m_icon2 = icon2;
    }
    void removeFilter() {
        this->removeEventFilter(this);
    }
protected:
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
    QIcon m_icon1{};//leave
    QIcon m_icon2{};//enter
public:
    int m_cnt = 0; //记录之前的图片
};


class MyMenu : public QMenu
{
    Q_OBJECT

public:
    enum MenuKind {
        SongOptionMenu,
        SortOptionMenu,
        None
    };
    explicit MyMenu(const MenuKind& kind, QWidget *parent = nullptr);

    void initUi();

    void initSongOptionMenu();

    void initSortOptionMenu();

    void checkHover();

    void checkSelection();

signals:
    void play();
    void nextPlay();
    void addToPlayQueue();
    void addToNewSongList();
    void addToLove();
    void addToCollect();
    void addToPlayList();
    void download();
    void share();
    void comment();
    void sameSong();
    void songInfo();
    void deleteSong();
    void openInFile();
    void search();
    void upload();

    void selected();
    void deselected();

    void defaultSort(const bool& down = true);

    void addTimeSort(const bool& down = true);

    void songNameSort(const bool& down = true);

    void singerSort(const bool& down = true);

    void durationSort(const bool& down = true);

    void playCountSort(const bool& down = true);

    void randomSort();

protected:
    void paintEvent(QPaintEvent *event) override;

    void showEvent(QShowEvent *event) override;

private:
    //为了解决QWidgetAction没有与hovered相反的离开事件设置下面两个变量
    QVector<QWidget*> m_lastHover{};
    QVector<QWidget*> m_currentHover{};

    //为了解决顺序图标只能显示一个的问题
    MenuBtn* m_lastSelect{};
    MenuBtn* m_curSelect{};
};


#endif //MYWIDGETMENU_H
