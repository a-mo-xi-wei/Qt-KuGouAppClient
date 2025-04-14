#include "ChatItemBase.h"
#include "BubbleFrame.h"

#include <QFont>
#include <QMovie>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ChatItemBase::ChatItemBase(const ChatRole role, QWidget *parent)
    : QWidget(parent)
    , m_role(role)
{
    m_pNameLabel    = new QLabel();
    m_pNameLabel->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    m_pNameLabel->setFont(font);
    m_pNameLabel->setFixedHeight(20);
    m_pIconLabel    = new QLabel();
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->setFixedSize(42, 42);
    m_pBubble       = new QWidget();

    auto pGLayout = new QGridLayout(this);
    pGLayout->setVerticalSpacing(3);
    pGLayout->setHorizontalSpacing(3);
    pGLayout->setContentsMargins(3,3,3,3);
    auto pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if(m_role == ChatRole::Self)
    {
        m_pNameLabel->setContentsMargins(0,0,8,0);
        m_pNameLabel->setAlignment(Qt::AlignRight);
        pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);
        pGLayout->addWidget(m_pIconLabel, 0, 2, 2,1, Qt::AlignTop);
        pGLayout->addItem(pSpacer, 1, 0, 1, 1);
        pGLayout->addWidget(m_pBubble, 1,1, 1,1);
        pGLayout->setColumnStretch(0, 2);
        pGLayout->setColumnStretch(1, 3);
    }
    else if (m_role == ChatRole::Other){
        initMovie();
        m_pNameLabel->setContentsMargins(8,0,0,0);
        m_pNameLabel->setAlignment(Qt::AlignLeft);
        pGLayout->addWidget(m_pIconLabel, 0, 0, 2,1, Qt::AlignTop);
        // pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);
        // pGLayout->addWidget(m_loading, 0,2, 1,1);
        // 新建一个水平布局，把 m_pNameLabel 和 m_loading 放在一起
        QHBoxLayout* nameLayout = new QHBoxLayout;
        nameLayout->setContentsMargins(0, 0, 0, 0); // 去除边距
        nameLayout->setSpacing(5); // 控件之间的间距，可以调小，比如 2
        nameLayout->addWidget(m_pNameLabel);
        nameLayout->addWidget(m_loading);
        nameLayout->addStretch();

        // 用 QWidget 包裹这个布局
        QWidget* nameWidget = new QWidget;
        nameWidget->setLayout(nameLayout);

        // 加入到网格布局中
        pGLayout->addWidget(nameWidget, 0, 1, 1, 2); // 横跨原来的 1,1 和 0,2

        pGLayout->addWidget(m_pBubble, 1,1, 1,1);
        pGLayout->addItem(pSpacer, 2, 2, 1, 1);
        pGLayout->setColumnStretch(1, 3);
        pGLayout->setColumnStretch(2, 2);
    }
    else if (m_role == ChatRole::Time) {
        m_pNameLabel->deleteLater();
        m_pIconLabel->deleteLater();
        pGLayout->addItem(pSpacer, 0, 0, 1, 1);
        pGLayout->addWidget(m_pBubble, 0,1, 1,1,Qt::AlignCenter);
        pGLayout->addItem(pSpacer, 0, 2, 1, 1);
    }
}

void ChatItemBase::setUserName(const QString &name) const {
    m_pNameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap &icon) const {
    m_pIconLabel->setPixmap(icon);
}

void ChatItemBase::setWidget(QWidget *w)
{
   QGridLayout *pGLayout = (qobject_cast<QGridLayout *>)(this->layout());
   pGLayout->replaceWidget(m_pBubble, w);
   delete m_pBubble;
   m_pBubble = w;
}

void ChatItemBase::startMovie(const bool &flag) {
    if(flag) {
        m_loading->show();
        m_loadingMovie->start();
    }
    else {
        m_loading->hide();
        m_loadingMovie->stop();
    }
}

void ChatItemBase::initMovie() {
    //初始化加载动画
    m_loadingMovie = new QMovie(this);
    m_loadingMovie->setFileName(":/Res/window/loading.gif");
    m_loading = new QLabel(this);
    m_loading->setMovie(m_loadingMovie);
    m_loading->setFixedSize(16,16);
    m_loading->setAttribute(Qt::WA_TranslucentBackground , true);
    m_loading->setAutoFillBackground(false);
    m_loading->hide();
    m_loadingMovie->stop();
}
