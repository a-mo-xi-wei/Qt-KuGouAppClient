//
// Created by WeiWang on 25-3-21.
//

#include "AboutDialog.h"
#include "qtmaterialdialog.h"
#include "logger.hpp"
#include "RippleButton.h"

#include <QFontDatabase>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QPainter>
#include <QRandomGenerator>
#include <QStyleOption>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent)
    : QWidget(parent)
    , m_dialog(new QtMaterialDialog)
    , m_topWidget(new QWidget(this))
{
    initFont();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setContentsMargins(0, 0, 0, 0);

    m_dialog->setParent(parent);
    auto dialogLayout = new QVBoxLayout;
    dialogLayout->setContentsMargins(0, 0, 0, 0);

    m_dialog->setWindowLayout(dialogLayout);
    m_dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto dialogWidget = new QWidget;
    dialogWidget->setMinimumHeight(400);

    dialogLayout->addWidget(dialogWidget);

    auto dialogWidgetLayout = new QVBoxLayout(dialogWidget);
    // 确保父布局边距清零
    dialogWidgetLayout->setContentsMargins(0, 0, 0, 0);
    dialogWidgetLayout->setSpacing(0);
    initDialog(dialogWidgetLayout);
}

void AboutDialog::initDialog(QVBoxLayout *lay) {
    //最上面先来个widget,填充背景图片
    this->m_topWidget->setFixedHeight(100);
    this->m_topWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_topWidget->setStyleSheet(QString("border-image: url(:/RectCover/Res/rectcover/music-rect-cover%1.jpg)").
        arg(QString::number(QRandomGenerator::global()->bounded(1,20))));

    lay->addWidget(this->m_topWidget);
    lay->addStretch();

    auto goToGiteeButton = new RippleButton(m_dialog);
    goToGiteeButton->setCursor(Qt::PointingHandCursor);
    goToGiteeButton->setText("前往Gitee");
    goToGiteeButton->setFillColor(QColor(QStringLiteral("#969696")));
    goToGiteeButton->setStyleSheet("background: transparent; border: none;");
    goToGiteeButton->setRadius(10);
    goToGiteeButton->setSpeed(5);
    this->m_font.setPointSize(10);
    goToGiteeButton->setFont(this->m_font);
    auto closeButton = new RippleButton(m_dialog);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setText("关闭");
    closeButton->setFillColor(QColor(QStringLiteral("#FF0066")));
    closeButton->setStyleSheet("background: transparent; border: none;");
    closeButton->setRadius(10);
    closeButton->setFont(this->m_font);

    auto bottomHLay = new QHBoxLayout;
    bottomHLay->addSpacing(15);
    bottomHLay->addWidget(goToGiteeButton);
    bottomHLay->addStretch();
    bottomHLay->addWidget(closeButton);
    bottomHLay->addSpacing(15);

    lay->addLayout(bottomHLay);
    lay->addSpacing(15);
    lay->setAlignment(bottomHLay, Qt::AlignBottom);

    closeButton->setMaximumWidth(150);
    connect(goToGiteeButton, &RippleButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://gitee.com/a-mo-xi-wei/KuGouApp"));
    });
    connect(closeButton, &RippleButton::pressed, m_dialog, &QtMaterialDialog::hideDialog);

}

void AboutDialog::initFont() {
    int fontId = QFontDatabase::addApplicationFont(":/Res/font/JetBrainsMonoNerdFont-Bold.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";
        return;
    }
    auto fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    this->m_font.setFamily(fontFamily);
}

void AboutDialog::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AboutDialog::onShowDialog() {
    this->m_topWidget->setStyleSheet(QString("border-image: url(:/RectCover/Res/rectcover/music-rect-cover%1.jpg)").
        arg(QString::number(QRandomGenerator::global()->bounded(1,100))));
    m_dialog->showDialog();
}

void AboutDialog::onHideDialog() {
    m_dialog->hideDialog();
}
