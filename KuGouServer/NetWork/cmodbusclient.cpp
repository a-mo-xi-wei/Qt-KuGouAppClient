#include "../../includes/network/cmodbusclient.h"

#include <QDateTime>
#include <QModbusRtuSerialMaster>
#include <QModbusTcpClient>

CModbusClient::CModbusClient(QObject *parent,ModbusConnection connection)
    : QObject(parent),
      m_modbusClient(NULL),
      m_ModbusConnection(connection),
      m_modbuslastRequest(NULL),
      m_responseTimeout(1000),
      m_NumberOfRetries(3),
      m_isClientStartupSuccessed(false),
      m_ServerAddress(-1),
      m_isClientExit(false)
{
    createModbusClient();
}

CModbusClient::~CModbusClient()
{
    m_isClientExit = true;

    if (m_modbusClient)
    {
        m_modbusClient->disconnectDevice();
        m_modbusClient->deleteLater();
    }
}

/**
 * @brief CModbusClient::createModbusClient 建立Modbus客户端
 */
void CModbusClient::createModbusClient(void)
{
    if (m_modbusClient)
    {
        m_modbusClient->disconnectDevice();
        m_modbusClient->deleteLater();
    }

    if (m_ModbusConnection == Modbus_Serial)
    {
#if QT_CONFIG(modbus_serialport)
        m_modbusClient = new QModbusRtuSerialMaster(this);
#endif
    }
    else if (m_ModbusConnection == Modbus_Tcp)
    {
        m_modbusClient = new QModbusTcpClient(this);
    }

    if(m_modbusClient == NULL) return;

    connect(m_modbusClient,&QModbusClient::stateChanged,
            this,&CModbusClient::onStateChanged);
    connect(m_modbusClient,&QModbusClient::errorOccurred,
            this,&CModbusClient::onErrorOccurred);
}

void CModbusClient::onStateChanged(int state)
{
    m_currentConnectingState = state;

    if (state == QModbusDevice::UnconnectedState)
    {
        m_isClientStartupSuccessed = false;

        printLog(QsLogging::Level::InfoLevel,
                 tr("CModbusClient::onStateChanged:") +
                 QString::fromLocal8Bit("与服务器断开连接."));
    }
    else if (state == QModbusDevice::ConnectedState)
    {
        printLog(QsLogging::Level::InfoLevel,
                 tr("CModbusClient::onStateChanged:") +
                 QString::fromLocal8Bit("与服务器建立连接成功."));
    }

    if(!m_isClientExit)
        emit signalStateChanged(state);
}

void CModbusClient::onErrorOccurred(QModbusDevice::Error newError)
{
    if (newError == QModbusDevice::NoError ||
        m_modbusClient == NULL)
        return;

    printLog(QsLogging::Level::ErrorLevel,
             tr("CModbusClient::onErrorOccurred:") + m_modbusClient->errorString());
}

/**
 * @brief CModbusClient::printLog 打印用户日志
 * @param type
 * @param msg
 */
void CModbusClient::printLog(QsLogging::Level type,QString msg)
{
    switch (type)
    {
    case QsLogging::Level::InfoLevel:
        QLOG_INFO()<<msg;
        break;
    case QsLogging::Level::WarnLevel:
        QLOG_WARN()<<msg;
        break;
    case QsLogging::Level::ErrorLevel:
        QLOG_ERROR()<<msg;
        break;
    case QsLogging::Level::FatalLevel:
        QLOG_FATAL()<<msg;
        break;
    default:
        break;
    }

    emit signalshowlog(getCurrentDate()+
                  //"<b>MODBUSSER-</b>"+
                  msg);
}

/**
 * @brief CModbusClient::getCurrentDate 得到当前时间
 * @return
 */
QString CModbusClient::getCurrentDate(void)
{
    QDateTime dateTime =QDateTime::currentDateTime();
    return dateTime.toString("<b>[yyyy-MM-dd hh:mm:ss]</b> ");
}

/**
 * @brief CModbusClient::closeClient 关闭客户端
 */
void CModbusClient::closeClient(void)
{
    m_isClientStartupSuccessed = false;

    if (m_modbusClient)
        m_modbusClient->disconnectDevice();
}

#if QT_CONFIG(modbus_serialport)
/**
 * @brief CModbusClient::startupSerialPort 启动串口
 * @param port
 * @param parity
 * @param baud
 * @param dataBits
 * @param stopBits
 * @return
 */
bool CModbusClient::startupSerialPort(QString port,
                       int parity,
                       int baud,
                       int dataBits,
                       int stopBits)
{
    if (m_modbusClient == NULL ||
        m_modbusClient->state() == QModbusDevice::ConnectedState)
        return false;

    bool isOk = true;

    m_modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
        port);

    m_modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter,
        parity);
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
        baud);
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
        dataBits);
    m_modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
        stopBits);

    m_modbusClient->setTimeout(m_responseTimeout);
    m_modbusClient->setNumberOfRetries(m_NumberOfRetries);

    if (!m_modbusClient->connectDevice()) {
        isOk = false;
        printLog(QsLogging::Level::ErrorLevel,
                 tr("CModbusClient::startupSerialPort fail:") + m_modbusClient->errorString());
    }

    printLog(QsLogging::Level::InfoLevel,
             QString::asprintf("CModbusClient::startupSerialPort succesed,port:%d;",port)+
             QString::asprintf("Parity:%d BaudRate:%d DataBits:%d StopBits:%d",parity,baud,dataBits,stopBits));

    m_isClientStartupSuccessed = isOk;

    return isOk;
}
#endif

/**
 * @brief CModbusClient::startupTCP 启动TCP
 * @param ip
 * @param port
 * @return
 */
bool CModbusClient::startupTCP(QString ip,int port)
{
    if (m_modbusClient == NULL ||
        m_modbusClient->state() == QModbusDevice::ConnectedState ||
        (ip == "" || port <= 0))
        return false;

    bool isOk = true;

    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);

    m_modbusClient->setTimeout(m_responseTimeout);
    m_modbusClient->setNumberOfRetries(m_NumberOfRetries);

    if (!m_modbusClient->connectDevice()) {
        isOk = false;
        printLog(QsLogging::Level::ErrorLevel,
                 tr("CModbusClient::startupTCP fail:") + m_modbusClient->errorString());
    }

    printLog(QsLogging::Level::InfoLevel,
             tr("CModbusClient::startupTCP succesed,ip:")+
             ip+
             tr(";port:")+
             QString::asprintf("%d",port));

    m_isClientStartupSuccessed = isOk;

    return isOk;
}

/**
 * @brief CModbusClient::startupTCP2 启动TCP
 * @param url
 * @return
 */
bool CModbusClient::startupTCP2(QUrl url)
{
    if (m_modbusClient == NULL ||
        m_modbusClient->state() == QModbusDevice::ConnectedState ||
        !url.isValid())
        return false;

    return this->startupTCP(url.host(),url.port());
}

/**
 * @brief CModbusClient::writeRequest 向服务器写入指定的数据
 * @param write
 * @param serverAddress
 * @return
 */
bool CModbusClient::writeRequest(const QModbusDataUnit &write, int serverAddress)
{
    if (m_modbusClient == NULL ||
        m_modbusClient->state() != QModbusDevice::ConnectedState ||
        (!write.isValid() || serverAddress <= 0))
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "CModbusClient::writeRequest:"+QString::fromLocal8Bit("客户端不存在或者连接没有建立成功."));
        return false;
    }

    if (auto *reply = m_modbusClient->sendWriteRequest(write, serverAddress))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    printLog(QsLogging::Level::ErrorLevel,
                                tr("CModbusClient::writeRequest Write response error: %1 (Mobus exception: 0x%2)")
                                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                }
                else if (reply->error() != QModbusDevice::NoError)
                {
                    printLog(QsLogging::Level::ErrorLevel,
                                tr("CModbusClient::writeRequest Write response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).arg(reply->error(), -1, 16));
                }
                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
        }
    }
    else
    {
        printLog(QsLogging::Level::ErrorLevel,
                    tr("CModbusClient::writeRequest error: ") + m_modbusClient->errorString());
    }

    return true;
}

/**
 * @brief CModbusClient::readwriteRequest 向服务器读写指定的数据
 * @param read
 * @param write
 * @param serverAddress
 * @return
 */
bool CModbusClient::readwriteRequest(const QModbusDataUnit &read, const QModbusDataUnit &write, int serverAddress)
{
    if (m_modbusClient == NULL ||
        m_modbusClient->state() != QModbusDevice::ConnectedState ||
        (!read.isValid() || !write.isValid() || serverAddress <= 0))
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "CModbusClient::readwriteRequest:"+QString::fromLocal8Bit("客户端不存在或者连接没有建立成功."));
        return false;
    }

    if (auto *reply = m_modbusClient->sendReadWriteRequest(read,write,serverAddress))
    {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &CModbusClient::onClientReadReady);
        else
            delete reply;
    }
    else
    {
        printLog(QsLogging::Level::ErrorLevel,
                    tr("CModbusClient::readwriteRequest Read error: ") + m_modbusClient->errorString());
    }

    return true;
}

/**
 * @brief CModbusClient::readRequest 从服务器读取指定的数据
 * @param read
 * @param serverAddress
 * @return
 */
bool CModbusClient::readRequest(const QModbusDataUnit &read, int serverAddress)
{
    if (m_modbusClient == NULL ||
        m_modbusClient->state() != QModbusDevice::ConnectedState ||
        (/*!read.isValid() ||*/ serverAddress <= 0))
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "CModbusClient::readRequest:"+QString::fromLocal8Bit("客户端不存在或者连接没有建立成功."));
        return false;
    }

    if (auto *reply = m_modbusClient->sendReadRequest(read, serverAddress))
    {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &CModbusClient::onClientReadReady);
        else
            delete reply; // broadcast replies return immediately
    }
    else
    {
        printLog(QsLogging::Level::ErrorLevel,
                 tr("CModbusClient::readRequest error:") + m_modbusClient->errorString());
    }

    return true;
}

void CModbusClient::onClientReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        QVector<quint16> tmpData;

        for (int i = 0, total = int(unit.valueCount()); i < total; ++i)
        {
            tmpData.push_back(unit.value(i));
        }

        emit signalDataWritten(unit.registerType(),unit.startAddress(),tmpData);
    }
    else if (reply->error() == QModbusDevice::ProtocolError)
    {
        printLog(QsLogging::Level::ErrorLevel,
                    tr("Read response error: %1 (Mobus exception: 0x%2)").
                        arg(reply->errorString()).
                        arg(reply->rawResult().exceptionCode(), -1, 16));
    }
    else
    {
        printLog(QsLogging::Level::ErrorLevel,
                    tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16));
    }

    reply->deleteLater();
}
