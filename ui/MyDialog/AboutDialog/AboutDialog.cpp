/**
 * @file AboutDialog.cpp
 * @brief 实现 AboutDialog 类，提供关于对话框功能
 * @author WeiWang
 * @date 2025-03-21
 * @version 1.0
 */

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

/**
 * @brief 构造函数，初始化关于对话框
 * @param parent 父控件指针，默认为 nullptr
 */
AboutDialog::AboutDialog(QWidget *parent)
    : QWidget(parent)
    , m_dialog(new QtMaterialDialog(this))
    , m_topWidget(new QWidget(this))
    , m_stars(90)
{
    this->setAutoFillBackground(true);
    this->m_font = QFont("AaSongLiuKaiTi");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setContentsMargins(0, 0, 0, 0);

    // 初始化对话框控件
    auto dialogWidget = new QWidget(this);
    dialogWidget->setObjectName("dialogWidget");
    dialogWidget->setStyleSheet(
        "background-color: qlineargradient("
        "spread:pad,"
        "x1:0, y1:0, x2:0, y2:1,"
        "stop:0    rgba(105, 225, 255, 200),"
        "stop:0.1875 rgba(105, 225, 255, 200),"
        "stop:0.1875 rgba(105, 225, 255, 200),"
        "stop:0.9167 rgba(255, 182, 193, 200),"
        "stop:1    rgba(105, 225, 255, 200)"
        ");"
    );
    dialogWidget->setFixedSize(500, 480);

    // 设置对话框布局
    auto dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addWidget(dialogWidget);

    m_dialog->setParent(parent);
    m_dialog->setWindowLayout(dialogLayout);
    m_dialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 初始化对话框内容
    auto dialogWidgetLayout = new QVBoxLayout(dialogWidget);
    dialogWidgetLayout->setContentsMargins(0, 0, 0, 0);
    dialogWidgetLayout->setSpacing(0);
    initDialog(dialogWidgetLayout);
}

/**
 * @brief 初始化对话框布局
 * @param lay 垂直布局指针
 */
void AboutDialog::initDialog(QVBoxLayout *lay)
{
    // 初始化顶部控件
    this->m_topWidget->setFixedHeight(90);
    this->m_topWidget->setContentsMargins(0, 0, 0, 0);
    this->m_topWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->m_topWidget->setObjectName("topWidget");
    this->m_topWidget->setStyleSheet(QString("QWidget#topWidget{background-color: transparent;border-image: url(:/RectCover/Res/rectcover/music-rect-cover%1.jpg);}")
                                         .arg(QRandomGenerator::global()->bounded(1, 20)));

    // 顶部布局
    auto topWidgetLayout = new QHBoxLayout(this->m_topWidget);
    topWidgetLayout->addStretch();
    auto lab1 = new QLabel(this->m_topWidget);
    lab1->setFixedSize(50, 50);
    lab1->setStyleSheet("background-color: transparent;border-image:url(':/Res/window/windowIcon.png')");
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

    // 初始化描述文本
    QString url = "https://gitee.com/api/v5/repos/a-mo-xi-wei/KuGouApp";
    Async::runAsync(QThreadPool::globalInstance(), &AboutDialog::getGiteeProjectStar, this, url);
    ElaText* descTitle = new ElaText(m_dialog);
    descTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    initDescText(descTitle);
    connect(this, &AboutDialog::gotStars, this, [this, descTitle] {
        initDescText(descTitle);
    });
    descTitle->setTextInteractionFlags(Qt::TextSelectableByMouse);
    descTitle->setStyleSheet(
        "background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:0, y2:1,stop:0 rgba(105, 225, 255, 200), stop:1 rgba(255, 182, 193, 200));"
        "selection-background-color: rgb(0, 120, 215);selection-color: rgb(255, 255, 255);"
    );
    descTitle->setTextStyle(ElaTextType::BodyStrong);
    auto font = QFont("AaSongLiuKaiTi");
    font.setPixelSize(17);
    font.setWeight(QFont::Bold);
    descTitle->setFont(font);

    // 初始化底部控件
    auto bottomWidget = new QWidget;
    bottomWidget->setFixedHeight(40);
    {
        auto goToGiteeButton = new RippleButton;
        auto closeButton = new RippleButton;

        goToGiteeButton->setCursor(Qt::PointingHandCursor);
        goToGiteeButton->setText("前往Gitee");
        goToGiteeButton->setFillColor(QColor(QStringLiteral("#969696")));
        goToGiteeButton->setStyleSheet("background: transparent; border: none;");
        goToGiteeButton->setRadius(10);
        goToGiteeButton->setSpeed(5);
        this->m_font.setPointSize(13);
        this->m_font.setBold(false);
        goToGiteeButton->setFont(this->m_font);

        closeButton->setCursor(Qt::PointingHandCursor);
        closeButton->setText("关闭");
        closeButton->setFillColor(QColor(QStringLiteral("#FF0066")));
        closeButton->setStyleSheet("background: transparent; border: none;");
        closeButton->setRadius(10);
        closeButton->setFont(this->m_font);

        bottomWidget->setStyleSheet(
            "background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:0, y2:1,stop:0 rgba(255, 182, 193, 200), stop:1 rgba(105, 225, 255, 200));"
        );
        auto bottomHLay = new QHBoxLayout(bottomWidget);
        bottomHLay->addSpacing(15);
        bottomHLay->addWidget(goToGiteeButton);
        bottomHLay->addStretch();
        bottomHLay->addWidget(closeButton);
        bottomHLay->addSpacing(15);

        connect(goToGiteeButton, &RippleButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://gitee.com/a-mo-xi-wei/KuGouApp"));
        });
        connect(closeButton, &RippleButton::pressed, this, &AboutDialog::onHideDialog);
    }

    // 添加控件到布局
    lay->addWidget(this->m_topWidget);
    lay->addWidget(descTitle);
    lay->addWidget(bottomWidget);
    //lay->setAlignment(bottomHLay, Qt::AlignBottom);
    //lay->addLayout(bottomHLay);

}

/**
 * @brief 获取 Gitee 项目的 Star 数量
 * @param url Gitee API 地址
 */
void AboutDialog::getGiteeProjectStar(const QString &url)
{
    const QString reply = m_libHttp.UrlRequestGet(url, "");
    connect(&m_libHttp, &CLibhttp::httpTimeout, [=] {
        STREAM_ERROR() << "请求超时,请检查网络连接是否正常";
        return 90;
    });
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(reply.toUtf8());

    QJsonObject jsonObj = jsonDoc.object();

    // 检查是否包含 stargazers_count 字段
    if (!jsonObj.contains("stargazers_count")) {
        qWarning() << "Invalid JSON response: stargazers_count not found";
        STREAM_WARN() << "Invalid JSON response: stargazers_count not found";
        return;
    }
    STREAM_INFO() << "获取到当前 Star 数量 : " << jsonObj["stargazers_count"].toInt();
    // 返回 Star 数量
    this->m_stars = jsonObj["stargazers_count"].toInt();
    emit gotStars();
}

/**
 * @brief 初始化描述文本
 * @param text ElaText 控件指针
 */
void AboutDialog::initDescText(ElaText* text)
{
    QString descText = QString(
        "\n  My KuGou Client Version : 1.0\n"
        " 开发环境: Windows x64 , Qt 6.6.2 , CLion , MinGW , Ninja\n\n"
        " 本项目代码仅限研究和学习使用，资源版权归音乐平台所有\n"
        " 代码已开源, 并无商用, 任何版权问题概不负责 No Copyright\n\n"
        " 开发者邮箱: 1428206861@qq.com | ww1428206861@gmail.com\n\n"
        " 本项目是临时起意所做, 许多功能有待完善, 在开发过程中本人\n 成长了很多"
        "也突破了各种各样的项目难点, 同时也引入了各个\n 大神开源的组件, 节省了许多时间, 特此致谢\n\n"
        " 🎁如果你喜欢该项目, 请点击左下角跳转至 Gitee项目网址, 点击\n 右上角的✨星星✨, 你的 Star 是我最大的动力\n\n"
        " 目前该项目在Gitee上已获 %1 颗星, 撸起袖子加油干 !!! \n\n"
    ).arg(m_stars);
    text->setText(descText);
}

/**
 * @brief 显示对话框
 */
void AboutDialog::onShowDialog()
{
    this->m_topWidget->setStyleSheet(QString("QWidget#topWidget{background-color: transparent;border-image: url(:/RectCover/Res/rectcover/music-rect-cover%1.jpg);}")
                                         .arg(QRandomGenerator::global()->bounded(1, 20)));
    m_dialog->showDialog();
    emit showDialog(true);
}

/**
 * @brief 隐藏对话框
 */
void AboutDialog::onHideDialog()
{
    m_dialog->hideDialog();
    emit showDialog(false);
}