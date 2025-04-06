#include "GalleryPhotoWidget.h"
#include "ElaToolTip.h"

#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

int GalleryPhotoWidget::fixed_width = 160;
int GalleryPhotoWidget::fixed_height = 240;
int GalleryPhotoWidget::content_width = 130;
int GalleryPhotoWidget::content_height = 200;
int GalleryPhotoWidget::pixmap_width = 130;
int GalleryPhotoWidget::pixmap_height = 130;

#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

GalleryPhotoWidget::GalleryPhotoWidget(QWidget *parent)
    : WaterZoomButton("", parent)
    , m_coverWidget(new MyBlockWidget(this))
    , m_titleLab(new QLabel(this))
    , m_descLab(new QLabel(this))
    , m_shadowEffect(new QGraphicsDropShadowEffect(this))
{
    setNormalColor(Qt::white);
    setHoverColor(Qt::white);
    setChoking(10);
    setRadius(15, 15);
    setFixedSize(fixed_width, fixed_height);
    //初始化ui界面
    initUi();
    {
        if (QFile file(GET_CURRENT_DIR + QStringLiteral("/photo.css")); file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

void GalleryPhotoWidget::setCoverPix(const QString &pixmapPath) const {
    this->m_coverWidget->setBorderImage(pixmapPath,10);
}

void GalleryPhotoWidget::setTitleText(const QString &title) {
    this->m_titleText = title;
    this->m_titleLab->setText(title);
    auto titleLab_toolTip = new ElaToolTip(this->m_titleLab);
    titleLab_toolTip->setToolTip(title);
    updateTitleText();
}

void GalleryPhotoWidget::setDescribeText(const QString &desc) {
    this->m_describeText = desc;
    this->m_descLab->setText(desc);
    updateDescText();
}

void GalleryPhotoWidget::setPopularText(const QString &text) const {
    this->m_coverWidget->setPopularBtnText(text);
}

void GalleryPhotoWidget::initUi() {
    // 创建阴影效果
    m_shadowEffect->setBlurRadius(10);            // 阴影模糊半径
    m_shadowEffect->setColor(Qt::gray);         // 阴影颜色
    m_shadowEffect->setOffset(3,3);      // 阴影偏移量 (x, y)
    m_shadowEffect->setEnabled(false);
    this->setGraphicsEffect(this->m_shadowEffect);
    //设置背景色
    this->setBgColor(QColor(QStringLiteral("#F0F8FF")));
    this->setBgColor(QColor(QStringLiteral("#ECF6FF")),this->press_bg);
    //设置对象名
    this->m_titleLab->setObjectName("titleLab");
    this->m_descLab->setObjectName("descLab");
    this->m_titleLab->setMouseTracking(true);
    //遮罩设置
    auto &mask = this->m_coverWidget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF")));
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setMaskColor(QColor(0,0,0,100));
    mask.setStander(120);
    //设置block
    this->m_coverWidget->setPopularDirection(1);
    this->m_coverWidget->setHaveNumberUnit(false);
    this->m_coverWidget->setLeftPopularBtnIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg"));
}

void GalleryPhotoWidget::updateTitleText() const {
    // 设置字体测量工具
    const auto font = this->m_titleLab->font();
    const QFontMetrics fm(font);

    // 获取当前宽度并计算剩余空间
    const int availableWidth = this->m_titleLab->width();

    // 获取当前文本的宽度
    const QString text = this->m_titleText;
    QString elidedText = text;
    // 找到合适的换行位置
    int breakIndex = 0;
    for (int i = 0; i < text.length(); ++i) {
        if (fm.horizontalAdvance(text.left(i)) > availableWidth) {
            breakIndex = i - 1;
            break;
        }
    }

    // 分割文本为两部分
    const QString firstLine = text.left(breakIndex);
    const QString secondLine = text.mid(breakIndex);
    //qDebug()<<"secondLine: "<<secondLine;
    // 处理第二行文本，超出部分使用省略号
    const QString secondLineElided = fm.elidedText(secondLine, Qt::ElideRight, availableWidth);
    // qDebug()<<"secondLineElided: "<<secondLineElided
    // <<" secondLineElided.width(): "<<fm.horizontalAdvance(secondLineElided)
    // <<" availableWidth : "<< availableWidth;

    // 拼接两行文本
    elidedText = firstLine + secondLineElided;

    // 设置文本到 QLabel
    this->m_titleLab->setText(elidedText);
    this->m_titleLab->setWordWrap(true);

    // 设置固定高度为两行文本的高度
    const int lineHeight = fm.lineSpacing(); // 单行高度（包含行间距）
    this->m_titleLab->setFixedHeight(2 * lineHeight); // 两行高度
}

void GalleryPhotoWidget::updateDescText() const {
    // 设置字体测量工具
    const auto font = this->m_descLab->font();
    const QFontMetrics fm(font);

    // 获取当前宽度并计算剩余空间
    const int availableWidth = this->m_descLab->width();

    // 获取当前文本的宽度
    const QString text = this->m_describeText;
    QString elidedText = text;
    // 找到合适的换行位置
    int breakIndex = 0;
    for (int i = 0; i < text.length(); ++i) {
        if (fm.horizontalAdvance(text.left(i)) > availableWidth) {
            breakIndex = i - 1;
            break;
        }
    }

    // 分割文本为两部分
    const QString firstLine = text.left(breakIndex);
    const QString secondLine = text.mid(breakIndex);
    //qDebug()<<"secondLine: "<<secondLine;
    // 处理第二行文本，超出部分使用省略号
    const QString secondLineElided = fm.elidedText(secondLine, Qt::ElideRight, availableWidth);
    // qDebug()<<"secondLineElided: "<<secondLineElided
    // <<" secondLineElided.width(): "<<fm.horizontalAdvance(secondLineElided)
    // <<" availableWidth : "<< availableWidth;

    // 拼接两行文本
    elidedText = firstLine + secondLineElided;

    // 设置文本到 QLabel
    this->m_descLab->setText(elidedText);
    this->m_descLab->setWordWrap(true);

    // 设置固定高度为两行文本的高度
    const int lineHeight = fm.lineSpacing(); // 单行高度（包含行间距）
    this->m_descLab->setFixedHeight(2 * lineHeight); // 两行高度
}

void GalleryPhotoWidget::paintEvent(QPaintEvent *event)
{
    WaterZoomButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    // 获取图片路径
    int c,margin;
    if (!hover_progress){
        c = choking;
        margin = 15;
        //qDebug()<<"未进入";
    }
    else{
        c = choking * (1 - getNolinearProg(hover_progress, hovering?FastSlower:SlowFaster));
        margin = sqrt(125-hover_progress);
        //qDebug()<<"进入";
    }
    const QRect rect(c+margin,c+margin,this->width()-c*2-margin*2,(this->width()-c*2-margin*2)*pixmap_height/pixmap_width);

    this->m_coverWidget->move(rect.left(),rect.top());
    this->m_coverWidget->setFixedSize(rect.size());
    // 设置标签位置
    this->m_titleLab->move(this->m_coverWidget->x(),this->m_coverWidget->y()+this->m_coverWidget->height()+5);
    this->m_descLab->move(this->m_titleLab->x(),this->m_titleLab->y()+this->m_titleLab->height()+5);
    // 设置标签宽度
    this->m_titleLab->setFixedWidth(this->m_coverWidget->width());
    updateTitleText();
    this->m_descLab->setFixedWidth(this->m_coverWidget->width());
    updateDescText();
}

void GalleryPhotoWidget::mouseMoveEvent(QMouseEvent *event) {
    WaterZoomButton::mouseMoveEvent(event);
    // 将鼠标位置转换为标题标签的本地坐标
    // 判断鼠标是否悬停在标签区域
    // 根据悬停状态设置样式
    if (this->m_titleLab->rect().contains(this->m_titleLab->mapFromParent(event->pos()))) {
        this->m_titleLab->setStyleSheet("QLabel#titleLab { color: #2291e6; font-size: 15px; }");
    }
    else {
        this->m_titleLab->setStyleSheet("QLabel#titleLab { color: black; font-size: 15px; }");
    }
}

void GalleryPhotoWidget::leaveEvent(QEvent *event) {
    WaterZoomButton::leaveEvent(event);
    this->m_titleLab->setStyleSheet("QLabel#titleLab { color: black; font-size: 15px; }");
    m_shadowEffect->setEnabled(false);
}

void GalleryPhotoWidget::enterEvent(QEnterEvent *event) {
    WaterZoomButton::enterEvent(event);
    m_shadowEffect->setEnabled(true);
}