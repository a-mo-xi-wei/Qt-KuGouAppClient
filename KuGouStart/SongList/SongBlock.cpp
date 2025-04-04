//
// Created by WeiWang on 25-1-20.
//

#include "SongBlock.h"
#include "MyBlockWidget.h"

#include <QLabel>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QVBoxLayout>


SongBlock::SongBlock(QWidget *parent)
    : QWidget(parent)
    , m_cover(new MyBlockWidget(this))
    , m_descLab(new QLabel(this))
{
    initUi();
}

void SongBlock::initUi() {
    //设置指针样式
    this->m_cover->setCursor(Qt::PointingHandCursor);
    this->m_descLab->setCursor(Qt::PointingHandCursor);
    //设置布局
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 10);
    mainLayout->setSpacing(3);
    mainLayout->addWidget(m_cover);
    mainLayout->addWidget(m_descLab);
    //设置样式
    m_descLab->setStyleSheet("QLabel{color:black;font-size:14px;}QLabel:hover{color:#3AA1FF;}");
    //遮罩设置
    auto& mask = this->m_cover->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF")));
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setEnterWidgetChangeCursor(true);
    mask.setStander(130);
    //MyBlockWidget设置
    this->m_cover->setPopularDirection(2);
    this->m_cover->setHaveNumberUnit(false);

    auto number = QRandomGenerator::global()->bounded(1,5000);
    if (number <= 500) {
        this->m_cover->setHaveNumberUnit(true);
        const auto n = QRandomGenerator::global()->generateDouble() * 100;
        this->m_cover->setPopularBtnText(QString::number(n, 'f', 2));
    }
    else {
        this->m_cover->setPopularBtnText(QString::number(number));
    }
    //设置图片固定大小
    this->m_cover->setFixedSize(160,160);
}

void SongBlock::setCoverPix(const QString &pixmapPath) const {
    this->m_cover->setBorderImage(pixmapPath);
}

void SongBlock::setDescText(const QString &text) const {
    this->m_descLab->setText(text);
    // 设置提示文本
    this->m_descLab->setToolTip(this->m_descLab->text());
    //更新宽度
    updateDescLab();
}

void SongBlock::updateDescLab() const {
    this->m_descLab->setFixedWidth(this->width());
    // 设置字体测量工具
    auto font = this->m_descLab->font();
    QFontMetrics fm(font);

    // 获取当前宽度并计算剩余空间
    const int availableWidth = this->m_descLab->width()+20;

    // 获取当前文本的宽度
    const QString text = this->m_descLab->text();
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

void SongBlock::setShowTip() const {
    this->m_cover->setShowTip();
    this->m_cover->setTipStyleSheet(QStringLiteral("border-radius:10px;background-color:#797978;color:white;"));
}

void SongBlock::setTipText(const QString &text) const {
    this->m_cover->setTipLabText(text);
}

void SongBlock::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateDescLab();
}

void SongBlock::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void SongBlock::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void SongBlock::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void SongBlock::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    updateDescLab();
    //qDebug()<<"当前大小: "<<this->size();
}
