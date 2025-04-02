//
// Created by WeiWang on 25-3-21.
//

#include "AboutDialog.h"
#include "qtmaterialdialog.h"
#include "logger.hpp"
#include "RippleButton.h"
#include "ElaText.h"
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent)
    : QWidget(parent)
    , m_dialog(new QtMaterialDialog)
    , m_topWidget(new QWidget(this))
{
    this->m_font = QFont("AaSongLiuKaiTi");

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setContentsMargins(0, 0, 0, 0);

    auto dialogLayout = new QVBoxLayout;
    dialogLayout->setContentsMargins(0, 0, 0, 0);

    m_dialog->setParent(parent);
    m_dialog->setWindowLayout(dialogLayout);
    m_dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto dialogWidget = new QWidget;
    dialogWidget->setAttribute(Qt::WA_TranslucentBackground);
    dialogWidget->setFixedSize(500,450);

    dialogLayout->addWidget(dialogWidget);

    auto dialogWidgetLayout = new QVBoxLayout(dialogWidget);
    // 确保父布局边距清零
    dialogWidgetLayout->setContentsMargins(0, 0, 0, 0);
    dialogWidgetLayout->setSpacing(0);
    initDialog(dialogWidgetLayout);
}

void AboutDialog::initDialog(QVBoxLayout *lay) {
    //最上面先来个widget,填充背景图片
    this->m_topWidget->setFixedHeight(90);
    this->m_topWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_topWidget->setObjectName("topWidget");
    this->m_topWidget->setStyleSheet(QString("QWidget#topWidget{background-color: transparent;border-image: url(:/RectCover/Res/rectcover/music-rect-cover%1.jpg);}").
        arg(QRandomGenerator::global()->bounded(1,20)));
    auto topWidgetLayout = new QHBoxLayout(this->m_topWidget);
    topWidgetLayout->addStretch();
    auto lab1 = new QLabel(this->m_topWidget);
    lab1->setFixedSize(50,50);
    lab1->setStyleSheet("border-image:url(':/Res/window/windowIcon.png')");
    auto lab2 = new QLabel(this->m_topWidget);
    lab2->setFixedHeight(80);
    lab2->setText("我的酷狗");
    lab2->setStyleSheet("background-color: transparent;color: #269dff;border: none;");
    this->m_font.setPointSize(28);
    this->m_font.setBold(true);
    lab2->setFont(this->m_font);
    topWidgetLayout->addWidget(lab1);
    topWidgetLayout->addWidget(lab2);
    topWidgetLayout->addStretch();

    //中间的介绍文本
    QString descText = "\n  My KuGou Client Version : 1.0\n"
        " 开发环境: Qt 6.6.2 , Windows x64 , CLion , MinGW , Ninja\n\n"
        " 本项目代码仅限研究和学习使用，资源版权归音乐平台所有\n"
        " 代码已开源, 并无商用, 任何版权问题概不负责 No Copyright\n\n"
        " 开发者邮箱: 1428206861@qq.com | ww1428206861@gmail.com\n\n"
        " 本项目是临时起意所做, 有许多功能有待完善, 在开发过程中本人\n 也成长了许多"
        " 也突破了各种各样的项目难点, 同时也引入了各个\n 大神开源的组件, 节省了许多时间, 特此致谢\n\n"
        " 🎁如果你喜欢该项目, 请点击左下角跳转至 Gitee项目网址, 点击\n 右上角的✨星星✨, 你的 Star 是我最大的动力\n\n";
    ElaText* descTitle = new ElaText(descText, this);
    descTitle->setTextStyle(ElaTextType::BodyStrong);
    auto font = QFont("AaSongLiuKaiTi");//需要通过KuGouApp::initFontRes()打印得知真实字体名
    font.setPixelSize(17);
    font.setWeight(QFont::Bold);
    descTitle->setFont(font);

    lay->addWidget(this->m_topWidget);
    lay->addWidget(descTitle);
    lay->addStretch();

    //底部两个按钮
    auto goToGiteeButton = new RippleButton(m_dialog);
    goToGiteeButton->setCursor(Qt::PointingHandCursor);
    goToGiteeButton->setText("前往Gitee");
    goToGiteeButton->setFillColor(QColor(QStringLiteral("#969696")));
    goToGiteeButton->setStyleSheet("background: transparent; border: none;");
    goToGiteeButton->setRadius(10);
    goToGiteeButton->setSpeed(5);
    this->m_font.setPointSize(13);
    this->m_font.setBold(false);
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

void AboutDialog::onShowDialog() {
    this->m_topWidget->setStyleSheet(QString("QWidget#topWidget{background-color: transparent;border-image: url(:/RectCover/Res/rectcover/music-rect-cover%1.jpg);}").
        arg(QRandomGenerator::global()->bounded(1,20)));
    m_dialog->showDialog();
}

void AboutDialog::onHideDialog() {
    m_dialog->hideDialog();
}
