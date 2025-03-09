#ifndef C_CANBUS_H_INCLUDE_
#define C_CANBUS_H_INCLUDE_

#include <QCanBusDevice>
#include <QObject>
#include <QVector>
#include <QVariant>
#include <QTimer>

#include "QsLog.h"

class ccanbus : public QObject
{
    Q_OBJECT

public:
    typedef QPair<QCanBusDevice::ConfigurationKey, QVariant> ConfigurationItem;

    explicit ccanbus(QObject *parent = nullptr);
    ~ccanbus() override;

    /// 建立设备
    bool createDevice(const QString &plugin,
                      const QString &interfaceName,
                      QVector<ConfigurationItem> configurationitems=QVector<ConfigurationItem>());
    /// 断开设备的连接
    void disconnectDevice();

    /// 发送一帧数据
    bool sendFrame(const QCanBusFrame &frame);
    /// 得到帧数据类型
    QString frameFlags(const QCanBusFrame &frame);

signals:
    /// 日志消息
    void signalshowlog(QString msg);
    /// 处理接收到的数据
    void signalReciveData(QCanBusFrame frame);

private:
    /// 打印用户日志
    void printLog(QsLogging::Level type,QString msg);
    /// 得到当前时间
    QString getCurrentDate();

private slots:
    void processReceivedFrames();
    void processErrors(QCanBusDevice::CanBusError);
    void processFramesWritten(qint64);
    void busStatus();

private:
    QCanBusDevice *m_CanBusDevice;
    QTimer *m_busStatusTimer;
    qint64 m_numberFramesWritten;
};

#endif // C_CANBUS_H_INCLUDE_
