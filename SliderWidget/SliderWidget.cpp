#include "SliderWidget.h"

SliderWidget::SliderWidget(QWidget *parent,Qt::Orientation orientation)
    : QSlider(orientation, parent)
    ,m_currentValue(20)
{
    this->setCursor(Qt::PointingHandCursor);
}

const int SliderWidget::getValue() const {
    return this->value()?this->m_currentValue:0;
}

void SliderWidget::mousePressEvent(QMouseEvent *event) {
    if(orientation() == Qt::Horizontal) {
        // 获取鼠标点击相对于滑动条的位置
        const auto pos = event->position().x();
        // 获取滑动条的可用长度 (即滑块可以滑动的范围)
        const auto sliderLength = this->width();
        // 计算点击位置相对于整个滑动条的百分比
        const auto percentage = static_cast<double>(pos) / sliderLength;
        // 根据百分比计算出滑动条的新值
        this->m_currentValue = this->m_minValue + static_cast<int>(percentage * (this->m_maxValue - this->m_minValue));
        //qDebug()<<"水平 "<< this->m_currentValue << " percentage "<<percentage;
        // 设置滑动条的新值
        this->setValue(this->m_currentValue);
    }else {
        //qDebug()<<"垂直";
        const auto pos = event->position().y();
        // 获取滑动条的可用长度 (即滑块可以滑动的范围)
        const auto sliderLength = this->height();
        // 计算点击位置相对于整个滑动条的百分比
        double percentage = static_cast<double>(pos) / sliderLength;
        percentage = 1 - percentage;
        // 根据百分比计算出滑动条的新值
        this->m_currentValue = this->m_minValue + static_cast<int>(percentage * (this->m_maxValue - this->m_minValue));
        // 设置滑动条的新值
        this->setValue(this->m_currentValue);
        emit noVolume(this->m_currentValue == 0);
        //qDebug()<<"m_currentValue : "<<this->m_currentValue;
        // 正在按下
        this->m_isPressing = true;
    }
    // 让父类处理其余的鼠标事件
    QSlider::mousePressEvent(event);
}

void SliderWidget::mouseMoveEvent(QMouseEvent *event) {
    //qDebug()<<"垂直";
    if(this->m_isPressing) {
        // 获取鼠标移动相对于滑动条的位置
        const auto pos = event->position().y();
        // 获取滑动条的可用长度
        const auto sliderLength = this->height();
        // 将鼠标位置转换为滑动条值
        double percentage = static_cast<double>(pos) / sliderLength;
        percentage = 1 - percentage;
        // 根据百分比计算出滑动条的新值
        this->m_currentValue = this->m_minValue + static_cast<int>(percentage * (this->m_maxValue - this->m_minValue));
        // 设置滑动条的新值
        this->m_currentValue = this->m_currentValue >= 0 ? this->m_currentValue : 0;
        this->setValue(this->m_currentValue);
        emit noVolume(this->m_currentValue == 0);
        //qDebug()<<"m_currentValue : "<<this->m_currentValue;
    }
    QSlider::mouseMoveEvent(event);
}

void SliderWidget::mouseReleaseEvent(QMouseEvent *event) {
    // 鼠标释放时，重置按下标志
    m_isPressing = false;
    QSlider::mouseReleaseEvent(event);
}

void SliderWidget::wheelEvent(QWheelEvent *event) {
    QSlider::wheelEvent(event);
    // 获取滚轮滚动的角度
    int delta = event->angleDelta().y();
    if(delta > 0) {
        this->m_currentValue += 2 ;
        this->m_currentValue = this->m_currentValue >= this->m_maxValue ? this->m_maxValue : this->m_currentValue;
    }
    else if(delta < 0) {
        this->m_currentValue -= 2;
        this->m_currentValue = this->m_currentValue < this->m_minValue ? this->m_minValue : this->m_currentValue;
    }
    this->setValue(this->m_currentValue);
}

void SliderWidget::showEvent(QShowEvent *event) {
    QSlider::showEvent(event);
    // 获取滑动条的最小值和最大值
    this->m_minValue = this->minimum();
    this->m_maxValue = this->maximum();
    this->m_parent = qobject_cast<MyWidget*>(this->parent());
    // 确保 m_parent 不为空再进行连接
    if (this->m_parent) {
        connect(this->m_parent,&MyWidget::noVolume,this,&SliderWidget::onNoVolume);
        connect(this->m_parent,&MyWidget::volumeChanged,this,[=](int delta) {
            if(delta > 0) {
                this->m_currentValue += 2 ;
                this->m_currentValue = this->m_currentValue >= this->m_maxValue ? this->m_maxValue : this->m_currentValue;
            }
            else if(delta < 0) {
                this->m_currentValue -= 2;
                this->m_currentValue = this->m_currentValue < this->m_minValue ? this->m_minValue : this->m_currentValue;
            }
            this->setValue(this->m_currentValue);
        });
    }
}
void SliderWidget::onNoVolume(bool flag) {
    if(flag) {
        this->setValue(0);
    }
    else {
        if(this->m_currentValue == 0) {
            this->m_currentValue = 1;
        }
        this->setValue(this->m_currentValue);
        //qDebug()<<"m_currentValue : "<<this->m_currentValue;
    }
}
