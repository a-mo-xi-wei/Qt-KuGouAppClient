/**
 * @file Chat.cpp
 * @brief 实现 Chat 类，提供与硅基流动 API 的聊天交互功能
 * @author WeiWang
 * @date 2025-04-14
 * @version 1.0
 */

#include "Chat.h"

#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

/**
 * @brief 构造函数，初始化聊天对象
 */
Chat::Chat()
{
    init();                                              ///< 初始化网络请求
}

/**
 * @brief 初始化网络请求
 */
void Chat::init()
{
    _manager = new QNetworkAccessManager();              ///< 创建网络管理器
    _request.setUrl(QUrl(_curl_https));                  ///< 设置 API 地址
    _request.setRawHeader("Authorization", ("Bearer " + _api_key).toUtf8()); ///< 设置授权头
    _request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); ///< 设置内容类型
    qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString(); ///< 输出 SSL 版本
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl(); ///< 输出 OpenSSL 支持情况
}

/**
 * @brief 发送聊天请求
 * @param word 用户输入的问题
 */
void Chat::send(const QString &word)
{
    qDebug() << word;                                    ///< 输出问题
    QJsonArray messages;
    QJsonObject text;
    text["role"] = "user";                               ///< 设置用户角色
    text["content"] = word;                              ///< 设置问题内容
    messages.append(text);                               ///< 添加消息
    QJsonObject data;
    data["model"] = _model;                              ///< 设置模型
    data["messages"] = messages;                         ///< 设置消息数组
    data["stream"] = true;                               ///< 启用流式响应
    data["max_tokens"] = 5120;                           ///< 设置最大令牌数
    data["stop"] = QJsonArray({"null"});                 ///< 设置停止条件
    data["temperature"] = 0.7;                           ///< 设置温度
    data["top_p"] = 0.7;                                 ///< 设置 Top P
    data["top_k"] = 50;                                  ///< 设置 Top K
    data["frequency_penalty"] = 0.5;                     ///< 设置频率惩罚
    data["n"] = 1;                                       ///< 设置响应数量
    data["response_format"] = QJsonObject{{"type", "text"}}; ///< 设置响应格式
    QJsonArray tools;
    QJsonObject tool;
    tool["type"] = "function";                           ///< 设置工具类型
    QJsonObject function;
    function["description"] = "";                         ///< 工具描述（待填充）
    function["name"] = "";                               ///< 工具名称（待填充）
    function["parameters"] = QJsonObject{};              ///< 工具参数
    function["strict"] = false;                          ///< 设置非严格模式
    tool["function"] = function;
    tools.append(tool);
    data["tools"] = tools;                               ///< 设置工具数组
    QJsonDocument doc(data);
    QByteArray postData = doc.toJson();                  ///< 转换为 JSON
    QSslConfiguration config = QSslConfiguration::defaultConfiguration(); ///< 获取默认 SSL 配置
    config.setProtocol(QSsl::AnyProtocol);               ///< 设置任意协议
    config.setPeerVerifyMode(QSslSocket::VerifyNone);    ///< 禁用对等验证
    _request.setSslConfiguration(config);                ///< 设置 SSL 配置
    _reply = _manager->post(_request, postData);         ///< 发送 POST 请求
    connect(_reply, &QNetworkReply::readyRead, this, &Chat::processStream); ///< 连接流式数据信号
    connect(_reply, &QNetworkReply::finished, this, &Chat::finalizeStream); ///< 连接结束信号
    connect(_reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError code) {
        emit errorOccurred(_reply->errorString());       ///< 发射错误信号
    });
}

/**
 * @brief 处理流式响应数据
 */
void Chat::processStream()
{
    if (!_reply || _reply->error() != QNetworkReply::NoError)
        return;                                          ///< 检查回复有效性
    m_buffer += _reply->readAll();                       ///< 累积数据
    while (true)
    {
        int endIdx = m_buffer.indexOf("\n\n");           ///< 查找数据分隔
        if (endIdx == -1)
            break;
        QByteArray eventData = m_buffer.left(endIdx).trimmed(); ///< 提取事件数据
        m_buffer = m_buffer.mid(endIdx + 2);             ///< 更新缓冲区
        if (eventData.startsWith("data: "))
        {
            QByteArray jsonData = eventData.mid(6);      ///< 提取 JSON 数据
            if (jsonData == "[DONE]")
            {
                emit streamFinished();                   ///< 发射结束信号
                break;
            }
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error); ///< 解析 JSON
            if (error.error != QJsonParseError::NoError)
                continue;
            QJsonObject obj = doc.object();
            QJsonArray choices = obj["choices"].toArray(); ///< 获取选项
            if (choices.isEmpty())
                continue;
            QJsonObject delta = choices[0].toObject()["delta"].toObject(); ///< 获取增量
            QString content = delta["content"].toString(); ///< 获取内容
            if (!content.isEmpty())
            {
                emit answered(content);                  ///< 发射回答信号
            }
        }
    }
}

/**
 * @brief 结束流式响应
 */
void Chat::finalizeStream()
{
    processStream();                                     ///< 处理剩余数据
    if (_reply)
    {
        _reply->deleteLater();                           ///< 安全删除回复
        _reply = nullptr;
    }
    m_buffer.clear();                                    ///< 清空缓冲区
    emit streamFinished();                               ///< 发射结束信号
}