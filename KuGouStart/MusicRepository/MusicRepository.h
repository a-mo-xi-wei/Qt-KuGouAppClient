//
// Created by WeiWang on 24-11-11.
//

#ifndef MUSICREPOSITORY_H
#define MUSICREPOSITORY_H

#include"MyBlockWidget.h"

#include <QWidget>
class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class MusicRepository; }
QT_END_NAMESPACE

class MusicRepository : public QWidget {
    Q_OBJECT

public:
    explicit MusicRepository(QWidget *parent = nullptr);

    ~MusicRepository() override;

private:
    void initButtonGroup();

    void initUi();

    void initVector();

protected:
    //想要通过事件过滤器去捕获每个list的 进入、离开事件是不行的，若是单个的话还行，所以还得要提升
    //bool eventFilter(QObject *watched, QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void on_chinese_pushButton_clicked();

    void on_west_pushButton_clicked();

    void on_korea_pushButton_clicked();

    void on_japan_pushButton_clicked();
private:
    Ui::MusicRepository *ui;
    QWidget* m_topWindow{};
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    //四个容器，响应四个按钮
    struct MusicListInfo {
        // 添加构造函数
        MusicListInfo(const QString& pixmapPath, const QString& song, const QString& singer)
            : pixPath(pixmapPath), song(song), singer(singer) {}
        QString pixPath;
        QString song;
        QString singer;
    };
    std::vector<std::pair<QString,QString>>m_songAndsinger{};
    std::vector<MusicListInfo>m_total{};
    std::vector<MusicListInfo>m_chineseVector{};
    std::vector<MusicListInfo>m_westVector{};
    std::vector<MusicListInfo>m_koreaVector{};
    std::vector<MusicListInfo>m_japanVector{};
};


#endif //MUSICREPOSITORY_H
