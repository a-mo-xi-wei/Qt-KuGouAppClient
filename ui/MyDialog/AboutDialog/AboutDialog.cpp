//
// Created by WeiWang on 25-3-21.
//

#include "AboutDialog.h"
#include "qtmaterialdialog.h"
#include "logger.hpp"
#include "RippleButton.h"
#include "ElaText.h"
#include "Async.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>

AboutDialog::AboutDialog(QWidget *parent)
    : QWidget(parent)
    , m_dialog(new QtMaterialDialog)
    , m_topWidget(new QWidget(this))
{
    this->setAutoFillBackground(true);  // 启用背景填充
    this->m_font = QFont("AaSongLiuKaiTi");

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setContentsMargins(0, 0, 0, 0);

    auto dialogLayout = new QVBoxLayout;
    dialogLayout->setContentsMargins(0, 0, 0, 0);

    m_dialog->setParent(parent);
    m_dialog->setWindowLayout(dialogLayout);
    m_dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto dialogWidget = new QWidget;
    dialogWidget->setAttribute(Qt::WA_TranslucentBackground,false);
    dialogWidget->setFixedSize(500,480);

    dialogLayout->addWidget(dialogWidget);

    auto dialogWidgetLayout = new QVBoxLayout(dialogWidget);
    // 确保父布局边距清零
    dialogWidgetLayout->setContentsMargins(0, 0, 0, 0);
    dialogWidgetLayout->setSpacing(0);
    initDialog(dialogWidgetLayout);
    this->setStyleSheet("background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:0, y2:1,stop:0 rgba(105, 225, 255, 200), stop:1 rgba(255, 182, 193, 200));");
}

void AboutDialog::initDialog(QVBoxLayout *lay) {
    //最上面先来个widget,填充背景图片
    this->m_topWidget->setFixedHeight(90);
    this->m_topWidget->setContentsMargins(0, 0, 0, 0);
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
    QString url = "https://gitee.com/api/v5/repos/a-mo-xi-wei/KuGouApp";
    const auto future = Async::runAsync(QThreadPool::globalInstance(), &AboutDialog::getGiteeProjectStar,
        this,url);
    Async::onResultReady(future, this, [=](const int &stars) {
        //中间的介绍文本
        QString descText = QString("\n  My KuGou Client Version : 1.0\n"
            " 开发环境: Windows x64 , Qt 6.6.2 , CLion , MinGW , Ninja\n\n"
            " 本项目代码仅限研究和学习使用，资源版权归音乐平台所有\n"
            " 代码已开源, 并无商用, 任何版权问题概不负责 No Copyright\n\n"
            " 开发者邮箱: 1428206861@qq.com | ww1428206861@gmail.com\n\n"
            " 本项目是临时起意所做, 许多功能有待完善, 在开发过程中本人\n 也成长了很多"
            "也突破了各种各样的项目难点, 同时也引入了各个\n 大神开源的组件, 节省了许多时间, 特此致谢\n\n"
            " 🎁如果你喜欢该项目, 请点击左下角跳转至 Gitee项目网址, 点击\n 右上角的✨星星✨, 你的 Star 是我最大的动力\n\n"
            " 目前该项目在Gitee上已获 %1 颗星, 撸起袖子加油干 !!! \n\n").arg(stars);
        ElaText* descTitle = new ElaText(descText, this);
        auto style = descTitle->styleSheet();
        descTitle->setStyleSheet("background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:0, y2:1,stop:0 rgba(105, 225, 255, 200), stop:1 rgba(255, 182, 193, 200));");
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

        auto bottomWidget = new QWidget(this);
        bottomWidget->setStyleSheet("background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:0, y2:1,stop:0 rgba(255, 182, 193, 200), stop:1 rgba(105, 225, 255, 200));");
        auto bottomHLay = new QHBoxLayout(bottomWidget);
        bottomHLay->addSpacing(15);
        bottomHLay->addWidget(goToGiteeButton);
        bottomHLay->addStretch();
        bottomHLay->addWidget(closeButton);
        bottomHLay->addSpacing(15);

        lay->addWidget(bottomWidget);
        lay->setAlignment(bottomHLay, Qt::AlignBottom);

        closeButton->setMaximumWidth(150);
        connect(goToGiteeButton, &RippleButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://gitee.com/a-mo-xi-wei/KuGouApp"));
        });
        connect(closeButton, &RippleButton::pressed, m_dialog, &QtMaterialDialog::hideDialog);
    });

}

int AboutDialog::getGiteeProjectStar(const QString &url) {
    const QString reply = m_libHttp.UrlRequestGet(url, "");
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(reply.toUtf8());

    QJsonObject jsonObj = jsonDoc.object();

    // 检查是否包含 stargazers_count 字段
    if (!jsonObj.contains("stargazers_count")) {
        qWarning() << "Invalid JSON response: stargazers_count not found";
        STREAM_WARN()<< "Invalid JSON response: stargazers_count not found";
        return -1;
    }
    STREAM_INFO()<<"获取到当前 Star 数量 : "<<jsonObj["stargazers_count"].toInt();
    // 返回 Star 数量
    return jsonObj["stargazers_count"].toInt();
}

void AboutDialog::onShowDialog() {
    this->m_topWidget->setStyleSheet(QString("QWidget#topWidget{background-color: transparent;border-image: url(:/RectCover/Res/rectcover/music-rect-cover%1.jpg);}").
        arg(QRandomGenerator::global()->bounded(1,20)));
    m_dialog->showDialog();
}

void AboutDialog::onHideDialog() {
    m_dialog->hideDialog();
}
