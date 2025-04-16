//
// Created by WeiWang on 25-4-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AiChat.h" resolved

#include "AiChat.h"
#include "ui_AiChat.h"
#include "TextBubble.h"
#include "ChatItemBase.h"
#include "ElaMessageBar.h"
#include "logger.hpp"
#include "qtmaterialfab.h"
#include "qtmaterialsnackbar.h"

#include <QMouseEvent>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QMovie>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

AiChat::AiChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AiChat)
    , m_sendBtn(new QtMaterialFloatingActionButton(QIcon(":/Res/window/send.svg")))
    , m_snackbar(std::make_unique<QtMaterialSnackbar>())
{
    ui->setupUi(this);
    this->setObjectName("AiChat");
    QFile file(GET_CURRENT_DIR + "/chat.css");
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }

    initUi();

    // 连接信号
    connect(&m_deepSeek, &Chat::answered, this, &AiChat::getAnswer);
    connect(&m_deepSeek, &Chat::streamFinished, this, &AiChat::onStreamFinished);
    connect(&m_deepSeek, &Chat::errorOccurred, this, [this](const QString &err) {
        //删除上一个回答气泡
        ui->chatView->removeLastItem();
        // 创建回答气泡
        m_currentResponseItem = new ChatItemBase(ChatRole::Other);
        m_currentResponseItem->setUserName("DeepSeek");
        m_currentResponseItem->setUserIcon(getRoundedPixmap(
            QPixmap(":/Res/window/deepseek.png").scaled(46, 46), {46, 46}, 23));

        m_currentResponseBubble = new TextBubble(ChatRole::Other, err);

        m_currentResponseItem->setWidget(m_currentResponseBubble);
        ui->chatView->appendChatItem(m_currentResponseItem);
        onStreamFinished();
    });

    connect(ui->clear_toolButton, &QToolButton::clicked, ui->chatView, [this] {
        //通过this->m_sendBtn是否能点击来判断是否可以清理
        if (this->m_sendBtn->isEnabled())
            ui->chatView->removeAllItem();
        else {
            ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                                   "请等待当前问题回答完毕",
                                   1000, this->window());
        }
    });
}

AiChat::~AiChat()
{
    delete ui;
}

void AiChat::initUi() {
    //clear_toolButton
    ui->clear_toolButton->setCursor(Qt::PointingHandCursor);
    ui->clear_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->clear_toolButton->setIcon(QIcon(":/Res/window/clear-black.svg"));
    ui->clear_toolButton->setText("清除历史对话");
    //question_textEdit
    auto font = QFont("AaSongLiuKaiTi");
    font.setPointSize(14);
    font.setWeight(QFont::Medium);
    ui->question_textEdit->setFont(font);
    ui->question_textEdit->setCursor(Qt::IBeamCursor);
    ui->question_textEdit->setPlaceholderText("请输入问题");
    ui->question_textEdit->installEventFilter(this);
    // send_btn
    this->m_sendBtn->setParent(ui->button_widget);
    this->m_sendBtn->setCursor(Qt::PointingHandCursor);
    this->m_sendBtn->setRippleStyle(Material::PositionedRipple);
    this->m_sendBtn->setCorner(Qt::BottomRightCorner);
    this->m_sendBtn->setXOffset(15);
    this->m_sendBtn->setYOffset(15);
    //m_snackbar
    m_snackbar->setParent(this);
    m_snackbar->setAutoHideDuration(1500);
    m_snackbar->setBackgroundColor(QColor(132, 202, 192, 200));
    m_snackbar->setStyleSheet("border-radius: 10px;");

    connect(this->m_sendBtn,&QPushButton::clicked,this,&AiChat::onSendBtnClicked);
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
    auto messageTime = new TextBubble(ChatRole::Time, "", itemTime);
    messageTime->resize(this->width(), 40);
    itemTime->setWidget(messageTime);
    ui->chatView->appendChatItem(itemTime);
}

void AiChat::onSendBtnClicked() {
    const QString question = ui->question_textEdit->toPlainText().trimmed();
    if(question.isEmpty()) {
        qWarning() << "Empty question";
        STREAM_WARN() << "Empty question";
        // 检查是否在冷却时间内
        if (m_snackbarTimer.isValid() && m_snackbarTimer.elapsed() < m_snackbar->autoHideDuration()) {
            qDebug() << "Snackbar cooling down";
            return; // 3秒内不再重复显示
        }
        // 重置计时器并显示
        m_snackbarTimer.start();
        m_snackbar->addInstantMessage("你干嘛，哎哟 ~");
        m_snackbar->show();
        return;
    }
    this->m_sendBtn->setEnabled(false);
    this->m_sendBtn->setCursor(Qt::WaitCursor);
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
    this->m_sendBtn->setEnabled(true);
    this->m_sendBtn->setCursor(Qt::PointingHandCursor);
}

bool AiChat::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->question_textEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                if (keyEvent->modifiers() & Qt::ShiftModifier) {
                    ui->question_textEdit->insertPlainText("\n");
                } else {
                    this->m_sendBtn->click(); // 直接点击按钮
                }
                return true;
            }
        }
        if (event->type() == QEvent::FocusIn) {
            m_snackbar->hide();
        }
    }
    return QWidget::eventFilter(watched, event);
}