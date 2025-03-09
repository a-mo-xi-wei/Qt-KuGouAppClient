#ifndef C_MODBUSSERVER_H_INCLUDE_
#define C_MODBUSSERVER_H_INCLUDE_

#include <QModbusServer>
#include <QSerialPort>
#include <QModbusTcpConnectionObserver>

#include "common.h"
#include "QsLog.h"

class CModbusServer;
class QTcpSocket;

class CModbusTcpConnectionObserver : public QModbusTcpConnectionObserver {
public:
    CModbusTcpConnectionObserver(CModbusServer *mServer);

    ~CModbusTcpConnectionObserver() override;

    bool acceptNewConnection(QTcpSocket *newClient) override;

private:
    CModbusServer *m_ModbusServer;
};

/////////////////////////////////////////////////////////////////////////////////////////////

class CModbusServer : public QObject {
    Q_OBJECT

public:
    //explicit CModbusServer(QObject *parent = nullptr);
    explicit CModbusServer(QObject *parent = nullptr, ModbusConnection connection = Modbus_Tcp);

    ~CModbusServer();

#if QT_CONFIG(modbus_serialport)
    /// 启动串口
    bool startupSerialPort(QString port,
                           int parity = QSerialPort::EvenParity,
                           int baud = QSerialPort::Baud19200,
                           int dataBits = QSerialPort::Data8,
                           int stopBits = QSerialPort::OneStop);
#endif
    /// 启动TCP
    bool startupTCP(QString ip, int port);

    /// 启动TCP
    bool startupTCP2(QUrl url);

    /// 关闭服务器
    void closeServer();

    /// 设置数据
    bool setData(QModbusDataUnit::RegisterType table, int address, quint16 data);

    bool setDatas(QModbusDataUnit::RegisterType table, int address, QVector<quint16> data);

    /// 服务器是否启动成功
    inline bool isServerStartupSuccessed() { return m_isServerStartupSuccessed; }

    /// 设置客户端是否连接成功
    void setIsClientConnected(bool isConn);

    /// 得到客户端是否连接成功
    inline bool isClientConnected() { return m_isClientConnected; }

    /// 设置ModbusDataUnitMap
    bool setModbusDataUnitMap(const QModbusDataUnitMap &map);

    /// 得到ModbusDataUnitMap
    inline QModbusDataUnitMap &getModbusDataUnitMap() { return m_ModbusDataUnitMap; }
    /// 设置服务器地址
    inline void setServerAddress(int ServerAddress) { m_ServerAddress = ServerAddress; }
    /// 得到服务器地址
    inline int getServerAddress() { return m_ServerAddress; }

    /// 设置服务器是否只侦听
    void setServerListenOnly(bool isenable);

    /// 设置是否设置服务器忙
    void setServerDeviceBusy(bool isenable);

    /// 服务器是否只侦听
    inline bool isServerListenOnly() { return m_isListenOnly; }
    /// 是否设置服务器忙
    inline bool isServerDeviceBusy() { return m_isDeviceBusy; }

signals:
    /// 日志消息
    void signalshowlog(QString msg);

    /// 网络状态变更
    void signalStateChanged(int state);

    /// 处理接收到的消息
    void signalDataWritten(QModbusDataUnit::RegisterType table, int address, QVector<quint16> data);

private:
    /// 建立Modbus服务器
    void createModbusServer();

    /// 打印用户日志
    void printLog(QsLogging::Level type, QString msg);

    /// 得到当前时间
    QString getCurrentDate();

private slots:
    /// 处理客户端断开连接
    void onprocessmodbusClientDisconnected(QTcpSocket *modbusClient);

    void onStateChanged(int state);

    void onDataWritten(QModbusDataUnit::RegisterType table, int address, int size);

    void onErrorOccurred(QModbusDevice::Error newError);

private:
    bool m_isClientConnected; /**< 客户端是否连接成功 */
    bool m_isServerStartupSuccessed; /**< 服务器是否启动成功 */
    int m_ServerAddress; /**< 要连接的服务器地址 */
    bool m_isListenOnly; /**< 服务器是否只侦听 */
    bool m_isDeviceBusy; /**< 是否设置服务器忙 */
    QModbusDataUnitMap m_ModbusDataUnitMap; /**< 服务器系统配置 */
    ModbusConnection m_ModbusConnection; /**< 使用类型 */
    QModbusServer *m_modbusServer; /**< modbus服务器 */
};

#endif // C_MODBUSSERVER_H_INCLUDE_
