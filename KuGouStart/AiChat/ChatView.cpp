#include "ChatView.h"
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

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");
    pMainLayout->addWidget(m_pScrollArea);

    auto w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);
    auto pVLayout_1 = new QVBoxLayout();
    pVLayout_1->addStretch();
    w->setLayout(pVLayout_1);
    m_pScrollArea->setWidget(w);    //应该时在QSCrollArea构造后执行 才对

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto pVScrollBar = m_pScrollArea->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);

    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->installEventFilter(this);
}

void ChatView::appendChatItem(QWidget *item)
{
   auto vl = getLayout();
   //qDebug() << "vl->count() is " << vl->count();
   vl->insertWidget(vl->count()-1, item);
   isAppended = true;
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}

void ChatView::removeAllItem() const {
    auto layout = getLayout();

   int count = layout->count();

    for (int i = 0; i < count - 1; ++i) {
        QLayoutItem *item = layout->takeAt(0); // 始终从第一个控件开始删除
        if (item) {
            if (auto widget = item->widget()) {
                delete widget;
            }
            delete item;
        }
    }

}

QVBoxLayout *ChatView::getLayout() const {
    if (!m_pScrollArea || !m_pScrollArea->widget()) {
        qWarning() << "ChatView::getLayout(): m_pScrollArea or its widget is null.";
        return nullptr;
    }

    auto layout = qobject_cast<QVBoxLayout *>(m_pScrollArea->widget()->layout());
    if (!layout) {
        qWarning() << "ChatView::getLayout(): Layout is not a QVBoxLayout.";
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