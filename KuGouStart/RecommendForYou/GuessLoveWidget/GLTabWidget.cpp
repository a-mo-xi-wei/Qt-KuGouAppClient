#include "GLTabWidget.h"
#include "IconBesideTextToolButton.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaMenu.h"

#include <QFile>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

GLTabWidget::GLTabWidget(QWidget *parent)
    : QWidget{parent}
    , m_modelBtn(new IconBesideTextToolButton(this))
    , m_playToolBtn(new QToolButton(this))
    , m_nextToolBtn(new QToolButton(this))
    , m_likeToolBtn(new QToolButton(this))
    , m_dislikeToolBtn(new QToolButton(this))
{
    initUi();
    layoutUi();
}

void GLTabWidget::initUi() {
    this->m_glLab = new QLabel(QStringLiteral("猜你喜欢"), this);
    this->m_songNameLab = new QLabel(QStringLiteral("青花瓷"), this);
    this->m_singerLab = new QLabel(QStringLiteral("周杰伦"), this);

    this->setObjectName("basic_window_widget");
    this->m_glLab->setObjectName("glLab");
    this->m_songNameLab->setObjectName("songNameLab");
    this->m_singerLab->setObjectName("singerLab");
    this->m_modelBtn->setObjectName("modelBtn");
    this->m_playToolBtn->setObjectName("playToolBtn");
    this->m_nextToolBtn->setObjectName("nextToolBtn");
    this->m_likeToolBtn->setObjectName("likeToolBtn");
    this->m_dislikeToolBtn->setObjectName("dislikeToolBtn");

    this->m_modelBtn->setCursor(Qt::CursorShape::PointingHandCursor);
    this->m_playToolBtn->setCursor(Qt::CursorShape::PointingHandCursor);
    this->m_nextToolBtn->setCursor(Qt::CursorShape::PointingHandCursor);
    this->m_likeToolBtn->setCursor(Qt::CursorShape::PointingHandCursor);
    this->m_dislikeToolBtn->setCursor(Qt::CursorShape::PointingHandCursor);

    //设置全部分类按钮
    this->m_modelBtn->setText(QStringLiteral("模式"));
    this->m_modelBtn->setFixedSize(45, 20);
    this->m_modelBtn->setHoverFontColor(Qt::white);
    this->m_modelBtn->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-white.svg")));
    this->m_modelBtn->setIconSize(QSize(10, 10));
    this->m_modelBtn->setApproach(true);
    connect(this->m_modelBtn, &IconBesideTextToolButton::clicked, this,&GLTabWidget::onModelBtnClicked);

    QFile file(GET_CURRENT_DIR + QStringLiteral("/table.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
}

void GLTabWidget::paintEvent(QPaintEvent *ev) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#define AspectRatio 2

void GLTabWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->setFixedHeight(event->size().width() / AspectRatio);
    update();
}

void GLTabWidget::layoutUi() {
    const auto hLayout1 = new QHBoxLayout;
    hLayout1->setSpacing(5);
    hLayout1->addSpacerItem(new QSpacerItem(20, 5, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout1->addWidget(this->m_glLab);
    hLayout1->addWidget(this->m_modelBtn);
    hLayout1->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed));

    const auto hLayout2 = new QHBoxLayout;
    hLayout2->setContentsMargins(0, 0, 0, 0);
    hLayout2->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout2->addWidget(this->m_songNameLab);
    hLayout2->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    const auto hLayout3 = new QHBoxLayout;
    hLayout3->setContentsMargins(0, 0, 0, 0);
    hLayout3->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout3->addWidget(this->m_singerLab);
    hLayout3->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    const auto vlayout1 = new QVBoxLayout;
    vlayout1->setSpacing(5);
    vlayout1->addLayout(hLayout2);
    vlayout1->addLayout(hLayout3);

    const auto vlayout2 = new QVBoxLayout;
    vlayout2->setSpacing(15);
    vlayout2->addLayout(hLayout1);
    vlayout2->addLayout(vlayout1);

    const auto hLayout4 = new QHBoxLayout;
    hLayout4->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout4->addWidget(this->m_playToolBtn);
    hLayout4->addStretch();
    hLayout4->addWidget(this->m_nextToolBtn);
    hLayout4->addStretch();
    hLayout4->addWidget(this->m_likeToolBtn);
    hLayout4->addStretch();
    hLayout4->addWidget(this->m_dislikeToolBtn);
    hLayout4->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Preferred));
    hLayout4->setStretch(0,0);
    hLayout4->setStretch(1,0);
    hLayout4->setStretch(2,1);
    hLayout4->setStretch(3,0);
    hLayout4->setStretch(4,1);
    hLayout4->setStretch(5,0);
    hLayout4->setStretch(6,1);
    hLayout4->setStretch(7,0);
    hLayout4->setStretch(8,14);

    const auto vLayout = new QVBoxLayout(this);
    vLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Fixed));
    vLayout->addLayout(vlayout2);
    vLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Preferred));
    vLayout->addLayout(hLayout4);
    vLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Preferred));
}

void GLTabWidget::onModelBtnClicked() {
    const QPoint globalPos = this->m_modelBtn->mapToGlobal(
        QPoint(this->m_modelBtn->width() - 45,
               this->m_modelBtn->height() - 5));

    ElaMenu* menu = new ElaMenu(this);
    menu->setOpacity(0.85);
    menu->setFixedWidth(60);
    menu->setMenuItemHeight(22);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    auto action1 = new QAction(menu);
    action1->setText("发现");
    connect(action1, &QAction::triggered,this,[this]{onGetModel("发现");});
    auto action2 = new QAction(menu);
    action2->setText("小众");
    connect(action2, &QAction::triggered,this,[this]{onGetModel("小众");});
    auto action3 = new QAction(menu);
    action3->setText("30s");
    connect(action3, &QAction::triggered,this,[this]{onGetModel("30s");});

    menu->addAction(action1);
    menu->addAction(action2);
    menu->addAction(action3);

    menu->popup(globalPos);

}

void GLTabWidget::onGetModel(const QString &model) {
    this->m_modelBtn->setText(model);
}

void GLTabWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void GLTabWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void GLTabWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}
