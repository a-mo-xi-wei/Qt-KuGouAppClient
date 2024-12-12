//
// Created by WeiWang on 24-12-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoChannelBlock.h" resolved

#include "VideoChannelBlock.h"
#include "ui_VideoChannelBlock.h"

#include <QFile>
#include <QRandomGenerator>
#include <QFontMetrics>
#include <QFontDatabase>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

VideoChannelBlock::VideoChannelBlock(QWidget *parent) : QWidget(parent)
                                                        , ui(new Ui::VideoChannelBlock) {
    ui->setupUi(this); {
        this->setObjectName("videoblock");
        QFile file(GET_CURRENT_DIR + QStringLiteral("/videoblock.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initCoverTextFont();
    initUi();
}

VideoChannelBlock::~VideoChannelBlock() {
    delete ui;
}

void VideoChannelBlock::setCoverPix(const QString &pixmapPath) {
    ui->cover_widget->setBorderImage(pixmapPath, 10);
}

void VideoChannelBlock::setDescription(const QString &description) {
    this->m_descriptionText = description;
}

void VideoChannelBlock::initUi() {
    //遮罩设置
    auto &mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(QColor(QStringLiteral("#525759")));
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#525759")));
    mask.setDefaultFillTriangleColor(Qt::white);
    mask.setHoverFillTriangleColor(Qt::white);
    ui->cover_widget->setAspectRatio(1.4);
    ui->cover_widget->installEventFilter(this);
}

void VideoChannelBlock::initCoverTextFont() {
    int fontId = QFontDatabase::addApplicationFont(":/Res/font/青柠幼圆.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        return;
    }
    auto fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    this->m_coverTextFont.setFamily(fontFamily);
    this->m_coverTextFont.setPixelSize(16);
}

void VideoChannelBlock::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    if (!this->m_isEnter) {
        QPainter painter(this);
        //设置字体
        painter.setFont(this->m_coverTextFont);
        QFontMetrics fm(this->m_coverTextFont);
        // 计算绘制区域
        int yPosition = height() - 100; // 在下方 100 像素的位置

        // 计算文字的宽度，确保水平居中
        int textWidth = fm.horizontalAdvance(this->m_coverText); // 获取文本宽度
        int xPosition = (width() - textWidth) / 2; // 计算水平居中位置

        // 设置字体颜色，可以根据需要调整
        painter.setPen(QPen(Qt::white));
        painter.drawText(xPosition, yPosition, this->m_coverText); // 绘制文字
    }
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
            if (!this->m_isEnter) {
                this->m_isEnter = true;
                update();
            }
        } else if (event->type() == QEvent::Leave) {
            if (this->m_isEnter) {
                this->m_isEnter = false;
                update();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
