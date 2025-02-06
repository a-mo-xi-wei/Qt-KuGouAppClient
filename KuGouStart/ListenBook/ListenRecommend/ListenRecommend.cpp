//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenRecommend.h" resolved

#include "ListenRecommend.h"
#include "ui_ListenRecommend.h"
#include "MyMenu.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListenRecommend::ListenRecommend(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenRecommend)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/recommend.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
    auto menu = new MyMenu(MyMenu::MenuKind::ListenOption,this);
    m_menu = menu->getMenu<ListenOptionMenu>();

}

ListenRecommend::~ListenRecommend() {
    delete ui;
}

void ListenRecommend::initUi() {
    //设置全部分类按钮
    ui->all_classify_toolButton->setHoverFontColor(QColor(QStringLiteral("#26A1FF")));
    ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
    ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
    ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
    ui->all_classify_toolButton->setIconSize(QSize(10, 10));
    ui->all_classify_toolButton->setApproach(true);

    //设置换一批按钮
    ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
    ui->refresh_toolButton->setText(" 换一批");
    ui->refresh_toolButton->installEventFilter(this);

    //设置每日推荐画廊
    QList<GalleryPhotoData> list;
    for (int i = 0; i < 10; i++)
    {
        const QPixmap pixmap(":/Res/window/portrait.jpg");
        const QString title = "凤翎谱" + QString::number(i);
        const QString subTitle = "作者：北宫懒懒" + QString::number(i);
        GalleryPhotoData data{pixmap, title, subTitle};
        list.append(data);
    }
    ui->daily_recommend_gallery_widget->loadData(list);

}

void ListenRecommend::on_all_classify_toolButton_clicked() {
    //qDebug()<<"点击了全部分类按钮";
    if (ui->all_classify_toolButton->isChecked()){
        // 更新按钮图标为向上图标
        ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
        ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));

        // 设置菜单的位置
        const QPoint globalPos = ui->all_classify_toolButton->mapToGlobal(
            QPoint(ui->all_classify_toolButton->width() - m_menu->width(),
                   ui->all_classify_toolButton->height() + 10));

        // 设置菜单属性，不夺取焦点
        //m_menu->setWindowFlags(static_cast<Qt::WindowFlags>(m_menu->windowFlags() | Qt::NoFocus));
        // 菜单显示为非阻塞模式
        //m_menu->setWindowFlags(Qt::Popup );
        m_menu->setFocusPolicy(Qt::NoFocus);
        m_menu->setAttribute(Qt::WA_TransparentForMouseEvents, false);

        // 连接菜单的隐藏信号到按钮状态更新槽
        connect(m_menu, &QMenu::aboutToHide, this, [this]() {
            ui->all_classify_toolButton->setChecked(false);
            ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
            ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
            ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        });

        // 显示菜单
        m_menu->exec(globalPos);
    }
    else {
        // 更新按钮图标为向下图标
        ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    }
}

bool ListenRecommend::eventFilter(QObject *watched, QEvent *event) {
    // 检查是否是 refresh_toolButton 的事件
    if (watched == ui->refresh_toolButton) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入 refresh_toolButton
            //qDebug() << "Mouse entered refresh_toolButton";
            ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-blue.svg")));
            return true; // 如果希望阻止默认行为，可以返回 true
        }
        if (event->type() == QEvent::Leave) {
            // 鼠标离开 refresh_toolButton
            //qDebug() << "Mouse left refresh_toolButton";
            ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}
