#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QWidget>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QWheelEvent>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

class ElaToolTip;

/*此处重写的QWidget的唯一目的就是作为中转传递信号。。。。*/
class MyWidget : public QWidget {
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr)
        : QWidget(parent)
          , m_effect(std::make_unique<QGraphicsDropShadowEffect>(this)) {
        this->setContentsMargins(0, 0, 0, 10);
        setFixedSize(64, 198);
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground); {
            //设置样式
            QFile file(GET_CURRENT_DIR + QStringLiteral("/slider.css"));
            if (file.open(QIODevice::ReadOnly)) {
                this->setStyleSheet(file.readAll());
            } else {
                qDebug() << "样式表打开失败QAQ";
                return;
            }
        }
        this->m_effect->setColor(QColor(80, 80, 80));
        this->m_effect->setOffset(0, 0);
        this->m_effect->setBlurRadius(30);
        this->setGraphicsEffect(this->m_effect.get());
    }

protected:
    void paintEvent(QPaintEvent *ev) override {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QColor(Qt::transparent));
        p.setBrush(QColor(QStringLiteral("#edf2ff")));
        p.drawRoundedRect(rect().x(), rect().y(), 64, 190, 6, 6);
        QPainterPath path;
        path.moveTo(rect().x() + 22, rect().bottom() - 8);
        path.lineTo(rect().x() + 32, rect().bottom());
        path.lineTo(rect().x() + 42, rect().bottom() - 8);
        p.drawPath(path);
    }

    void wheelEvent(QWheelEvent *event) override {
        QWidget::wheelEvent(event);
        this->m_delta = event->angleDelta().y();
        emit volumeChanged(this->m_delta);
    }

signals:
    void noVolume(const bool &flag);

    void volumeChanged(const int &value);

private:
    std::unique_ptr<QGraphicsDropShadowEffect> m_effect;
    int m_delta;
};

class SliderWidget : public QSlider {
    Q_OBJECT

public:
    explicit SliderWidget(QWidget *parent = nullptr, Qt::Orientation orientation = Qt::Horizontal);

    const int getValue() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void showEvent(QShowEvent *event) override;

public slots:
    void onNoVolume(bool flag);

private slots:
    void handleDelta(const int& delta);

signals:
    void noVolume(bool flag);

public:
    bool m_isPressing = false;

private:
    int m_minValue;
    int m_maxValue;
    int m_currentValue;
    ElaToolTip* m_sliderToolTip;

    MyWidget *m_parent;
};


#endif //SLIDERWIDGET_H
