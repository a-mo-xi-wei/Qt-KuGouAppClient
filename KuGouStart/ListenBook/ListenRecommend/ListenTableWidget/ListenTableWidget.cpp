//
// Created by WeiWang on 25-2-7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenTableWidget.h" resolved

#include "ListenTableWidget.h"
#include "ui_ListenTableWidget.h"


ListenTableWidget::ListenTableWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ListenTableWidget) {
    ui->setupUi(this);
    ui->toolButton->setStyleSheet(R"(
        QToolButton#toolButton{
            color: black;
        }
        QToolButton#toolButton:hover{
            color: #26A1FF;
        }
    )");
}

ListenTableWidget::~ListenTableWidget() {
    delete ui;
}

QToolButton * ListenTableWidget::getToolBtn() const {
    return ui->toolButton;
}

void ListenTableWidget::setTitle(const QString &title) const {
    ui->title_label->setText(title);
}

GalleryWidget * ListenTableWidget::getGalleryWidget() const {
    return ui->gallery_widget;
}

void ListenTableWidget::on_toolButton_clicked() {
    emit toolBtnClicked();
}
