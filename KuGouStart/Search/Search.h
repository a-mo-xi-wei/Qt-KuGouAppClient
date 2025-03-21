//
// Created by WeiWang on 24-11-18.
//

#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>

#if defined(SEARCH_LIBRARY)
#define SEARCH_EXPORT Q_DECL_EXPORT
#else
#define SEARCH_EXPORT Q_DECL_IMPORT
#endif

class QButtonGroup;
class QPushButton;

QT_BEGIN_NAMESPACE

namespace Ui {
    class Search;
}

QT_END_NAMESPACE

class SEARCH_EXPORT Search : public QWidget {
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);

    ~Search() override;
private:
    void initUi();
    // 初始化堆栈窗口
    void initStackWidget();
    //初始化封面库
    void initCoverVector();
    //初始化描述库
    void initDescVector();

    void refresh();

    void enableButton(const bool& flag) const;

protected:
    void resizeEvent(QResizeEvent *event) override;

    void showEvent(QShowEvent *event) override;

private slots:
    void on_recommend_pushButton_clicked();

    void on_rank_pushButton_clicked();

    void on_special_pushButton_clicked();

    void on_channel_pushButton_clicked();
private:
    Ui::Search *ui;
    QWidget* m_parent{};
    QPushButton* m_currentBtn{};
    std::unique_ptr<QButtonGroup> m_buttonGroup;
    std::vector<QPixmap> m_coverVector;//存放图片
    std::vector<QString> m_descVector;//存放描述
    //四个堆栈窗口
    std::unique_ptr<QWidget>m_recommendWidget{};
    std::unique_ptr<QWidget>m_rankWidget{};
    std::unique_ptr<QWidget>m_specialWidget{};
    std::unique_ptr<QWidget>m_channelWidget{};
};


#endif //SEARCH_H
