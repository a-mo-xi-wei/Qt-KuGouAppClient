#ifndef _C_EQUIPMENTMANAGER_SERVER_H
#define _C_EQUIPMENTMANAGER_SERVER_H

#include "csvnversionserver.h"
#include "cwebsocketserver.h"
//#include "cudpsocket.h"
#include "networkframemanager.h"
#include "sqlitedataprovider.h"

#include <QObject>
#include <QHash>
#include <QUdpSocket>

#define IDD_EMS_MESSAGE_SUCCESS               9000          // 操作成功
#define IDD_EMS_MESSAGE_FAIL                  9001          // 操作失败
#define IDD_EMS_MESSAGE_REGISTERD             9002          // 设备注册
#define IDD_EMS_MESSAGE_GETDEVICELIST         9003          // 得到设备列表
#define IDD_EMS_MESSAGE_CONTROLDEVICE         9004          // 设备控制
#define IDD_EMS_MESSAGE_CLOSECONTROLDEVICE    9005          // 关闭当前设备控制
#define IDD_EMS_MESSAGE_LOGIN                 9006          // 用户登录
#define IDD_EMS_MESSAGE_GETDEVICEDATA         9007          // 得到某台设备的数据
#define IDD_EMS_MESSAGE_DEVICE_STATE          9008          // 设备状态
#define IDD_EMS_MESSAGE_VERSIONTODEVICE       9009          // 下载版本到设备上
#define IDD_EMS_MESSAGE_ADD_DEVICE            9010          // 添加设备
#define IDD_EMS_MESSAGE_DELETE_DEVICE         9011          // 删除设备
#define IDD_EMS_MESSAGE_UPDATE_DEVICE         9012          // 更新设备信息
#define IDD_EMS_MESSAGE_PROJECTINFO           9013          // 得到所有的项目信息
#define IDD_EMS_MESSAGE_P2P_DEVICE            9014          // 和设备建立P2P连接

struct tagP2pDevice
{
    tagP2pDevice()
        : senderPort(-1) {}
    tagP2pDevice(QHostAddress psender,quint16 psenderPort)
        : sender(psender),senderPort(psenderPort) {}

    QHostAddress sender;
    quint16 senderPort;
};

struct tagEquipmentItem
{
    tagEquipmentItem()
        : webClient(NULL),deviceID(-1) { }
    tagEquipmentItem(tagP2pDevice pudpsocket,QWebSocket *pwebclient,int ptype,int pdeviceID)
        : udpClient(pudpsocket),webClient(pwebclient),deviceID(pdeviceID),type(ptype) {}

    int deviceID;
    int type;                      /**< 0 - 控制设备；1 - 受控设备 */
    tagP2pDevice udpClient;         /**< 反向代理的设备 */
    QWebSocket *webClient;         /**< 代理的设备 */
};

class CEquipmentManagerServer : public QObject , public NetworkFrameManager
{
    Q_OBJECT

public:
    explicit CEquipmentManagerServer(QObject *parent = nullptr);
    ~CEquipmentManagerServer();

    /// 设置版本库所使用数据库
    void setDataBase(QString dbPath);
    /// 设置代理服务器启动参数
    void setAgentServerStartupParams(int serverPort,int maxClients);
    /// 设置反向代理服务器启动参数
    inline void setReverseProxyServerStartupParams(int serverPort) { m_ReverseProxyServerPort = serverPort; }
    /// 服务器是否已经启动
    bool isStartup(void);

    /// 启动服务
    void startupService(void);
    /// 停止服务
    void stopService(void);

    /// 处理网络字符串消息
    virtual void OnProcessNetText(QWebSocket *conn,QString mes);
    /// 处理网络二进制消息
    virtual void OnProcessNetBinary(QWebSocket *conn,QByteArray &data);
    /// 处理一个新的连接到达
    virtual void OnProcessConnectedNetMes(QWebSocket *conn);
    /// 处理一个连接关闭
    virtual void OnProcessDisconnectedNetMes(QWebSocket *conn);

private:
    /// 设备注册
    void onProcessNetEquipmentRegister(QWebSocket *conn,QJsonObject &mesObj);
    /// 得到设备列表
    void onProcessNetEquipmentGetDeviceList(QWebSocket *conn,QJsonObject &mesObj);
    /// 用户登录
    void onProcessNetEquipmentUserLogin(QWebSocket *conn,QJsonObject &mesObj);
    /// 得到指定设备的数据
    void onProcessNetEquipmentGetData(QWebSocket *conn,QJsonObject &mesObj);
    /// 推送资源到指定设备
    void onProcessNetPutResourceToDevice(QWebSocket *conn,QJsonObject &mesObj);
    /// 用户命令
    void onProcessNetUserCommand(QWebSocket *conn,QJsonObject &mesObj);
    /// 下载版本到设备上
    void onProcessNetDownloadVersiontoDevice(QWebSocket *conn,QJsonObject &mesObj);
    /// 添加设备
    void onProcessNetAddDevice(QWebSocket *conn,QJsonObject &mesObj);
    /// 删除设备
    void onProcessNetDeleteDevice(QWebSocket *conn,QJsonObject &mesObj);
    /// 更新设备信息
    void onProcessNetUpdateDevice(QWebSocket *conn,QJsonObject &mesObj);
    /// 得到所有项目的信息
    void onProcessNetGetAllProjectInfo(QWebSocket *conn,QJsonObject &mesObj);
    /// 和设备建立P2P连接
    void onProcessNetP2pDevice(QWebSocket *conn,QJsonObject &mesObj);

private:
    /// 检测指定用户ID和密码的用户是否存在
    bool isExistUser(QWebSocket *conn,int id,QString pwd);
    /// 根据设备ID得到设备信息
    tagEquipmentItem* getDeviceInfoByID(int usertype,int deviceID);
    /// 向所有控制设备广播指定ID的受控设备的状态
    void sendDeviceStateToAllUsers(int devicdID,int state);
    /// 得到指定玩家的设备列表
    QVector<int> getUserControlDevices(int userId);
    /// 得到指定ID保存的版本数据
    QByteArray getVersionData(int id);

private slots:
    /// 处理收到的客户端信息
    void ReverseProxyServer_reciverPendingDatagram();

private:
    SqliteDataProvider m_sqliteDataProvider;                     /**< 用于数据库操作  */
    CWebSocketServer m_AgentServer;                              /**< 代理服务器 */
    int m_AgentServerPort,m_AgentServerMaxClients;               /**< 代理服务器启动的端口和能接受的最大客户端数量 */
    QUdpSocket m_ReverseProxyServer;                             /**< 反向代理服务器，用于设备之间p2p通讯 */
    int m_ReverseProxyServerPort;                                /**< 反向代理服务器端口 */

    QHash<QWebSocket*,tagEquipmentItem> m_Onlineequipmentlist;   /**< 在线设备列表 */
    QHash<int,QHash<int,QWebSocket*>> m_onlineDevicelist;        /**< 当前在线的设备列表 */
    QHash<int,QWebSocket*> m_onlineUserList;                     /**< 当前在线的用户列表 */
    QHash<QWebSocket*,QWebSocket*> m_onlinecontrolClientlist;    /**< 在线控制用户和受控设备的关系 */
};

#endif // CEQUIPMENTMANAGERSERVER_H
