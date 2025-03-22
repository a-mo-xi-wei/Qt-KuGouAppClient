//
// Created by WeiWang on 25-3-21.
//

#include "AboutDialog.h"
#include "qtmaterialdialog.h"

#include <QPushButton>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent)
    : QWidget(parent)
    , m_dialog(new QtMaterialDialog)
{
    m_dialog->setParent(parent);
    auto dialogLayout = new QVBoxLayout;
    m_dialog->setWindowLayout(dialogLayout);

    auto dialogWidget = new QWidget;
    dialogWidget->setMinimumHeight(300);

    dialogLayout->addWidget(dialogWidget);

    auto dialogWidgetLayout = new QVBoxLayout(dialogWidget);
    initDialog(dialogWidgetLayout);
}

void AboutDialog::initDialog(QVBoxLayout *lay) {
    auto closeButton = new QPushButton("Close");
    lay->addWidget(closeButton);
    lay->setAlignment(closeButton, Qt::AlignBottom | Qt::AlignCenter);

    closeButton->setMaximumWidth(150);

    connect(closeButton, &QPushButton::pressed, m_dialog, &QtMaterialDialog::hideDialog);
}

void AboutDialog::onShowDialog() {
    m_dialog->showDialog();
}

void AboutDialog::onHideDialog() {
    m_dialog->hideDialog();
}
