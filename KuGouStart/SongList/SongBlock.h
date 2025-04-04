//
// Created by WeiWang on 25-1-20.
//

#ifndef SONGBLOCK_H
#define SONGBLOCK_H

#include "MyBlockWidget.h"

class QLabel;

class SongBlock : public QWidget {
Q_OBJECT

public:
    explicit SongBlock(QWidget *parent = nullptr);

    void setCoverPix(const QString &pixmapPath) const;

    void setDescText(const QString& text) const;

    void setShowTip() const;

    void setTipText(const QString &text) const;//可以直接使用默认的

private:
    void initUi();

    void updateDescLab() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void showEvent(QShowEvent *event) override;

private:
    MyBlockWidget* m_cover{};
    QLabel* m_descLab{};
};


#endif //SONGBLOCK_H
