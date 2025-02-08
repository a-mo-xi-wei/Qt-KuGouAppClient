//
// Created by WeiWang on 25-2-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenTableWidget.h" resolved

#include "ListenTableWidget.h"
#include "ui_ListenTableWidget.h"


ListenTableWidget::ListenTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenTableWidget)
{
    ui->setupUi(this);
    initUi();
}

ListenTableWidget::~ListenTableWidget() {
    delete ui;
}

void ListenTableWidget::setTitle(const QString &title) const {
    ui->title_label->setText(title);
}

GalleryWidget * ListenTableWidget::getGalleryWidget() const {
    return ui->gallery_widget;
}

void ListenTableWidget::setCnt(const int &cnt) {
    this->m_cnt = cnt;
}

int ListenTableWidget::getCnt() const {
    return this->m_cnt;
}

void ListenTableWidget::initUi() const {
    ui->toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
    ui->toolButton->setStyleSheet(R"(
        QToolButton#toolButton{
            color: black;
        }
        QToolButton#toolButton:hover{
            color: #26A1FF;
        }
    )");

}

bool ListenTableWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->toolButton) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入 toolButton
            //qDebug() << "Mouse entered toolButton";
            ui->toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-blue.svg")));
            return true; // 如果希望阻止默认行为，可以返回 true
        }
        if (event->type() == QEvent::Leave) {
            // 鼠标离开 toolButton
            //qDebug() << "Mouse left toolButton";
            ui->toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ListenTableWidget::on_toolButton_clicked() {
    emit toolBtnClicked(this->m_cnt);
}
