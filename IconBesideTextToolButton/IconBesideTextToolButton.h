#ifndef ICONBESIDETEXTTOOLBUTTON_H
#define ICONBESIDETEXTTOOLBUTTON_H

#include <QToolButton>

class IconBesideTextToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit IconBesideTextToolButton(QWidget *parent = nullptr);
    void setEnterIconSize(const QSize& size);
    void setLeaveIconSize(const QSize& size);
    void setEnterFontSize(const int& size);
    void setLeaveFontSize(const int& size);
protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
private:
    QSize m_enterIconSize;
    QSize m_leaveIconSize;
    int m_enterFontSize;
    int m_leaveFontSize;
};


#endif //ICONBESIDETEXTTOOLBUTTON_H
