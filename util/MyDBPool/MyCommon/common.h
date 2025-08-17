#ifndef COMMON_H
#define COMMON_H

#include <QApplication>
#include <QVector>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QHostAddress>
#include <QLibrary>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <windows.h>
#elif !defined(__unix)
#define __unix
#endif

#ifdef __unix
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif

#define BUF_SIZE                            1024*4           // 最大文件发送大小
#define MAX_FILENAME                        256              // 能发送的最大文件数量
#define SERIALPORT_BUF_SIZE                 1024             // 每次串口发送数据大小
#define IDD_TCP_VERSION                     1000             // tcp消息版本号
#define IDD_UDP_VERSION                     1001             // udp消息版本号
#define IDD_SERIALPORT_VERSION              2002             // 串口消息版本号

/********************< 版本管理协议 >**********************************/
#define IDD_SVNVERION_USER                  5100             // 用户操作
#define IDD_SVNVERION_REGISTER_USER         5101             // 注册用户
#define IDD_SVNVERION_UPDATE_USER           5102             // 更新用户
#define IDD_SVNVERION_DELETE_USER           5103             // 删除用户
#define IDD_SVNVERION_LOGIN_USER            5104             // 用户登陆
#define IDD_SVNVERION_GET_USERS             5105             // 得到用户
#define IDD_SVNVERION_USER_SUC              5106             // 用户操作成功
#define IDD_SVNVERION_USER_FAIL             5107             // 用户操作失败
#define IDD_SVNVERION_REGISTER_USER_EXIST   5108             // 用户已经存在

#define IDD_SVNVERSION_PROJECT              5200             // 项目管理
#define IDD_SVNVERSION_PROJECT_ADD          5201             // 添加项目
#define IDD_SVNVERSION_PROJECT_DEL          5202             // 删除项目
#define IDD_SVNVERSION_VERSION_ADD          5203             // 添加版本
#define IDD_SVNVERSION_VERSION_DAT_ADD      5204             // 添加版本数据
#define IDD_SVNVERSION_VERSION_DAT_GET      5205             // 得到版本数据
#define IDD_SVNVERSION_PROJECT_INFO_GET     5206             // 得到指定项目的信息
#define IDD_SVNVERSION_PROJECT_VERSIONS_GET 5207             // 得到指定项目的版本信息
#define IDD_SVNVERSION_PROJECT_SUC          5208             // 用户操作成功
#define IDD_SVNVERSION_PROJECT_FAIL         5209             // 用户操作失败
#define IDD_SVNVERSION_PROJECT_EXIST        5210             // 项目已经存在

#define IDD_SUBANDPUB_SUBSCRIBE             1000             // 订阅
#define IDD_SUBANDPUB_UNSUBSCRIBE           1001             // 退订
#define IDD_SUBANDPUB_PUBSCRIBE             1002             // 发布订阅消息
#define IDD_SUBANDPUB_SYNCPWD               1003             // 同步消息订阅密码
#define IDD_SUBANDPUB_IDENTITY              1004             // 身份认证
#define IDD_SUBANDPUB_SUCCESS               2000             // 操作成功
#define IDD_SUBANDPUB_FAIL                  2001             // 操作失败
#define IDD_SUBANDPUB_SUBMES_EXIST          2002             // 要订阅的消息已存在
#define IDD_SUBANDPUB_SUBMES_NOEXIST        2003             // 要订阅的消息不存在
#define IDD_SUBANDPUB_SUBMES_MISMATCH       2004             // 消息和相应的订阅人匹配
#define IDD_SUBANDPUB_PASSWORD_ERROR        2005             // 密码错误
#define IDD_SUBANDPUB_CLIENT_EXIST          2006             // 客户端已经存在

#define IDD_JS_MESSAGE_LOG                  11000           // 日志信息
#define IDD_JS_MESSAGE_SUCCESS              11001           // 操作成功
#define IDD_JS_MESSAGE_FAIL                 11002           // 操作失败
#define IDD_JS_MESSAGE_GETRESOURCEDATA      11003           // 得到资源数据
#define IDD_JS_MESSAGE_REGISTER             11004           // 注册设备
#define IDD_JS_MESSAGE_PUTRESOURCEDATA      11005           // 推送资源数据到设备
#define IDD_JS_MESSAGE_NODEVICE             11006           // 找不到设备
#define IDD_JS_MESSAGE_COMMAND              11007           // 控制命令
#define IDD_JS_MESSAGE_NODATA               11008           // 当前版本没有数据

#define IDD_JS_MESSAGE_COMMAND_RELOAD       11100           // 控制命令 - 重新导入脚本
#define IDD_JS_MESSAGE_COMMAND_AUTOLOAD     11101           // 控制命令 - 自动导入脚本

#define IDD_MYKVDB_CONTROL_SET              10000           // mykvdb - set
#define IDD_MYKVDB_CONTROL_GET              10001           // mykvdb - get
#define IDD_MYKVDB_CONTROL_REPLASE          10002           // mykvdb - replase
#define IDD_MYKVDB_CONTROL_DEL              10003           // mykvdb - del
#define IDD_MYKVDB_CONTROL_FAIL             10004           // mykvdb - fail
#define IDD_MYKVDB_CONTROL_SUCCESS          10005           // mykvdb - success

/********************************************************************/

//#define ENABLEREMOTEVOICE      1                // 是否启用remotevoice
//#define ENABLE_REMOTECAMERA                        // 是否启用remotecamera
//#define REMOTECAMERA_ENABLE_DARKNET               // remotecamera是否启用darknet
//#define ENABLE_IRRLICHT                           // 是否启用irrlicht
//#define ENABLE_QCEFVIEW                             // 是否启用QCefView
//#define ENABLE_WEBENGINE                          // 是否使用QWebEngine

class QWebSocket;

/**
 * @enum FileRecvError
 * @brief 文件接收错误返回。
 */
enum FileRecvError
{
    STATE_CHECKNUM = 0,        ///< 文件校验码不对
    STATE_SIZE,                ///< 文件长度不对
    STATE_NOTWRITE,            ///< 路径不对，无法写入
    STATE_SUCCESS,             ///< 接收成功
    STATE_START,               ///< 开始接收
    STATE_NULL                 ///< 空状态
};

/**
 * @enum DB_Type
 * @brief 要连接的数据库类型。
 */
enum DB_Type
{
    QMYSQL = 0,           ///< MYSQL
    QODBC,                ///< MSSQL
    QSQLITE,              ///< QSQLITE
    TYPE_NULL             ///< 空类型
};

/**
 * @enum LogLevel
 * @brief 日志等级。
 */
enum LogLevel
{
    LOG_DEBUG = 0,        ///< 调试
    LOG_INFO,             ///< 信息
    LOG_WARN,             ///< 警告
    LOG_ERROR,            ///< 错误
    LOG_FATAL             ///< 致命错误
};

/**
 * @enum ClientIdentity
 * @brief 客户端身份。
 */
enum ClientIdentity
{
    IDENTITY_PUBLER = 0,       ///< 发布者
    IDENTITY_SERVER,           ///< 服务器
    IDENTITY_NULL              ///< 空身份
};

/**
 * @struct tagPubMessage
 * @brief 发布的消息结构。
 */
struct tagPubMessage
{
    tagPubMessage(){}
    tagPubMessage(QString m,QString c,QString p,QString t)
        : message(m),content(c),password(p),time(t) {}

    QString message;             ///< 消息名称
    QString content;             ///< 消息内容
    QString password;            ///< 消息密码
    QString time;                ///< 消息发送时间
};

/**
 * @enum ModbusConnection
 * @brief Modbus 连接类型。
 */
enum ModbusConnection
{
    Modbus_Serial = 0,           ///< 串口连接
    Modbus_Tcp                   ///< TCP 连接
};

/**
 * @struct tagDatagramHearder
 * @brief 要发送串口报文头。
 */
#pragma pack(push,1)
struct tagDatagramHearder
{
    int version;                 ///< 版本号
    unsigned int srcdataSize;    ///< 原始数据大小
    unsigned int compressdataSize; ///< 压缩后数据大小
    unsigned int checknum;       ///< 数据校验码
};
#pragma pack(pop)

/**
 * @struct tagUdpDatagramHearder
 * @brief 要发送 UDP 报文头。
 */
#pragma pack(push,1)
struct tagUdpDatagramHearder
{
    char version[3];             ///< 版本号 'K','C','P'
    unsigned int srcdataSize;    ///< 原始数据大小
    unsigned int compressdataSize; ///< 压缩后数据大小
    unsigned int checknum;       ///< 数据校验码
};
#pragma pack(pop)

/**
 * @struct tagFileStruct
 * @brief 要发送的文件结构。
 */
#pragma pack(push,1)
struct tagFileStruct
{
    char mark[3];                ///< 标记 'F','I','L'
    char fileName[MAX_FILENAME]; ///< 文件所在相对路径
    qint64 srcfileSize;          ///< 原始文件大小
    qint64 compressfileSize;     ///< 压缩后文件大小
    quint16 checknum;            ///< 数据校验码
};
#pragma pack(pop)

/**
 * @struct tagDataStruct
 * @brief 要发送的数据结构。
 */
#pragma pack(push,1)
struct tagDataStruct
{
    char mark[3];                ///< 标记 'D','A','T'
    qint64 srcdataSize;          ///< 原始数据大小
    qint64 compressdataSize;     ///< 压缩后数据大小
    quint16 checknum;            ///< 数据校验码
};
#pragma pack(pop)

/**
 * @struct tagVersionOper
 * @brief 用于版本操作。
 */
#pragma pack(push,1)
struct tagVersionOper
{
    tagVersionOper()
        : tagIndex(0),versionid(0),userid(0),
          mainid(0),subid(0)
    {
        memset(userpwd,0,sizeof(userpwd));
    }

    int tagIndex;                ///< 标签索引
    int mainid;                  ///< 主 ID
    int subid;                   ///< 子 ID
    int versionid;               ///< 版本 ID
    int userid;                  ///< 用户 ID
    char userpwd[64];            ///< 用户密码
};
#pragma pack(pop)

/**
 * @struct tagSVNVersionFileStruct
 * @brief SVN 版本文件结构。
 */
struct tagSVNVersionFileStruct
{
    char fileName[MAX_FILENAME]; ///< 文件所在相对路径
    int fileSize;                ///< 文件大小
    int fileChanged;             ///< 文件是否改变：0-没改变；1-改变了
};

/**
 * @struct tagSVNVersionPathStruct
 * @brief SVN 版本工作目录结构。
 */
struct tagSVNVersionPathStruct
{
    char tag[3];                 ///< 标记 'S','V','N'
    int fileCount;               ///< 文件数量
    int srcDataSize;             ///< 原始数据大小
};

/**
 * @struct tagKVFileInfo
 * @brief KV 文件信息。
 */
struct tagKVFileInfo
{
    tagKVFileInfo():keyCount(0) {}

    char tag[2];                 ///< 标记 'K','V'
    int keyCount;                ///< key 的数量
};

/**
 * @struct tagKVFileItemInfo
 * @brief KV 文件项信息。
 */
struct tagKVFileItemInfo
{
    tagKVFileItemInfo()
        : dateType(-1),dataSize(0),index(-1),
          storageType(-1)
    {
        memset(key,0,64);
    }
    tagKVFileItemInfo(int iindex,QString kkey,int stype)
        : index(iindex),storageType(stype)
    {
        memset(key,0,64);
        QByteArray tmpData = kkey.toUtf8();
        memcpy((char*)key,(char*)tmpData.data(),tmpData.length());
    }

    int index;                   ///< 块索引
    char key[64];                ///< key
    int dateType;                ///< 块数据类型
    int dataSize;                ///< 块数据大小
    int storageType;             ///< 存储类型:0-增加；1-更改；2-删除
};

/**
 * @struct tagUserData
 * @brief 用户登录后获取到的用户数据。
 */
struct tagUserData
{
    tagUserData():userid(0),type(0) {}
    tagUserData(int puserid,QString pname,QString ppwd,int ptype)
        : userid(puserid),name(pname),pwd(ppwd),type(ptype) {}

    int userid;                  ///< 用户 ID
    QString name;                ///< 用户名
    QString pwd;                 ///< 密码
    int type;                    ///< 用户类型
    QMap<int,QVector<int>> projects; ///< 项目列表
    QVector<int> devices;        ///< 设备列表
};

/**
 * @struct tagSubMsgItem
 * @brief 订阅消息项。
 */
struct tagSubMsgItem
{
    tagSubMsgItem() {}
    tagSubMsgItem(QString mi,QString mes,QString con)
        : msgId(mi),message(mes),content(con) {}

    QString msgId;          ///< 消息 ID
    QString message;        ///< 要订阅的消息
    QString content;        ///< 订阅的消息内容
};

/**
 * @struct tagSendFailMsgItem
 * @brief 发送失败的消息项。
 */
struct tagSendFailMsgItem
{
    tagSendFailMsgItem()
        : receiver(IDENTITY_NULL) {}
    tagSendFailMsgItem(ClientIdentity identity,QString c,QString m)
        : receiver(identity),contents(c),message(m) {}

    QString message;                ///< 订阅的消息
    QString contents;               ///< 要发送的消息
    ClientIdentity receiver;        ///< 接收者
};

/**
 * @struct tagSendFailClient
 * @brief 发送失败的客户端。
 */
struct tagSendFailClient
{
    tagSendFailClient()
        : identity(IDENTITY_NULL),socket(NULL) {}
    tagSendFailClient(ClientIdentity ci,QString id,QWebSocket *s)
        : identity(ci),uuid(id),socket(s) {}

    ClientIdentity identity;        ///< 接收者类型
    QString uuid;                   ///< 接收者 ID
    QWebSocket *socket;             ///< socket
};

/**
 * @struct tagBrotherServerItem
 * @brief 兄弟服务器项。
 */
struct tagBrotherServerItem
{
    tagBrotherServerItem()
        : port(0) {}
    tagBrotherServerItem(QString iip,int pport)
        : ip(iip),port(pport) {}

    QString ip;             ///< 服务器 IP
    int port;               ///< 服务器端口
};

/**
 * @struct tagpluginItem
 * @brief 插件项。
 */
struct tagpluginItem
{
    tagpluginItem()
        : obj(NULL),lib(NULL) {}
    tagpluginItem(QString pname,QObject *pobj,QLibrary *plib=NULL)
        : name(pname),obj(pobj),lib(plib) {}

    /**
     * @brief 清除插件项。
     *
     * 删除对象和卸载库。
     */
    inline void clear()
    {
        if(obj) obj->deleteLater();

        if(lib)
        {
            lib->unload();
            lib->deleteLater();
        }
    }

    QString name;           ///< 插件名称
    QObject *obj;           ///< 插件对象
    QLibrary *lib;          ///< 插件库
};

/**
 * @brief 获取系统时间。
 *
 * @param sec 秒指针。
 * @param usec 微秒指针。
 */
static inline void itimeofday(long *sec, long *usec)
{
#if defined(__unix)
    struct timeval time;
    gettimeofday(&time, NULL);
    if (sec) *sec = time.tv_sec;
    if (usec) *usec = time.tv_usec;
#else
    static long mode = 0, addsec = 0;
    BOOL retval;
    static long long freq = 1;
    long long qpc;
    if (mode == 0) {
        retval = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        freq = (freq == 0) ? 1 : freq;
        retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
        addsec = (long)time(NULL);
        addsec = addsec - (long)((qpc / freq) & 0x7fffffff);
        mode = 1;
    }
    retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
    retval = retval * 2;
    if (sec) *sec = (long)(qpc / freq) + addsec;
    if (usec) *usec = (long)((qpc % freq) * 1000000 / freq);
#endif
}

/**
 * @brief 获取 64 位毫秒时钟。
 *
 * @return long long 当前时间的毫秒数。
 */
static inline long long iclock64()
{
    long s, u;
    long long value;
    itimeofday(&s, &u);
    value = ((long long)s) * 1000 + (u / 1000);
    return value;
}

/**
 * @brief 获取 32 位毫秒时钟。
 *
 * @return unsigned int 当前时间的毫秒数（32 位）。
 */
static inline unsigned int iclock()
{
    return (unsigned)(iclock64() & 0xfffffffful);
}

/** @def MAX_FASTLZ_BUF_SIZE
 * @brief FastLZ 缓冲区最大大小。
 */
#define MAX_FASTLZ_BUF_SIZE 10240

/**
 * @brief 初始化日志文件。
 *
 * @param filepath 日志文件路径。
 */
void init_log_file(const QString& filepath);

/**
 * @brief 打印日志。
 *
 * @param level 日志等级。
 * @param msg 日志消息。
 * @param isprintscreen 是否输出到屏幕。
 */
void print_log(LogLevel level, const QString &msg, const QString &isprintscreen="");

/**
 * @brief 保存源文件到目标文件（主要网络传输中使用）。
 *
 * @param srcfile 源文件路径（相对路径）。
 * @param decfile 目标文件路径（绝对路径）。
 * @return bool 保存成功返回 true，否则返回 false。
 */
bool sava_file(QString srcfile,QString decfile);

/**
 * @brief 初始化崩溃系统。
 */
void init_dump_system();

/**
 * @brief 初始化数据库连接池。
 *
 * @param paramIsDebug 是否为调试模式。
 * @param parent 父对象。
 */
void init_dbpool(bool paramIsDebug = false,QObject * parent = 0);

/**
 * @brief 卸载数据库连接池。
 */
void clean_dbpool();

/**
 * @brief 得到当前 IP。
 *
 * @param protocolType 协议类型（默认为 IPv4）。
 * @return QHostAddress 当前 IP 地址。
 */
QHostAddress getLocalIP(int protocolType=QAbstractSocket::IPv4Protocol);

/**
 * @brief 拷贝文件。
 *
 * @param sourceDir 源文件路径。
 * @param toDir 目标文件路径。
 * @param coverFileIfExist 是否覆盖已存在的文件。
 * @return bool 拷贝成功返回 true，否则返回 false。
 */
bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);

/**
 * @brief 拷贝文件夹。
 *
 * @param fromDir 源文件夹路径。
 * @param toDir 目标文件夹路径。
 * @param coverFileIfExist 是否覆盖已存在的文件。
 * @return bool 拷贝成功返回 true，否则返回 false。
 */
bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);

/**
 * @brief 得到指定目录下所有的文件。
 *
 * @param _filePath 要得到文件的目录。
 * @param filelist 用于存放读取到的文件。
 * @param isContanDir 是否包含目录。
 * @param isContanSubDir 是否包含子目录。
 * @return int 成功返回 0，失败返回 -1。
 */
int FindFile(const QString& _filePath,QVector<QString> &filelist,
             bool isContanDir=false,
             bool isContanSubDir=true);

/**
 * @brief QJsonObject 转 QString。
 *
 * @param json 要转换的 QJsonObject。
 * @return QString 转换后的字符串。
 */
QString JsonToString(const QJsonObject& json);

/**
 * @brief QJsonArray 转 QString。
 *
 * @param json 要转换的 QJsonArray。
 * @return QString 转换后的字符串。
 */
QString JsonArrayToString(const QJsonArray& json);

/**
 * @brief QString 转 QJsonObject。
 *
 * @param str 要转换的字符串。
 * @return QJsonObject 转换后的 QJsonObject。
 */
QJsonObject StringToJson(const QString& str);

/**
 * @brief QString 转 QJsonArray。
 *
 * @param str 要转换的字符串。
 * @return QJsonArray 转换后的 QJsonArray。
 */
QJsonArray StringToJsonArray(const QString& str);

/**
 * @brief QJsonObject 转 QByteArray。
 *
 * @param json 要转换的 QJsonObject。
 * @return QByteArray 转换后的字节数组。
 */
QByteArray ObjectToByteArray(const QJsonObject& json);

/**
 * @brief QByteArray 转 QJsonObject。
 *
 * @param array 要转换的字节数组。
 * @return QJsonObject 转换后的 QJsonObject。
 */
QJsonObject ByteArrayToObject(const QByteArray& array);

/**
 * @brief 删除目录下所有文件和文件夹。
 *
 * @param folderDir 目录路径。
 * @param isDeleteDir 是否删除目录。
 * @return bool 删除成功返回 true，否则返回 false。
 */
bool removeFolderContent(const QString &folderDir,bool isDeleteDir=true);

/**
 * @brief 设置开机自启动。
 *
 * @param bAutoRun 是否开机自启动。
 */
void appAutoRun(bool bAutoRun);

/**
 * @brief fastlz 压缩数据。
 *
 * @param srcdata 源数据。
 * @param decdata 压缩后的数据。
 * @return bool 压缩成功返回 true，否则返回 false。
 */
bool FastlzCompressData(QByteArray srcdata,QByteArray &decdata);

/**
 * @brief fastlz 解压数据。
 *
 * @param srcdata 压缩数据。
 * @param decdata 解压后的数据。
 * @param deccSize 解压后数据大小。
 * @return bool 解压成功返回 true，否则返回 false。
 */
bool FastlzDecompressData(QByteArray srcdata, QByteArray &decdata,qint32 deccSize);

/**
 * @brief IP 地址格式化。
 *
 * @param addr IP 地址。
 * @return QString 格式化后的 IP 地址。
 */
QString convert_to_ipv4_addr(const QHostAddress& addr);

/**
 * @brief 创建指定路径的文件或者文件夹。
 *
 * @param path 路径。
 * @param type 0-文件；1-文件夹。
 * @return bool 创建成功返回 true，否则返回 false。
 */
bool CreateTheFileOrFolder(const QString& path,int type=0);

/**
 * @brief 删除指定路径的文件或者文件夹。
 *
 * @param path 路径。
 * @return bool 删除成功返回 true，否则返回 false。
 */
bool DeleteTheFileOrFolder(const QString& path);

/**
 * @brief 简单加解密。
 *
 * @param data 数据。
 * @param keyone 密钥一。
 * @param keytwo 密钥二。
 * @return QByteArray 加解密后的数据。
 */
QByteArray EncData(QByteArray data,const QString& keyone,const QString& keytwo);

/**
 * @brief 生成字符串的 MD5 值。
 *
 * @param str 字符串。
 * @return QString MD5 值。
 */
QString generateMd5(const QString& str);

/**
 * @brief 生成文件内容的 MD5 值。
 *
 * @param filePath 文件路径。
 * @return QString MD5 值。
 */
QString generateFileMd5(const QString& filePath);

/**
 * @brief 得到相应数据的 MD5。
 *
 * @param data 数据。
 * @return QString MD5 值。
 */
QString getByteArrayMd5(QByteArray data);

/**
 * @brief 转换 RGB888 到 YUV420P。
 *
 * @param imagePath 图像路径。
 * @param width 宽度。
 * @param height 高度。
 * @param yuvData YUV 数据。
 * @return bool 转换成功返回 true，否则返回 false。
 */
bool convertRGBToYUV420P(const QString& imagePath, int width, int height, QByteArray& yuvData);

/**
 * @brief 转换 QImage 到 YUV420P。
 *
 * @param image QImage 对象。
 * @param yuvData YUV 数据。
 * @return bool 转换成功返回 true，否则返回 false。
 */
bool convertImageToYUV420P(const QImage& image, QByteArray& yuvData);

/**
 * @brief 字符串转 QSize。
 *
 * @param str 字符串。
 * @param separator 分隔符。
 * @return QSize 转换后的 QSize。
 */
QSize convertStringToSize(const QString& str,const QString& separator=",");

/**
 * @brief 字符串转 QPoint。
 *
 * @param str 字符串。
 * @param separator 分隔符。
 * @return QPoint 转换后的 QPoint。
 */
QPoint convertStringToPoint(const QString& str,const QString& separator=",");

/**
 * @brief 字符串转 QRect。
 *
 * @param str 字符串。
 * @param separator 分隔符。
 * @return QRect 转换后的 QRect。
 */
QRect convertStringToRect(const QString& str,const QString& separator=",");

#endif // COMMON_H