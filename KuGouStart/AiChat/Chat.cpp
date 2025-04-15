#include "Chat.h"

#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

Chat::Chat()
{
    init();
}

void Chat::init()
{
    _manager = new QNetworkAccessManager();
    _request.setUrl(QUrl(_curl_https));

    // 头部信息
    _request.setRawHeader("Authorization", ("Bearer " + _api_key).toUtf8());
    _request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
}

void Chat::send(const QString& word)
{
    qDebug() << word;
    // messages部分
    QJsonArray messages;
    QJsonObject text;
    text["role"] = "user";
    text["content"] = word;
    messages.append(text);

    QJsonObject data;
    data["model"] = _model;
    data["messages"] = messages;
    data["stream"] = true;

    data["max_tokens"] = 5120;
    data["stop"] = QJsonArray({"null"});
    data["temperature"] = 0.7;
    data["top_p"] = 0.7;
    data["top_k"] = 50;
    data["frequency_penalty"] = 0.5;
    data["n"] = 1;
    data["response_format"] = QJsonObject{{"type", "text"}};


    QJsonArray tools;
    QJsonObject tool;
    tool["type"] = "function";
    QJsonObject function;
    function["description"] = ""; //Fill in if needed
    function["name"] = "";       //Fill in if needed
    function["parameters"] = QJsonObject{};
    function["strict"] = false;
    tool["function"] = function;
    tools.append(tool);
    data["tools"] = tools;

    QJsonDocument doc(data);
    QByteArray postData = doc.toJson();

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    _request.setSslConfiguration(config);

    //发送post请求
    _reply = _manager->post(_request, postData);

    connect(_reply, &QNetworkReply::readyRead, this, &Chat::processStream);
    connect(_reply, &QNetworkReply::finished, this, &Chat::finalizeStream);
    connect(_reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError code) {
        emit errorOccurred(_reply->errorString());
    });
}

void Chat::processStream() {
    if (!_reply || _reply->error() != QNetworkReply::NoError) return;

    m_buffer += _reply->readAll();

    // SSE格式解析
    while (true) {
        int endIdx = m_buffer.indexOf("\n\n");
        if (endIdx == -1) break;

        QByteArray eventData = m_buffer.left(endIdx).trimmed();
        m_buffer = m_buffer.mid(endIdx + 2);

        if (eventData.startsWith("data: ")) {
            QByteArray jsonData = eventData.mid(6);
            if (jsonData == "[DONE]") {
                emit streamFinished();
                break;
            }

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
            if (error.error != QJsonParseError::NoError) continue;

            QJsonObject obj = doc.object();
            QJsonArray choices = obj["choices"].toArray();
            if (choices.isEmpty()) continue;

            QJsonObject delta = choices[0].toObject()["delta"].toObject();
            QString content = delta["content"].toString();

            if (!content.isEmpty()) {
                emit answered(content);  // 实时发射每个内容片段
            }
        }
    }
}

void Chat::finalizeStream() {
    // 处理剩余数据
    processStream();

    // 最终清理
    if (_reply) {
        _reply->deleteLater();
        _reply = nullptr;
    }
    m_buffer.clear();
    emit streamFinished();
}