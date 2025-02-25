//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveBigLeftWidget.h" resolved

#include "LiveBigLeftWidget.h"
#include "ui_LiveBigLeftWidget.h"
#include "Async.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>
#include <QRandomGenerator>


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

    // 异步解析 JSON 文件
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/../text.json");
    const auto future = Async::runAsync(QThreadPool::globalInstance(), &LiveBigLeftWidget::parseJsonFile,
        this,jsonPath);
    // 结果处理回调
    Async::onResultReady(future, this, [this](const QList<QString> &texts) {
        if (texts.isEmpty()) {
            qWarning() << "No valid data parsed from JSON";
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
        return texts;
    }
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
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
    ui->left_widget->setLeftBottomText(this->m_leftBottomTextVec[5]);
    ui->widget_1->setLeftBottomText(this->m_leftBottomTextVec[10]);
    ui->widget_2->setLeftBottomText(this->m_leftBottomTextVec[15]);
    ui->widget_3->setLeftBottomText(this->m_leftBottomTextVec[20]);
    ui->widget_4->setLeftBottomText(this->m_leftBottomTextVec[25]);
    ui->widget_5->setLeftBottomText(this->m_leftBottomTextVec[30]);
    ui->widget_6->setLeftBottomText(this->m_leftBottomTextVec[35]);
    ui->widget_7->setLeftBottomText(this->m_leftBottomTextVec[40]);
    ui->widget_8->setLeftBottomText(this->m_leftBottomTextVec[45]);

}

void LiveBigLeftWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    static QHBoxLayout* lay = static_cast<QHBoxLayout*>(ui->content_widget->layout());
    if (this->width() > 1200) {
        ui->widget_4->show();
        ui->widget_8->show();
        lay->setStretch(1,2);
    }
    else {
        ui->widget_4->hide();
        ui->widget_8->hide();
        lay->setStretch(2,3);
    }
}
