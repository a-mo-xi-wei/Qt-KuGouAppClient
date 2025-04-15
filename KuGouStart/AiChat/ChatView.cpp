#include "ChatView.h"
#include "logger.hpp"
#include "MyScrollArea.h"

#include <QScrollBar>
#include <QEvent>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>

ChatView::ChatView(QWidget *parent)
   : QWidget(parent)
   , isAppended(false)
{
    auto pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0,0,0,0);

    m_pScrollArea = new MyScrollArea();
    m_pScrollArea->setObjectName("chat_scroll_area");
    m_pScrollArea->setFrameShape(QFrame::NoFrame);
    pMainLayout->addWidget(m_pScrollArea);
    pMainLayout->addSpacerItem(new QSpacerItem(0,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    auto w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);
    auto pVLayout_1 = new QVBoxLayout(w);
    pVLayout_1->addStretch();
    m_pScrollArea->setWidget(w);    //应该在QSCrollArea构造后执行才对

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto pVScrollBar = m_pScrollArea->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);

    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->installEventFilter(this);
}

void ChatView::appendChatItem(QWidget *item)
{
    const auto vl = getLayout();
    //qDebug() << "vl->count() is " << vl->count();
    if (vl)vl->insertWidget(vl->count()-1, item);
    else STREAM_WARN()<< "ChatView::appendChatItem(): layout is nullptr";
    isAppended = true;
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}

void ChatView::removeLastItem() {
    auto layout = getLayout();
    if (!layout) {
        qWarning() << "ChatView::removeLastItem(): layout is null.";
        STREAM_WARN() << "ChatView::removeLastItem(): layout is null.";
        return;
    }
    qDebug()<<"layout count : "<<layout->count();
    // 布局包含拉伸项（count >=1），只有当count >1 时才有聊天项
    if (layout->count() > 1) {
        // 最后一个聊天项的索引是 count-2（因为最后一个是拉伸项）
        const int lastItemIndex = layout->count() - 2;

        if (QLayoutItem* item = layout->takeAt(lastItemIndex)) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater(); // 安全删除控件
            }
            delete item; // 删除布局项
        }
    }
    update();
}

void ChatView::removeAllItem() {
    auto layout = getLayout();
    if (!layout) return;

    // 反向遍历避免索引错位
    if (layout->count() > 1) { // 保留最后的 stretch 项（count=1）
        for (int i = layout->count() - 2; i >= 0; --i) {
            QLayoutItem* item = layout->takeAt(i);
            if (item && item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
    }
    // 非widget项（如stretch/spacer）自动保留
    update();
}

QVBoxLayout *ChatView::getLayout() const {
    if (!m_pScrollArea || !m_pScrollArea->widget()) {
        qWarning() << "ChatView::getLayout(): m_pScrollArea or its widget is null.";
        STREAM_WARN() << "ChatView::getLayout(): m_pScrollArea or its widget is null.";
        return nullptr;
    }

    auto layout = qobject_cast<QVBoxLayout *>(m_pScrollArea->widget()->layout());
    if (!layout) {
        qWarning() << "ChatView::getLayout(): Layout is not a QVBoxLayout.";
        STREAM_WARN() << "ChatView::getLayout(): Layout is not a QVBoxLayout.";
    }
    return layout;
}

bool ChatView::eventFilter(QObject *o, QEvent *e)
{
    /*if(e->type() == QEvent::Resize && o == )
    {

    }
    else */if(e->type() == QEvent::Enter && o == m_pScrollArea)
    {
        m_pScrollArea->verticalScrollBar()->setHidden(m_pScrollArea->verticalScrollBar()->maximum() == 0);
    }
    else if(e->type() == QEvent::Leave && o == m_pScrollArea)
    {
         m_pScrollArea->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(o, e);
}

void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void ChatView::onVScrollBarMoved(int min, int max)
{
    if(isAppended) //添加item可能调用多次
    {
        QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());
        //500毫秒内可能调用多次
        QTimer::singleShot(500, [this]()
        {
            isAppended = false;
        });
    }
}