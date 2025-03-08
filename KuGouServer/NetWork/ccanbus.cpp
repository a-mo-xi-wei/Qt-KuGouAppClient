#include "../../includes/network/ccanbus.h"
//#include "../../includes/QsLog/QsLog.h"
#include "../../includes/common/common.h"

#include <QCanBus>
#include <QDateTime>

ccanbus::ccanbus(QObject *parent)
    : QObject(parent),
      m_CanBusDevice(NULL),
      m_busStatusTimer(new QTimer(this)),
      m_numberFramesWritten(0)
{
    connect(m_busStatusTimer, &QTimer::timeout, this, &ccanbus::busStatus);
}

ccanbus::~ccanbus()
{
    delete m_busStatusTimer;

    if(m_CanBusDevice)
    {
        m_CanBusDevice->disconnectDevice();
        m_CanBusDevice->deleteLater();
    }
}

/**
 * @brief ccanbus::createDevice 建立设备
 * @param plugin
 * @param interfaceName
 * @param configurationitems
 * @return
 */
bool ccanbus::createDevice(const QString &plugin,
                           const QString &interfaceName,
                           QVector<ConfigurationItem> configurationitems)
{
    if(m_CanBusDevice || plugin == "" || interfaceName == "")
        return false;

    QString errorString;
    m_CanBusDevice = QCanBus::instance()->createDevice(plugin,interfaceName,&errorString);

    if (!m_CanBusDevice)
    {
        printLog(QsLogging::Level::ErrorLevel,
                 tr("ccanbus::createDevice error: '%1', reason: '%2'")
                          .arg(plugin).arg(interfaceName));
        return false;
    }

    connect(m_CanBusDevice, &QCanBusDevice::errorOccurred,
            this, &ccanbus::processErrors);
    connect(m_CanBusDevice, &QCanBusDevice::framesReceived,
            this, &ccanbus::processReceivedFrames);
    connect(m_CanBusDevice, &QCanBusDevice::framesWritten,
            this, &ccanbus::processFramesWritten);

    for(int i=0;i<configurationitems.size();i++)
    {
        m_CanBusDevice->setConfigurationParameter(configurationitems[i].first,
                                                  configurationitems[i].second);
    }

    if (!m_CanBusDevice->connectDevice())
    {
        printLog(QsLogging::Level::ErrorLevel,
                 tr("Connection error: %1").arg(m_CanBusDevice->errorString()));
    }
    else
    {
        if (m_CanBusDevice->hasBusStatus())
            m_busStatusTimer->start(2000);
        else
            printLog(QsLogging::Level::ErrorLevel,tr("No CAN bus status available."));
    }

    m_numberFramesWritten = 0;

    printLog(QsLogging::Level::InfoLevel,
             tr("ccanbus::createDevice successed: '%1', reason: '%2'")
                      .arg(plugin).arg(interfaceName));

    return true;
}

void ccanbus::busStatus()
{
    if (!m_CanBusDevice || !m_CanBusDevice->hasBusStatus()) {
        printLog(QsLogging::Level::ErrorLevel,
                 tr("No CAN bus status available."));
        m_busStatusTimer->stop();
        return;
    }

    switch (m_CanBusDevice->busStatus()) {
    case QCanBusDevice::CanBusStatus::Good:
        printLog(QsLogging::Level::InfoLevel,"CAN bus status: Good.");
        break;
    case QCanBusDevice::CanBusStatus::Warning:
        printLog(QsLogging::Level::InfoLevel,"CAN bus status: Warning.");
        break;
    case QCanBusDevice::CanBusStatus::Error:
        printLog(QsLogging::Level::InfoLevel,"CAN bus status: Error.");
        break;
    case QCanBusDevice::CanBusStatus::BusOff:
        printLog(QsLogging::Level::InfoLevel,"CAN bus status: Bus Off.");
        break;
    default:
        printLog(QsLogging::Level::InfoLevel,"CAN bus status: Unknown.");
        break;
    }
}

/**
 * @brief CModbusClient::getCurrentDate 得到当前时间
 * @return
 */
QString ccanbus::getCurrentDate(void)
{
    QDateTime dateTime =QDateTime::currentDateTime();
    return dateTime.toString("<b>[yyyy-MM-dd hh:mm:ss]</b> ");
}

/**
 * @brief ccanbus::printLog 打印用户日志
 * @param type
 * @param msg
 */
void ccanbus::printLog(QsLogging::Level type,QString msg)
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
 * @brief ccanbus::disconnectDevice 断开设备的连接
 */
void ccanbus::disconnectDevice()
{
    if (!m_CanBusDevice)
        return;

    m_busStatusTimer->stop();

    m_CanBusDevice->disconnectDevice();

    printLog(QsLogging::Level::InfoLevel,tr("ccanbus::disconnectDevice."));
}

QString ccanbus::frameFlags(const QCanBusFrame &frame)
{
    QString result = QLatin1String(" --- ");

    if (frame.hasBitrateSwitch())
        result[1] = QLatin1Char('B');
    if (frame.hasErrorStateIndicator())
        result[2] = QLatin1Char('E');
    if (frame.hasLocalEcho())
        result[3] = QLatin1Char('L');

    return result;
}

void ccanbus::processReceivedFrames()
{
    if (!m_CanBusDevice)
        return;

    while (m_CanBusDevice->framesAvailable()) {
        emit signalReciveData(m_CanBusDevice->readFrame());
    }
}

void ccanbus::processErrors(QCanBusDevice::CanBusError error)
{
    if(m_CanBusDevice == NULL) return;

    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
    {
        printLog(QsLogging::Level::ErrorLevel,
                 tr("ccanbus::processErrors:%s").arg(m_CanBusDevice->errorString()));
    }
        break;
    default:
        break;
    }
}

void ccanbus::processFramesWritten(qint64 size)
{
    m_numberFramesWritten += size;

    printLog(QsLogging::Level::InfoLevel,
             tr("ccanbus::processFramesWritten:%1 frames written").arg(m_numberFramesWritten));
}

/**
 * @brief ccanbus::sendFrame 发送一帧数据
 * @param frame
 * @return
 */
bool ccanbus::sendFrame(const QCanBusFrame &frame)
{
    if(m_CanBusDevice == NULL || !frame.isValid()) return false;

    return m_CanBusDevice->writeFrame(frame);
}
