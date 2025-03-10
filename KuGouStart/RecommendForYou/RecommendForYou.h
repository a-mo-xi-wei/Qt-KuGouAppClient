//
// Created by WeiWang on 24-10-9.
//

#ifndef RECOMMENDFORYOU_H
#define RECOMMENDFORYOU_H

#include <QWidget>

#if defined(RECOMMENDFORYOU_LIBRARY)
#define RECOMMENDFORYOU_EXPORT Q_DECL_EXPORT
#else
#define RECOMMENDFORYOU_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class RecommendForYou; }
QT_END_NAMESPACE

class RECOMMENDFORYOU_EXPORT RecommendForYou : public QWidget {
Q_OBJECT

public:
    explicit RecommendForYou(QWidget *parent = nullptr);
    ~RecommendForYou() override;

private:
    void initAdvertiseBoard()const;

    void initClassifyWidget()const;

    void initTabWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::RecommendForYou *ui;
    QWidget* m_parent{};
};


#endif //RECOMMENDFORYOU_H
