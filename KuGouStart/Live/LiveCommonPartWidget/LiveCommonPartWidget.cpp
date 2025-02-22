//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveCommonPartWidget.h" resolved

#include "LiveCommonPartWidget.h"
#include "ui_LiveCommonPartWidget.h"
#include "LiveBlockWidget/LiveBlockWidget.h"
#include "Async.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

LiveCommonPartWidget::LiveCommonPartWidget(QWidget *parent, const int lines)
    : QWidget(parent)
    , ui(new Ui::LiveCommonPartWidget)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/common.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi(lines);
}

LiveCommonPartWidget::~LiveCommonPartWidget() {
    delete ui;
}

void LiveCommonPartWidget::setTitleName(const QString &name) {
    ui->title_label->setText(name);
}

void LiveCommonPartWidget::initUi(const int& lines) {
    //初始化右上角两个按钮图标
    const auto leftLabImgPath = GET_CURRENT_DIR + QStringLiteral("/liveRes/left.svg");
    ui->left_label->setStyleSheet(QString("border-image:url('%1');").arg(leftLabImgPath));

    const auto rightLabImgPath = GET_CURRENT_DIR + QStringLiteral("/liveRes/right.svg");
    ui->right_label->setStyleSheet(QString("border-image:url('%1');").arg(rightLabImgPath));
    //qDebug()<<"ui->left_label样式："<<ui->left_label->styleSheet()
    //<<"ui->right_label样式："<<ui->right_label->styleSheet();

    //初始化block左下角文字vec
    // 异步解析 JSON 文件
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/liveRes/text.json");
    const auto future = Async::runAsync(QThreadPool::globalInstance(), &LiveCommonPartWidget::parseJsonFile,
        this,jsonPath);
    // 结果处理回调
    Async::onResultReady(future, this, [this,lines](const QList<QString> &texts) {
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
        //qDebug()<<"当前m_leftBottomTextVec大小："<<this->m_leftBottomTextVec.size();
        this->initLineOne();
        if (lines == 2)initLineTwo();
    });
}

QList<QString> LiveCommonPartWidget::parseJsonFile(const QString &filePath) {
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

void LiveCommonPartWidget::initLineOne() {
    //初始化line_widget_1
    const auto lay1 = new QHBoxLayout(ui->line_widget_1);
    //lay1->setSpacing(15);
    lay1->setContentsMargins(0,0,0,0);
    for (int i = 0; i < 6; ++i) {
        const auto w = new LiveBlockWidget(ui->line_widget_1);
        w->setLeftBottomText(this->m_leftBottomTextVec[i]);
        lay1->addWidget(w);
        w->show();
        if (i == 5)w->hide();
        this->m_blockArr[i] = w;
    }
    ui->line_widget_1->setLayout(lay1);
}

void LiveCommonPartWidget::initLineTwo() {
    //初始化line_widget_2
    const auto lay2 = new QHBoxLayout(ui->line_widget_2);
    //lay2->setSpacing(15);
    lay2->setContentsMargins(0,0,0,0);
    for (int i = 6; i < 12; ++i) {
        const auto w = new LiveBlockWidget(ui->line_widget_2);
        w->setLeftBottomText(this->m_leftBottomTextVec[i + 20]);
        lay2->addWidget(w);
        w->show();
        if (i == 11)w->hide();
        this->m_blockArr[i] = w;
    }
    ui->line_widget_2->setLayout(lay2);
}

void LiveCommonPartWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"当前宽度为："<<this->width();
    if (this->width() < 1180) {
        if (this->m_blockArr[5]) {
            this->m_blockArr[5]->hide();
        }
        if (this->m_blockArr[11]) {
            this->m_blockArr[11]->hide();
        }
    }
    else {
        if (this->m_blockArr[5]) {
            this->m_blockArr[5]->show();
        }
        if (this->m_blockArr[11]) {
            this->m_blockArr[11]->show();
        }
    }
}
