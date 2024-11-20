#ifndef ICONBESIDETEXTTOOLBUTTON_H
#define ICONBESIDETEXTTOOLBUTTON_H

#include <QToolButton>

class IconBesideTextToolButton : public QToolButton {
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
    QSize m_enterIconSize;
    QSize m_leaveIconSize;
    int m_enterFontSize;
    int m_leaveFontSize;
    QColor m_hoverFontColor = QColor(255, 255, 255);
    bool m_changeSize = false;
    bool m_isHover = false;
    bool m_isApproach = false;
    //水平方向左右两边边距
    int m_padding = 15;
    //hover图标
    QIcon m_enterIcon = {};
    QIcon m_leaveIcon = {};
};


#endif //ICONBESIDETEXTTOOLBUTTON_H
