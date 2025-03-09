#include "cmodbusserver.h"

#include <qmodbusrtuserialserver>
#include <QModbusTcpServer>
#include <QRegularExpressionValidator>
#include <QTcpSocket>
#include <QDateTime>

CModbusTcpConnectionObserver::CModbusTcpConnectionObserver(CModbusServer *mServer)
    : m_ModbusServer(mServer)
{

}

CModbusTcpConnectionObserver::~CModbusTcpConnectionObserver()
{

}

bool CModbusTcpConnectionObserver::acceptNewConnection(QTcpSocket *newClient)
{
    if(m_ModbusServer)
        m_ModbusServer->setIsClientConnected(true);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////

/*CModbusServer::CModbusServer(QObject *parent)
    : QObject(parent),
      m_ModbusConnection(Modbus_Tcp),
      m_ServerAddress(-1)
{
    createModbusServer();
}*/

CModbusServer::CModbusServer(QObject *parent,ModbusConnection connection)
    : QObject(parent),
      m_ModbusConnection(connection),
      m_isClientConnected(false),
      m_modbusServer(nullptr),
      m_isServerStartupSuccessed(false)
{
    createModbusServer();
}

CModbusServer::~CModbusServer()
{
    if (m_modbusServer)
    {
        m_modbusServer->disconnectDevice();
        m_modbusServer->deleteLater();
    }
}

/**
 * @brief CModbusServer::createModbusServer 建立Modbus服务器
 */
void CModbusServer::createModbusServer()
{
    if (m_modbusServer)
    {
        m_modbusServer->disconnect();
        m_modbusServer->deleteLater();
    }

    if (m_ModbusConnection == Modbus_Serial)
    {
        /*
#if QT_CONFIG(modbus_serialport)
        m_modbusServer = new QModbusRtuSerialSlave(this);
#endif
*/
#if QT_CONFIG(modbus_serialport)
        m_modbusServer = new QModbusRtuSerialServer(this);
#endif

    }
    else if (m_ModbusConnection == Modbus_Tcp)
    {
        // 在连接信号前注册QTcpSocket*类型
        qRegisterMetaType<QTcpSocket*>();
        QModbusTcpServer *pModbusTcpServer = new QModbusTcpServer(this);
        m_modbusServer = pModbusTcpServer;

        pModbusTcpServer->installConnectionObserver(new CModbusTcpConnectionObserver(this));

        connect(pModbusTcpServer,
                &QModbusTcpServer::modbusClientDisconnected,
                this,
                &CModbusServer::onprocessmodbusClientDisconnected);
    }

    if(m_modbusServer == nullptr) return;

    connect(m_modbusServer,
            &QModbusServer::dataWritten,
            this,
            &CModbusServer::onDataWritten);
    connect(m_modbusServer,
            &QModbusServer::stateChanged,
            this,
            &CModbusServer::onStateChanged);
    connect(m_modbusServer,
            &QModbusServer::errorOccurred,
            this,
            &CModbusServer::onErrorOccurred);
}

/**
 * @brief CModbusServer::onprocessmodbusClientDisconnected 处理客户端断开连接
 * @param modbusClient
 */
void CModbusServer::onprocessmodbusClientDisconnected(QTcpSocket *modbusClient)
{
    this->setIsClientConnected(false);
}

/**
 * @brief CModbusServer::closeServer 关闭服务器
 */
void CModbusServer::closeServer()
{
    m_isClientConnected = false;
    m_isServerStartupSuccessed = false;

    if (m_modbusServer)
        m_modbusServer->disconnect();

    printLog(QsLogging::Level::InfoLevel,
             "CModbusServer::closeServer succesed.");
}

/**
 * @brief CModbusServer::setModbusDataUnitMap 为来自其他ModBus客户端的请求设置要映射的注册映射结构
 * @param map
 * @example:
 *    QModbusDataUnitMap reg;
 *    reg.insert(QModbusDataUnit::Coils, { QModbusDataUnit::Coils, 0, 10 });
 *    reg.insert(QModbusDataUnit::DiscreteInputs, { QModbusDataUnit::DiscreteInputs, 0, 10 });
 *    reg.insert(QModbusDataUnit::InputRegisters, { QModbusDataUnit::InputRegisters, 0, 10 });
 *    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0, 10 });
 *    modbusDevice->setMap(reg);
 * @return
 */
bool CModbusServer::setModbusDataUnitMap(const QModbusDataUnitMap &map)
{
    if(m_modbusServer == nullptr)
        return false;

    m_ModbusDataUnitMap = map;

    m_modbusServer->setMap(map);

    return true;
}

/**
 * @brief CModbusServer::setServerListenOnly 设置服务器是否只侦听
 * @param isenable
 */
void CModbusServer::setServerListenOnly(bool isenable)
{
    if(m_modbusServer == nullptr)
        return;

    m_isListenOnly = isenable;

    m_modbusServer->setValue(QModbusServer::ListenOnlyMode, m_isListenOnly);
}

/**
 * @brief CModbusServer::setServerDeviceBusy 设置是否设置服务器忙
 * @param isenable
 */
void CModbusServer::setServerDeviceBusy(bool isenable)
{
    if(m_modbusServer == nullptr)
        return;

    m_isDeviceBusy = isenable;

    m_modbusServer->setValue(QModbusServer::DeviceBusy, m_isDeviceBusy ? 0xffff : 0x0000);
}

/**
 * @brief CModbusServer::getCurrentDate 得到当前时间
 * @return
 */
QString CModbusServer::getCurrentDate()
{
    QDateTime dateTime =QDateTime::currentDateTime();
    return dateTime.toString("<b>[yyyy-MM-dd hh:mm:ss]</b> ");
}

/**
 * @brief CModbusServer::printLog 打印用户日志
 * @param type
 * @param msg
 */
void CModbusServer::printLog(QsLogging::Level type,QString msg)
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

#if QT_CONFIG(modbus_serialport)
/**
 * @brief CModbusServer::startupSerialPort 启动串口
 * @param port
 * @param parity
 * @param baud
 * @param dataBits
 * @param stopBits
 * @return
 */
bool CModbusServer::startupSerialPort(QString port,
                       int parity,
                       int baud,
                       int dataBits,
                       int stopBits)
{
    if(m_modbusServer == nullptr ||
       m_isServerStartupSuccessed ||
       port.isEmpty() ||
       m_ServerAddress < 0)
        return false;

    bool isOk = true;

    m_modbusServer->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
        port);

    m_modbusServer->setConnectionParameter(QModbusDevice::SerialParityParameter,
        parity);
    m_modbusServer->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
        baud);
    m_modbusServer->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
        dataBits);
    m_modbusServer->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
        stopBits);

    m_modbusServer->setServerAddress(m_ServerAddress);

    if (!m_modbusServer->connectDevice()) {
        isOk = false;
        printLog(QsLogging::Level::ErrorLevel,
                 tr("CModbusServer::startupSerialPort fail:") + m_modbusServer->errorString());
    }

    /*printLog(QsLogging::Level::InfoLevel,
             QString::asprintf("CModbusServer::startupSerialPort succesed,port:%d;",port)+
             QString::asprintf("server address:%d;",m_ServerAddress)+
             QString::asprintf("Parity:%d BaudRate:%d DataBits:%d StopBits:%d",parity,baud,dataBits,stopBits));*/
    printLog(QsLogging::Level::InfoLevel,
         QString("CModbusServer::startupSerialPort succesed, port:%1; server address:%2; Parity:%3 BaudRate:%4 DataBits:%5 StopBits:%6")
             .arg(port)
             .arg(m_ServerAddress)
             .arg(parity)
             .arg(baud)
             .arg(dataBits)
             .arg(stopBits));


    m_isServerStartupSuccessed = isOk;

    return isOk;
}
#endif

/**
 * @brief CModbusServer::startupTCP2 启动TCP
 * @param url
 * @return
 */
bool CModbusServer::startupTCP2(QUrl url)
{
    if(m_modbusServer == nullptr ||
       !url.isValid())
        return false;

    return this->startupTCP(url.host(),url.port());
}

/**
 * @brief CModbusServer::startupTCP 启动TCP
 * @param ip
 * @param port
 * @return
 */
bool CModbusServer::startupTCP(QString ip,int port)
{
    if(m_modbusServer == nullptr ||
       m_isServerStartupSuccessed ||
       m_ServerAddress < 0 ||
       ip == "" || port <= 0)
        return false;

    m_modbusServer->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    m_modbusServer->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);

    m_modbusServer->setServerAddress(m_ServerAddress);

    bool isOk = true;

    if (!m_modbusServer->connectDevice()) {
        isOk = false;
        printLog(QsLogging::Level::ErrorLevel,
                 tr("CModbusServer::startupTCP fail:") + m_modbusServer->errorString());
    }

    printLog(QsLogging::Level::InfoLevel,
             tr("CModbusServer::startupTCP succesed,ip:")+
             ip+
             tr(";port:")+
             QString::asprintf("%d",port));

    m_isServerStartupSuccessed = isOk;

    return isOk;
}

/**
 * @brief CModbusServer::setIsClientConnected 设置客户端是否连接成功
 * @param isConn
 */
void CModbusServer::setIsClientConnected(bool isConn)
{
    m_isClientConnected = isConn;

    printLog(QsLogging::Level::InfoLevel,
             tr("CModbusServer::setIsClientConnected:")+
             (m_isClientConnected ? QString::fromLocal8Bit("客户端连接成功!") : QString::fromLocal8Bit("客户端断开连接!")));
}

void CModbusServer::onStateChanged(int state)
{
    if (state == QModbusDevice::UnconnectedState)
    {
        this->setIsClientConnected(false);
    }
    else if (state == QModbusDevice::ConnectedState)
    {
        this->setIsClientConnected(true);
    }

    emit signalStateChanged(state);
}

void CModbusServer::onDataWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    if(m_modbusServer == nullptr ||
       m_modbusServer->state() == QModbusDevice::UnconnectedState)
        return;

    QVector<quint16> tmpData;

    for (int i = 0; i < size; ++i)
    {
        quint16 value = 0;
        if(!m_modbusServer->data(table, quint16(address + i), &value))
        {
            printLog(QsLogging::Level::ErrorLevel,
                     tr("CModbusServer::onDataWritten:") + m_modbusServer->errorString());
            break;
        }

        tmpData.push_back(value);
    }

    emit signalDataWritten(table,address,tmpData);
}

/**
 * @brief CModbusServer::setDatas 设置数据
 * @param table
 * @param address
 * @param data
 * @return
 */
bool CModbusServer::setDatas(QModbusDataUnit::RegisterType table, int address, QVector<quint16> data)
{
    if(m_modbusServer == nullptr ||
       m_modbusServer->state() == QModbusDevice::UnconnectedState ||
       table == QModbusDataUnit::Invalid ||
       address < 0 || data.isEmpty())
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "CModbusServer::setDatas:"+QString::fromLocal8Bit("服务器不存在或者连接没有建立成功."));
        return false;
    }

    bool isOk = true;

    for(int i=0;i<data.size();i++)
    {
        if(!m_modbusServer->setData(table,address+i,data[i]))
        {
            isOk = false;

            printLog(QsLogging::Level::ErrorLevel,
                     tr("CModbusServer::sendData:") + m_modbusServer->errorString());
            break;
        }
    }

    return isOk;
}

bool CModbusServer::setData(QModbusDataUnit::RegisterType table, int address, quint16 data)
{
    if(m_modbusServer == nullptr ||
       m_modbusServer->state() == QModbusDevice::UnconnectedState ||
       table == QModbusDataUnit::Invalid ||
       address < 0 || data < 0)
    {
        printLog(QsLogging::Level::ErrorLevel,
                 "CModbusServer::setData:"+QString::fromLocal8Bit("服务器不存在或者连接没有建立成功."));
        return false;
    }

    return m_modbusServer->setData(table,address,data);
}

void CModbusServer::onErrorOccurred(QModbusDevice::Error newError)
{
    if (newError == QModbusDevice::NoError ||
        m_modbusServer == nullptr)
        return;

    printLog(QsLogging::Level::ErrorLevel,
             tr("CModbusServer::onErrorOccurred:") + m_modbusServer->errorString());
}
