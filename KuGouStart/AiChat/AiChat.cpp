/**
 * @file AiChat.cpp
 * @brief 实现 AiChat 类，提供 AI 聊天界面功能
 * @author WeiWang
 * @date 2025-04-14
 * @version 1.0
 */

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

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化 AI 聊天界面
 * @param parent 父控件指针，默认为 nullptr
 */
AiChat::AiChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AiChat)
    , m_sendBtn(new QtMaterialFloatingActionButton(QIcon(":/Res/window/send.svg")))
    , m_snackbar(std::make_unique<QtMaterialSnackbar>())
{
    ui->setupUi(this);
    this->setObjectName("AiChat");                       ///< 设置对象名称
    QFile file(GET_CURRENT_DIR + "/chat.css");           ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        return;
    }

    initUi();                                            ///< 初始化界面

    connect(&m_deepSeek, &Chat::answered, this, &AiChat::getAnswer); ///< 连接回答信号
    connect(&m_deepSeek, &Chat::streamFinished, this, &AiChat::onStreamFinished); ///< 连接流式结束信号
    connect(&m_deepSeek, &Chat::errorOccurred, this, [this](const QString &err) {
        ui->chatView->removeLastItem();                  ///< 删除上一个回答
        m_currentResponseItem = new ChatItemBase(ChatRole::Other); ///< 创建新回答项
        m_currentResponseItem->setUserName("DeepSeek");  ///< 设置用户名
        m_currentResponseItem->setUserIcon(getRoundedPixmap(
            QPixmap(":/Res/window/deepseek.png").scaled(46, 46), {46, 46}, 23)); ///< 设置头像
        m_currentResponseBubble = new TextBubble(ChatRole::Other, err); ///< 创建错误气泡
        m_currentResponseItem->setWidget(m_currentResponseBubble); ///< 设置气泡
        ui->chatView->appendChatItem(m_currentResponseItem); ///< 追加聊天项
        onStreamFinished();                              ///< 处理流式结束
    });

    connect(ui->clear_toolButton, &QToolButton::clicked, ui->chatView, [this] {
        if (this->m_sendBtn->isEnabled())
            ui->chatView->removeAllItem();               ///< 清除历史对话
        else
        {
            ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                                   "请等待当前问题回答完毕", 1000, this->window()); ///< 显示警告
        }
    });
}

/**
 * @brief 析构函数，清理资源
 */
AiChat::~AiChat()
{
    delete ui;
}

/**
 * @brief 初始化界面
 */
void AiChat::initUi()
{
    ui->clear_toolButton->setCursor(Qt::PointingHandCursor); ///< 设置清除按钮光标
    ui->clear_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置按钮样式
    ui->clear_toolButton->setIcon(QIcon(":/Res/window/clear-black.svg")); ///< 设置清除图标
    ui->clear_toolButton->setText("清除历史对话");          ///< 设置清除文本
    auto font = QFont("AaSongLiuKaiTi");                    ///< 设置字体
    font.setPointSize(14);                                  ///< 设置字号
    font.setWeight(QFont::Medium);                          ///< 设置字重
    ui->question_textEdit->setFont(font);                   ///< 设置问题输入框字体
    ui->question_textEdit->setCursor(Qt::IBeamCursor);      ///< 设置输入框光标
    ui->question_textEdit->setPlaceholderText("请输入问题");  ///< 设置占位文本
    ui->question_textEdit->installEventFilter(this);        ///< 安装事件过滤器
    this->m_sendBtn->setParent(ui->button_widget);          ///< 设置发送按钮父对象
    this->m_sendBtn->setCursor(Qt::PointingHandCursor);     ///< 设置发送按钮光标
    this->m_sendBtn->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    this->m_sendBtn->setCorner(Qt::BottomRightCorner);      ///< 设置按钮位置
    this->m_sendBtn->setXOffset(15);                        ///< 设置 X 偏移
    this->m_sendBtn->setYOffset(15);                        ///< 设置 Y 偏移
    m_snackbar->setParent(this);                            ///< 设置消息条父对象
    m_snackbar->setAutoHideDuration(1500);                  ///< 设置消息条显示时长
    m_snackbar->setBackgroundColor(QColor(132, 202, 192, 200)); ///< 设置消息条背景色
    m_snackbar->setStyleSheet("border-radius: 10px;");      ///< 设置消息条样式
    connect(this->m_sendBtn, &QPushButton::clicked, this, &AiChat::onSendBtnClicked); ///< 连接发送按钮信号
}

/**
 * @brief 生成圆角图片
 * @param src 源图片
 * @param size 目标尺寸
 * @param radius 圆角半径
 * @return 圆角图片
 */
QPixmap AiChat::getRoundedPixmap(const QPixmap &src, const QSize &size, const int &radius)
{
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); ///< 缩放图片
    QPixmap dest(size);
    dest.fill(Qt::transparent);                          ///< 填充透明背景
    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);       ///< 启用抗锯齿
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius); ///< 创建圆角路径
    painter.setClipPath(path);                           ///< 设置裁剪路径
    painter.drawPixmap(0, 0, scaled);                    ///< 绘制图片
    return dest;
}

/**
 * @brief 处理消息时间气泡
 */
void AiChat::dealMessageTime()
{
    auto itemTime = new ChatItemBase(ChatRole::Time);    ///< 创建时间聊天项
    auto messageTime = new TextBubble(ChatRole::Time, "", itemTime); ///< 创建时间气泡
    messageTime->resize(this->width(), 40);              ///< 设置气泡尺寸
    itemTime->setWidget(messageTime);                    ///< 设置气泡
    ui->chatView->appendChatItem(itemTime);              ///< 追加时间项
}

/**
 * @brief 处理发送按钮点击
 */
void AiChat::onSendBtnClicked()
{
    const QString question = ui->question_textEdit->toPlainText().trimmed(); ///< 获取问题
    if (question.isEmpty())
    {
        qWarning() << "Empty question";
        STREAM_WARN() << "Empty question";                ///< 记录警告日志
        if (m_snackbarTimer.isValid() && m_snackbarTimer.elapsed() < m_snackbar->autoHideDuration())
        {
            qDebug() << "Snackbar cooling down";
            return;                                      ///< 检查冷却时间
        }
        m_snackbarTimer.start();                         ///< 重置计时器
        m_snackbar->addInstantMessage("你干嘛，哎哟 ~");  ///< 显示消息
        m_snackbar->show();
        return;
    }
    this->m_sendBtn->setEnabled(false);                  ///< 禁用发送按钮
    this->m_sendBtn->setCursor(Qt::WaitCursor);          ///< 设置等待光标
    dealMessageTime();                                   ///< 添加时间气泡
    auto pChatItem = new ChatItemBase(ChatRole::Self);   ///< 创建自己聊天项
    pChatItem->setUserName("我");                        ///< 设置用户名
    pChatItem->setUserIcon(getRoundedPixmap(QPixmap(":/Res/window/portrait.jpg"), {50, 50}, 25)); ///< 设置头像
    auto pBubble = new TextBubble(ChatRole::Self, question); ///< 创建问题气泡
    pChatItem->setWidget(pBubble);                       ///< 设置气泡
    ui->chatView->appendChatItem(pChatItem);             ///< 追加聊天项
    m_currentResponseItem = new ChatItemBase(ChatRole::Other); ///< 创建回答项
    m_currentResponseItem->setUserName("DeepSeek");      ///< 设置用户名
    m_currentResponseItem->setUserIcon(getRoundedPixmap(
        QPixmap(":/Res/window/deepseek.png").scaled(46, 46), {46, 46}, 23)); ///< 设置头像
    m_currentResponseItem->startMovie(true);             ///< 启动加载动画
    m_currentResponseBubble = new TextBubble(ChatRole::Other, ""); ///< 创建回答气泡
    m_currentResponseBubble->startStreaming();           ///< 启动流式显示
    m_currentResponseItem->setWidget(m_currentResponseBubble); ///< 设置气泡
    ui->chatView->appendChatItem(m_currentResponseItem); ///< 追加回答项
    m_deepSeek.send(question);                           ///< 发送请求
    ui->question_textEdit->clear();                      ///< 清空输入框
}

/**
 * @brief 接收回答内容
 * @param chunk 回答片段
 */
void AiChat::getAnswer(const QString &chunk)
{
    if (m_currentResponseBubble)
    {
        m_currentResponseBubble->appendStreamingContent(chunk); ///< 追加流式内容
    }
}

/**
 * @brief 处理流式响应结束
 */
void AiChat::onStreamFinished()
{
    if (m_currentResponseBubble)
    {
        m_currentResponseBubble->finishStreaming();      ///< 结束流式显示
        m_currentResponseItem->startMovie(false);        ///< 停止加载动画
    }
    this->m_sendBtn->setEnabled(true);                   ///< 启用发送按钮
    this->m_sendBtn->setCursor(Qt::PointingHandCursor);  ///< 恢复光标
}

/**
 * @brief 事件过滤器
 * @param watched 目标对象
 * @param event 事件对象
 * @return 是否处理事件
 */
bool AiChat::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->question_textEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                if (keyEvent->modifiers() & Qt::ShiftModifier)
                {
                    ui->question_textEdit->insertPlainText("\n"); ///< 插入换行
                }
                else
                {
                    this->m_sendBtn->click();               ///< 触发发送
                }
                return true;
            }
        }
        if (event->type() == QEvent::FocusIn)
        {
            m_snackbar->hide();                         ///< 隐藏消息条
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用基类方法
}