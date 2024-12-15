//
// Created by WeiWang on 24-11-23.
//

#ifndef MYBLOCKWIDGET_H
#define MYBLOCKWIDGET_H

#include"SMaskWidget.h"

#include <QWidget>

class QLabel;
class QToolButton;

class MyBlockWidget : public QWidget {
    Q_OBJECT

public:
    explicit MyBlockWidget(QWidget *parent = nullptr);

    void setBorderImage(const QString &path,const int& border = 8)const;

    void setTipLabText(const QString &text)const;

    void setPopularDirection(const int &direction);

    void setPopularBtnText(const QString &text)const;

    void setShowTip(const bool &show)const;

    void setExpandRespond(const bool &expandRespond);

    void setRightPopularBtnIcon(const QString& icon)const;

    void setLeftPopularBtnIcon(const QString& icon)const;

    void setAspectRatio(const float &aspectRatio);

    void setPopularBtnLeftPadding(const int &leftPadding);

    void setHaveNumberUnit(const bool &haveNumberUnit);

    void setTipArr(const std::vector<QString>& tipArr);

    void setTipStyleSheet(const QString& style);

    SMaskWidget &getMask()const;

public slots:
    void onShowMask();

    void onHideMask();

private:
    void initUi();

    void initTipArr();

protected:
    void paintEvent(QPaintEvent *ev) override;

    void enterEvent(QEnterEvent *ev) override;

    void leaveEvent(QEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QWidget *m_bacWidget{};
    std::unique_ptr<SMaskWidget> m_mask;
    QLabel *m_tipLab{};
    int m_popularDirection = 0;     //0 没有 ， 1 左边 ， 2 右边
    QToolButton *m_rightPopularBtn{};
    QToolButton *m_leftPopularBtn{};
    QString m_rightPopularBtnStyle;
    QString m_leftPopularBtnStyle;
    bool m_isHoverCover = false;    //是否悬停在图像上
    std::vector<QString> m_tipArr;
    bool m_isExpandRespond = false; //是否扩展响应，即是否让遮罩出现的响应范围扩大 信号响应
    float m_aspectRatio = 1;          //宽高比
    bool m_haveUnit = true;         //是否带单位（万）

};
#endif //MYBLOCKWIDGET_H
