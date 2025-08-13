#include "LyricViewer.h"
#include "LrcProcessor.h"
#include "ElaMessageBar.h"
#include "ElaScrollArea.h"
#include "ElaScrollBar.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QTimer>
#include <QScrollBar>

LyricPanel::LyricPanel(QWidget *parent)
    : QWidget(parent)
{
    bIsLrcLyric = false;
    lrcLyrics.clear();
    currentLine = -1;

    normalColor = QColor(0xACAEA9);
    installEventFilter(this); // 只监听自己
    m_hideTimer = new QTimer(this);
    m_hideTimer->setSingleShot(true);
    m_hideTimer->setInterval(3000); // 3秒
    connect(m_hideTimer,
            &QTimer::timeout,
            this,
            [this]() {
                m_showExtras = false;
                update();
            });
    this->setMouseTracking(true);
}

void LyricPanel::setLyric(const QVector<QPair<int, QString>> &lyrics, bool isLrcLyric)
{
    // qDebug()<<"lyrics.size"<<lyrics.size();
    lrcLyrics.clear();
    if (lyrics.isEmpty()) {
        lrcLyrics = {
            {0, "没有可显示的歌词"},
        };
        m_headPad = 0;
        m_tailPad = 0;
    } else {
        // 头/尾空行数量（可按需调整）
        m_headPad = 5;
        m_tailPad = 7;
        // 在开头添加m_headPad行空白，用于撑开顶部空间
        for (int i = 0; i < m_headPad; ++i)
            lrcLyrics.emplace_back(QPair<int, QString>(0, ""));
        // 插入正常歌词内容
        for (const auto &line : lyrics) {
            lrcLyrics.emplace_back(line);
        }
        // 结尾再添加m_tailPad行空白撑开底部空间
        for (int i = 0; i < m_tailPad; ++i)
            lrcLyrics.emplace_back(QPair<int, QString>(0, ""));
    }
    // qDebug()<<"lrcLyrics.size() : "<<lrcLyrics.size();

    bIsLrcLyric = isLrcLyric;
    currentLine = -1;
    setMinimumHeight(lrcLyrics.size() * m_lineH);
    update();
}

void LyricPanel::highlightLineLyricAtPos(int pos)
{
    // qDebug() << " pos: " << pos;
    if (lrcLyrics.size() <= m_headPad + m_tailPad) {
        // 只有空行
        if (currentLine != 0) {
            currentLine = 0;
            emit lineChanged(currentLine);
            update();
        }
        return;
    }

    int startIdx = m_headPad;
    int endIdx = lrcLyrics.size() - m_tailPad; // 不包含尾空行

    int selectedIdx = startIdx;
    for (int i = startIdx; i < endIdx; ++i) {
        if (pos < lrcLyrics[i].first) {
            selectedIdx = i - 1;
            break;
        }
        if (i == endIdx - 1) {
            selectedIdx = endIdx - 1;
        }
    }
    if (selectedIdx < startIdx)
        selectedIdx = startIdx;

    if (selectedIdx != currentLine) {
        currentLine = selectedIdx;
        emit lineChanged(currentLine);
        update();
    }
}

void LyricPanel::setViewportOffset(int y)
{
    m_viewTop = y;
    update();
}

void LyricPanel::setViewportHeight(int height)
{
    m_viewportH = height;
    setMinimumHeight(lrcLyrics.size() * m_lineH);
    update();
}

void LyricPanel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int viewportH = (m_viewportH > 0) ? m_viewportH : height();
    int viewTop = m_viewTop;
    int viewCenterY = viewTop + viewportH / 2;

    int firstVisible = std::max(0, (viewTop - 2 * m_lineH) / m_lineH);
    int lastVisible = std::min(static_cast<int>(lrcLyrics.size() - 1),
                               (viewTop + viewportH + 2 * m_lineH) / m_lineH);

    constexpr int baseFontSize = 16;
    constexpr int centerFontSize = 24;
    const double fadeRadiusPx = std::max(1, viewportH);
    int highlightLine = std::max(0, (viewCenterY / m_lineH) - 1);

    for (int i = firstVisible; i <= lastVisible; ++i) {
        QRectF rect(2, i * m_lineH, width() - 4, m_lineH);

        double distRatio = std::min(1.0,
                                    std::abs(static_cast<int>(rect.center().y()) -
                                             (highlightLine * m_lineH)) / fadeRadiusPx);
        double alphaRatio = 1.0 - std::sin(distRatio * (M_PI / 2.0));

        int alpha = static_cast<int>(255 * alphaRatio);
        int fontSize = baseFontSize + static_cast<int>(
                           (centerFontSize - baseFontSize) * alphaRatio);
        fontSize = (i == highlightLine) ? fontSize : fontSize - 4;

        QFont font("Microsoft YaHei");
        font.setPixelSize(fontSize);
        font.setWeight(i == highlightLine ? QFont::Bold : QFont::Normal);
        painter.setFont(font);

        QColor color = (i == highlightLine) ? QColor(255, 255, 255) : normalColor;
        color.setAlpha(alpha);
        painter.setPen(color);

        painter.drawText(rect, Qt::AlignCenter, lrcLyrics[i].second);

        // ======= 绘制高亮行两边的渐变线 =======
        // qDebug()<<"lrcLyrics.size() : "<<lrcLyrics.size();
        if (i == highlightLine && lrcLyrics.size() != m_headPad + m_tailPad + 1) {
            ///< 没有歌词的时候不显示
            m_highlightTimeMs = lrcLyrics[i].first; // 保存当前高亮时间

            if (m_showExtras) {
                constexpr int lineLength = 120; // 线条长度
                constexpr int margin = 55;      // 离控件边缘的间距
                int centerY = rect.center().y();

                // 左边线：从控件右边缘向左渐变
                QLinearGradient leftGrad(margin, 0, margin + lineLength, 0);
                leftGrad.setColorAt(0.0, QColor(168, 170, 132, 255));
                leftGrad.setColorAt(1.0, QColor(255, 255, 255, 0));
                painter.setPen(QPen(QBrush(leftGrad), 1));
                painter.drawLine(QPointF(margin, centerY), QPointF(margin + lineLength, centerY));

                // 右边线：从控件左边缘向右渐变
                QLinearGradient rightGrad(width() - margin - lineLength, 0, width() - margin, 0);
                rightGrad.setColorAt(0.0, QColor(255, 255, 255, 0));
                rightGrad.setColorAt(1.0, QColor(168, 170, 132, 255));
                painter.setPen(QPen(QBrush(rightGrad), 1));
                painter.drawLine(QPointF(width() - margin - lineLength, centerY),
                                 QPointF(width() - margin, centerY));

                // ======= 左侧时间 =======
                // qDebug()<<lrcLyrics[i].first;
                QString timeText = QString("%1:%2")
                                   .arg(lrcLyrics[i].first / 1000 / 60, 2, 10, QChar('0'))
                                   .arg(lrcLyrics[i].first / 1000 % 60, 2, 10, QChar('0'));
                QFont timeFont("Microsoft YaHei", 12);
                painter.setFont(timeFont);
                QFontMetrics fm(timeFont);
                int timeTextWidth = fm.horizontalAdvance(timeText);

                // 渐变线起点
                int lineStartX = margin;
                // 时间文字右边紧贴渐变线左边
                int timeX = lineStartX - 4 - timeTextWidth;
                int timeY = centerY + fm.ascent() / 2 - 1;
                painter.setPen(QColor(0xFFEEA1));
                painter.drawText(QPointF(timeX, timeY), timeText);

                // ======= 右侧播放圆形 =======
                constexpr int circleRadius = 10;
                int circleX = width() - margin + 30 - circleRadius; // 右边线左移20px + 半径
                int circleY = centerY - circleRadius;
                m_playBtnRect = QRectF(circleX, circleY, circleRadius * 2, circleRadius * 2);

                painter.setPen(QPen(QColor(0xFFEEA1), 2));
                painter.drawEllipse(m_playBtnRect);

                // 播放按钮（三角形）
                QPointF triangle[3] = {
                    QPointF(m_playBtnRect.left() + circleRadius * 0.7,
                            m_playBtnRect.top() + circleRadius * 0.5),
                    QPointF(m_playBtnRect.left() + circleRadius * 0.7,
                            m_playBtnRect.bottom() - circleRadius * 0.5),
                    QPointF(m_playBtnRect.right() - circleRadius * 0.5,
                            m_playBtnRect.center().y())
                };
                painter.setBrush(QColor(0xFFEEA1));
                painter.drawPolygon(triangle, 3);
            }
        }
    }

    QWidget::paintEvent(e);
}

void LyricPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int viewportH = (m_viewportH > 0) ? m_viewportH : height();
    int visibleLines = viewportH / m_lineH;

    int dynamicHeadPad = visibleLines / 2 - 1;
    int dynamicTailPad = visibleLines / 2 + 1;

    // 去掉旧头尾空行
    if (!lrcLyrics.isEmpty()) {
        lrcLyrics = lrcLyrics.mid(m_headPad, lrcLyrics.size() - m_headPad - m_tailPad);
    }

    m_headPad = dynamicHeadPad;
    m_tailPad = dynamicTailPad;

    // 添加新的头部空行
    for (int i = 0; i < m_headPad; ++i)
        lrcLyrics.prepend({0, ""});

    // 添加新的尾部空行
    for (int i = 0; i < m_tailPad; ++i)
        lrcLyrics.append({0, ""});

    setMinimumHeight(lrcLyrics.size() * m_lineH);
    update();
}

bool LyricPanel::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Wheel:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease: m_showExtras = true;
        m_hideTimer->start();
        update();
        break;
    default: break;
    }
    return QWidget::eventFilter(obj, event);
}

void LyricPanel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_showExtras && m_playBtnRect.contains(event->pos())) {
        setCursor(Qt::PointingHandCursor);
    } else {
        unsetCursor();
    }
    QWidget::mouseMoveEvent(event);
}

void LyricPanel::mousePressEvent(QMouseEvent *event)
{
    if (m_showExtras && m_playBtnRect.contains(event->pos())) {
        emit jumpToTime(m_highlightTimeMs);
        //qDebug() << "发送当前点击位置时间： " << m_highlightTimeMs;
    }
    QWidget::mousePressEvent(event);
}

ScrollLyricPanel::ScrollLyricPanel(QWidget *parent)
    : QWidget(parent)
{
    lyricPanel = new LyricPanel(this);
    lyricPanel->setObjectName("lyricPanel");
    lyricPanel->setMinimumWidth(500);
    lyricPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    scrollArea = new ElaScrollArea(this);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setObjectName("ScrollLyricPanelScrollArea");
    scrollArea->viewport()->setStyleSheet("background-color:transparent;");
    scrollArea->setWidget(lyricPanel);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->verticalScrollBar()->setSingleStep(10); // 每次滚轮滚动 5px
    scrollArea->setIsAnimation(Qt::Vertical, true);
    scrollArea->setCursor(Qt::OpenHandCursor);
    scrollArea->installEventFilter(this);
    scrollbar = dynamic_cast<ElaScrollBar *>(scrollArea->verticalScrollBar());

    scrollAnima = new QPropertyAnimation(scrollbar, "value", this);
    scrollAnima->setEasingCurve(QEasingCurve::OutSine);
    scrollAnima->setDuration(1000);

    QTimer::singleShot(0,
                       this,
                       [=] {
                           lyricPanel->setViewportHeight(scrollArea->viewport()->height());
                           lyricPanel->setViewportOffset(scrollbar->value());
                       });

    // 滚动时更新视口顶部 y
    connect(scrollbar,
            &QScrollBar::valueChanged,
            lyricPanel,
            &LyricPanel::setViewportOffset);

    // 视口大小变化时更新高度
    scrollArea->viewport()->installEventFilter(this);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(scrollArea);

    scrollTimer = new QTimer(this);
    scrollTimer->setInterval(100); //歌词滚动的判定间隔

    userScrollBlockTimer = new QTimer(this);
    userScrollBlockTimer->setSingleShot(true);
    connect(userScrollBlockTimer,
            &QTimer::timeout,
            [=]() {
                blockAutoScroll = false; // 冷却结束
            });

    nMin = 0;
    nMax = 0;

    connect(scrollbar,
            &QScrollBar::rangeChanged,
            [=](int min, int max) {
                nMin = min;
                nMax = max;
            });
    connect(lyricPanel,
            &LyricPanel::lineChanged,
            [=](int currentLine) {
                if (blockAutoScroll)
                    return;

                int pageH = scrollbar->pageStep(); // 视口高度
                int lineH = lyricPanel->getLineHeight();

                /*
                    +5（头部空行）
                    - 1（你手动提前一行显示）
                    - 2（highlightLine 计算方式里，中心点落在下一行的时间差）
                    = 最终差 2 行
                */
                int logicalLine = currentLine + 2;
                if (logicalLine < 0)
                    logicalLine = 0;

                int centerRow = logicalLine - 1;
                if (centerRow < 0)
                    centerRow = 0;

                int targetPos = centerRow * lineH - pageH / 2 + lineH / 2;
                targetPos = std::clamp(targetPos, nMin, nMax);
                animaScrollTo(targetPos);
            });

    connect(scrollTimer, &QTimer::timeout, this, &ScrollLyricPanel::scrollTimerTimeout);
}

void ScrollLyricPanel::updateLyric(const QVector<QPair<int, QString>> &lyrics,
                                   bool isLrcLyric)
{
    lyricPanel->setLyric(lyrics, isLrcLyric);

    // 重置滚动位置
    animaScrollTo(0);
}

void ScrollLyricPanel::scrollToTargetPos() const
{
    if (!scrollTimer->isActive())
        scrollTimer->start();
}

void ScrollLyricPanel::animaScrollTo(const int &targetValue)
{
    if (scrollAnima->state() == QAbstractAnimation::Running)
        scrollAnima->stop(); // 停掉旧动画

    scrollAnima->setStartValue(scrollbar->value());
    scrollAnima->setEndValue(targetValue);
    scrollAnima->start();
}

bool ScrollLyricPanel::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == scrollArea) {
        static bool dragging = false;
        static QPoint lastPos;

        switch (event->type()) {
        case QEvent::MouseButtonPress: {
            auto mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                dragging = true;
                lastPos = mouseEvent->pos();
                scrollArea->setCursor(Qt::ClosedHandCursor);
                // 如果有动画正在运行，停止它
                if (scrollAnima && scrollAnima->state() == QAbstractAnimation::Running) {
                    scrollAnima->stop();
                }
            }
            break;
        }
        case QEvent::MouseMove: {
            blockAutoScroll = true;
            userScrollBlockTimer->start(3000);
            auto mouseEvent = static_cast<QMouseEvent *>(event);
            if (dragging) {
                QPoint delta = (mouseEvent->pos() - lastPos) * 2;
                lastPos = mouseEvent->pos();
                scrollArea->verticalScrollBar()->setValue(
                    scrollArea->verticalScrollBar()->value() - delta.y());
                return true; // 拖动时阻止默认处理
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton && dragging) {
                dragging = false;
                scrollArea->setCursor(Qt::OpenHandCursor);
            }
            break;
        }
        default: break;
        }
    }
    if (obj == scrollArea->viewport()) {
        if (event->type() == QEvent::Wheel) {
            blockAutoScroll = true;
            userScrollBlockTimer->start(3000);
        }
        if (event->type() == QEvent::Resize) {
            blockAutoScroll = false;
            QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
            lyricPanel->setViewportHeight(resizeEvent->size().height());
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ScrollLyricPanel::scrollTimerTimeout()
{
    int minStep = 10;
    int maxStep = 1000;

    int current = scrollbar->value();

    if (current == nTargetPos)
        scrollTimer->stop();
    else {
        if (current < nTargetPos) {
            int step = (nTargetPos - current) * 3 / 4;
            if (step > maxStep)
                step = maxStep;
            if (step < minStep)
                step = minStep;

            current += step;
            if (current > nTargetPos)
                current = nTargetPos;
        } else {
            int step = (current - nTargetPos) * 3 / 4;
            if (step > maxStep)
                step = maxStep;
            if (step < minStep)
                step = minStep;

            current -= step;
            if (current < nTargetPos)
                current = nTargetPos;
        }
        animaScrollTo(nTargetPos);
    }
}

LyricViewer::LyricViewer(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);

    initEntity();
    initLayout();
}

void LyricViewer::initEntity()
{
    lrcLyrics.clear();
    bIsLrcLyric = false;
    bIsLyricValid = false;
    bIsNeteaseLrcFormat = true;
}

void LyricViewer::initLayout()
{
    songLab = new QLabel(this);
    songLab->setObjectName("songLab");
    songLab->setText("");
    songLab->setAlignment(Qt::AlignCenter);

    singerLab = new QLabel(this);
    singerLab->setObjectName("singerLab");
    singerLab->setText("网络歌手");

    singerLab->setAlignment(Qt::AlignCenter);

    scrollLyricPanel = new ScrollLyricPanel(this);
    scrollLyricPanel->setObjectName("scrollLyricPanel");
    scrollLyricPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignCenter);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(10);
    vLayout->addWidget(songLab);
    vLayout->addWidget(singerLab);
    vLayout->addWidget(scrollLyricPanel);
    vLayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));

    this->setStyleSheet("QWidget{font-family:Microsoft YaHei ;}"
        "QLabel#songLab{color: white;font-size:17pt;font-weight: DemiBold;}"
        "QLabel#singerLab{color: #ACAEA9;font-size:10pt;}"
        );
}

//歌词是否有效(路径不存在或者为空都是无效)
bool LyricViewer::isLyricValid()
{
    return bIsLyricValid;
}

//歌词是否是LRC歌词
bool LyricViewer::isLrcLyric() const
{
    return bIsLrcLyric;
}

void LyricViewer::setLyricPanelHighlightLineLyricAtPos(const int &pos) const
{
    scrollLyricPanel->lyricPanel->highlightLineLyricAtPos(pos);
}

void LyricViewer::setMusicTitle(const QString &title) const
{
    songLab->setText(title);
    songLab->setToolTip(title);
}

void LyricViewer::setMusicSinger(const QString &singer) const
{
    singerLab->setText(singer);
    singerLab->setToolTip(singer);
}

void LyricViewer::setLyricPath(const QString &path)
{
    //使用该路径初始化歌词预览面板
    if (!loadLyricFromFile(path)) {
        ElaMessageBar::information(ElaMessageBarType::Top,
                                   "提示",
                                   tr("打开载入lrc歌词时发生错误：") + path
                                   + "\n\n" + tr("请注意是否歌词是否存在？"),
                                   2000,
                                   this->window());

        lrcLyrics.clear();
        bIsLrcLyric = false;
    }
    scrollLyricPanel->updateLyric(lrcLyrics, bIsLrcLyric);
}

//设置空的歌词
void LyricViewer::setEmptyLyric()
{
    singerLab->setText(tr("网络歌手"));

    lrcLyrics.clear();
    scrollLyricPanel->updateLyric(lrcLyrics, true);

    bIsLyricValid = false;
}

void LyricViewer::setOriginStyle()
{
    setEmptyLyric();
    songLab->setText("网络歌曲");
    singerLab->setText("网络歌手");
}

void LyricViewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // qDebug() << " 当前宽度 ： " << width();
    QTimer::singleShot(0,
                       [this]() {
                           emit scrollLyricPanel->lyricPanel->lineChanged(
                               scrollLyricPanel->lyricPanel->getCurrentLine());
                       });
}

bool LyricViewer::loadLyricFromFile(const QString &lyricPath)
{
    LrcProcessor lrcProcessor;

    if (!lrcProcessor.LoadFromFile(lyricPath)) {
        return false;
    }

    lrcLyrics = lrcProcessor.GetLrcLyric();

    bIsLyricValid = lrcProcessor.bIsLrcLyric;
    bIsLrcLyric = lrcProcessor.bIsLrcLyric;
    bIsNeteaseLrcFormat = lrcProcessor.bIsNeteaseLrcFormat;

    return true;
}