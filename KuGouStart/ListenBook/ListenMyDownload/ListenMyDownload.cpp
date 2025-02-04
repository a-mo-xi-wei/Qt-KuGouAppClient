//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenMyDownload.h" resolved

#include "ListenMyDownload.h"
#include "ui_ListenMyDownload.h"

#include <QFile>

#include <QButtonGroup>
#include <QMouseEvent>


#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListenMyDownload::ListenMyDownload(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenMyDownload)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/download.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        }
        else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

ListenMyDownload::~ListenMyDownload() {
    delete ui;
}

void ListenMyDownload::initStackedWidget() {
    initDownloadedWidget();
    initDownloadingWidget();
    //设置互斥
    this->m_buttonGroup->addButton(ui->downloaded_pushButton);
    this->m_buttonGroup->addButton(ui->downloading_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void ListenMyDownload::initUi() {
    initStackedWidget();
    initIndexLab();
    ui->downloaded_pushButton->clicked();
}

void ListenMyDownload::initIndexLab() {
    //下标图片
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->idx2_lab->hide();
    //同步状态
    ui->guide_widget1->installEventFilter(this);
    ui->guide_widget2->installEventFilter(this);
}

void ListenMyDownload::initDownloadedWidget() {
    this->m_downloaded = std::make_unique<DownloadedWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_downloaded.get());
    ui->stackedWidget->setCurrentWidget(this->m_downloaded.get());
}

void ListenMyDownload::initDownloadingWidget() {
    this->m_downloading = std::make_unique<DownloadingWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_downloading.get());
}

bool ListenMyDownload::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->downloaded_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->downloaded_pushButton->isChecked())
                ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->downloaded_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->downloaded_pushButton->isChecked())
                ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->downloaded_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->downloading_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->downloading_pushButton->isChecked())
                ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->downloading_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->downloading_pushButton->isChecked())
                ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->downloading_number_label->setStyleSheet("");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ListenMyDownload::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        const auto labelRect1 = ui->downloaded_number_label     ->geometry();
        const auto labelRect2 = ui->downloading_number_label       ->geometry();
        // 将点击坐标转换为标签父控件的坐标系
        const QPoint clickPos1 = ui->downloaded_number_label    ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos2 = ui->downloading_number_label      ->parentWidget()->mapFrom(this, event->pos());

        if (labelRect1.contains(clickPos1)) {
            ui->downloaded_pushButton->clicked();
        }
        if (labelRect2.contains(clickPos2)) {
            ui->downloading_pushButton->clicked();
        }
    }
}

void ListenMyDownload::on_downloaded_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_downloaded.get())return;
    ui->downloaded_pushButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(this->m_downloaded.get());
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->downloading_number_label->setStyleSheet("");
}

void ListenMyDownload::on_downloading_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_downloading.get())return;
    ui->downloading_pushButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(this->m_downloading.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->downloaded_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
}
