//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DailyRecommend.h" resolved

#include "DailyRecommend.h"
#include "ui_DailyRecommend.h"
#include "IconBesideTextToolButton.h"

#include <QFile>
#include <QDateTime>


#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

DailyRecommend::DailyRecommend(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DailyRecommend)
    , m_monthLab(new QLabel(this))
    , m_dayLab(new QLabel(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/daily.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    //初始化日期标签
    initUi();

}

DailyRecommend::~DailyRecommend() {
    delete ui;
}

void DailyRecommend::initUi() {
    //初始化日期标签
    initDateLab();
    //初始化历史推荐按钮
    ui->history_recommend_toolButton->setIconSize(QSize(10,10));
    ui->history_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    ui->history_recommend_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
    ui->history_recommend_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    ui->history_recommend_toolButton->setApproach(true);
    ui->history_recommend_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF")));
    //初始化播放按钮
    ui->play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->play_toolButton->setText(QStringLiteral("播放"));
    //初始化vip按钮
    ui->vip_toolButton->setToolTip(QStringLiteral("威哥出品，不存在VIP"));
    ui->vip_toolButton->setIconSize(QSize(18,18));
    ui->vip_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/yellow-diamond.svg")));
    ui->vip_toolButton->setText(QStringLiteral("+30"));
    ui->vip_toolButton->setApproach(true);
    //初始化剩余按钮
    ui->collect_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/like-gray.svg")));
    ui->download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
    ui->batch_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/batch-gray.svg")));
    //初始化数量按钮
    ui->count_label->setText(QStringLiteral("30"));
    //初始化ico按钮
    ui->ico_label->setPixmap(QPixmap(QStringLiteral(":/Res/tabIcon/yellow-diamond.svg")).scaled(18,18));
    //初始化tableWidget
    initTableWidget();
}

void DailyRecommend::initDateLab() {
    this->m_monthLab->setObjectName("monthLab");
    this->m_dayLab->setObjectName("dayLab");
    // 获取当前日期
    const QDate currentDate = QDate::currentDate();

    // 设置月份文本（数字形式）
    const QString monthStr = QString::number(currentDate.month()) + "月";
    this->m_monthLab->setText(monthStr);

    // 设置日期文本（数字形式）
    const QString dayStr = QString::number(currentDate.day());
    this->m_dayLab->setFixedHeight(40);
    this->m_dayLab->setText(dayStr);

    // 设置适应内容
    this->m_monthLab->setScaledContents(true);
    this->m_dayLab->setScaledContents(true);

    // 移动位置
    // 获取 top_cover_label 在父组件中的位置
    const QPoint targetPos = ui->top_cover_label->pos();

    this->m_monthLab->move(targetPos.x() + 30, targetPos.y() + 30);

    this->m_dayLab->move(targetPos.x() + 30, targetPos.y() + 35 + this->m_monthLab->height());

    this->m_monthLab->raise();
    this->m_dayLab->raise();
}

void DailyRecommend::initTableWidget() {
    for (int i = 0; i < 30; i++) {SongInfor tempInformation;
        tempInformation.index = i;
        tempInformation.cover = QPixmap(QString(":/Res/tablisticon/pix%1.png").arg(i%10+1));
        tempInformation.songName = "未知歌曲";
        tempInformation.singer = "未知歌手";
        tempInformation.duration = "未知时长";
        tempInformation.mediaPath = "未知路径";
        tempInformation.addTime = QDateTime::currentDateTime();
        tempInformation.playCount = 0;

        //加载相关信息
        auto item = new MusicItemWidget(tempInformation, this);
        //初始化item
        initMusicItem(item);
        const auto layout = dynamic_cast<QVBoxLayout *>(ui->daily_song_list_widget->layout());
        if (!layout)return;
        layout->insertWidget(layout->count(), item);
    }
}

void DailyRecommend::initMusicItem(MusicItemWidget *item) {
    item->setFillColor(QColor(QStringLiteral("#B0EDF6")));
    item->setRadius(12);
    item->setInterval(1);
}
