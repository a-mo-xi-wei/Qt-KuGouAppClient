#ifndef C_SERIALPORT_MANAGER_H_INCLUDE_
#define C_SERIALPORT_MANAGER_H_INCLUDE_

#include <QTimer>
#include <QSerialPort>
#include <utility>

#include "singleton.h"
#include "common.h"
#include "networkframemanager.h"

/**
 * @brief The CSerialPort class 串口操作类
 */
class CSerialPort : public QSerialPort /*, public NedAllocatedObject*/
{
    Q_OBJECT

public:
    explicit CSerialPort(const QString &name,NetworkFrameManager *pNetworkFrameManager=nullptr,QObject *parent = nullptr);
    ~CSerialPort() override;

    /// 查找系统中Arduino专用串口，并打开
    void openArduinoSerialPort();
    /// 打开串口
    bool openSerialPort(QString PortName,
                        qint32 BaudRate=QSerialPort::Baud19200,
                        QSerialPort::DataBits DataBits=QSerialPort::Data8,
                        QSerialPort::Parity Parity=QSerialPort::NoParity,
                        QSerialPort::StopBits StopBits=QSerialPort::OneStop,
                        QSerialPort::FlowControl FlowControl=QSerialPort::NoFlowControl);
    /// 关闭串口
    void closeSerialPort();
    /// 设置串口名称
    inline void setName(QString name) { m_Name = std::move(name); }
    /// 得到串口名称
    inline QString getName() { return m_Name; }
    /// 得到错误原因
    inline QString getErrorMsg() { return m_errorMessage; }
    /// 设置是否使用自定义协议
    inline void setusemyselfprotocol(bool isuse) { m_usemyselfprotocol = isuse; }
    /// 得到是否使用自定义协议
    inline bool isusemyselfprotocol() const { return m_usemyselfprotocol; }
    /// 检测串口是否已经打开
    inline bool isOpened() const { return m_isOpened; }
    /// 设置串口是否打开
    inline void setIsOpened(bool opened) { m_isOpened = opened; }
    /// 设置串口是否断线重连
    inline void setIsReconnected(bool conn) { m_isReconnected = conn; }
    /// 是否断线重连
    inline bool isReconnected() const { return m_isReconnected; }
    /// 发送json数据
    void sendJson(QJsonObject mes);
    /// 发送二进制数据
    bool SendData(QByteArray data);

signals:
    /// 处理接收到的数据
    void signalSerialPortBinary(CSerialPort* pSerialPort,QByteArray data);
    /// 处理系统日志
    void signalSerialPortLog(CSerialPort* pSerialPort,QString logstr);

private slots:
    qint64 writeData(const QByteArray &data);
    /// 读取接收到的数据
    void readData();

    void handleError(QSerialPort::SerialPortError error);
    /// 处理断线重连
    void timerheartprocess();

private:
    /// 解析信息包
    bool parsePacket();

private:
    QString m_Name;                                             /**< 串口名称 */
    QByteArray m_dataPacket;                                    /**< 用于存放接收的数据 */
    bool m_recvDataState;                                       /**< 用于接收数据处理 */
    bool m_usemyselfprotocol;                                   /**< 是否使用自定义协议 */
    bool m_isOpened;                                            /**< 串口是否已经打开 */
    bool m_isReconnected;                                       /**< 串口是否断线重连 */
    QString m_errorMessage;                                     /**< 错误原因 */
    tagDatagramHearder m_PacketHearder;                         /**< 用于存放每次接收的包头数据 */
    NetworkFrameManager *m_NetworkFrameManager;                 /**< 网络消息处理框架 */
    QTimer m_heartTimer;
    int m_reconnectCount;                                       /**< 串口重连次数 */
};

/**
 * @brief The CSerialPortManager class 串口管理类
 */
class CSerialPortManager : public QObject
{
    Q_OBJECT

public:
    explicit CSerialPortManager(QObject *parent = nullptr);
    ~CSerialPortManager() override;

    /// 添加一个串口
    CSerialPort* addSerialPort(CSerialPort* pSerialPort,
                       QString PortName,
                       qint32 BaudRate=QSerialPort::Baud19200,
                       QSerialPort::DataBits DataBits=QSerialPort::Data8,
                       QSerialPort::Parity Parity=QSerialPort::NoParity,
                       QSerialPort::StopBits StopBits=QSerialPort::OneStop,
                       QSerialPort::FlowControl FlowControl=QSerialPort::NoFlowControl,
                       bool reconnectCount=false);
    /// 得到一个串口
    CSerialPort* getSerialPort(QString name);
    /// 检测指定串口是否已经打开
    bool isSerialPortOpened(QString name);
    /// 删除所有的串口
    void deleteAllSerialPorts();
    /// 清空所有串口的缓冲区
    void clearAllSerialPorts();
    /// 关闭所有的串口
    void closeAllSerialPorts();
    /// 打开所有的串口
    bool openAllSerialPorts();
    /// 清除指定的串口
    bool deleteSerialPort(QString name);
    /// 操作一组串口发送数据
    bool sendData(QStringList portNames,QByteArray data);
    /// 得到当前可用的所有串口
    QStringList getavailablePorts();
    /// 设置一组串口是否使用自定义协议
    void useMyselfProtocol(QStringList portNames,bool isUsing);
    /// 设置一组串口是否自动重连
    void useAutoReconnected(QStringList portNames,bool isUsing);

signals:
    /// 处理接收到的数据
    void signalSerialPortBinary(CSerialPort* pSerialPort,QByteArray data);
    /// 处理系统日志
    void signalSerialPortLog(CSerialPort* pSerialPort,QString logstr);

private slots:
    /// 处理接收到的数据
    void onprocessSerialPortBinary(CSerialPort* pSerialPort,QByteArray data);
    /// 处理接收到的日志
    void onprocessSerialPortLog(CSerialPort* pSerialPort,QString logstr);

private:
    QHash<QString,CSerialPort*> m_SerialPortList;
    QMutex m_MutexSerialPort;
};

#endif
