#ifndef C_MODBUSCLIENT_H_INCLUDE_
#define C_MODBUSCLIENT_H_INCLUDE_

#include <QModbusClient>
#include <QModbusReply>
#include <QSerialPort>
#include <QObject>

#include "common.h"
#include "QsLog.h"

class CModbusClient : public QObject
{
    Q_OBJECT

public:
    explicit CModbusClient(QObject *parent = nullptr,ModbusConnection connection=Modbus_Tcp);
    ~CModbusClient() override;

#if QT_CONFIG(modbus_serialport)
    /// 启动串口
    bool startupSerialPort(QString port,
                           int parity=QSerialPort::EvenParity,
                           int baud = QSerialPort::Baud19200,
                           int dataBits = QSerialPort::Data8,
                           int stopBits = QSerialPort::OneStop);
#endif
    /// 启动TCP
    bool startupTCP(QString ip,int port);
    /// 启动TCP
    bool startupTCP2(QUrl url);
    /// 关闭客户端
    void closeClient();

    /// 从服务器读取指定的数据
    bool readRequest(const QModbusDataUnit &read, int serverAddress);
    /// 向服务器写入指定的数据
    bool writeRequest(const QModbusDataUnit &write, int serverAddress);
    /// 向服务器读写指定的数据
    bool readwriteRequest(const QModbusDataUnit &read, const QModbusDataUnit &write, int serverAddress);

    /// 设置响应服务器的超时时间
    inline void setResponseServerTimeOut(int time=1000) { m_responseTimeout = time; }
    /// 得到响应服务器的超时时间
    inline int getResponseServerTimeOut() { return m_responseTimeout; }
    /// 设置客户端在请求失败之前将执行的重试次数。默认值设置为3
    inline void setNumberOfRetries(int retriy=3) { m_NumberOfRetries = retriy; }
    /// 得到客户端在请求失败之前将执行的重试次数
    inline int getNumberOfRetries() { return m_NumberOfRetries; }
    /// 检测客户端是否启动成功
    inline bool isClientStartupSuccessed() { return m_isClientStartupSuccessed; }
    /// 得到当前客户端连接状态
    inline int getCurrentConnectingState() { return m_currentConnectingState; }
    /// 设置服务器地址
    inline void setServerAddress(int ServerAddress) { m_ServerAddress = ServerAddress; }
    /// 得到服务器地址
    inline int getServerAddress() { return m_ServerAddress; }

signals:
    /// 日志消息
    void signalshowlog(QString msg);
    /// 网络状态变更
    void signalStateChanged(int state);
    /// 处理接收到的消息
    void signalDataWritten(QModbusDataUnit::RegisterType table, int address, QVector<quint16> data);

private:
    /// 建立Modbus客户端
    void createModbusClient();
    /// 打印用户日志
    void printLog(QsLogging::Level type,QString msg);
    /// 得到当前时间
    QString getCurrentDate();

private slots:
    void onStateChanged(int state);
    void onErrorOccurred(QModbusDevice::Error newError);
    void onClientReadReady();

private:
    QModbusClient *m_modbusClient;                /**< modbus客户端 */
    ModbusConnection m_ModbusConnection;          /**< 使用类型 */
    QModbusReply *m_modbuslastRequest;            /**< 处理modbus最新返回数据 */
    bool m_isClientStartupSuccessed;              /**< 客户端是否启动成功 */
    bool m_isClientExit;                          /**< 客户端是否已经退出 */
    int m_currentConnectingState;                 /**< 当前客户端连接状态 */
    int m_responseTimeout;                        /**< 响应服务器的超时时间 */
    int m_NumberOfRetries;                        /**< 客户端在请求失败之前将执行的重试次数 */
    int m_ServerAddress;                          /**< 要连接的服务器地址 */
};

#endif // C_MODBUSCLIENT_H_INCLUDE_
