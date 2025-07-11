/**
* @file RecommendForYou.h
 * @brief 推荐界面类的头文件，定义 RecommendForYou 类
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef RECOMMENDFORYOU_H
#define RECOMMENDFORYOU_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RecommendForYou; } ///< 前向声明 UI 类
QT_END_NAMESPACE

/**
 * @class RecommendForYou
 * @brief 推荐界面类，管理广告轮播、分类按钮和推荐表格
 */
class RecommendForYou : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RecommendForYou(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     * @note 释放 UI 资源
     */
    ~RecommendForYou() override;

private:
    /**
     * @brief 初始化广告轮播
     * @note 加载海报图片并添加到广告板
     */
    void initAdvertiseBoard() const;

    /**
     * @brief 初始化分类按钮
     * @note 设置分类按钮的图标和动态大小
     */
    void initClassifyWidget() const;

    /**
     * @brief 初始化推荐表格
     * @note 添加推荐表格到布局
     */
    void initTabWidget();

private:
    Ui::RecommendForYou *ui; ///< UI 界面指针
};

#endif // RECOMMENDFORYOU_H