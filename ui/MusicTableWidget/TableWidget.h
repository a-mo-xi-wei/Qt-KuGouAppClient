#ifndef TABLEWIDGET_H_
#define TABLEWIDGET_H_

#include<QHBoxLayout>
#include<QGridLayout>

#include"SMaskWidget.h"

class QLabel;
class QToolButton;
class QPaintEvent;
class QEnterEvent;
class QEvent;
class QLine;
class QTimer;

class TableWidget : public QWidget {
    Q_OBJECT

public:
    enum KIND {
        BlockList,
        ItemList
    };

    explicit TableWidget(const QString &title, KIND kind, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *ev) override;

    void enterEvent(QEnterEvent *ev) override;

    void leaveEvent(QEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void initUi();

    //初始化BlockCover
    void initBlockCover();

    //打乱BlockCover
    void shuffleBlockCover();

    //初始化ListCover
    void initListCover();

    //打乱ListCover
    void shuffleListCover();

    //初始化歌曲名和歌手
    void initSongInfo();

    //初始化描述文字
    void initDescVector();

    //1.大图标TabWidget
    void initItemBlockWidget();

    //2.小图标TabListWidget
    void initItemListWidget();

signals:
    void hideTitle();

    void gridChange(const int &len);

public slots:
    void onGridChange(int len);

    void onRefreshBtnClicked();

    void onRefreshTimeout();

private:
    QWidget *m_topWindow{};
    std::unique_ptr<QHBoxLayout> m_tabHLayout;
    std::unique_ptr<QGridLayout> m_gridLayout;
    std::unique_ptr<QWidget> m_gridContainer;
    QLabel *m_titleLab{};
    KIND m_kind;
    QToolButton *m_play_ToolBtn{};
    QToolButton *m_adjust_ToolBtn{};
    QLabel *line1{};
    QToolButton *m_refresh_ToolBtn{};
    QLabel *m_more_Lab{};
    int m_showCol = 5;

    //存储图片
    QVector<QString> m_blockCoverPaths;
    QVector<QString> m_listCoverPaths;
    QVector<QString> m_descVector;

    //存储歌曲名与歌手
    QVector<QPair<QString, QString> > m_songInfo;
    //隐藏列数
    bool m_hide_col_6 = true;
    bool m_hide_col_7 = true;
    //设置定时器，防止刷新次数太多，导致显示问题
    QTimer *m_refreshTimer;
};

class ItemBlockWidget : public QWidget {
    Q_OBJECT

public:
    ItemBlockWidget(const QString &path, const QString &desc, QWidget *parent = nullptr);

    void setTipLabText(const QString &text) const;

    void setPopularBtnText(const QString &text) const;

    void setDescribeText(QString desc) const;

private:
    void initUi();

protected:
    void paintEvent(QPaintEvent *ev) override;

    void enterEvent(QEnterEvent *ev) override;

    void leaveEvent(QEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QWidget *m_bacWidget{};
    std::unique_ptr<SMaskWidget> m_mask;
    QLabel *m_describeLab{};
    QLabel *m_tipLab{};
    QToolButton *m_popularBtn{};
    bool m_isHoverCoverLab = false;
    QString m_descText;
};

class ItemListWidget : public QWidget {
    Q_OBJECT

public:
    ItemListWidget(QPixmap coverPix, const QString &name, const QString &author, QWidget *parent = nullptr);

    void setNameText(const QString &name);

    void setSingerText(const QString &singer);

private:
    void initUi();

    void updateSongName() const;

    void updateSinger() const;

protected:
    void paintEvent(QPaintEvent *ev) override;

    void enterEvent(QEnterEvent *ev) override;

    void leaveEvent(QEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

public slots:
    void onHide();

private:
    std::unique_ptr<SMaskWidget> m_mask;
    QLabel *m_coverLab{};
    QLabel *m_songNameLab{};
    QLabel *m_singerLab{};
    QWidget *m_optionWidget{}; //唯一的目的是方便计算lab宽度
    QToolButton *m_play_add_ToolBtn{};
    QToolButton *m_like_ToolBtn{};
    QToolButton *m_more_ToolBtn{};

    QString m_songName;
    QString m_singer;
    bool m_isHoverCoverLab = false;
    int m_aspectRatio = 6;
};


#endif // TABLEWIDGET_H_
