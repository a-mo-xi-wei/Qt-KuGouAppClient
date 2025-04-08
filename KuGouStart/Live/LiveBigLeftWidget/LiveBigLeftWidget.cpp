//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveBigLeftWidget.h" resolved

#include "LiveBigLeftWidget.h"
#include "ui_LiveBigLeftWidget.h"
#include "Async.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <random>
#include <QDir>


#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

LiveBigLeftWidget::LiveBigLeftWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveBigLeftWidget)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/bigleft.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

LiveBigLeftWidget::~LiveBigLeftWidget() {
    delete ui;
}

void LiveBigLeftWidget::setTitleName(const QString &name) const {
    ui->title_label->setText(name);
}

void LiveBigLeftWidget::setNoTipLab() const {
    ui->left_widget->setNoTipLab();
    ui->widget_1->setNoTipLab();
    ui->widget_2->setNoTipLab();
    ui->widget_3->setNoTipLab();
    ui->widget_4->setNoTipLab();
    ui->widget_5->setNoTipLab();
    ui->widget_6->setNoTipLab();
    ui->widget_7->setNoTipLab();
    ui->widget_8->setNoTipLab();
}

void LiveBigLeftWidget::initUi() {
    //初始化右上角两个按钮图标
    const auto leftLabImgPath = ":/Live/Res/live/left.svg";
    ui->left_label->setStyleSheet(QString("border-image:url('%1');").arg(leftLabImgPath));

    const auto rightLabImgPath = ":/Live/Res/live/right.svg";
    ui->right_label->setStyleSheet(QString("border-image:url('%1');").arg(rightLabImgPath));

    ui->left_label->installEventFilter(this);
    ui->right_label->installEventFilter(this);
    // 异步解析 JSON 文件
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/../text.json");
    const auto future = Async::runAsync(QThreadPool::globalInstance(), &LiveBigLeftWidget::parseJsonFile,
        this,jsonPath);
    // 结果处理回调
    Async::onResultReady(future, this, [=](const QList<QString> &texts) {
        if (texts.isEmpty()) {
            qWarning() << "No valid data parsed from JSON";
            STREAM_WARN() << "No valid data parsed from JSON";
            return;
        }
        // 更新成员变量
        this->m_leftBottomTextVec = texts;

        // 打乱数据顺序
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(
            this->m_leftBottomTextVec.begin(),
            this->m_leftBottomTextVec.end(),
            std::default_random_engine(seed)
        );
        initBlockWidget();
    });

    ui->widget_4->hide();
    ui->widget_8->hide();
}

QList<QString> LiveBigLeftWidget::parseJsonFile(const QString &filePath) {
    QList<QString> texts;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open JSON file:" << filePath;
        STREAM_WARN() << "Failed to open JSON file:" << filePath.toStdString();
        return texts;
    }
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        STREAM_WARN() << "JSON parse error:" << parseError.errorString().toStdString();
        return texts;
    }
    QJsonArray arr = doc.array();
    for (const auto &item : arr) {
        QString text = item.toObject().value("text").toString();
        texts.append(text);
    }
    file.close();
    return texts;
}

void LiveBigLeftWidget::initBlockWidget() {
    ui->left_widget->setCoverHeightExpanding();
    ui->left_widget->setAspectRatio(ui->left_widget->width() * 1.0 / ui->left_widget->height());
    //qDebug()<<"left_widget->width() : "<<ui->left_widget->width()<<"  height() : "<<ui->left_widget->height();
    ui->left_widget->setCircleStander(120);
    ui->left_widget->setLeftPopularBtnFontSize(14,true);
    ui->left_widget->setLeftBottomText(this->m_leftBottomTextVec[5]);
    ui->left_widget->setTipStyleSheet(QStringLiteral("font-size: 12px;border-radius:10px;background-color:black;color:white;height: 30px;"));
    ui->left_widget->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));

    ui->widget_1->setCoverHeightExpanding();
    ui->widget_1->setAspectRatio(0.78);
    ui->widget_1->setLeftBottomText(this->m_leftBottomTextVec[10]);
    ui->widget_1->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_2->setCoverHeightExpanding();
    ui->widget_2->setAspectRatio(0.78);
    ui->widget_2->setLeftBottomText(this->m_leftBottomTextVec[15]);
    ui->widget_2->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_3->setCoverHeightExpanding();
    ui->widget_3->setAspectRatio(0.78);
    ui->widget_3->setLeftBottomText(this->m_leftBottomTextVec[20]);
    ui->widget_3->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_4->setCoverHeightExpanding();
    ui->widget_4->setAspectRatio(0.78);
    ui->widget_4->setLeftBottomText(this->m_leftBottomTextVec[25]);
    ui->widget_4->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_5->setCoverHeightExpanding();
    ui->widget_5->setAspectRatio(0.78);
    ui->widget_5->setLeftBottomText(this->m_leftBottomTextVec[30]);
    ui->widget_5->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_6->setCoverHeightExpanding();
    ui->widget_6->setAspectRatio(0.78);
    ui->widget_6->setLeftBottomText(this->m_leftBottomTextVec[35]);
    ui->widget_6->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_7->setCoverHeightExpanding();
    ui->widget_7->setAspectRatio(0.78);
    ui->widget_7->setLeftBottomText(this->m_leftBottomTextVec[40]);
    ui->widget_7->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_8->setCoverHeightExpanding();
    ui->widget_8->setAspectRatio(0.78);
    ui->widget_8->setLeftBottomText(this->m_leftBottomTextVec[45]);
    ui->widget_8->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));

}

int LiveBigLeftWidget::getFileCount(const QString &folderPath) {
    QDir dir(folderPath);

    if (!dir.exists()) {
        qWarning("目录不存在: %s", qPrintable(folderPath));
        PRINT_WARN("目录不存在: %s", folderPath.toStdString());
        return 0;
    }

    const auto filters = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot;

    // 仅统计当前目录下的文件
    const int fileCount = static_cast<int>(dir.entryList(filters, QDir::Name).size());

    return fileCount;
}

void LiveBigLeftWidget::on_all_pushButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                QString("暂无更多 %1").arg(ui->title_label->text()),1000,this->window());
}

void LiveBigLeftWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (this->width() > 1200) {
        ui->widget_4->show();
        ui->widget_8->show();
    }
    else {
        ui->widget_4->hide();
        ui->widget_8->hide();
    }
}

bool LiveBigLeftWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->left_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                        QString("暂无更多 %1").arg(ui->title_label->text()),1000,this->window());
        }
    }
    if (watched == ui->right_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                        QString("暂无更多 %1").arg(ui->title_label->text()),1000,this->window());
        }
    }
    return QWidget::eventFilter(watched, event);
}
