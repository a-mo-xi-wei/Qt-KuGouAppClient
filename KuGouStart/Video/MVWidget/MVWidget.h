//
// Created by WeiWang on 24-11-12.
//

#ifndef MVWIDGET_H
#define MVWIDGET_H

#include <QWidget>

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class MVWidget; }
QT_END_NAMESPACE

class MVWidget : public QWidget {
Q_OBJECT

public:
    explicit MVWidget(QWidget *parent = nullptr);
    ~MVWidget() override;
private:
    void initButtonGroup();

    void initUi();

    void initLiveScene();

    void initHonorOfKings();

    void initAwardCeremony();

    void initHotMV();

    void initVector();

    const QString parseTitle(const QString &title);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void showEvent(QShowEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_recommend_pushButton_clicked();

    void on_chinese_pushButton_clicked();

    void on_west_pushButton_clicked();

    void on_koreaAndJapan_pushButton_clicked();

    void on_more_pushButton1_clicked();

    void on_more_pushButton2_clicked();

    void on_more_pushButton3_clicked();

    void on_more_pushButton4_clicked();

    void on_more_pushButton5_clicked();

private:
    Ui::MVWidget *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    QWidget* m_parent{};
    QAction* m_searchAction{};

    struct MusicInfo {
        // 添加构造函数
        // 左值引用版本
        MusicInfo(const QString &pixmapPath, const QString &title, const QString &description)
            : pixPath(pixmapPath), title(title), description(description) {
        }
        // 右值引用版本
        MusicInfo(QString&& pixmapPath, QString&& title, QString&& description)
            : pixPath(std::move(pixmapPath)), title(std::move(title)), description(std::move(description)) {
        }
        QString pixPath;
        QString title;
        QString description;
    };
    std::vector<std::pair<QString, QString> > m_titleAndDesc{};
    std::vector<MusicInfo> m_total{};
    std::vector<MusicInfo> m_recommendVector{};
    std::vector<MusicInfo> m_chineseVector{};
    std::vector<MusicInfo> m_westVector{};
    std::vector<MusicInfo> m_koreaAndJapanVector{};
    std::vector<MusicInfo> m_liveSceneVector{};
    std::vector<MusicInfo> m_honorOfKingsVector{};
    std::vector<MusicInfo> m_awardCeremonyVector{};
    std::vector<MusicInfo> m_hotMVVector{};
};


#endif //MVWIDGET_H
