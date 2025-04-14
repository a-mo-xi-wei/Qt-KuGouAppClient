#include "TextBubble.h"

#include <QFontMetricsF>
#include <QTimer>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextLayout>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

TextBubble::TextBubble(const ChatRole role, const QString &text, QWidget *parent)
    : BubbleFrame(role, parent)
{
    m_pTextEdit = new QTextEdit();
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->installEventFilter(this);
    m_pTextEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");

    auto font = QFont("AaSongLiuKaiTi");
    font.setPointSize(14);
    font.setWeight(QFont::Medium);
    m_pTextEdit->setFont(font);
    setPlainText(text);
    setWidget(m_pTextEdit);
}

void TextBubble::startStreaming() {
    m_streamBuffer.clear();
    m_pTextEdit->clear();

    // 设置定时器间隔为50ms
    m_streamTimer.setInterval(50);
    connect(&m_streamTimer, &QTimer::timeout, this, [this](){
        if(!m_streamBuffer.isEmpty()) {
            // 追加缓冲区内容
            m_pTextEdit->setPlainText(m_pTextEdit->toPlainText() + m_streamBuffer);

            // 触发尺寸更新
            setPlainText(m_pTextEdit->toPlainText());
            adjustTextHeight();

            // 清空缓冲区
            m_streamBuffer.clear();

            // 滚动到底部
            // 滚动优化
           QMetaObject::invokeMethod(m_pTextEdit, [this](){
               QTextCursor c = m_pTextEdit->textCursor();
               c.movePosition(QTextCursor::End);
               m_pTextEdit->setTextCursor(c);
           }, Qt::QueuedConnection);
        }
    });
    m_streamTimer.start();
}

void TextBubble::appendStreamingContent(const QString &chunk) {
    m_streamBuffer += chunk; // 将分块内容存入缓冲区
}

void TextBubble::finishStreaming() {
    m_streamTimer.stop();
    // 强制更新剩余内容
    if(!m_streamBuffer.isEmpty()) {
        m_pTextEdit->setPlainText(m_pTextEdit->toPlainText() + m_streamBuffer);
        setPlainText(m_pTextEdit->toPlainText());
        adjustTextHeight();
    }
}

bool TextBubble::eventFilter(QObject *o, QEvent *e) {
    if (m_pTextEdit == o && e->type() == QEvent::Paint) {
        adjustTextHeight(); //PaintEvent中设置
    }
    return BubbleFrame::eventFilter(o, e);
}

void TextBubble::resizeEvent(QResizeEvent *event) {
    BubbleFrame::resizeEvent(event);
    emit changeSize();
}

void TextBubble::setPlainText(const QString &text) {
    m_pTextEdit->setPlainText(text);
    //m_pTextEdit->setHtml(text);
    //找到段落中最大宽度
    qreal doc_margin = m_pTextEdit->document()->documentMargin();
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    QFontMetricsF fm(m_pTextEdit->font());
    QTextDocument *doc = m_pTextEdit->document();
    int max_width = 0;
    /*
    //遍历每一段找到 最宽的那一段
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) //字体总长
    {
        int txtW = int(fm.horizontalAdvance(it.text()));
        max_width = max_width < txtW ? txtW : max_width; //找到最长的那段
    }
*/
    // 使用更精确的boundingRect计算宽度
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
        QString currentText = it.text();
        qreal txtW = fm.boundingRect(currentText).width();
        max_width = max_width < txtW ? txtW : max_width; //找到最长的那段
    }

    // 添加3像素补偿防止计算误差
    max_width += 3;

    //设置这个气泡的最大宽度 只需要设置一次
    setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right)); //设置最大宽度
}

void TextBubble::adjustTextHeight() {
    qreal doc_margin = m_pTextEdit->document()->documentMargin(); //字体到边框的距离默认为4
    QTextDocument *doc = m_pTextEdit->document();
    qreal text_height = 0;
    //把每一段的高度相加=文本高
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect(); //这段的rect
        text_height += text_rect.height();
    }
    int vMargin = this->layout()->contentsMargins().top();
    //设置这个气泡需要的高度 文本高+文本边距+TextEdit边框到气泡边框的距离
    setFixedHeight(text_height + doc_margin * 2 + vMargin * 2);
}