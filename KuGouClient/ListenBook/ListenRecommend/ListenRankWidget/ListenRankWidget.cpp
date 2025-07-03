/**
 * @file ListenRankWidget.cpp
 * @brief 实现 ListenRankWidget 类，提供排行榜界面功能
 * @author WeiWang
 * @date 2025-02-08
 * @version 1.0
 */

#include "ListenRankWidget.h"
#include "logger.hpp"
#include "Async.h"

#include <QHBoxLayout>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化排行榜界面
 * @param parent 父控件指针，默认为 nullptr
 */
ListenRankWidget::ListenRankWidget(QWidget *parent)
    : QWidget(parent)
    , rank_free(new RankPartWidget(this))
    , rank_skyrocket(new RankPartWidget(this))
    , rank_sell(new RankPartWidget(this))
    , rank_new(new RankPartWidget(this))
{
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 初始化界面
 * @note 设置水平布局并添加榜单控件
 */
void ListenRankWidget::initUi()
{
    const auto lay = new QHBoxLayout(this);              ///< 创建水平布局
    lay->setContentsMargins(10, 0, 10, 10);              ///< 设置布局边距
    lay->addWidget(this->rank_free);                     ///< 添加免费榜控件
    lay->addWidget(this->rank_skyrocket);                ///< 添加飙升榜控件
    lay->addWidget(this->rank_sell);                     ///< 添加热销榜控件
    lay->addWidget(this->rank_new);                      ///< 添加新品榜控件
    rank_new->hide();                                    ///< 初始隐藏新品榜
    this->rank_free->setTitle(" 免费榜 ›");              ///< 设置免费榜标题
    this->rank_skyrocket->setTitle(" 飙升榜 ›");         ///< 设置飙升榜标题
    this->rank_sell->setTitle(" 热销榜 ›");              ///< 设置热销榜标题
    this->rank_new->setTitle(" 新品榜 ›");               ///< 设置新品榜标题
    initRankFree();                                      ///< 初始化免费榜
    initRankSkyrocket();                                 ///< 初始化飙升榜
    initRankSell();                                      ///< 初始化热销榜
    initRankNew();                                       ///< 初始化新品榜
}

/**
 * @brief 初始化免费榜
 * @note 异步加载 JSON 数据并设置榜单内容
 */
void ListenRankWidget::initRankFree()
{
    const auto future = Async::runAsync(QThreadPool::globalInstance(), []() {
        QList<QJsonObject> data;                         ///< 数据列表
        QFile file(GET_CURRENT_DIR + QStringLiteral("/../jsonFiles/rank-free.json")); ///< 加载 JSON 文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading rank-free.json";
            STREAM_WARN() << "Could not open file for reading rank-free.json"; ///< 记录警告日志
            return data;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        auto arr = obj.array();
        for (const auto &item : arr)
        {
            data.append(item.toObject());                ///< 添加数据项
        }
        file.close();
        return data;
    });

    Async::onResultReady(future, this, [this](const QList<QJsonObject> &data) {
        if (data.isEmpty())
        {
            qWarning() << "rank-free.json is empty or failed to parse";
            STREAM_WARN() << "rank-free.json is empty or failed to parse"; ///< 记录警告日志
            return;
        }
        const QString pathArr[] = {
            ":/ListenBook/Res/listenbook/first.svg",
            ":/ListenBook/Res/listenbook/second.svg",
            ":/ListenBook/Res/listenbook/third.svg"
        };                                           ///< 奖牌图标路径
        for (int i = 0; i < 5; ++i)
        {
            QString desc = data[i].value("desc").toString(); ///< 获取描述
            this->rank_free->getRankListWidget(i)->setDescText(desc); ///< 设置描述文本
            this->rank_free->getRankListWidget(i)->setCoverImg(
                QString(":/ListCover/Res/listcover/music-list-cover%1.jpg").arg(20 + i)); ///< 设置封面图片
            if (i < 3)
                this->rank_free->getRankListWidget(i)->setRankMedal(pathArr[i]); ///< 设置前三名奖牌
            else
                this->rank_free->getRankListWidget(i)->setRankNumber(QString("%1").arg(i + 1)); ///< 设置排名数字
        }
    });
}

/**
 * @brief 初始化飙升榜
 * @note 异步加载 JSON 数据并设置榜单内容
 */
void ListenRankWidget::initRankSkyrocket()
{
    const auto future = Async::runAsync(QThreadPool::globalInstance(), []() {
        QList<QJsonObject> data;                         ///< 数据列表
        QFile file(GET_CURRENT_DIR + QStringLiteral("/../jsonFiles/rank-skyrocket.json")); ///< 加载 JSON 文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading rank-skyrocket.json";
            STREAM_WARN() << "Could not open file for reading rank-skyrocket.json"; ///< 记录警告日志
            return data;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        const auto arr = obj.array();
        for (const auto &item : arr)
        {
            data.append(item.toObject());                ///< 添加数据项
        }
        file.close();
        return data;
    });

    Async::onResultReady(future, this, [this](const QList<QJsonObject> &data) {
        if (data.isEmpty())
        {
            qWarning() << "rank-skyrocket.json is empty or failed to parse";
            STREAM_WARN() << "rank-skyrocket.json is empty or failed to parse"; ///< 记录警告日志
            return;
        }
        const QString pathArr[] = {
            ":/ListenBook/Res/listenbook/first.svg",
            ":/ListenBook/Res/listenbook/second.svg",
            ":/ListenBook/Res/listenbook/third.svg"
        };                                           ///< 奖牌图标路径
        for (int i = 0; i < 5; ++i)
        {
            QString desc = data[i].value("desc").toString(); ///< 获取描述
            this->rank_skyrocket->getRankListWidget(i)->setDescText(desc); ///< 设置描述文本
            this->rank_skyrocket->getRankListWidget(i)->setCoverImg(
                QString(":/ListCover/Res/listcover/music-list-cover%1.jpg").arg(30 + i)); ///< 设置封面图片
            if (i < 3)
                this->rank_skyrocket->getRankListWidget(i)->setRankMedal(pathArr[i]); ///< 设置前三名奖牌
            else
                this->rank_skyrocket->getRankListWidget(i)->setRankNumber(QString("%1").arg(i + 1)); ///< 设置排名数字
        }
    });
}

/**
 * @brief 初始化热销榜
 * @note 异步加载 JSON 数据并设置榜单内容
 */
void ListenRankWidget::initRankSell()
{
    const auto future = Async::runAsync(QThreadPool::globalInstance(), []() {
        QList<QJsonObject> data;                         ///< 数据列表
        QFile file(GET_CURRENT_DIR + QStringLiteral("/../jsonFiles/rank-sell.json")); ///< 加载 JSON 文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading rank-sell.json";
            STREAM_WARN() << "Could not open file for reading rank-sell.json"; ///< 记录警告日志
            return data;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        const auto arr = obj.array();
        for (const auto &item : arr)
        {
            data.append(item.toObject());                ///< 添加数据项
        }
        file.close();
        return data;
    });

    Async::onResultReady(future, this, [this](const QList<QJsonObject> &data) {
        if (data.isEmpty())
        {
            qWarning() << "rank-sell.json is empty or failed to parse";
            STREAM_WARN() << "rank-sell.json is empty or failed to parse"; ///< 记录警告日志
            return;
        }
        const QString pathArr[] = {
            ":/ListenBook/Res/listenbook/first.svg",
            ":/ListenBook/Res/listenbook/second.svg",
            ":/ListenBook/Res/listenbook/third.svg"
        };                                           ///< 奖牌图标路径
        for (int i = 0; i < 5; ++i)
        {
            QString desc = data[i].value("desc").toString(); ///< 获取描述
            this->rank_sell->getRankListWidget(i)->setDescText(desc); ///< 设置描述文本
            this->rank_sell->getRankListWidget(i)->setCoverImg(
                QString(":/ListCover/Res/listcover/music-list-cover%1.jpg").arg(40 + i)); ///< 设置封面图片
            if (i < 3)
                this->rank_sell->getRankListWidget(i)->setRankMedal(pathArr[i]); ///< 设置前三名奖牌
            else
                this->rank_sell->getRankListWidget(i)->setRankNumber(QString("%1").arg(i + 1)); ///< 设置排名数字
        }
    });
}

/**
 * @brief 初始化新品榜
 * @note 异步加载 JSON 数据并设置榜单内容
 * @warning 当前实现使用了 rank-sell.json，可能应为 rank-new.json
 */
void ListenRankWidget::initRankNew()
{
    const auto future = Async::runAsync(QThreadPool::globalInstance(), []() {
        QList<QJsonObject> data;                         ///< 数据列表
        QFile file(GET_CURRENT_DIR + QStringLiteral("/../jsonFiles/rank-sell.json")); ///< 加载 JSON 文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading rank-sell.json";
            STREAM_WARN() << "Could not open file for reading rank-sell.json"; ///< 记录警告日志
            return data;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        const auto arr = obj.array();
        for (const auto &item : arr)
        {
            data.append(item.toObject());                ///< 添加数据项
        }
        file.close();
        return data;
    });

    Async::onResultReady(future, this, [this](const QList<QJsonObject> &data) {
        if (data.isEmpty())
        {
            qWarning() << "rank-sell.json (initRankNew) is empty or failed to parse";
            STREAM_WARN() << "rank-sell.json (initRankNew) is empty or failed to parse"; ///< 记录警告日志
            return;
        }
        if (data.size() < 10)
        {
            qWarning() << "Insufficient data items in rank-sell.json for initRankNew";
            return;
        }
        const QString pathArr[] = {
            ":/ListenBook/Res/listenbook/first.svg",
            ":/ListenBook/Res/listenbook/second.svg",
            ":/ListenBook/Res/listenbook/third.svg"
        };                                           ///< 奖牌图标路径
        for (int i = 0; i < 5; ++i)
        {
            QString desc = data[i + 5].value("desc").toString(); ///< 获取描述（偏移 5 项）
            this->rank_new->getRankListWidget(i)->setDescText(desc); ///< 设置描述文本
            this->rank_new->getRankListWidget(i)->setCoverImg(
                QString(":/ListCover/Res/listcover/music-list-cover%1.jpg").arg(50 + i)); ///< 设置封面图片
            if (i < 3)
                this->rank_new->getRankListWidget(i)->setRankMedal(pathArr[i]); ///< 设置前三名奖牌
            else
                this->rank_new->getRankListWidget(i)->setRankNumber(QString("%1").arg(i + 1)); ///< 设置排名数字
        }
    });
}

/**
 * @brief 调整大小事件，动态显示或隐藏新品榜
 * @param event 调整大小事件
 */
void ListenRankWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类调整大小事件
    if (this->parentWidget()->width() > 1100)
    {
        this->rank_new->show();                          ///< 显示新品榜
    }
    else
    {
        this->rank_new->hide();                          ///< 隐藏新品榜
    }
}