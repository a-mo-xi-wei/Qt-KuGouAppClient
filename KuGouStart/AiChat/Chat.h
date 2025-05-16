/**
 * @file Chat.h
 * @brief 定义 Chat 类，提供与硅基流动 API 的聊天交互
 * @author WeiWang
 * @date 2025-04-14
 * @version 1.0
 */

#ifndef CHAT_H
#define CHAT_H

#include <QNetworkAccessManager>


//API手册地址 https://docs.siliconflow.cn/cn/api-reference/chat-completions/chat-completions

/*
curl --request POST \
  --url https://api.siliconflow.cn/v1/chat/completions \
  --header 'Authorization: Bearer <token>' \
  --header 'Content-Type: application/json' \
  --data '{
  "model": "Qwen/QwQ-32B",
  "messages": [
    {
      "role": "user",
      "content": "What opportunities and challenges will the Chinese large model industry face in 2025?"
    }
  ],
  "stream": false,
  "max_tokens": 512,
  "stop": null,
  "temperature": 0.7,
  "top_p": 0.7,
  "top_k": 50,
  "frequency_penalty": 0.5,
  "n": 1,
  "response_format": {
    "type": "text"
  },
  "tools": [
    {
      "type": "function",
      "function": {
        "description": "<string>",
        "name": "<string>",
        "parameters": {},
        "strict": false
      }
    }
  ]
}'
*/

/**
 * @class Chat
 * @brief 聊天类，与硅基流动 API 交互，支持流式响应
 * @note API 手册：https://docs.siliconflow.cn/cn/api-reference/chat-completions/chat-completions
 */

class Chat final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化聊天对象
     */
    Chat();

    /**
     * @brief 发送聊天请求
     * @param word 用户输入的问题
     */
    void send(const QString &word);

private:
    /**
     * @brief 初始化网络请求
     */
    void init();

private slots:
    /**
     * @brief 处理流式响应数据
     */
    void processStream();

    /**
     * @brief 结束流式响应
     */
    void finalizeStream();

private:
    QNetworkRequest      _request;       ///< 网络请求对象
    QNetworkAccessManager *_manager;     ///< 网络管理器
    QNetworkReply        *_reply;        ///< 网络响应
    QString              _curl_https    = "https://api.siliconflow.cn/v1/chat/completions";    ///< API 地址
    QString              _model         = "deepseek-ai/DeepSeek-V3";                           ///< 模型名称
    QString              _api_key       = "Your API Key"; ///< API 密钥,可参考 https://gitee.com/a-mo-xi-wei/qt6-access-deepseek#%E4%BB%8B%E7%BB%8D
    QString              _read_text;     ///< 读取文本
    QByteArray           m_buffer;       ///< 数据缓冲区

signals:
    /**
     * @brief 接收到回答信号
     * @param word 回答内容
     */
    void answered(const QString &word);

    /**
     * @brief 流式响应结束信号
     */
    void streamFinished();

    /**
     * @brief 错误发生信号
     * @param error 错误信息
     */
    void errorOccurred(const QString &error);
};

#endif // CHAT_H