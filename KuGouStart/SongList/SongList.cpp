//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SongList.h" resolved

#include "SongList.h"
#include "ui_SongList.h"
#include "SongBlock.h"
#include "MyFlowLayout.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

SongList::SongList(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::SongList) {
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/list.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initCoverVector();
    initDescVector();
    initUi();

    auto menu = new MyMenu(MyMenu::MenuKind::ListOption, this);
    m_menu = menu->getMenu<ListOptionMenu>();
    connect(m_menu, &ListOptionMenu::clickedFuncName, this, &SongList::onMenuFuncClicked);
    connect(ui->all_toolButton, &QToolButton::clicked, this, &SongList::on_all_toolButton_clicked);
}

SongList::~SongList() {
    delete ui;
}

void SongList::initUi() { {
        //初始化全部按钮
        ui->all_toolButton->setMouseTracking(true);
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        ui->all_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF")));
        ui->all_toolButton->setApproach(true);
        ui->all_toolButton->setChangeSize(true);
        ui->all_toolButton->setEnterIconSize(QSize(10, 10));
        ui->all_toolButton->setLeaveIconSize(QSize(10, 10));
    }
    shuffleVector();
    //ui->table_widget->setContentsMargins(9,9,9,10);
    //auto lay = new MyFlowLayout(ui->table_widget,16, -1, 10);
    auto lay = new MyFlowLayout(ui->table_widget, true, 0);
    lay->setContentsMargins(0, 20, 0, 20);
    ui->table_widget->setLayout(lay);
    //for (int  i = 0; i < this->m_descVector.size(); ++i) {
    const auto size = std::min(this->m_coverVector.size(), this->m_descVector.size());
    for (int i = 0; i < size; ++i) {
        auto block = new SongBlock(this);
        block->setCoverPix(this->m_coverVector[i]);
        block->setShowTip();
        block->setDescText(this->m_descVector[i]);
        lay->addWidget(block);
    }
    //按钮连接槽
    QList<QToolButton*> buttons = ui->widget->findChildren<QToolButton*>();
    for (const auto& button : buttons) {
        connect(button, &QToolButton::clicked, this, [this, button] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                        QString("%1 功能未实现 敬请期待").arg(button->text()),1000,this->window());
        });
    }
}

void SongList::initCoverVector() {
    for (int i = 1; i <= 210; ++i) {
        this->m_coverVector.emplace_back(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i));
    }
}

void SongList::initDescVector() {
    QFile file(GET_CURRENT_DIR + QStringLiteral("/descs.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file for reading descs.json";
        STREAM_WARN() << "Could not open file for reading descs.json";
        return;
    }
    auto obj = QJsonDocument::fromJson(file.readAll());
    auto arr = obj.array();
    for (const auto &item: arr) {
        QString title = item.toObject().value("desc").toString();
        this->m_descVector.emplace_back(title);
    }
    file.close();
    // 对 vector 排序，按 pair 的第一个元素和第二个元素排序
    std::sort(m_descVector.begin(), m_descVector.end());

    // 使用 std::unique 去重，并调整容器大小
    auto last = std::unique(m_descVector.begin(), m_descVector.end());
    m_descVector.erase(last, m_descVector.end());
}

void SongList::shuffleVector() {
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_coverVector.begin(), this->m_coverVector.end(), std::default_random_engine(seed));
    std::shuffle(this->m_descVector.begin(), this->m_descVector.end(), std::default_random_engine(seed));
}

void SongList::on_all_toolButton_clicked() {
    //显示菜单
    // 计算 Y 坐标，确保 m_menu 位于 all_toolButton 下方 20 个像素
    //const int yPos = ui->all_toolButton->mapToGlobal(QPoint(0,0)).y() + ui->all_toolButton->height() + 10;
    //
    //// 计算 X 坐标，使 m_menu 位于主窗口最右边减去自身宽度再左移 20 个像素
    //const int xPos = this->window()->geometry().right() - this->m_menu->width() - 20;
    //this->m_menu->setGeometry(xPos,yPos, this->m_menu->width(), this->m_menu->height());
    //this->m_menu->show();
    if (ui->all_toolButton->isChecked()) {
        // 更新按钮图标为向上图标
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));

        // 设置菜单的位置
        const QPoint globalPos = ui->all_toolButton->mapToGlobal(
            QPoint(ui->all_toolButton->width() * 2 - m_menu->width(),
                   ui->all_toolButton->height() + 10));

        // 设置菜单属性，不夺取焦点
        //m_menu->setWindowFlags(static_cast<Qt::WindowFlags>(m_menu->windowFlags() | Qt::NoFocus));
        // 菜单显示为非阻塞模式
        //m_menu->setWindowFlags(Qt::Popup );
        m_menu->setFocusPolicy(Qt::NoFocus);
        m_menu->setAttribute(Qt::WA_TransparentForMouseEvents, false);

        // 连接菜单的隐藏信号到按钮状态更新槽
        connect(m_menu, &QMenu::aboutToHide, this, [this] {
            ui->all_toolButton->setChecked(false);
            ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
            ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
            ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        });

        // 显示菜单
        m_menu->exec(globalPos);
    } else {
        // 更新按钮图标为向下图标
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    }
}

void SongList::onMenuFuncClicked(const QString &funcName) {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                QString("%1 功能未实现 敬请期待").arg(funcName),1000,this->window());
}
