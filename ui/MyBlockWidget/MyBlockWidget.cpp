/**
 * @file MyBlockWidget.cpp
 * @brief 实现 MyBlockWidget 类，提供块状控件功能
 * @author WeiWang
 * @date 24-11-23
 * @version 1.0
 */

#include "MyBlockWidget.h"

#include <QToolButton>
#include <QLabel>
#include <QPainter>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QStyleOption>

/**
 * @brief 构造函数，初始化块状控件
 * @param parent 父控件指针，默认为 nullptr
 */
MyBlockWidget::MyBlockWidget(QWidget *parent) :
    QWidget(parent)
    , m_bacWidget(new QWidget(this))
    , m_mask(std::make_unique<SMaskWidget>( this))
    , m_tipLab(new QLabel(this))
    , m_rightPopularBtn(new QToolButton(this))
    , m_leftPopularBtn(new QToolButton(this))
    , m_durationBtn(new QToolButton(this))
{
    initUi();
    m_mask->setParent(m_bacWidget);
    m_mask->move(m_bacWidget->pos());
    m_mask->setFixedSize(m_bacWidget->size());
    m_mask->hide();
    //先隐藏流行人数按钮
    m_leftPopularBtn->hide();
    m_rightPopularBtn->hide();
    //默认隐藏tipLab
    m_tipLab->hide();
    //默认隐藏时长按钮
    m_durationBtn->hide();
}

/**
 * @brief 初始化用户界面
 */
void MyBlockWidget::initUi() {
    this->setFixedSize(200,200);
    this->setCursor(Qt::PointingHandCursor);
    this->setMouseTracking(true);
    initTipArr();
    this->m_tipLab->setFixedHeight(20);
    this->m_tipLab->setScaledContents(true);
    this->m_tipLab->setContentsMargins(5,2,5,2);
    this->setTipLabText(m_tipArr[QRandomGenerator::global()->bounded(0, static_cast<int>(m_tipArr.size()))]);
    this->m_tipLab->setAlignment(Qt::AlignCenter);
    this->m_tipLab->setStyleSheet(QStringLiteral("border-radius:10px;background-color:black;color:white;"));
    this->m_tipLab->move(6, 6);
    //两个流行人数按钮都初始化
    this->m_rightPopularBtn->setEnabled(false);
    this->m_rightPopularBtn->setFixedSize(80, 20);
    this->m_rightPopularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_rightPopularBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/popular-white.svg")));
    this->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 1000, 'f', 1));
    this->m_rightPopularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_rightPopularBtnStyle = "color:white;border:none;border-radius:10px;";
    this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle+"background-color: rgba(128, 128, 128, 127);");

    this->m_leftPopularBtn->setEnabled(false);
    this->m_leftPopularBtn->setFixedSize(80, 20);
    this->m_leftPopularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_leftPopularBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/person-white.svg")));
    this->m_leftPopularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_leftPopularBtnStyle = "color:white;border:none;border-radius:10px;background-color: rgba(255,255,255,0);";
    this->m_leftPopularBtn->setStyleSheet(this->m_leftPopularBtnStyle);

    this->m_durationBtn->setEnabled(false);
    this->m_durationBtn->setFixedSize(50,20);
    this->m_leftPopularBtn->setContentsMargins(8, 0, 0, 0);
    this->m_durationBtn->setStyleSheet("color:white;border:none;background-color: rgba(255,255,255,0);");

}

/**
 * @brief 初始化提示标签数组
 */
void MyBlockWidget::initTipArr() {
    m_tipArr = {QStringLiteral("流行"),QStringLiteral("经典"),
                QStringLiteral("轻音乐"),QStringLiteral("影视"),
                QStringLiteral("爵士"),QStringLiteral("轻松"),
                QStringLiteral("日语"),QStringLiteral("中国风"),
                QStringLiteral("英语"),QStringLiteral("电子"),
                QStringLiteral("80后"),QStringLiteral("90后"),
                QStringLiteral("70后"),QStringLiteral("励志"),
                QStringLiteral("乐器演奏"),QStringLiteral("国语"),
                QStringLiteral("民谣"),QStringLiteral("校园"),
                QStringLiteral("安静"),QStringLiteral("寂寞"),
                QStringLiteral("网络"),QStringLiteral("法语"),
                QStringLiteral("ACG"),QStringLiteral("兴奋"),
                QStringLiteral("快乐"),QStringLiteral("金属"),
                QStringLiteral("说唱"),QStringLiteral("DJ热碟"),
                QStringLiteral("甜蜜"),QStringLiteral("广场舞"),};
}

/**
 * @brief 设置边框图片
 * @param path 图片路径
 * @param border 圆角半径，默认为 8
 */
void MyBlockWidget::setBorderImage(const QString &path, const int &border) const
{
    QString style = QString("border-radius:%1px;border-image:url(%2);").arg(border).arg(path);
    //qDebug()<<"当前样式："<<style;
    m_bacWidget->setStyleSheet(style);
    if (border != 8)
    {
        m_mask->setBorderRadius(border);
    }
}

/**
 * @brief 设置提示标签文本
 * @param text 文本内容
 */
void MyBlockWidget::setTipLabText(const QString &text) const
{
    m_tipLab->setText(text);
    m_tipLab->adjustSize();
}

/**
 * @brief 设置流行度按钮方向
 * @param direction 方向（0:无，1:左，2:右）
 */
void MyBlockWidget::setPopularDirection(const int &direction)
{
    m_popularDirection = direction;
    if (direction == 1)
    {
        m_leftPopularBtn->show();
    }
    else if (direction == 2)
    {
        m_rightPopularBtn->show();
    }
    else
    {
        m_leftPopularBtn->hide();
        m_rightPopularBtn->hide();
    }
}

/**
 * @brief 设置流行度按钮文本
 * @param text 文本内容
 */
void MyBlockWidget::setPopularBtnText(const QString &text)const {
    if(!this->m_popularDirection)return;

    if(this->m_popularDirection == 1) {
        if (this->m_haveUnit)
            this->m_leftPopularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
        else
            this->m_leftPopularBtn->setText(QStringLiteral(" ") + text);
        if (this->m_leftPopularBtn->icon().isNull()) {
            this->m_leftPopularBtn->setFixedWidth(this->m_leftPopularBtn->fontMetrics().horizontalAdvance(m_leftPopularBtn->text()) +
            this->m_leftPopularBtn->contentsMargins().left() + this->m_leftPopularBtn->contentsMargins().right());
        }
        else {
            this->m_leftPopularBtn->setFixedWidth(this->m_leftPopularBtn->fontMetrics().horizontalAdvance(m_leftPopularBtn->text()) +
            this->m_leftPopularBtn->contentsMargins().left() + this->m_leftPopularBtn->contentsMargins().right() +
            this->m_leftPopularBtn->iconSize().width());
        }
    }
    else if(this->m_popularDirection == 2) {
        if (this->m_haveUnit)
            this->m_rightPopularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
        else
            this->m_rightPopularBtn->setText(QStringLiteral(" ") + text);
        if (this->m_rightPopularBtn->icon().isNull()) {
            this->m_rightPopularBtn->setFixedWidth(this->m_rightPopularBtn->fontMetrics().horizontalAdvance(this->m_rightPopularBtn->text()) +
            this->m_rightPopularBtn->contentsMargins().left() + this->m_rightPopularBtn->contentsMargins().right());
        }
        else {
            this->m_rightPopularBtn->setFixedWidth(this->m_rightPopularBtn->fontMetrics().horizontalAdvance(this->m_rightPopularBtn->text()) +
            this->m_rightPopularBtn->contentsMargins().left() + this->m_rightPopularBtn->contentsMargins().right() +
            this->m_rightPopularBtn->iconSize().width() + 10);
        }
    }
}

/**
 * @brief 设置是否显示提示标签
 * @param show 是否显示，默认为 true
 */
void MyBlockWidget::setShowTip(bool show) const
{
    if (show)
        m_tipLab->show();
    else
        m_tipLab->hide();
}

/**
 * @brief 设置是否扩展响应范围
 * @param expandRespond 是否扩展
 */
void MyBlockWidget::setExpandRespond(const bool &expandRespond)
{
    m_isExpandRespond = expandRespond;
}

/**
 * @brief 设置右侧流行度按钮图标
 * @param icon 图标路径
 */
void MyBlockWidget::setRightPopularBtnIcon(const QString &icon) const
{
    if (icon.isEmpty())
    {
        //qDebug()<<"icon为空";
        m_rightPopularBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        m_rightPopularBtn->setFixedWidth(30);
        return;
    }
    m_rightPopularBtn->setIcon(QIcon(icon));
}

/**
 * @brief 设置左侧流行度按钮图标
 * @param icon 图标路径
 */
void MyBlockWidget::setLeftPopularBtnIcon(const QString &icon)
{
    if (icon.isEmpty())
    {
        m_leftPopularBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        // 1. 移除固定大小限制      没有效果，不知道为什么
        m_leftPopularBtn->setMinimumSize(0, 0);
        m_leftPopularBtn->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        m_leftPopularBtn->setIconSize(QSize(0, 0));
        return;
    }
    m_leftPopularBtn->setIcon(QIcon(icon));
}

/**
 * @brief 设置右侧流行度按钮宽度
 * @param width 宽度
 */
void MyBlockWidget::setRightPopularBtnWidth(const int &width) const
{
    m_rightPopularBtn->setFixedWidth(width);
}

/**
 * @brief 设置左侧流行度按钮宽度
 * @param width 宽度
 */
void MyBlockWidget::setLeftPopularBtnWidth(const int &width) const
{
    m_leftPopularBtn->setFixedWidth(width);
}

/**
 * @brief 设置右侧流行度按钮字体大小
 * @param size 字体大小
 */
void MyBlockWidget::setRightPopularBtnFontSize(const int &size) const
{
    auto font = m_leftPopularBtn->font();
    font.setPixelSize(size);
    m_leftPopularBtn->setFont(font);
}

/**
 * @brief 设置左侧流行度按钮字体大小和加粗
 * @param size 字体大小
 * @param isBold 是否加粗
 */
void MyBlockWidget::setLeftPopularBtnFontSize(const int &size, const bool &isBold) const
{
    auto font = m_leftPopularBtn->font();
    font.setPixelSize(size);
    font.setBold(isBold);
    m_leftPopularBtn->setFont(font);
}

/**
 * @brief 设置宽高比
 * @param aspectRatio 宽高比
 */
void MyBlockWidget::setAspectRatio(const float &aspectRatio)
{
    m_aspectRatio = aspectRatio;
}

/**
 * @brief 设置流行度按钮左边距
 * @param leftPadding 左边距
 */
void MyBlockWidget::setPopularBtnLeftPadding(const int &leftPadding)
{
    if (!m_popularDirection)
        return;
    if (m_popularDirection == 1)
    {
        if (leftPadding == 0)
        {
            m_leftPopularBtnStyle += "text-align: left;";
            m_leftPopularBtn->setStyleSheet(m_leftPopularBtnStyle);
            return;
        }
        m_leftPopularBtnStyle += QString("padding-left:%1;").arg(leftPadding);
        m_leftPopularBtn->setStyleSheet(m_leftPopularBtnStyle);
    }
    else if (m_popularDirection == 2)
    {
        if (leftPadding == 0)
        {
            m_rightPopularBtnStyle += "text-align : left;";
            m_leftPopularBtn->setStyleSheet(m_leftPopularBtnStyle);
            return;
        }
        m_rightPopularBtnStyle += QString("padding-left:%1;").arg(leftPadding);
        m_rightPopularBtn->setStyleSheet(m_rightPopularBtnStyle + "background-color: rgba(128, 128, 128, 127);");
    }
}

/**
 * @brief 设置是否带单位（万）
 * @param haveNumberUnit 是否带单位
 */
void MyBlockWidget::setHaveNumberUnit(const bool &haveNumberUnit)
{
    m_haveUnit = haveNumberUnit;
}

/**
 * @brief 设置提示标签数组
 * @param tipArr 提示标签数组
 */
void MyBlockWidget::setTipArr(const QList<QString> &tipArr)
{
    m_tipArr = tipArr;
    setTipLabText(m_tipArr[QRandomGenerator::global()->bounded(0, static_cast<int>(m_tipArr.size()))]);
}

/**
 * @brief 设置提示标签样式
 * @param style 样式表
 */
void MyBlockWidget::setTipStyleSheet(const QString &style) const
{
    m_tipLab->setStyleSheet(style);
}

/**
 * @brief 显示时长按钮
 */
void MyBlockWidget::setDurationBtnShow() const
{
    m_durationBtn->show();
}

/**
 * @brief 设置时长按钮文本
 * @param text 文本内容
 */
void MyBlockWidget::setDurationBtnText(const QString &text) const
{
    m_durationBtn->setText(text);
}

/**
 * @brief 获取遮罩控件
 * @return 遮罩控件引用
 */
SMaskWidget &MyBlockWidget::getMask() const
{
    return *m_mask;
}

/**
 * @brief 显示遮罩
 */
void MyBlockWidget::onShowMask()
{
    m_mask->show();
    m_mask->raise();
    QEvent enter(QEvent::Enter);
    leaveEvent(&enter);
    update();
}

/**
 * @brief 隐藏遮罩
 */
void MyBlockWidget::onHideMask()
{
    m_mask->hide();
    QEvent leave(QEvent::Leave);
    leaveEvent(&leave);
    update();
}

/**
 * @brief 绘制事件
 * @param ev 绘图事件对象
 */
void MyBlockWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/**
 * @brief 鼠标进入事件
 * @param ev 进入事件对象
 */
void MyBlockWidget::enterEvent(QEnterEvent *ev)
{
    QWidget::enterEvent(ev);
    if (!m_isExpandRespond)
    {
        if (!m_isHoverCover)
        {
            m_isHoverCover = true;
            m_mask->show();
            if (m_mask->getMove())
                m_mask->animationUp();
            m_mask->raise();
            m_rightPopularBtn->setStyleSheet(m_rightPopularBtnStyle + "background-color: rgba(60,60,60, 127);");
            update();
        }
    }
}

/**
 * @brief 鼠标离开事件
 * @param ev 事件对象
 */
void MyBlockWidget::leaveEvent(QEvent *ev)
{
    QWidget::leaveEvent(ev);
    if (!m_isExpandRespond)
    {
        if (m_isHoverCover)
        {
            m_isHoverCover = false;
            if (m_mask->getMove())
                m_mask->animationDown();
            m_mask->hide();
            m_rightPopularBtn->setStyleSheet(m_rightPopularBtnStyle + "background-color: rgba(60,60,60, 127);");
            update();
        }
    }
}

/**
 * @brief 大小调整事件
 * @param event 大小调整事件对象
 */
void MyBlockWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_bacWidget->setFixedSize(static_cast<int>(event->size().width() / 1.01), static_cast<int>(event->size().width() / (1.01 * m_aspectRatio)));
    m_mask->setFixedSize(m_bacWidget->size());
    m_rightPopularBtn->move(m_bacWidget->width() - m_rightPopularBtn->width() - 5, m_bacWidget->height() - m_rightPopularBtn->height() - 5);
    m_leftPopularBtn->move(5, m_bacWidget->height() - m_leftPopularBtn->height() - 5);
    m_durationBtn->move(m_bacWidget->width() - m_durationBtn->width() - 5, m_bacWidget->height() - m_durationBtn->height() - 5);
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件对象
 */
void MyBlockWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件对象
 */
void MyBlockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件对象
 */
void MyBlockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
}