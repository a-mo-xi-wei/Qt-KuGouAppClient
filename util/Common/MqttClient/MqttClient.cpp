#include "MqttClient.h"
#include <QJsonOBject>
#include <QJsonDocument>

#include <QSslSocket>
#include <QFile>

MqttClient::MqttClient(QObject* parent)
	: QObject(parent)
{

	m_mqttClient = nullptr;
	m_mqttClient = new QMqttClient;
}

MqttClient::~MqttClient()
{
	disconnectFromBroker();
	delete m_mqttClient;
}

void MqttClient::init()
{
	this->connectToBroker("host", "KuGouClient", "ww192837465");

	connect(m_mqttClient, &QMqttClient::connected, this, [=]
		{
			initSubscribe();
			// 连接信号槽
			connect(m_mqttClient, &QMqttClient::stateChanged, this, &MqttClient::onStateChanged);
			connect(m_mqttClient, &QMqttClient::messageReceived, this, &MqttClient::onMessageReceived);
		});

}

//连接Broker
void MqttClient::connectToBroker(const QString& clientId, const QString& userName, const QString& password)
{

	// OneNET 的固定连接信息
	m_mqttClient->setHostname("vab112f6.ala.cn-hangzhou.emqxsl.cn");
	m_mqttClient->setPort(8883);

	m_mqttClient->setClientId(clientId);
	m_mqttClient->setUsername(userName);
	m_mqttClient->setPassword(password);
	m_mqttClient->setProtocolVersion(QMqttClient::MQTT_5_0);

	m_mqttClient->setKeepAlive(60);
	m_mqttClient->setCleanSession(true);

	// 配置 SSL
	QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
	sslConfig.setProtocol(QSsl::TlsV1_2OrLater); // 强制 TLS 1.2+
	sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer); // 验证证书

	// 发起加密连接
	m_mqttClient->connectToHostEncrypted(sslConfig);

}

//Broker断开连接
void MqttClient::disconnectFromBroker()
{
	if (m_mqttClient->state() != QMqttClient::Disconnected)
	{
		m_mqttClient->disconnectFromHost();
	}
}
//断开和Broker的连接
void MqttClient::disconnect()
{
	if (m_mqttClient->state() != QMqttClient::Disconnected)
	{
		m_mqttClient->disconnect();
	}
}

bool MqttClient::isConnected() const
{
	return m_mqttClient->state() == QMqttClient::Connected;
}

QMqttClient::ClientState MqttClient::state() const
{
	return m_mqttClient->state();
}

void MqttClient::onStateChanged(QMqttClient::ClientState state)
{
	switch (state)
	{
	case QMqttClient::Connected:
		initSubscribe();
		break;
	case QMqttClient::Disconnected:

		break;
	default:
		break;
	}
}

//订阅初始化
void MqttClient::initSubscribe()
{
	this->subscribe("TransmitData", 0);
}

//主题订阅
void MqttClient::subscribe(const QString& topic, quint8 qos)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		auto subscription = m_mqttClient->subscribe(topic, qos);
		if (!subscription)
		{
			qWarning() << "Failed to subscribe to topic:" << topic;
		}
	}
	else
	{
		qWarning() << "Cannot subscribe - client not connected";
	}
}

//取消订阅
void MqttClient::unsubscribe(const QString& topic)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		m_mqttClient->unsubscribe(topic);
	}
}

//信息发布
void MqttClient::publishCmd(const QString& topic, const QByteArray& message, quint8 qos, bool retain)
{
	if (m_mqttClient->state() == QMqttClient::Connected)
	{
		qint32 id = m_mqttClient->publish(topic, message, qos, retain);
		qDebug() << "public:" << topic << message;
		if (id < 0)
		{
			qWarning() << "Failed to publish message to topic:" << topic;
		}
		else
		{

		}
	}
	else
	{
		qWarning() << "Cannot publish - client not connected";
	}
}

//信息接收
void MqttClient::onMessageReceived(const QByteArray& message, const QMqttTopicName& topic)
{
	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(message, &err);
	if (err.error != QJsonParseError::NoError)
	{
		qWarning() << "JSON error:" << err.errorString();
		return;
	}
	if (topic.name() == "TransmitData")
	{
		QJsonObject obj = doc.object();
		if (obj.contains("msg")) {
			QString command = obj["msg"].toString();
			qDebug() << "Extracted command:" << command;
			emit MqttMessageReceived(command);
		}
	}
}