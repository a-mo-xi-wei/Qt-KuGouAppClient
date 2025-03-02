//
// Created by WeiWang on 24-12-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoChannelBlock.h" resolved

#include "VideoChannelBlock.h"
#include "ui_VideoChannelBlock.h"
#include "logger.hpp"

#include <QFile>
#include <QRandomGenerator>
#include <QFontDatabase>
#include <QLabel>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

VideoChannelBlock::VideoChannelBlock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoChannelBlock)
    , m_coverTextLab(new QLabel(this))
{
    ui->setupUi(this); {
        this->setObjectName("videoblock");
        QFile file(GET_CURRENT_DIR + QStringLiteral("/videoblock.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initCoverTextFont();
    initUi();
}

VideoChannelBlock::~VideoChannelBlock() {
    delete ui;
}

void VideoChannelBlock::setCoverPix(const QString &pixmapPath)const {
    ui->cover_widget->setBorderImage(pixmapPath, 10);
}

void VideoChannelBlock::setDescription(const QString &description) {
    this->m_descriptionText = description;
}

void VideoChannelBlock::setCoverText(const QString &text)const {
    this->m_coverTextLab->setFont(this->m_coverTextFont);
    this->m_coverTextLab->setText(text);
    // 计算绘制区域
    int yPosition = height() - 95; // 在下方 40 像素的位置
    // 计算文字的宽度，确保水平居中
    int textWidth = this->m_coverTextLab->width(); // 获取文本宽度
    int xPosition = (width() - textWidth) / 2; // 计算水平居中位置
    this->m_coverTextLab->move(xPosition,yPosition);
    this->m_coverTextLab->raise();
    // 打印位置和文本宽度信息
    //qDebug() << "Text position: (" << xPosition << ", " << yPosition << ")";
    //qDebug() << "Text width: " << textWidth;
}

void VideoChannelBlock::initUi() {
    //遮罩设置
    auto &mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(QColor(QStringLiteral("#525759")));
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#525759")));
    mask.setDefaultFillTriangleColor(Qt::white);
    mask.setHoverFillTriangleColor(Qt::white);
    mask.setMaskColor(QColor(0,0,0,100));
    ui->cover_widget->setAspectRatio(1.5);
    ui->cover_widget->installEventFilter(this);
    //设置coverTextLab
    this->m_coverTextLab->setScaledContents(true);
    this->m_coverTextLab->setAlignment(Qt::AlignCenter);
    this->m_coverTextLab->setFixedWidth(this->width()-20);
    //设置toolButton
    ui->desc_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->desc_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/eye-gray.svg")));
    ui->desc_toolButton->setText(QString::number(QRandomGenerator::global()->bounded(1,500))+"人在看");
}

void VideoChannelBlock::initCoverTextFont() {
    int fontId = QFontDatabase::addApplicationFont(":/Res/font/qing-ning-you-yuan.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";
        return;
    }
    auto fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    this->m_coverTextFont.setFamily(fontFamily);
    this->m_coverTextFont.setPixelSize(16);
}

void VideoChannelBlock::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void VideoChannelBlock::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void VideoChannelBlock::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

bool VideoChannelBlock::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->cover_widget) {
        if (event->type() == QEvent::Enter) {
            this->m_coverTextLab->hide();
        } else if (event->type() == QEvent::Leave) {
            this->m_coverTextLab->show();
            this->m_coverTextLab->raise();
        }
    }
    return QWidget::eventFilter(watched, event);
}
