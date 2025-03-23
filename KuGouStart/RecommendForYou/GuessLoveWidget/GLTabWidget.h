#ifndef GLTABWIDGET_H
#define GLTABWIDGET_H

#include "GLOptionMenu/GLOptionMenu.h"
#include <QWidget>

class IconBesideTextToolButton;
class QLabel;
class QToolButton;
class QPaintEvent;

class GLTabWidget : public QWidget {
    Q_OBJECT

public:
    explicit GLTabWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    void initUi();

    void layoutUi();

private slots:
    void onModelBtnClicked();

    void onGetModel(const QString& model);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QLabel *m_glLab{};
    QLabel *m_songNameLab{};
    QLabel *m_singerLab{};
    IconBesideTextToolButton* m_modelBtn{};
    GLOptionMenu* m_modelMenu{};
    QToolButton *m_playToolBtn{};
    QToolButton *m_nextToolBtn{};
    QToolButton *m_likeToolBtn{};
    QToolButton *m_dislikeToolBtn{};
};

#endif // GLTABWIDGET_H
