/**
* @file global.h
 * @brief 定义全局变量、枚举和结构体
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>

/**
 * @brief 刷新 QSS 样式的函数对象
 */
extern std::function<void(QWidget*)> repolish;

/**
 * @brief 字符串异或加密/解密的函数对象
 */
extern std::function<QString(QString)> xorString;

/**
 * @brief 网关 URL 前缀
 */
extern QString gate_url_prefix;

/**
 * @enum ChatRole
 * @brief 聊天角色枚举
 */
enum class ChatRole
{
    Self,   ///< 自己
    Time,   ///< 时间标签
    Other   ///< 他人
};

/**
 * @struct MsgInfo
 * @brief 消息信息结构体
 */
struct MsgInfo
{
    QString msgFlag;   ///< 消息类型（text, image, file）
    QString content;   ///< 消息内容（文本或 URL）
    QPixmap pixmap;    ///< 缩略图（文件或图片）
};

#endif // GLOBAL_H