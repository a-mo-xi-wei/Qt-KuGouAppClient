#ifndef ADVERTISEBOARD_H
#define ADVERTISEBOARD_H

#include <QWidget>
#include<QList>
#include<QLabel>

class QPixmap;
class QTimer;
class MyLLabel : public QLabel {
    Q_OBJECT

public:
    MyLLabel(QWidget* parent = nullptr) : QLabel(parent) {
        setMouseTracking(true);
    }
signals:
    void clicked();
protected:
    void enterEvent(QEnterEvent* event) override {
        setPixmap(QPixmap(QStringLiteral(":/Res/window/left-pink.svg"))); // 悬停时的图片
        QLabel::enterEvent(event);
    }

    void leaveEvent(QEvent* event) override {
        setPixmap(QPixmap(QStringLiteral(":/Res/window/left.svg"))); // 悬停时的图片
        QLabel::leaveEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override {
        emit clicked();
        QLabel::mousePressEvent(event);
    }
};
class MyRLabel : public QLabel {
    Q_OBJECT

public:
    MyRLabel(QWidget* parent = nullptr) : QLabel(parent) {
        setMouseTracking(true);
    }
signals:
    void clicked();

protected:
    void enterEvent(QEnterEvent* event) override {
        setPixmap(QPixmap(QStringLiteral(":/Res/window/right-pink.svg"))); // 悬停时的图片
        QLabel::enterEvent(event);
    }

    void leaveEvent(QEvent* event) override {
        setPixmap(QPixmap(QStringLiteral(":/Res/window/right.svg"))); // 悬停时的图片
        QLabel::leaveEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override {
        emit clicked();
        QLabel::mousePressEvent(event);
    }
};

class AdvertiseBoard : public QWidget
{
    Q_OBJECT
public:
    explicit AdvertiseBoard(QWidget *parent = nullptr);
    ~AdvertiseBoard() override;
public:
    void addPoster(const QPixmap& posterPix);
    void updateLabPosition();
protected:
    void paintEvent(QPaintEvent* ev)override;
    void resizeEvent(QResizeEvent* ev)override;
    void enterEvent(QEnterEvent* ev)override;
    void leaveEvent(QEvent* ev)override;

private:
    QList<QPixmap*>m_posters;
    MyLLabel* m_leftLab{};
    MyRLabel* m_rightLab{};
    QTimer* m_timer{};
};


#endif // ADVERTISEBOARD_H
