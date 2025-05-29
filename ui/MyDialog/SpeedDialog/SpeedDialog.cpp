#include "SpeedDialog.h"
#include "ElaToggleSwitch.h"
#include "ElaPushButton.h"

#include <QCoreApplication>
#include <QButtonGroup>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

/** @brief 获取当前文件目录的宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

SpeedDialog::SpeedDialog(QWidget *parent)
    : QWidget(parent, Qt::Popup)
    , m_effect(std::make_unique<QGraphicsDropShadowEffect>(this))
{
    this->setContentsMargins(0, 10, 0, 20);
    setFixedSize(280, 295);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose); // 关闭后自动delete

    initUi();

    //设置样式
    QFile file(GET_CURRENT_DIR + QStringLiteral("/speed.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    this->m_effect->setColor(QColor(80, 80, 80));
    this->m_effect->setOffset(0, 0);
    this->m_effect->setBlurRadius(30);
    this->setGraphicsEffect(this->m_effect.get());
    qApp->installEventFilter(this); // 安装全局事件过滤器
}

SpeedDialog::~SpeedDialog() {
    qApp->removeEventFilter(this); // 移除事件过滤器
}

void SpeedDialog::initUi() {
    /// 整体垂直布局
    auto mainLay = new QVBoxLayout(this);
    mainLay->setSpacing(0);
    /// 第一个水平布局
    auto hlay1 = new QHBoxLayout;
    hlay1->setSpacing(10);
    hlay1->setContentsMargins(10, 0, 10, 0);
    auto diskLab = new QLabel(this);
    diskLab->setObjectName("diskLab");
    diskLab->setFixedSize(20, 20);
    auto textLab1 = new QLabel(this);
    textLab1->setObjectName("textLab1");
    textLab1->setText("一键DJ");
    auto textLab2 = new QLabel(this);
    textLab2->setObjectName("textLab2");
    textLab2->setText("自动DJ打碟模式");
    auto switchBtn = new ElaToggleSwitch(this);
    switchBtn->setFixedSize(40, 20);
    switchBtn->setEnabled(false);

    hlay1->addWidget(diskLab);
    hlay1->addWidget(textLab1);
    hlay1->addWidget(textLab2);
    hlay1->addStretch();
    hlay1->addWidget(switchBtn);
    hlay1->addStretch();

    ///  第二个水平布局
    auto hlay2 = new QHBoxLayout;
    hlay2->setSpacing(12);
    hlay2->setContentsMargins(10, 0, 15, 0);
    auto btnGroup = new QButtonGroup(this);
    m_lastBtn = nullptr;
    auto btn1 = new QPushButton("劲爆");
    btn1->setCheckable(true);
    btn1->setCursor(Qt::PointingHandCursor);
    btn1->setObjectName("btn1");
    btn1->setFixedSize(44, 20);
    auto btn2 = new QPushButton("社会摇");
    btn2->setCheckable(true);
    btn2->setCursor(Qt::PointingHandCursor);
    btn2->setObjectName("btn2");
    btn2->setFixedSize(54, 20);
    auto btn3 = new QPushButton("慢摇");
    btn3->setCheckable(true);
    btn3->setCursor(Qt::PointingHandCursor);
    btn3->setObjectName("btn3");
    btn3->setFixedSize(44, 20);
    auto btn4 = new QPushButton("抖腿");
    btn4->setCheckable(true);
    btn4->setCursor(Qt::PointingHandCursor);
    btn4->setObjectName("btn4");
    btn4->setFixedSize(44, 20);
    btnGroup->addButton(btn1);
    btnGroup->addButton(btn2);
    btnGroup->addButton(btn3);
    btnGroup->addButton(btn4);
    btnGroup->setExclusive(true);

    hlay2->addWidget(btn1);
    hlay2->addWidget(btn2);
    hlay2->addWidget(btn3);
    hlay2->addWidget(btn4);

    ///  第三个水平布局
    auto hlay3 = new QHBoxLayout;
    hlay3->setContentsMargins(0, 0, 0, 0);
    auto adjustmentLab = new QLabel("升降调播放", this);
    adjustmentLab->setContentsMargins(0, 0, 0, 0);
    adjustmentLab->setFixedHeight(30);
    adjustmentLab->setObjectName("adjustmentLab");
    adjustmentLab->setAlignment(Qt::AlignCenter);

    hlay3->addStretch();
    hlay3->addWidget(adjustmentLab);
    hlay3->addStretch();

    /// 第四个水平布局
    auto hlay4 = new QHBoxLayout;
    hlay4->setContentsMargins(0, 0, 0, 0);
    auto adjustmentSlider = new SnapSlider(this);
    /// adjustmentSlider->setFixedWidth(280);
    adjustmentSlider->setContentsMargins(0, 0, 0, 0);
    adjustmentSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    adjustmentSlider->setOrientation(Qt::Horizontal);
    adjustmentSlider->setUseThemeColors(false);
    adjustmentSlider->setDisabledColor(QColor("#29A2FF"));
    adjustmentSlider->setTrackColor(QColor("#29A2FF"));
    adjustmentSlider->setThumbColor(QColor("#29A2FF"));
    adjustmentSlider->setMaximum(100); // 设置为100便于计算
    adjustmentSlider->setDisabled(false);
    // 延迟设置初始值,解决焦点问题
    QTimer::singleShot(0, this, [adjustmentSlider]() {
        // adjustmentSlider->setValue(adjustmentSlider->maximum() / 2); // 50%位置
        adjustmentSlider->setFocus();
        adjustmentSlider->clearFocus(); // 立即取消焦点，避免永久聚焦状态
    });
    hlay4->addWidget(adjustmentSlider);

    /// 第五个水平布局
    auto hlay5 = new QHBoxLayout;
    hlay5->setContentsMargins(28, 0, 28, 0);
    /// 三个等级Lab
    auto adjustmentLv1Lab = new QLabel("降调", this);
    adjustmentLv1Lab->setObjectName("adjustmentLv1Lab");
    adjustmentLv1Lab->setContentsMargins(0, 0, 0, 0);
    auto adjustmentLv2Lab = new QLabel("正常", this);
    adjustmentLv2Lab->setObjectName("adjustmentLv2Lab");
    adjustmentLv2Lab->setContentsMargins(0, 0, 0, 0);
    auto adjustmentLv3Lab = new QLabel("升调", this);
    adjustmentLv3Lab->setObjectName("adjustmentLv3Lab");
    adjustmentLv3Lab->setContentsMargins(0, 0, 0, 0);
    hlay5->addWidget(adjustmentLv1Lab);
    hlay5->addStretch();
    hlay5->addWidget(adjustmentLv2Lab);
    hlay5->addStretch();
    hlay5->addWidget(adjustmentLv3Lab);

    /// 第六个水平布局
    auto hlay6 = new QHBoxLayout;
    hlay6->setContentsMargins(0, 0, 0, 0);
    auto speedLab = new QLabel("倍速播放", this);
    speedLab->setContentsMargins(0, 0, 0, 0);
    speedLab->setFixedHeight(30);
    speedLab->setObjectName("speedLab");
    speedLab->setAlignment(Qt::AlignCenter);

    hlay6->addStretch();
    hlay6->addWidget(speedLab);
    hlay6->addStretch();

    /// 第七个水平布局
    auto hlay7 = new QHBoxLayout;
    hlay7->setContentsMargins(0, 0, 0, 0);
    auto speedSlider = new SnapSlider(this);
    /// speedSlider->setFixedWidth(280);
    speedSlider->setContentsMargins(0, 0, 0, 0);
    speedSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    speedSlider->setOrientation(Qt::Horizontal);
    speedSlider->setUseThemeColors(false);
    speedSlider->setDisabledColor(QColor("#29A2FF"));
    speedSlider->setTrackColor(QColor("#29A2FF"));
    speedSlider->setThumbColor(QColor("#29A2FF"));
    speedSlider->setMaximum(100); // 设置为100便于计算
    speedSlider->setDisabled(false);
    // 延迟设置初始值，确保布局完成后再定位
    QTimer::singleShot(0, this, [speedSlider]() {
        // speedSlider->setValue(speedSlider->maximum() / 2); // 50%位置
        speedSlider->setFocus();
        speedSlider->clearFocus(); // 立即取消焦点，避免永久聚焦状态
    });
    hlay7->addWidget(speedSlider);

    /// 第八个水平布局
    auto hlay8 = new QHBoxLayout;
    hlay8->setContentsMargins(28, 0, 28, 0);
    /// 三个等级Lab
    auto speedLv1Lab = new QLabel("减慢", this);
    speedLv1Lab->setObjectName("speedLv1Lab");
    speedLv1Lab->setContentsMargins(0, 0, 0, 0);
    auto speedLv2Lab = new QLabel("正常", this);
    speedLv2Lab->setObjectName("speedLv2Lab");
    speedLv2Lab->setContentsMargins(0, 0, 0, 0);
    auto speedLv3Lab = new QLabel("加快", this);
    speedLv3Lab->setObjectName("speedLv3Lab");
    speedLv3Lab->setContentsMargins(0, 0, 0, 0);
    hlay8->addWidget(speedLv1Lab);
    hlay8->addStretch();
    hlay8->addWidget(speedLv2Lab);
    hlay8->addStretch();
    hlay8->addWidget(speedLv3Lab);

    mainLay->addLayout(hlay1);
    mainLay->addSpacing(25);
    mainLay->addLayout(hlay2);
    mainLay->addSpacing(22);
    mainLay->addLayout(hlay3);
    mainLay->addSpacing(0);
    mainLay->addLayout(hlay4);
    mainLay->addSpacing(0);
    mainLay->addLayout(hlay5);
    mainLay->addSpacing(0);
    mainLay->addLayout(hlay6);
    mainLay->addSpacing(0);
    mainLay->addLayout(hlay7);
    mainLay->addSpacing(0);
    mainLay->addLayout(hlay8);
    mainLay->addStretch();

    auto changeText = [this] {
        if (m_speedText.isEmpty()) {
            if (m_adjustmentText.isEmpty()) {
                emit btnTextChanged(m_preText);
            } else {
                if (m_preText == "倍速")
                    emit btnTextChanged(m_adjustmentText);
                else
                    emit btnTextChanged(m_preText + "/" + m_adjustmentText);
            }
        } else {
            if (m_preText == "倍速") {
                emit btnTextChanged(m_speedText);
            }
            else
                emit btnTextChanged(m_preText + "/" + m_speedText);
        }
    };

    /// 相关信号处理
    connect(btn1, &QPushButton::clicked, this, [=] {
        m_lastBtn = btn1;
        m_preText = "DJ";
        changeText();
        switchBtn->setEnabled(true);
        switchBtn->setIsToggled(true);
    });
    connect(btn2, &QPushButton::clicked, this, [=] {
        m_lastBtn = btn2;
        m_preText = "DJ";
        changeText();
        switchBtn->setEnabled(true);
        switchBtn->setIsToggled(true);
    });
    connect(btn3, &QPushButton::clicked, this, [=] {
        m_lastBtn = btn3;
        m_preText = "DJ";
        changeText();
        switchBtn->setEnabled(true);
        switchBtn->setIsToggled(true);
    });
    connect(btn4, &QPushButton::clicked, this, [=] {
        m_lastBtn = btn4;
        m_preText = "DJ";
        changeText();
        switchBtn->setEnabled(true);
        switchBtn->setIsToggled(true);
    });


    connect(adjustmentSlider, &SnapSlider::numChanged, this, [this, adjustmentLab,changeText](int num) {
        if (num != abs(num - 5) % 10) {
            if (num > 5) {
                num = (num - 5) % 10;
                adjustmentLab->setText(QString("升%1调播放").arg(num));
                m_adjustmentText = "升调";
            } else if (num < 5) {
                num = abs(num - 5) % 10;
                adjustmentLab->setText(QString("降%1调播放").arg(num));
                m_adjustmentText = "降调";
            } else {
                adjustmentLab->setText("升降调播放");
                m_adjustmentText = "";
            }
            changeText();
        }
        // qDebug()<<"当前倍数："<<num;
    });

    connect(speedSlider, &SnapSlider::numChanged, this, [this, speedLab,changeText](int num) {
        if (num != abs(num - 5) % 10) {
            if (num > 5) {
                auto speed = 1 + (num - 5) % 10 / 10.0;
                speedLab->setText(QString("%1倍播放").arg(speed));
                m_speedText = QString::number(speed) + "X";
            } else if (num < 5) {
                auto speed = 1 - abs(num - 5) % 10 / 10.0;
                speedLab->setText(QString("%1倍播放").arg(speed));
                m_speedText = QString::number(speed) + "X";
            } else {
                speedLab->setText("倍速播放");
                m_speedText = "";
            }
            changeText();
        }
        // qDebug()<<"当前倍数："<<num;
    });


    connect(switchBtn, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        /// m_lastBtn必定指向其中一个按钮
        if (!m_lastBtn) {
            qWarning() << "重大错误，应该无法点击";
            return;
        }

        // m_lastBtn->setChecked(checked);///< 直接这样的话，样式不会其效果，因为按钮组设置了setExclusive(true)

        if (checked) {
            btnGroup->setExclusive(true); // 打开开关时恢复排他性
            m_lastBtn->setChecked(true);
            m_preText = "DJ";
            changeText();
        } else {
            btnGroup->setExclusive(false); // 关闭开关时取消排他性
            m_lastBtn->setChecked(false);
            m_preText = "倍速";
            changeText();
        }
        // qDebug()<<"m_lastBtn "<<(checked?"true":"false");
    });
}

bool SpeedDialog::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 判断点击位置是否在对话框外
        if (!this->rect().contains(this->mapFromGlobal(mouseEvent->globalPosition()).toPoint())) {
            this->close(); // 触发WA_DeleteOnClose自动销毁
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void SpeedDialog::paintEvent(QPaintEvent *ev) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QColor(Qt::transparent));
    p.setBrush(QColor(QStringLiteral("#edf2ff")));
    p.drawRoundedRect(rect().x(), rect().y(), 280, 287, 8, 8);
    QPainterPath path;
    path.moveTo(rect().x() + 130, rect().bottom() - 8);
    path.lineTo(rect().x() + 140, rect().bottom());
    path.lineTo(rect().x() + 150, rect().bottom() - 8);
    p.drawPath(path);
}
