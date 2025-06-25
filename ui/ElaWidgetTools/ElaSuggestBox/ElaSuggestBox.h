/**
 * @file ElaSuggestBox.h
 * @brief 定义 ElaSuggestBox 类，搜索建议框控件
 * @author WeiWang
 * @date 2025-06-25
 * @version 1.0
 */

#ifndef ELASUGGESTBOX_H
#define ELASUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "Def.h"
#include "stdafx.h"

class ElaSuggestBoxPrivate;
class ElaLineEdit;

/**
 * @class ElaSuggestBox
 * @brief 搜索建议框控件，支持添加/移除建议项和事件过滤
 */
class ELA_EXPORT ElaSuggestBox : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSuggestBox)                                ///< 声明私有类指针
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)                 ///< 边框圆角半径
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity) ///< 搜索大小写敏感性

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaSuggestBox(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaSuggestBox() override;

    /**
     * @brief 设置占位文本
     * @param placeholderText 占位文本
     */
    void setPlaceholderText(const QString &placeholderText);

    /**
     * @brief 添加建议项
     * @param suggestText 建议文本
     * @param suggestData 建议数据
     * @return 建议项唯一键
     */
    QString addSuggestion(const QString &suggestText, const QVariantMap &suggestData = {});

    /**
     * @brief 添加带图标的建议项
     * @param icon 图标
     * @param suggestText 建议文本
     * @param suggestData 建议数据
     * @return 建议项唯一键
     */
    QString addSuggestion(ElaIconType::IconName icon, const QString &suggestText, const QVariantMap &suggestData = {});

    /**
     * @brief 移除建议项
     * @param suggestKey 建议项唯一键
     */
    void removeSuggestion(const QString &suggestKey);

    /**
     * @brief 移除指定索引的建议项
     * @param index 索引
     */
    void removeSuggestion(int index);

    /**
     * @brief 移除所有建议项
     */
    void removeAllSuggestion();

    /**
     * @brief 设置搜索编辑框
     * @param lineEdit 编辑框指针
     * @note 替换默认编辑框
     */
    void setLineEdit(ElaLineEdit* lineEdit);

    /**
     * @brief 移除默认尾部动作
     */
    void removeDefaultTrailAction();

    /**
     * @brief 建议框位置变化
     * @note 取消编辑框焦点
     */
    void suggestBoxPositionChanged();

protected:
    /**
     * @brief 事件过滤
     * @param watched 监控对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理点击外部关闭建议框
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
    /**
     * @brief 建议项点击信号
     * @param suggestText 建议文本
     * @param suggestData 建议数据
     */
    Q_SIGNAL void suggestionClicked(const QString& suggestText, const QVariantMap& suggestData);
};

#endif // ELASUGGESTBOX_H