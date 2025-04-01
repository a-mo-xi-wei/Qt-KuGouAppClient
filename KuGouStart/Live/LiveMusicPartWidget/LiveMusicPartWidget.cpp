//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveMusicPartWidget.h" resolved

#include "LiveMusicPartWidget.h"
#include "ui_LiveMusicPartWidget.h"
#include "Async.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>
#include <QRandomGenerator>



#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

LiveMusicPartWidget::LiveMusicPartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveMusicPartWidget)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/music.css"));
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

LiveMusicPartWidget::~LiveMusicPartWidget() {
    delete ui;
}

void LiveMusicPartWidget::setTitleName(const QString &name) const {
    ui->title_label->setText(name);
}

void LiveMusicPartWidget::initUi() {
    //初始化右上角两个按钮图标
    const auto leftLabImgPath = ":/Live/Res/live/left.svg";
    ui->left_label->setStyleSheet(QString("border-image:url('%1');").arg(leftLabImgPath));

    const auto rightLabImgPath = ":/Live/Res/live/right.svg";
    ui->right_label->setStyleSheet(QString("border-image:url('%1');").arg(rightLabImgPath));

    ui->left_label->installEventFilter(this);
    ui->right_label->installEventFilter(this);

    // 异步解析 JSON 文件
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/../text.json");
    const auto future = Async::runAsync(QThreadPool::globalInstance(), &LiveMusicPartWidget::parseJsonFile,
        this,jsonPath);
    // 结果处理回调
    Async::onResultReady(future, this, [this](const QList<QString> &texts) {
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
    ui->desc_label_1->setToolTip(QStringLiteral("TOP500热歌"));
    ui->desc_label_2->setToolTip(QStringLiteral("TOP500热歌"));
    ui->desc_label_3->setToolTip(QStringLiteral("TOP500热歌"));
    ui->desc_label_4->setToolTip(QStringLiteral("TOP500热歌"));

    ui->title_label_1->setToolTip(ui->title_label_1->text());
    ui->title_label_2->setToolTip(ui->title_label_2->text());
    ui->title_label_3->setToolTip(ui->title_label_3->text());
    ui->title_label_4->setToolTip(ui->title_label_4->text());

    ui->desc_label_1->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->desc_label_2->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->desc_label_3->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->desc_label_4->setTextInteractionFlags(Qt::TextSelectableByMouse);

    ui->title_label_1->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->title_label_2->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->title_label_3->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->title_label_4->setTextInteractionFlags(Qt::TextSelectableByMouse);

    ui->widget_4->hide();
}

QList<QString> LiveMusicPartWidget::parseJsonFile(const QString &filePath) {
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

void LiveMusicPartWidget::initBlockWidget() {
    ui->cover_label_1->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')").
        arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/blockcover")))));
    ui->left_block_widget_1->setLeftBottomText(this->m_leftBottomTextVec[10]);
    ui->left_block_widget_1->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->left_block_widget_1->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));
    ui->right_block_widget_1->setLeftBottomText(this->m_leftBottomTextVec[11]);
    ui->right_block_widget_1->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->right_block_widget_1->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));

    ui->cover_label_2->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')").
        arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/blockcover")))));
    ui->left_block_widget_2->setLeftBottomText(this->m_leftBottomTextVec[20]);
    ui->left_block_widget_2->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->left_block_widget_2->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));
    ui->right_block_widget_2->setLeftBottomText(this->m_leftBottomTextVec[21]);
    ui->right_block_widget_2->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->right_block_widget_2->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));

    ui->cover_label_3->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')").
        arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/blockcover")))));
    ui->left_block_widget_3->setLeftBottomText(this->m_leftBottomTextVec[30]);
    ui->left_block_widget_3->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->left_block_widget_3->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));
    ui->right_block_widget_3->setLeftBottomText(this->m_leftBottomTextVec[31]);
    ui->right_block_widget_3->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->right_block_widget_3->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));

    ui->cover_label_4->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')").
        arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/blockcover")))));
    ui->left_block_widget_4->setLeftBottomText(this->m_leftBottomTextVec[40]);
    ui->left_block_widget_4->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->left_block_widget_4->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));
    ui->right_block_widget_4->setLeftBottomText(this->m_leftBottomTextVec[41]);
    ui->right_block_widget_4->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000)));
    ui->right_block_widget_4->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg").
            arg(QString::number(QRandomGenerator::global()->bounded(1,getFileCount(GET_CURRENT_DIR + "/../../Res&Qrc/Res/standcover")))));

}

int LiveMusicPartWidget::getFileCount(const QString &folderPath) {
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

void LiveMusicPartWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"this->width(): "<<this->width();
    if (this->width() > 1200) {
        ui->widget_4->show();
    }
    else {
        ui->widget_4->hide();
    }
}
bool LiveMusicPartWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->left_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                        QString("暂无更多 %1").arg(ui->title_label->text()),2000,this->window());
        }
    }
    if (watched == ui->right_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                        QString("暂无更多 %1").arg(ui->title_label->text()),2000,this->window());
        }
    }
    return QWidget::eventFilter(watched, event);
}