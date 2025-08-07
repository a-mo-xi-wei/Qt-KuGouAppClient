/**
 * @file TextBubble.cpp
 * @brief 实现 TextBubble 类，提供文本气泡功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "TextBubble.h"

#include <QFontMetricsF>
#include <QTimer>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextLayout>

/**
 * @brief 构造函数，初始化文本气泡
 * @param role 聊天角色（自己、他人、时间）
 * @param text 初始文本
 * @param parent 父控件指针，默认为 nullptr
 */
TextBubble::TextBubble(const ChatRole role, const QString &text, QWidget *parent)
    : BubbleFrame(role, parent)
{
    m_pTextEdit = new QTextEdit();                           ///< 创建文本编辑器
    m_pTextEdit->setReadOnly(true);                         ///< 设置只读
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); ///< 隐藏垂直滚动条
    m_pTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); ///< 隐藏水平滚动条
    m_pTextEdit->installEventFilter(this);                  ///< 安装事件过滤器
    m_pTextEdit->setStyleSheet("QTextEdit{background:transparent;border:none}"); ///< 设置透明样式
    auto font = QFont("TaiwanPearl");                    ///< 设置字体
    font.setPointSize(12);                                  ///< 设置字号
    font.setWeight(QFont::Medium);                          ///< 设置字重
    m_pTextEdit->setFont(font);
    setPlainText(text);                                     ///< 设置初始文本
    setWidget(m_pTextEdit);                                 ///< 设置内容控件
}

/**
 * @brief 启动流式显示
 */
void TextBubble::startStreaming()
{
    m_streamBuffer.clear();                                 ///< 清空缓冲区
    m_pTextEdit->clear();                                   ///< 清空文本
    m_streamTimer.setInterval(50);                          ///< 设置定时器间隔
    connect(&m_streamTimer, &QTimer::timeout, this, [this]()
    {
        if (!m_streamBuffer.isEmpty())
        {
            m_pTextEdit->setPlainText(m_pTextEdit->toPlainText() + m_streamBuffer); ///< 追加内容
            setPlainText(m_pTextEdit->toPlainText());       ///< 更新文本
            adjustTextHeight();                             ///< 调整高度
            m_streamBuffer.clear();                         ///< 清空缓冲区
            QMetaObject::invokeMethod(m_pTextEdit, [this]()
            {
                QTextCursor c = m_pTextEdit->textCursor();
                c.movePosition(QTextCursor::End);
                m_pTextEdit->setTextCursor(c);              ///< 滚动到底部
            }, Qt::QueuedConnection);
        }
    });
    m_streamTimer.start();                                  ///< 启动定时器
}

/**
 * @brief 追加流式内容
 * @param chunk 分块内容
 */
void TextBubble::appendStreamingContent(const QString &chunk)
{
    m_streamBuffer += chunk;                                ///< 追加到缓冲区
}

/**
 * @brief 结束流式显示
 */
void TextBubble::finishStreaming()
{
    m_streamTimer.stop();                                   ///< 停止定时器
    if (!m_streamBuffer.isEmpty())
    {
        m_pTextEdit->setPlainText(m_pTextEdit->toPlainText() + m_streamBuffer); ///< 更新剩余内容
        setPlainText(m_pTextEdit->toPlainText());           ///< 设置文本
        adjustTextHeight();                                 ///< 调整高度
    }
}

/**
 * @brief 事件过滤器
 * @param o 目标对象
 * @param e 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool TextBubble::eventFilter(QObject *o, QEvent *e)
{
    if (m_pTextEdit == o && e->type() == QEvent::Paint)
    {
        adjustTextHeight();                                 ///< 在绘制时调整高度
    }
    return BubbleFrame::eventFilter(o, e);                  ///< 调用基类方法
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件对象
 * @note 重写基类方法
 */
void TextBubble::resizeEvent(QResizeEvent *event)
{
    BubbleFrame::resizeEvent(event);
    emit changeSize();                                      ///< 触发尺寸变化信号
}

/**
 * @brief 设置纯文本内容
 * @param text 文本内容
 */
void TextBubble::setPlainText(const QString &text)
{
    m_pTextEdit->setPlainText(text);                        ///< 设置文本
    qreal doc_margin = m_pTextEdit->document()->documentMargin(); ///< 获取文档边距
    int margin_left = this->layout()->contentsMargins().left(); ///< 获取左侧边距
    int margin_right = this->layout()->contentsMargins().right(); ///< 获取右侧边距
    QFontMetricsF fm(m_pTextEdit->font());                  ///< 获取字体测量工具
    QTextDocument *doc = m_pTextEdit->document();           ///< 获取文档
    int max_width = 0;
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        QString currentText = it.text();
        qreal txtW = fm.boundingRect(currentText).width();  ///< 计算文本宽度
        max_width = max_width < txtW ? txtW : max_width;    ///< 更新最大宽度
    }
    max_width += 10;                                        ///< 添加宽度补偿
    setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right)); ///< 设置最大宽度
}

/**
 * @brief 调整文本高度
 */
void TextBubble::adjustTextHeight()
{
    qreal doc_margin = m_pTextEdit->document()->documentMargin(); ///< 获取文档边距
    QTextDocument *doc = m_pTextEdit->document();           ///< 获取文档
    qreal text_height = 0;
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect();         ///< 获取文本块矩形
        text_height += text_rect.height();                  ///< 累加高度
    }
    int vMargin = this->layout()->contentsMargins().top();  ///< 获取垂直边距
    setFixedHeight(text_height + doc_margin * 2 + vMargin * 2); ///< 设置固定高度
}