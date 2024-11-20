//
// Created by WeiWang on 24-11-18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenBook.h" resolved

#include "ListenBook.h"

#include <QButtonGroup>

#include "ui_ListenBook.h"

#include<QFile>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListenBook::ListenBook(QWidget *parent) :
    QWidget(parent), ui(new Ui::ListenBook)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/listen.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();
    ui->refresh_toolButton->installEventFilter(this);
}

ListenBook::~ListenBook() {
    delete ui;
}

void ListenBook::initUi() {
    {
        //设置互斥
        this->m_buttonGroup->addButton(ui->recommend_toolButton);
        this->m_buttonGroup->addButton(ui->download_toolButton);
        this->m_buttonGroup->addButton(ui->recently_toolButton);
        this->m_buttonGroup->setExclusive(true);

        //设置按钮形式
        ui->recommend_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->download_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->recently_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //设置图标
        ui->recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-black.svg")));
        ui->download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-gray.svg")));
        ui->recently_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-gray.svg")));

        ui->recommend_toolButton->setIconSize(QSize(17,17));
        ui->download_toolButton->setIconSize(QSize(21,21));
        ui->recently_toolButton->setIconSize(QSize(19,19));

        //设置文本
        ui->recommend_toolButton->setText(QStringLiteral("   推荐"));
        ui->download_toolButton->setText(QStringLiteral("  下载"));
        ui->recently_toolButton->setText(QStringLiteral("   最近"));

        //设置选中图标
        connect(ui->recommend_toolButton, &QToolButton::toggled, ui->recommend_toolButton, [=](bool checked){
            if (checked) {
                ui->recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-black.svg")));
            } else {
                ui->recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-gray.svg")));
            }
        });
        connect(ui->download_toolButton, &QToolButton::toggled, ui->download_toolButton, [=](bool checked){
            if (checked) {
                ui->download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-black.svg")));
            } else {
                ui->download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-gray.svg")));
            }
        });
        connect(ui->recently_toolButton, &QToolButton::toggled, ui->recently_toolButton, [=](bool checked){
            if (checked) {
                ui->recently_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-black.svg")));
            } else {
                ui->recently_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-gray.svg")));
            }
        });
    }
    {
        //显示隐藏按钮
        ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                    background-color:rgba(255,255,255,0);
                                                    border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
                                                }
                                                 QToolButton:hover{
                                                    border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
                                                })");

        connect(ui->indicator_toolButton, &QToolButton::toggled, ui->indicator_toolButton, [=](bool checked){
            if (checked) {
                ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                            background-color:rgba(255,255,255,0);
                                                            border-image:url(:/ListenBook/Res/listenbook/down-black.svg);
                                                        }
                                                        QToolButton:hover{
                                                            border-image:url(:/ListenBook/Res/listenbook/down-blue.svg);
                                                        })");
            } else {
                ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                            background-color:rgba(255,255,255,0);
                                                            border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
                                                        }
                                                        QToolButton:hover{
                                                            border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
                                                        })");
            }
        });
    }
    {
        //设置全部分类按钮
        ui->all_classify_toolButton->setHoverFontColor(QColor(QStringLiteral("#26A1FF")));
        ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
        ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
        ui->all_classify_toolButton->setIconSize(QSize(10,10));
        ui->all_classify_toolButton->setApproach(true);
    }
    {
        //设置换一批按钮
        ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
        ui->refresh_toolButton->setText(" 换一批");
    }

}

bool ListenBook::eventFilter(QObject *watched, QEvent *event) {
    // 检查是否是 refresh_toolButton 的事件
    if (watched == ui->refresh_toolButton) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入 refresh_toolButton
            //qDebug() << "Mouse entered refresh_toolButton";
            ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-blue.svg")));
            return true; // 如果希望阻止默认行为，可以返回 true
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开 refresh_toolButton
            //qDebug() << "Mouse left refresh_toolButton";
            ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}
