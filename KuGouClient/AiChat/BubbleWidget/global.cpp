/**
* @file global.cpp
 * @brief 实现全局变量和函数
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "global.h"
#include <QStyle>

/**
 * @brief 刷新 QSS 样式的函数对象
 */
std::function<void(QWidget*)> repolish = [](QWidget *w) {
    w->style()->unpolish(w);                            ///< 取消样式
    w->style()->polish(w);                              ///< 应用样式
};

/**
 * @brief 字符串异或加密/解密的函数对象
 */
std::function<QString(QString)> xorString = [](QString input) {
    QString result = input;                              ///< 复制输入字符串
    int length = input.length();                        ///< 获取字符串长度
    ushort xor_code = length % 255;                     ///< 计算异或码
    for (int i = 0; i < length; ++i)
    {
        result[i] = QChar(static_cast<ushort>(input[i].unicode() ^ xor_code)); ///< 异或操作
    }
    return result;                                      ///< 返回结果
};

/**
 * @brief 网关 URL 前缀
 */
QString gate_url_prefix = "";