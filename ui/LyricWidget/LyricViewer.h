#ifndef LYRIC_VIEWER_H
#define LYRIC_VIEWER_H

//https://github.com/BensonLaur/Beslyric-for-X

#include <QLabel>

class QPropertyAnimation;
class QScrollArea;
class QScrollBar;

class LyricPanel : public QWidget
{
    Q_OBJECT

public:
    explicit LyricPanel(QWidget *parent);

    void setLyric(const QVector<QPair<int, QString>> &lyrics, bool isLrcLyric);

    int getLineHeight() const { return m_lineH; }

    int getCurrentLine() const { return currentLine; }

public slots:
    void highlightLineLyricAtPos(int pos);

    void setViewportOffset(int y);

    void setViewportHeight(int h);

protected:
    void paintEvent(QPaintEvent *e) override;

    void resizeEvent(QResizeEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

signals:
    void lineChanged(int lineNum);

    void jumpToTime(int ms); // 点击播放按钮时发出信号

private:
    int currentLine;
    QColor normalColor;
    bool bIsLrcLyric;

    int m_viewTop = 0;
    int m_viewportH = 0;
    int m_lineH = 45; // 行高

    // 头/尾空行 & 原始歌词（用于按时间查找）
    int m_headPad = 0;
    int m_tailPad = 0;

    bool m_showExtras = false;
    QTimer *m_hideTimer = nullptr;
    QRectF m_playBtnRect;      // 播放按钮区域
    int m_highlightTimeMs = 0; // 当前高亮行对应时间

    QVector<QPair<int, QString>> lrcLyrics;
};

class ScrollLyricPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollLyricPanel(QWidget *parent);

    void updateLyric(const QVector<QPair<int, QString>> &lyrics, bool isLrcLyric);

    void setBlockAutoScroll(bool block) { blockAutoScroll = block; }

private:
    void scrollToTargetPos() const;

    void animaScrollTo(const int &targetValue);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private slots:
    void scrollTimerTimeout();

public:
    LyricPanel *lyricPanel;

private:
    QScrollArea *scrollArea;
    QScrollBar *scrollbar;

    QTimer *userScrollBlockTimer; // 用户滚动冷却
    QTimer *scrollTimer;          //滚动用的定时器
    QPropertyAnimation *scrollAnima;

    bool blockAutoScroll = false;

    int nMin;
    int nMax;

    int nTargetPos;
};


//歌词查看类
class LyricViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LyricViewer(QWidget *parent = nullptr);

    bool isLyricValid();     //歌词是否有效(路径不存在或者为空都是无效)
    bool isLrcLyric() const; //歌词是否是LRC歌词

    void setLyricPanelHighlightLineLyricAtPos(const int &pos) const;

    LyricPanel *getLyricPanel() const { return scrollLyricPanel->lyricPanel; }

    void setBlockAutoScroll(bool block) { scrollLyricPanel->setBlockAutoScroll(block); }

private:
    void initEntity();
    void initLayout();
    bool loadLyricFromFile(const QString &lyricPath);
    bool loadLyricFromRawText(const QString &content);

public slots:
    void setMusicTitle(const QString &title) const;
    void setMusicSinger(const QString &singer) const;
    void setLyricPath(const QString &path);       //设置歌词
    void setLyricRawText(const QString &content); //设置歌词文本内容
    void setEmptyLyric();                         //设置空的歌词
    void setOriginStyle();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *songLab;   //歌曲标题
    QLabel *singerLab; //歌手
    QVector<QPair<int, QString>> lrcLyrics;
    bool bIsLrcLyric;
    bool bIsLyricValid;
    bool bIsNeteaseLrcFormat;           //标记是否是网易云标准格式
    ScrollLyricPanel *scrollLyricPanel; //歌词滚动面板
};


#endif //LYRIC_VIEWER_H