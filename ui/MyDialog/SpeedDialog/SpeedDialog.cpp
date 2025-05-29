/**
 * @file SpeedDialog.cpp
 * @brief 实现 SpeedDialog 类，倍速和调音控制弹窗
 * @author WeiWang
 * @date 2025-05-27
 * @version 1.0
 */

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

/**
 * @def GET_CURRENT_DIR
 * @brief 获取当前文件目录路径
 * @note 从 __FILE__ 中解析目录
 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数
 * @param parent 父控件指针，默认为 nullptr
 */
SpeedDialog::SpeedDialog(QWidget *parent)
    : QWidget(parent, Qt::Popup)
    , m_effect(std::make_unique<QGraphicsDropShadowEffect>(this))
{
    this->setContentsMargins(0, 10, 0, 20);                   ///< 设置边距
    setFixedSize(280, 295);                                   ///< 设置固定尺寸
    setWindowFlags(Qt::FramelessWindowHint);                  ///< 无边框窗口
    setAttribute(Qt::WA_TranslucentBackground);               ///< 透明背景
    setAttribute(Qt::WA_DeleteOnClose);                       ///< 关闭时自动删除

    initUi();                                                 ///< 初始化界面

    // @note 设置样式表
    QFile file(GET_CURRENT_DIR + QStringLiteral("/speed.css"));
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());                  ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";                     ///< 调试输出
    }
    this->m_effect->setColor(QColor(80, 80, 80));             ///< 设置阴影颜色
    this->m_effect->setOffset(0, 0);                          ///< 设置阴影偏移
    this->m_effect->setBlurRadius(30);                        ///< 设置阴影模糊半径
    this->setGraphicsEffect(this->m_effect.get());            ///< 应用阴影效果
    qApp->installEventFilter(this);                           ///< 安装事件过滤器
}

/**
 * @brief 析构函数
 */
SpeedDialog::~SpeedDialog()
{
    qApp->removeEventFilter(this);                            ///< 移除事件过滤器
}

/**
 * @brief 设置弹窗状态
 * @param state 状态对象
 */
void SpeedDialog::setState(const SpeedDialogState& state)
{
    // @note 恢复 DJ 按钮选择
    if (!state.selectedDJButton.isEmpty())
    {
        if (state.selectedDJButton == "劲爆") m_btn1->clicked();
        else if (state.selectedDJButton == "社会摇") m_btn2->clicked();
        else if (state.selectedDJButton == "慢摇") m_btn3->clicked();
        else if (state.selectedDJButton == "抖腿") m_btn4->clicked();
    }

    // @note 恢复滑块值
    QTimer::singleShot(50, [this, state] {
        m_adjustmentSlider->setValue(state.adjustmentValue);  ///< 设置升降调值
        m_speedSlider->setValue(state.speedValue);            ///< 设置倍速值

        // @note 强制更新文本显示
        Q_EMIT m_adjustmentSlider->numChanged(state.adjustmentValue / 10);
        Q_EMIT m_speedSlider->numChanged(state.speedValue / 10);
    });
}

/**
 * @brief 获取弹窗状态
 * @return 状态对象
 */
SpeedDialogState SpeedDialog::getState() const
{
    SpeedDialogState state;

    state.isDJMode = m_switchBtn->getIsToggled();              ///< 获取 DJ 模式状态

    // @note 获取选中的 DJ 按钮
    if (m_btn1->isChecked()) state.selectedDJButton = "劲爆";
    else if (m_btn2->isChecked()) state.selectedDJButton = "社会摇";
    else if (m_btn3->isChecked()) state.selectedDJButton = "慢摇";
    else if (m_btn4->isChecked()) state.selectedDJButton = "抖腿";

    state.adjustmentValue = m_adjustmentSlider->value();       ///< 获取升降调值
    state.speedValue = m_speedSlider->value();                 ///< 获取倍速值
    // qDebug()<<"当前获取到adjustmentValue: "<<m_adjustmentSlider->value()<< " speedValue : "<<m_speedSlider->value();
    return state;
}

/**
 * @brief 初始化界面
 * @note 设置布局、控件和信号连接
 */
void SpeedDialog::initUi()
{
    // @note 整体垂直布局
    auto mainLay = new QVBoxLayout(this);
    mainLay->setSpacing(0);

    // @note 第一个水平布局 - DJ 模式
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
    m_switchBtn = new ElaToggleSwitch(this);
    m_switchBtn->setFixedSize(40, 20);
    m_switchBtn->setEnabled(false);

    hlay1->addWidget(diskLab);
    hlay1->addWidget(textLab1);
    hlay1->addWidget(textLab2);
    hlay1->addStretch();
    hlay1->addWidget(m_switchBtn);
    hlay1->addStretch();

    // @note 第二个水平布局 - DJ 按钮
    auto hlay2 = new QHBoxLayout;
    hlay2->setSpacing(12);
    hlay2->setContentsMargins(10, 0, 15, 0);
    auto btnGroup = new QButtonGroup(this);
    m_lastBtn = nullptr;
    m_btn1 = new QPushButton("劲爆");
    m_btn1->setCheckable(true);
    m_btn1->setCursor(Qt::PointingHandCursor);
    m_btn1->setObjectName("btn1");
    m_btn1->setFixedSize(44, 20);
    m_btn2 = new QPushButton("社会摇");
    m_btn2->setCheckable(true);
    m_btn2->setCursor(Qt::PointingHandCursor);
    m_btn2->setObjectName("btn2");
    m_btn2->setFixedSize(54, 20);
    m_btn3 = new QPushButton("慢摇");
    m_btn3->setCheckable(true);
    m_btn3->setCursor(Qt::PointingHandCursor);
    m_btn3->setObjectName("btn3");
    m_btn3->setFixedSize(44, 20);
    m_btn4 = new QPushButton("抖腿");
    m_btn4->setCheckable(true);
    m_btn4->setCursor(Qt::PointingHandCursor);
    m_btn4->setObjectName("btn4");
    m_btn4->setFixedSize(44, 20);
    btnGroup->addButton(m_btn1);
    btnGroup->addButton(m_btn2);
    btnGroup->addButton(m_btn3);
    btnGroup->addButton(m_btn4);
    btnGroup->setExclusive(true);

    hlay2->addWidget(m_btn1);
    hlay2->addWidget(m_btn2);
    hlay2->addWidget(m_btn3);
    hlay2->addWidget(m_btn4);

    // @note 第三个水平布局 - 升降调标签
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

    // @note 第四个水平布局 - 升降调滑块
    auto hlay4 = new QHBoxLayout;
    hlay4->setContentsMargins(0, 0, 0, 0);
    m_adjustmentSlider = new SnapSlider(this);
    m_adjustmentSlider->setContentsMargins(0, 0, 0, 0);
    m_adjustmentSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_adjustmentSlider->setOrientation(Qt::Horizontal);
    m_adjustmentSlider->setUseThemeColors(false);
    m_adjustmentSlider->setDisabledColor(QColor("#29A2FF"));
    m_adjustmentSlider->setTrackColor(QColor("#29A2FF"));
    m_adjustmentSlider->setThumbColor(QColor("#29A2FF"));
    m_adjustmentSlider->setMaximum(100);                       ///< 设置最大值
    m_adjustmentSlider->setDisabled(false);
    // @note 延迟设置初始值
    QTimer::singleShot(0, this, [this]() {
        m_adjustmentSlider->setFocus();
        m_adjustmentSlider->clearFocus();                     ///< 避免永久聚焦
    });
    hlay4->addWidget(m_adjustmentSlider);

    // @note 第五个水平布局 - 升降调等级标签
    auto hlay5 = new QHBoxLayout;
    hlay5->setContentsMargins(28, 0, 28, 0);
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

    // @note 第六个水平布局 - 倍速标签
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

    // @note 第七个水平布局 - 倍速滑块
    auto hlay7 = new QHBoxLayout;
    hlay7->setContentsMargins(0, 0, 0, 0);
    m_speedSlider = new SnapSlider(this);
    m_speedSlider->setContentsMargins(0, 0, 0, 0);
    m_speedSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_speedSlider->setOrientation(Qt::Horizontal);
    m_speedSlider->setUseThemeColors(false);
    m_speedSlider->setDisabledColor(QColor("#29A2FF"));
    m_speedSlider->setTrackColor(QColor("#29A2FF"));
    m_speedSlider->setThumbColor(QColor("#29A2FF"));
    m_speedSlider->setMaximum(100);                           ///< 设置最大值
    m_speedSlider->setDisabled(false);
    // @note 延迟设置初始值
    QTimer::singleShot(0, this, [this]() {
        m_speedSlider->setFocus();
        m_speedSlider->clearFocus();                         ///< 避免永久聚焦
    });
    hlay7->addWidget(m_speedSlider);

    // @note 第八个水平布局 - 倍速等级标签
    auto hlay8 = new QHBoxLayout;
    hlay8->setContentsMargins(28, 0, 28, 0);
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

    // @note 更新按钮文本的 lambda 函数
    auto changeText = [this] {
        if (m_speedText.isEmpty())
        {
            if (m_adjustmentText.isEmpty())
            {
                emit btnTextChanged(m_preText);               ///< 默认文本
            }
            else
            {
                if (m_preText == "倍速")
                    emit btnTextChanged(m_adjustmentText);    ///< 仅升降调
                else
                    emit btnTextChanged(m_preText + "/" + m_adjustmentText); ///< DJ+升降调
            }
        }
        else
        {
            if (m_preText == "倍速")
            {
                emit btnTextChanged(m_speedText);             ///< 仅倍速
            }
            else
                emit btnTextChanged(m_preText + "/" + m_speedText); ///< DJ+倍速
        }
    };

    // @note DJ 按钮信号连接
    connect(m_btn1, &QPushButton::clicked, this, [=] {
        m_lastBtn = m_btn1;
        m_preText = "DJ";
        changeText();
        m_switchBtn->setEnabled(true);
        m_switchBtn->setIsToggled(true);
    });
    connect(m_btn2, &QPushButton::clicked, this, [=] {
        m_lastBtn = m_btn2;
        m_preText = "DJ";
        changeText();
        m_switchBtn->setEnabled(true);
        m_switchBtn->setIsToggled(true);
    });
    connect(m_btn3, &QPushButton::clicked, this, [=] {
        m_lastBtn = m_btn3;
        m_preText = "DJ";
        changeText();
        m_switchBtn->setEnabled(true);
        m_switchBtn->setIsToggled(true);
    });
    connect(m_btn4, &QPushButton::clicked, this, [=] {
        m_lastBtn = m_btn4;
        m_preText = "DJ";
        changeText();
        m_switchBtn->setEnabled(true);
        m_switchBtn->setIsToggled(true);
    });

    // @note 升降调滑块信号连接
    connect(m_adjustmentSlider, &SnapSlider::numChanged, this, [this, adjustmentLab, changeText](int num) {
        if (num != abs(num - 5) % 10)
        {
            if (num > 5)
            {
                num = (num - 5) % 10;
                adjustmentLab->setText(QString("升%1调播放").arg(num)); ///< 升调显示
                m_adjustmentText = "升调";
            }
            else if (num < 5)
            {
                num = abs(num - 5) % 10;
                adjustmentLab->setText(QString("降%1调播放").arg(num)); ///< 降调显示
                m_adjustmentText = "降调";
            }
            else
            {
                adjustmentLab->setText("升降调播放");        ///< 正常显示
                m_adjustmentText = "";
            }
            changeText();
        }
    });

    // @note 倍速滑块信号连接
    connect(m_speedSlider, &SnapSlider::numChanged, this, [this, speedLab, changeText](int num) {
        if (num != abs(num - 5) % 10)
        {
            if (num > 5)
            {
                auto speed = 1 + (num - 5) % 10 / 10.0;
                speedLab->setText(QString("%1倍播放").arg(speed)); ///< 加快显示
                m_speedText = QString::number(speed) + "X";
            }
            else if (num < 5)
            {
                auto speed = 1 - abs(num - 5) % 10 / 10.0;
                speedLab->setText(QString("%1倍播放").arg(speed)); ///< 减慢显示
                m_speedText = QString::number(speed) + "X";
            }
            else
            {
                speedLab->setText("倍速播放");                ///< 正常显示
                m_speedText = "";
            }
            changeText();
        }
    });

    // @note DJ 模式开关信号连接
    connect(m_switchBtn, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        if (!m_lastBtn)
        {
            qWarning() << "重大错误，应该无法点击";         ///< 调试输出
            return;
        }

        if (checked)
        {
            btnGroup->setExclusive(true);                     ///< 恢复排他性
            m_lastBtn->setChecked(true);
            m_preText = "DJ";
            changeText();
        }
        else
        {
            btnGroup->setExclusive(false);                    ///< 取消排他性
            m_lastBtn->setChecked(false);
            m_preText = "倍速";
            changeText();
        }
    });
}

/**
 * @brief 事件过滤
 * @param obj 对象
 * @param event 事件
 * @return 是否处理事件
 * @note 点击弹窗外关闭
 */
bool SpeedDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (!this->rect().contains(this->mapFromGlobal(mouseEvent->globalPosition()).toPoint()))
        {
            this->close();                                    ///< 触发关闭
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);                  ///< 调用父类处理
}

/**
 * @brief 关闭事件
 * @param event 关闭事件
 */
void SpeedDialog::closeEvent(QCloseEvent *event)
{
    emit aboutToClose();                                      ///< 触发关闭信号
    QWidget::closeEvent(event);                               ///< 调用父类处理
}

/**
 * @brief 绘制事件
 * @param ev 绘制事件
 * @note 绘制圆角背景和小三角形底部
 */
void SpeedDialog::paintEvent(QPaintEvent *ev)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setRenderHint(QPainter::Antialiasing);                  ///< 启用抗锯齿
    p.setPen(QColor(Qt::transparent));
    p.setBrush(QColor(QStringLiteral("#edf2ff")));            ///< 设置背景颜色
    p.drawRoundedRect(rect().x(), rect().y(), 280, 287, 8, 8); ///< 绘制圆角矩形
    QPainterPath path;
    path.moveTo(rect().x() + 130, rect().bottom() - 8);
    path.lineTo(rect().x() + 140, rect().bottom());
    path.lineTo(rect().x() + 150, rect().bottom() - 8);
    p.drawPath(path);                                         ///< 绘制底部三角形
}