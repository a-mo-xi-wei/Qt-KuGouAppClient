//
// Created by WeiWang on 25-4-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AiChat.h" resolved

#include "AiChat.h"
#include "ui_AiChat.h"
#include "TextBubble.h"
#include "ChatItemBase.h"

#include <QMouseEvent>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QMovie>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

AiChat::AiChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AiChat)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + "/chat.css");
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    auto font = QFont("AaSongLiuKaiTi");
    font.setPointSize(12);
    // 连接信号
    connect(&m_deepSeek, &Chat::answered, this, &AiChat::getAnswer);
    connect(&m_deepSeek, &Chat::streamFinished, this, &AiChat::onStreamFinished);
    connect(&m_deepSeek, &Chat::errorOccurred, this, [this](const QString& err){
        dealMessageTime();
        // 创建回答气泡
        m_currentResponseItem = new ChatItemBase(ChatRole::Other);
        m_currentResponseItem->setUserName("DeepSeek");
        m_currentResponseItem->setUserIcon(getRoundedPixmap(
            QPixmap(":/Res/window/deepseek.png").scaled(50,40), {50,40}, 20));

        m_currentResponseBubble = new TextBubble(ChatRole::Other, err);

        m_currentResponseItem->setWidget(m_currentResponseBubble);
        ui->chatView->appendChatItem(m_currentResponseItem);
        onStreamFinished();
    });
}

AiChat::~AiChat()
{
    delete ui;
}

QPixmap AiChat::getRoundedPixmap(const QPixmap &src, const QSize &size, const int &radius) {
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);

    return dest;
}

void AiChat::dealMessageTime() {
    auto itemTime = new ChatItemBase(ChatRole::Time);
    auto messageTime = new TextBubble(ChatRole::Time, "", ui->chatView);
    messageTime->resize(this->width(), 40);
    itemTime->setWidget(messageTime);
    ui->chatView->appendChatItem(itemTime);
}

void AiChat::on_send_btn_clicked() {
    const QString question = ui->question_textEdit->toPlainText().trimmed();
    if(question.isEmpty()) {
        qWarning() << "Empty question";
        return;
    }
    ui->send_btn->setEnabled(false);
    ui->send_btn->setCursor(Qt::ForbiddenCursor);
    //处理时间
    dealMessageTime();
    // 自己
    auto pChatItem = new ChatItemBase(ChatRole::Self);
    pChatItem->setUserName("我");
    pChatItem->setUserIcon(getRoundedPixmap(QPixmap(":/Res/window/portrait.jpg"),{50,50},25));
    auto pBubble = new TextBubble(ChatRole::Self, question);
    pChatItem->setWidget(pBubble);
    ui->chatView->appendChatItem(pChatItem);
    // DeepSeek
    // 创建回答气泡
    m_currentResponseItem = new ChatItemBase(ChatRole::Other);
    m_currentResponseItem->setUserName("DeepSeek");
    m_currentResponseItem->setUserIcon(getRoundedPixmap(
        QPixmap(":/Res/window/deepseek.png").scaled(46,46), {46,46}, 23));
    m_currentResponseItem->startMovie(true);

    m_currentResponseBubble = new TextBubble(ChatRole::Other, "");
    m_currentResponseBubble->startStreaming(); // 启动流式

    m_currentResponseItem->setWidget(m_currentResponseBubble);
    ui->chatView->appendChatItem(m_currentResponseItem);

    // 发送请求
    m_deepSeek.send(question);
    ui->question_textEdit->clear();
}

void AiChat::getAnswer(const QString& chunk) {
    if (m_currentResponseBubble) {
        m_currentResponseBubble->appendStreamingContent(chunk); // 直接传递给气泡
    }
}

void AiChat::onStreamFinished() {
    if (m_currentResponseBubble) {
        m_currentResponseBubble->finishStreaming(); // 结束流式
        m_currentResponseItem->startMovie(false);
    }
    ui->send_btn->setEnabled(true);
    ui->send_btn->setCursor(Qt::PointingHandCursor);
}