#ifndef GLTABWIDGET_H
#define GLTABWIDGET_H

#include <QWidget>
class QLabel;
class QToolButton;
class QPaintEvent;

class GLTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GLTabWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* ev)override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void initUi();
    void layoutUi();

private:
    QLabel* m_glLab{};
    QLabel* m_songNameLab{};
    QToolButton* m_playToolBtn{};
    QToolButton* m_nextToolBtn{};
    QToolButton* m_likeToolBtn{};
    QToolButton* m_dislikeToolBtn{};

};

#endif // GLTABWIDGET_H
