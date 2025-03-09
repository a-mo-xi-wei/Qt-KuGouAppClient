#include "cserialportmanager.h"
#include "QsLog.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

//initialiseSingleton(CSerialPortManager);

CSerialPort::CSerialPort(QString name,NetworkFrameManager *pNetworkFrameManager,QObject *parent) :
    QSerialPort(parent),
    m_Name(name),
    m_NetworkFrameManager(pNetworkFrameManager),
    m_usemyselfprotocol(false),
    m_isOpened(false),
    m_isReconnected(false),
    m_reconnectCount(5)
{
    connect(this, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&m_heartTimer, SIGNAL(timeout()), this, SLOT(timerheartprocess()));
    m_heartTimer.setSingleShot(true);

    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
}


CSerialPort::~CSerialPort()
{
    closeSerialPort();
}

/**
 * @brief CSerialPort::openSerialPort 打开串口
 * @param PortName 串口名
 * @param BaudRate 波特率
 * @param DataBits 数据位数
 * @param Parity 奇偶校验
 * @param StopBits 停止位
 * @param FlowControl 流控制
 *
 * @return 如果串口打开成功返回真，否则返回假
 */
bool CSerialPort::openSerialPort(QString PortName,qint32 BaudRate,QSerialPort::DataBits DataBits,QSerialPort::Parity Parity,QSerialPort::StopBits StopBits,QSerialPort::FlowControl FlowControl)
{
    //设置串口名
    this->setPortName(PortName);
    //设置波特率
    this->setBaudRate(BaudRate);
    //设置数据位数
    this->setDataBits(DataBits);
    //设置奇偶校验
    this->setParity(Parity);
    //设置停止位
    this->setStopBits(StopBits);
    //设置流控制
    this->setFlowControl(FlowControl);

    //打开串口
    if(!this->open(QIODevice::ReadWrite))
    {
        QLOG_ERROR()<<"SerialPort:"<<PortName<<" open fail.";
        emit signalSerialPortLog(this,"CSerialPort::openSerialPort fail.");
        return false;
    }

    qDebug()<<"Open SerialPort success:"<<PortName
           <<" "<<BaudRate
           <<" "<<DataBits
           <<" "<<Parity
           <<" "<<StopBits
           <<" "<<FlowControl;

    QLOG_INFO()<<"Open SerialPort success:"<<PortName
              <<" "<<BaudRate
              <<" "<<DataBits
              <<" "<<Parity
              <<" "<<StopBits
              <<" "<<FlowControl;

    m_isOpened = true;
    m_reconnectCount = 5;

    emit signalSerialPortLog(this,
                             this->portName()+
                             QString::fromLocal8Bit("打开成功."));

    return true;
}

/**
 * @brief CSerialPort::openArduinoSerialPort 查找系统中Arduino专用串口，并打开
 */
void CSerialPort::openArduinoSerialPort()
{
    QSerialPortInfo portToUse;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString s = QObject::tr("Port:") + info.portName() + "\n"
                    + QObject::tr("Location:") + info.systemLocation() + "\n"
                    + QObject::tr("Description:") + info.description() + "\n"
                    + QObject::tr("Manufacturer:") + info.manufacturer() + "\n"
                    + QObject::tr("Serial number:") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier:") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier:") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n";
                    //+ QObject::tr("Busy:") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

        if(/*!info.isBusy() && */(info.description().contains("Arduino") || info.manufacturer().contains("Arduino")))
            portToUse = info;
        qDebug() << s;
        QLOG_INFO() << "CSerialPortManager::openSerialPort:" << s;
    }

    if(portToUse.isNull() || !portToUse.isNull())
    {
        qDebug() << "port is not valid:" << portToUse.portName();
        QLOG_ERROR() << "port is not valid:" << portToUse.portName();
        return;
    }

    this->setPortName(portToUse.portName());
    this->setBaudRate(QSerialPort::Baud115200);
    this->setDataBits(QSerialPort::Data8);
    this->setParity(QSerialPort::NoParity);
    this->setStopBits(QSerialPort::OneStop);
    this->setFlowControl(QSerialPort::NoFlowControl);

    if (this->open(QIODevice::ReadWrite)) {
        qDebug() << "Connected to" << portToUse.description() << "on" << portToUse.portName();
        QLOG_INFO() << "Connected to" << portToUse.description() << "on" << portToUse.portName();

        m_isOpened = true;
    } else {
        qCritical() << "Serial Port error:" << this->errorString();
        QLOG_ERROR() << "Serial Port error:" << this->errorString();
    }
}

/**
 * @brief CSerialPort::timerheartprocess 处理断线重连
 */
void CSerialPort::timerheartprocess(void)
{
    if(m_reconnectCount-- > 0)
    {
        //打开串口
        if(this->open(QIODevice::ReadWrite))
        {
            m_reconnectCount=5;
        }
        else
        {
            m_heartTimer.start(10000);
        }
    }
    else
    {
        m_reconnectCount=5;
        this->closeSerialPort();
    }
}

/**
 * @brief CSerialPort::closeSerialPort 关闭串口
 */
void CSerialPort::closeSerialPort()
{
    if(!m_isOpened) return;

    emit signalSerialPortLog(this,
                             this->portName()+
                             QString::fromLocal8Bit("关闭成功."));

    m_isOpened = false;
    this->close();
}

qint64 CSerialPort::writeData(const QByteArray &data)
{
    if(data.isEmpty() || !this->isOpen())
        return -1;

    qint64 ret = this->write(data);
    this->flush();

    return ret;
}

/**
 * @brief CSerialPort::sendJson 发送json数据
 * @param mes 要发送的数据
 */
void CSerialPort::sendJson(QJsonObject mes)
{
    if(mes.isEmpty() || !this->isOpen())
        return;

    QJsonDocument document=QJsonDocument(mes);
    QByteArray array = document.toJson();// 转换成QByteArray

    this->SendData(array);
}

/**
 * @brief CSerialPort 发送二进制数据
 */
bool CSerialPort::SendData(QByteArray data)
{
    if(data.isEmpty() || !this->isOpen())
        return false;

    if(!m_usemyselfprotocol)
    {
        return writeData(data) == data.size() ? true : false;
    }

    qint64 m_sendsize,m_totalsize;

    tagDatagramHearder pTcpPacketHearder;
    pTcpPacketHearder.version = IDD_SERIALPORT_VERSION;
    pTcpPacketHearder.srcdataSize = data.size();

    QByteArray sendData;

    if(m_usemyselfprotocol)
    {
        //添加包头
        sendData.append((const char*)&pTcpPacketHearder,sizeof(tagDatagramHearder));
    }

    //添加包数据
    sendData.append(data);

    m_totalsize = sendData.size();
    m_sendsize=0;

    do
    {
        QByteArray psendbytearray;

        if(sendData.size()-m_sendsize < SERIALPORT_BUF_SIZE)
            psendbytearray = sendData.mid(m_sendsize);
        else
            psendbytearray = sendData.mid(m_sendsize,SERIALPORT_BUF_SIZE);

        m_sendsize += writeData(psendbytearray);

        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    while(m_sendsize < m_totalsize);

    return m_sendsize == m_totalsize ? true : false;
}

void CSerialPort::readData()
{
    if(m_usemyselfprotocol)
    {
        m_dataPacket.append(this->readAll());

        // 循环解析包数据，数据中可能不只一包数据
        parsePacket();
    }
    else
    {
        QByteArray precvData = this->readAll();

        if(m_NetworkFrameManager)
            m_NetworkFrameManager->OnProcessSerialPortBinary(this,precvData);

        emit signalSerialPortBinary(this,precvData);
    }
}

void CSerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        qDebug() << "Serial Port error:" << this->errorString();
        QLOG_ERROR() << "Serial Port error:" << this->errorString();

        m_errorMessage = this->portName()+":"+this->errorString();

        emit signalSerialPortLog(this,"CSerialPort::handleError:"+m_errorMessage);

        if(m_isReconnected)
            m_heartTimer.start(10000);
        else
            closeSerialPort();
    }
}

/**
 * @brief CSerialPort::parsePacket 解包
 * @param packet 要解包的数据
 *
 * @return 如果数据解包成功返回真，否则返回假
 */
bool CSerialPort::parsePacket(void)
{
    while(!m_dataPacket.isEmpty())
    {
        if(!m_recvDataState && m_dataPacket.size() >= sizeof(tagDatagramHearder))
        {
            // 先取包头
            memcpy(&m_PacketHearder,m_dataPacket.constData(),sizeof(tagDatagramHearder));

            // 检查版本号是否正确
            if(m_PacketHearder.version != IDD_SERIALPORT_VERSION)
                break;

            m_dataPacket.remove(0,sizeof(tagDatagramHearder));
            m_recvDataState = true;
        }

        if(!m_recvDataState || (unsigned int)m_dataPacket.size() < m_PacketHearder.srcdataSize)
            break;

        // 得到当前数据
        QByteArray precvData = m_dataPacket.mid(0,m_PacketHearder.srcdataSize);
        m_dataPacket.remove(0,m_PacketHearder.srcdataSize);

        if(m_NetworkFrameManager)
            m_NetworkFrameManager->OnProcessSerialPortBinary(this,precvData);

        emit signalSerialPortBinary(this,precvData);

        m_recvDataState=false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSerialPortManager::CSerialPortManager(QObject *parent)
    : QObject(parent)
{

}

CSerialPortManager::~CSerialPortManager()
{
    clearAllSerialPorts();
}

/**
 * @brief CSerialPortManager::addSerialPort 添加一个串口
 * @param name 串口名称
 * @param pSerialPort 要添加的串口
 * @param reconnectCount 是否自动重连
 * @return 如果串口添加成功返回真，否则返回假
 */
CSerialPort* CSerialPortManager::addSerialPort(CSerialPort* pSerialPort,
                                       QString PortName,
                                       qint32 BaudRate,
                                       QSerialPort::DataBits DataBits,
                                       QSerialPort::Parity Parity,
                                       QSerialPort::StopBits StopBits,
                                       QSerialPort::FlowControl FlowControl,
                                       bool reconnectCount)
{
    if(pSerialPort == NULL)
        return NULL;

    QMutexLocker locker(&m_MutexSerialPort);

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.find(pSerialPort->getName());
    if(iter != m_SerialPortList.end())
        return NULL;

    //设置串口名
    pSerialPort->setPortName(PortName);
    //设置波特率
    pSerialPort->setBaudRate(BaudRate);
    //设置数据位数
    pSerialPort->setDataBits(DataBits);
    //设置奇偶校验
    pSerialPort->setParity(Parity);
    //设置停止位
    pSerialPort->setStopBits(StopBits);
    //设置流控制
    pSerialPort->setFlowControl(FlowControl);

    //设置是否自动重连
    pSerialPort->setIsReconnected(reconnectCount);

    m_SerialPortList[pSerialPort->getName()] = pSerialPort;

    connect(pSerialPort, SIGNAL(signalSerialPortBinary(CSerialPort*,QByteArray)),
            this, SLOT(onprocessSerialPortBinary(CSerialPort*,QByteArray)),
            Qt::QueuedConnection);
    connect(pSerialPort, SIGNAL(signalSerialPortLog(CSerialPort*,QString)),
            this, SLOT(onprocessSerialPortLog(CSerialPort*,QString)));

    return pSerialPort;
}

/**
 * @brief CSerialPortManager::getSerialPort 得到一个串口
 * @param name 要得到的串口的名称
 * @return 如果存在这个串口返回这个串口，否则返回NULL
 */
CSerialPort* CSerialPortManager::getSerialPort(QString name)
{
    if(name.isEmpty())
        return NULL;

    QMutexLocker locker(&m_MutexSerialPort);

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.find(name);
    if(iter == m_SerialPortList.end())
        return NULL;

    return iter.value();
}

/**
 * @brief CSerialPortManager::openAllSerialPorts 打开所有的串口
 */
bool CSerialPortManager::openAllSerialPorts(void)
{
    if(m_SerialPortList.isEmpty()) return false;

    QMutexLocker locker(&m_MutexSerialPort);

    bool isOk = true;

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.begin();
    for(;iter != m_SerialPortList.end();++iter)
    {
        if(iter.value() == NULL)
            continue;

        //打开串口
        if(iter.value()->open(QIODevice::ReadWrite))
        {
            iter.value()->setIsOpened(true);

            emit signalSerialPortLog(iter.value(),
                                     QString::fromLocal8Bit("打开串口")+
                                     iter.value()->portName()+
                                     QString::fromLocal8Bit("成功."));
        }
        else
        {
            emit signalSerialPortLog(iter.value(),
                                     QString::fromLocal8Bit("打开串口")+
                                     iter.value()->portName()+
                                     QString::fromLocal8Bit("失败."));
            isOk = false;
        }
    }

    return isOk;
}

/**
 * @brief CSerialPortManager::closeAllSerialPorts 关闭所有的串口
 */
void CSerialPortManager::closeAllSerialPorts(void)
{
    if(m_SerialPortList.isEmpty()) return;

    QMutexLocker locker(&m_MutexSerialPort);

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.begin();
    for(;iter != m_SerialPortList.end();++iter)
    {
        if(iter.value())
        {
            iter.value()->closeSerialPort();
        }
    }
}

/**
 * @brief CSerialPortManager::clearAllSerialPorts 清空所有串口的缓冲区
 */
void CSerialPortManager::clearAllSerialPorts(void)
{
    if(m_SerialPortList.isEmpty()) return;

    QMutexLocker locker(&m_MutexSerialPort);

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.begin();
    for(;iter != m_SerialPortList.end();++iter)
    {
        if(iter.value())
            iter.value()->clear();
    }
}

/**
 * @brief CSerialPortManager::clearAllSerialPorts 删除所有的串口
 */
void CSerialPortManager::deleteAllSerialPorts(void)
{
    if(m_SerialPortList.isEmpty()) return;

    QMutexLocker locker(&m_MutexSerialPort);

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.begin();
    for(;iter != m_SerialPortList.end();++iter)
    {
        if(iter.value())
        {
            disconnect(iter.value(), SIGNAL(signalSerialPortBinary(CSerialPort*,QByteArray)),
                    this, SLOT(onprocessSerialPortBinary(CSerialPort*,QByteArray)));
            disconnect(iter.value(), SIGNAL(signalSerialPortLog(CSerialPort*,QString)),
                    this, SLOT(onprocessSerialPortLog(CSerialPort*,QString)));

            iter.value()->deleteLater();
        }
    }

    m_SerialPortList.clear();
}

/**
 * @brief CSerialPortManager::isSerialPortOpened 检测指定串口是否已经打开
 * @param name
 * @return
 */
bool CSerialPortManager::isSerialPortOpened(QString name)
{
    QMutexLocker locker(&m_MutexSerialPort);

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.find(name);
    if(iter != m_SerialPortList.end())
    {
        return iter.value()->isOpened();
    }

    return false;
}

/**
 * @brief CSerialPortManager::useMyselfProtocol 设置一组串口是否使用自定义协议
 * @param portNames
 * @param isUsing
 */
void CSerialPortManager::useMyselfProtocol(QStringList portNames,bool isUsing)
{
    if(portNames.isEmpty())
        return;

    for(int i=0;i<portNames.size();i++)
    {
        CSerialPort *pSerialPort = getSerialPort(portNames[i]);
        if(pSerialPort == NULL)
            continue;

        pSerialPort->setusemyselfprotocol(isUsing);
    }
}

/**
 * @brief CSerialPortManager::useAutoReconnected 设置一组串口是否自动重连
 * @param portNames
 * @param isUsing
 */
void CSerialPortManager::useAutoReconnected(QStringList portNames,bool isUsing)
{
    if(portNames.isEmpty())
        return;

    for(int i=0;i<portNames.size();i++)
    {
        CSerialPort *pSerialPort = getSerialPort(portNames[i]);
        if(pSerialPort == NULL)
            continue;

        pSerialPort->setIsReconnected(isUsing);
    }
}

/**
 * @brief CSerialPortManager::sendData 操作一组串口发送数据
 * @param portNames
 * @param data
 */
bool CSerialPortManager::sendData(QStringList portNames,QByteArray data)
{
    if(portNames.isEmpty() || data.isEmpty())
        return false;

    int totalSuc=0;

    for(int i=0;i<portNames.size();i++)
    {
        CSerialPort *pSerialPort = getSerialPort(portNames[i]);
        if(pSerialPort == NULL)
            continue;

        if(pSerialPort->SendData(data) == false)
        {
            emit signalSerialPortLog(pSerialPort,
                                     pSerialPort->portName()+
                                     QString::fromLocal8Bit("发送数据失败."));
        }
        else
        {
            emit signalSerialPortLog(pSerialPort,
                                     pSerialPort->portName()+
                                     QString::fromLocal8Bit("发送数据成功."));

            totalSuc+=1;
        }
    }

    return (totalSuc > 0 ? true : false);
}

/**
 * @brief CSerialPortManager::deleteSerialPort 清除指定的串口
 * @param name 要清除的串口名称
 * @return 如果串口清除成功返回真，否则返回假
 */
bool CSerialPortManager::deleteSerialPort(QString name)
{
    if(name.isEmpty())
        return false;

    QMutexLocker locker(&m_MutexSerialPort);

    QHash<QString,CSerialPort*>::iterator iter = m_SerialPortList.find(name);
    if(iter != m_SerialPortList.end())
    {
        if(iter.value())
        {
            if(iter.value()->isOpened())
            {
                disconnect(iter.value(), SIGNAL(signalSerialPortBinary(CSerialPort*,QByteArray)),
                        this, SLOT(onprocessSerialPortBinary(CSerialPort*,QByteArray)));
                disconnect(iter.value(), SIGNAL(signalSerialPortLog(CSerialPort*,QString)),
                        this, SLOT(onprocessSerialPortLog(CSerialPort*,QString)));
            }

            iter.value()->deleteLater();
        }

        m_SerialPortList.erase(iter);

        return true;
    }

    return false;
}

/**
 * @brief CSerialPortManager::getavailablePorts 得到当前可用的所有串口
 * @return
 */
QStringList CSerialPortManager::getavailablePorts(void)
{
    QStringList serialportinfo;

    QMutexLocker locker(&m_MutexSerialPort);

    foreach(QSerialPortInfo info,QSerialPortInfo::availablePorts())
    {
        serialportinfo<<info.portName();
    }

    return serialportinfo;
}

/**
 * @brief CSerialPortManager::onprocessSerialPortBinary 处理接收到的数据
 * @param data
 */
void CSerialPortManager::onprocessSerialPortBinary(CSerialPort* pSerialPort,QByteArray data)
{
    emit signalSerialPortBinary(pSerialPort,data);
}

/**
 * @brief CSerialPortManager::onprocessSerialPortLog 处理接收到的日志
 * @param pSerialPort
 * @param logstr
 */
void CSerialPortManager::onprocessSerialPortLog(CSerialPort* pSerialPort,QString logstr)
{
    emit signalSerialPortLog(pSerialPort,logstr);
}
