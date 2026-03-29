#ifndef MQTTCLIENT_H_
#define MQTTCLIENT_H_

#include <QMqttClient>
#include <QObject>

#if defined(COMMON_LIBRARY)
#define COMMON_EXPORT Q_DECL_EXPORT
#else
#define COMMON_EXPORT Q_DECL_IMPORT
#endif

class COMMON_EXPORT MqttClient : public QObject
{
	Q_OBJECT

public:
	explicit MqttClient(QObject* parent = nullptr);
	~MqttClient();

	// 连接/断开连接
	void init();
	void connectToBroker(const QString& clientId, const QString& userName, const QString& password);
	void disconnectFromBroker();
	void disconnect();
	// 订阅/取消订阅
	void subscribe(const QString& topic, quint8 qos = 0);
	void unsubscribe(const QString& topic);

	// 发布消息
	void publishCmd(const QString& topic, const QByteArray& message, quint8 qos = 1, bool retain = false);

	// 状态查询
	bool isConnected() const;
	QMqttClient::ClientState state() const;

private:
	void initSubscribe();

signals:
	void MqttMessageReceived(const QString& message);

private slots:
	void onStateChanged(QMqttClient::ClientState state);
	void onMessageReceived(const QByteArray& message, const QMqttTopicName& topic);

private:
	QMqttClient* m_mqttClient;
	QString m_productId;
	QString m_deviceName;
	QString m_messagePrefix;
	QString m_selfEventMessage;
};

#endif // MQTTCLIENT_H_
