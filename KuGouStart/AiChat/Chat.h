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
class Chat final : public QObject {
    Q_OBJECT
public:
    Chat();

    void send(const QString &word);

private:
    void init();

private slots:
    void processStream();

    void finalizeStream();

private:
    QNetworkRequest _request; // 封装网络请求信息
    QNetworkAccessManager *_manager; // 管理网络请求和响应
    QNetworkReply *_reply; // 网络请求响应

    QString _curl_https = "https://api.siliconflow.cn/v1/chat/completions";
    QString _model = "deepseek-ai/DeepSeek-V3";
    QString _api_key = "sk-nfryarnkivaypbbtbzttkpboxcsfxixxqquyasiuuvdclihi";//填入自己的硅基流动的API,参考 https://gitee.com/a-mo-xi-wei/qt6-access-deepseek#%E4%BB%8B%E7%BB%8D
    QString _read_text;

    QByteArray m_buffer; // 用于累积接收的数据

signals:
    void answered(const QString& word);
    void streamFinished();
    void errorOccurred(const QString& error);  // 新增错误信号
};

#endif // CHAT_H
