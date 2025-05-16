/**
 * @file SliderWidget.h
 * @brief 定义 SliderWidget 类，提供自定义滑块控件
 * @author [WeiWang]
 * @date 2025-05-15
 * @version 1.0
 */

#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QWidget>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QWheelEvent>

/** @brief 获取当前文件目录的宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

class ElaToolTip;

/**
 * @class MyWidget
 * @brief 中间控件类，用于传递信号和绘制音量控件背景
 */
class MyWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief 构造函数，初始化中间控件
     * @param parent 父控件指针，默认为 nullptr
     */
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
    /**
     * @brief 绘制事件，绘制音量控件背景
     * @param ev 绘图事件
     */
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
    /**
     * @brief 滚轮事件，传递滚轮信号
     * @param event 滚轮事件
     */
    void wheelEvent(QWheelEvent *event) override {
        QWidget::wheelEvent(event);
        this->m_delta = event->angleDelta().y();
        emit volumeChanged(this->m_delta);
    }

signals:
    /**
     * @brief 静音状态信号
     * @param flag 是否静音
     */
    void noVolume(const bool &flag);

    /**
     * @brief 音量变化信号
     * @param value 滚轮变化值
     */
    void volumeChanged(const int &value);

private:
    std::unique_ptr<QGraphicsDropShadowEffect> m_effect;     ///< 阴影效果
    int                     m_delta;                        ///< 滚轮变化值
};

/**
 * @class SliderWidget
 * @brief 自定义滑块控件类，继承自 QSlider，支持音量调节
 */
class SliderWidget : public QSlider {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化滑块控件
     * @param parent 父控件指针，默认为 nullptr
     * @param orientation 滑块方向，默认为水平
     */
    explicit SliderWidget(QWidget *parent = nullptr, Qt::Orientation orientation = Qt::Horizontal);

    /**
     * @brief 获取当前滑块值
     * @return 滑块值
     */
    const int getValue() const;

protected:
    /**
     * @brief 鼠标按下事件，计算滑块值
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标移动事件，更新滑块值
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标松开事件，重置按下状态
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 滚轮事件，调整滑块值
     * @param event 滚轮事件
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * @brief 控件显示事件，初始化信号连接
     * @param event 显示事件
     */
    void showEvent(QShowEvent *event) override;

public slots:
    /**
     * @brief 处理静音状态变化
     * @param flag 是否静音
     */
    void onNoVolume(bool flag);

private slots:
    /**
     * @brief 处理滚轮变化值
     * @param delta 滚轮变化值
     */
    void handleDelta(const int& delta);

signals:
    /**
     * @brief 静音状态信号
     * @param flag 是否静音
     */
    void noVolume(bool flag);

public:
    bool                    m_isPressing;           ///< 是否正在按下
private:
    int                     m_minValue;             ///< 最小值
    int                     m_maxValue;             ///< 最大值
    int                     m_currentValue;         ///< 当前值
    ElaToolTip*             m_sliderToolTip;        ///< 滑块提示
    MyWidget*               m_parent;               ///< 父控件
};

#endif // SLIDERWIDGET_H