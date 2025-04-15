#include "ChatView.h"
#include "logger.hpp"
#include "MyScrollArea.h"

#include <QScrollBar>
#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>
#include <QPropertyAnimation>

ChatView::ChatView(QWidget *parent)
    : QWidget(parent)
    , isAppended(false)
    , m_logo(new QLabel(this))
    , m_helloText(new QLabel(this))
    , m_funcText(new QLabel(this))
    , m_centerInitWidget(new QWidget(this))
{
    {
        this->m_logo->setFixedSize(60, 60);
        this->m_logo->setPixmap(QPixmap(":/Res/window/deepseek.png").scaled(this->m_logo->size()));
        this->m_helloText->setFixedHeight(70);
        this->m_helloText->setText("我是DeepSeek, 很高兴见到你!");
        this->m_helloText->setStyleSheet("color: black;font-wight: bold;font-size: 20px;");
        auto hlay = new QHBoxLayout;
        hlay->addWidget(this->m_logo);
        hlay->addWidget(this->m_helloText);
        this->m_funcText->setText("我可以帮你写代码、读文件、写作各种创意内容，请把你的任务交给我吧~");
        this->m_funcText->setStyleSheet("color: #404040;font-size: 12px;");
        auto vlay = new QVBoxLayout(this->m_centerInitWidget);
        vlay->addLayout(hlay);
        vlay->addWidget(this->m_funcText);
        this->m_centerInitWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    auto pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0,0,0,0);

    m_pScrollArea = new MyScrollArea();
    m_pScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pScrollArea->setObjectName("chat_scroll_area");
    m_pScrollArea->setFrameShape(QFrame::NoFrame);
    pMainLayout->addWidget(m_pScrollArea);

    auto w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto pVLayout_1 = new QVBoxLayout(w);
    pVLayout_1->setContentsMargins(0, 0, 0, 0);
    pVLayout_1->setSpacing(0);
    pVLayout_1->addStretch(); // 顶部stretch
    pVLayout_1->addWidget(m_centerInitWidget, 0, Qt::AlignCenter); // 居中显示
    pVLayout_1->addStretch(); // 底部stretch
    m_pScrollArea->setWidget(w);    //应该在QScrollArea构造后执行才对

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto pVScrollBar = m_pScrollArea->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);

    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->installEventFilter(this);

    // 设置透明度效果
    auto* opacityEffect = new QGraphicsOpacityEffect(m_centerInitWidget);
    opacityEffect->setOpacity(1.0);
    m_centerInitWidget->setGraphicsEffect(opacityEffect);
}

void ChatView::appendChatItem(QWidget *item)
{
    const auto vl = getLayout();
    //qDebug() << "vl->count() is " << vl->count();
    if (vl) {
        vl->insertWidget(vl->count()-1, item);
        if (vl->count() == 2) {
            startFadeOutAnimation();
        }
    }
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
            //qDebug() << "Removing item at index:" << lastItemIndex << "type:" << (item->widget() ? item->widget()->metaObject()->className() : "null");
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
    qDebug()<<"layout count : "<<layout->count();

    // 移除所有非初始化部件
    QList<QLayoutItem*> itemsToRemove;
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem* item = layout->itemAt(i);
        if (item->widget() && item->widget() != m_centerInitWidget) {
            itemsToRemove.append(item);
        }
    }

    for (QLayoutItem* item : itemsToRemove) {
        layout->removeItem(item);
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    if (layout->count() == 1) {
        startFadeOutAnimation();
    }
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

void ChatView::startFadeOutAnimation() {
    auto* effect = qobject_cast<QGraphicsOpacityEffect*>(m_centerInitWidget->graphicsEffect());
    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(300);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    connect(anim, &QPropertyAnimation::finished, [this]() {
        m_centerInitWidget->hide(); // 动画完成后隐藏
    });
}

void ChatView::startFadeInAnimation() {
    m_centerInitWidget->show(); // 先显示再动画
    auto* effect = qobject_cast<QGraphicsOpacityEffect*>(m_centerInitWidget->graphicsEffect());
    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(300);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
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