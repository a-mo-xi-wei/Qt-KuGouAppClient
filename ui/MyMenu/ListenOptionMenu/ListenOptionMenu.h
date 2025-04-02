//
// Created by WeiWang on 25-2-3.
//

#ifndef LISTENOPTIONMENU_H
#define LISTENOPTIONMENU_H

#include "../BaseMenu.h"

#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

class QFrame;

class MYMENU_EXPORT ListenOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    explicit ListenOptionMenu(QWidget *parent = nullptr);

private:
    void initMenu() override;

    //初始化各个窗口
    void initAudioNovelWidget(QWidget* widget);

    void initChildrenWorldWidget(QWidget* widget);

    void initCommentBookWidget(QWidget* widget);

    void initSleepHelpingWidget(QWidget* widget);

    void initHumanityWidget(QWidget* widget);

    void initSelfChargingWidget(QWidget* widget);

    void initCrosstalkWidget(QWidget* widget);

    void initSentimentalityWidget(QWidget* widget);

    void initRadioDramaWidget(QWidget* widget);

    void initEntertainmentJokesWidget(QWidget* widget);

    void initACGNWidget(QWidget* widget);

    void initPodcastWidget(QWidget* widget);

    void initCantoneseWidget(QWidget* widget);

    void initForeignLanguageWidget(QWidget* widget);

    void initCreateCoverWidget(QWidget* widget);

    void initElectronicSoundWidget(QWidget* widget);

    //初始化水平分割线
    void initSeparator(QFrame* frame);

    //重写获取菜单
    const ListenOptionMenu* getMenu()const override;
};

#endif //LISTENOPTIONMENU_H
