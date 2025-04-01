#ifndef ICONBESIDETEXTTOOLBUTTON_H
#define ICONBESIDETEXTTOOLBUTTON_H

#include <QToolButton>

#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

class MYBUTTON_EXPORT IconBesideTextToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit IconBesideTextToolButton(QWidget *parent = nullptr);

    void setEnterIconSize(const QSize &size);

    void setLeaveIconSize(const QSize &size);

    void setEnterFontSize(const int &size);

    void setLeaveFontSize(const int &size);

    void setChangeSize(const bool &change); //指定是否需要改变按钮内图标或文字大小

    void setHoverFontColor(const QColor &color);

    void setPadding(const int &padding);

    void setApproach(const bool &approach);

    void setEnterIcon(const QIcon &icon);

    void setLeaveIcon(const QIcon &icon);

protected:
    void paintEvent(QPaintEvent *event) override;

    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

private:
    QSize m_enterIconSize;  //进入时图标大小
    QSize m_leaveIconSize;  //离开时图标大小
    int m_enterFontSize;    //进入时字体大小
    int m_leaveFontSize;    //离开时字体大小
    QColor m_hoverFontColor = QColor(255, 255, 255);    //悬停时字体颜色
    bool m_changeSize = false;  //是否需要改变按钮内图标或文字大小
    bool m_isHover = false;     //判断是否悬停
    bool m_isApproach = false;  //是否靠近
    //水平方向左右两边边距
    int m_padding = 15;         //边距默认为15
    //hover图标
    QIcon m_enterIcon = {};     //悬停时的图标
    QIcon m_leaveIcon = {};     //离开时的图标
};


#endif //ICONBESIDETEXTTOOLBUTTON_H
