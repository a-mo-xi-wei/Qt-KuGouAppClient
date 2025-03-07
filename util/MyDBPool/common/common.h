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

#if defined (WIN32)
    //#define ENALBE_NEDALLOC  1                      // 默认启用Nedalloc
    #define ENABLE_TCMALLOC  1                      // 启用tcmalloc
#else
    #define ENABLE_TCMALLOC  1                      // 启用tcmalloc
#endif

class QWebSocket;

/**
 * @brief The FileRecvError enum 文件接收错误返回
 */
enum FileRecvError
{
    STATE_CHECKNUM = 0,        // 文件校验码不对
    STATE_SIZE,                // 文件长度不对
    STATE_NOTWRITE,            // 路径不对，无法写入
    STATE_SUCCESS,             // 接收成功
    STATE_START,               // 开始接收
    STATE_NULL
};

/**
 * @brief The DB_Type enum 要连接的数据库类型
 */
enum DB_Type
{
    QMYSQL = 0,           // MYSQL
    QODBC,                // MSSQL
    QSQLITE,              // QSQLITE
    TYPE_NULL
};

enum LogLevel
{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

enum ClientIdentity
{
    IDENTITY_PUBLER = 0,       // 发布者
    IDENTITY_SERVER,           // 服务器
    IDENTITY_NULL
};

struct tagPubMessage
{
    tagPubMessage(){}
    tagPubMessage(QString m,QString c,QString p,QString t)
        : message(m),content(c),password(p),time(t) {}

    QString message;             /**< 消息名称 */
    QString content;             /**< 消息内容 */
    QString password;            /**< 消息密码 */
    QString time;                /**< 消息发送时间 */
};

enum ModbusConnection
{
    Modbus_Serial = 0,
    Modbus_Tcp
};

/**
 * @brief 要发送串口报文头
 */
#pragma pack(push,1)
struct tagDatagramHearder
{
    int version;
    unsigned int srcdataSize;            // 原始数据大小
    unsigned int compressdataSize;       // 压缩后数据大小
    unsigned int checknum;              // 数据校验码
};
#pragma pack(pop)

/**
 * @brief 要发送udp报文头
 */
#pragma pack(push,1)
struct tagUdpDatagramHearder
{
    char version[3];                     // 'K','C','P'
    unsigned int srcdataSize;            // 原始数据大小
    unsigned int compressdataSize;       // 压缩后数据大小
    unsigned int checknum;              // 数据校验码
};
#pragma pack(pop)

/**
 * @brief The tagFileStruct struct 要发送的文件结构
 */
#pragma pack(push,1)
struct tagFileStruct
{
    char mark[3];                  // 'F','I','L'
    char fileName[MAX_FILENAME];            // 文件所在相对路径
    qint64 srcfileSize;            // 原始文件大小
    qint64 compressfileSize;       // 压缩后文件大小
    quint16 checknum;              // 数据校验码
};
#pragma pack(pop)

/**
 * 要发送的数据结构
 */
#pragma pack(push,1)
struct tagDataStruct
{
    char mark[3];                  // 'D','A','T'
    qint64 srcdataSize;            // 原始数据大小
    qint64 compressdataSize;       // 压缩后数据大小
    quint16 checknum;              // 数据校验码
};
#pragma pack(pop)

/**
 * 用于版本操作
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

    int tagIndex;
    int mainid;
    int subid;
    int versionid;
    int userid;
    char userpwd[64];
};
#pragma pack(pop)

/**
 * @brief The tagSVNVersionFileStruct struct svn版本文件结构
 */
struct tagSVNVersionFileStruct
{
    char fileName[MAX_FILENAME];            // 文件所在相对路径
    int fileSize;                           // 文件大小
    int fileChanged;                        // 文件是否改变：0-没改变；1-改变了
};

/**
 * @brief The tagSVNVersionPathStruct struct svn版本工作目录结构
 */
struct tagSVNVersionPathStruct
{
    char tag[3];                            // 'S','V','N'
    int fileCount;                          // 文件数量
    int srcDataSize;               // 原始数据大小
};

struct tagKVFileInfo
{
    tagKVFileInfo():keyCount(0) {}

    char tag[2];                 // 'K','V'
    int keyCount;                // key的数量
};

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

    int index;                   // 块索引
    char key[64];                // key
    int dateType;                // 块数据类型
    int dataSize;                // 块数据大小
    int storageType;             // 存储类型:0-增加；1-更改；2-删除
};

/**
 * @brief The tagUserData struct 用户登陆以后获取到的用户数据
 */
struct tagUserData
{
    tagUserData():userid(0),type(0) {}
    tagUserData(int puserid,QString pname,QString ppwd,int ptype)
        : userid(puserid),name(pname),pwd(ppwd),type(ptype) {}

    int userid;
    QString name;
    QString pwd;
    int type;
    QMap<int,QVector<int>> projects;
    QVector<int> devices;
};

struct tagSubMsgItem
{
    tagSubMsgItem() {}
    tagSubMsgItem(QString mi,QString mes,QString con)
        : msgId(mi),message(mes),content(con) {}

    QString msgId;          // 消息ID
    QString message;        // 要订阅的消息
    QString content;        // 订阅的消息内容
};

struct tagSendFailMsgItem
{
    tagSendFailMsgItem()
        : receiver(IDENTITY_NULL) {}
    tagSendFailMsgItem(ClientIdentity identity,QString c,QString m)
        : receiver(identity),contents(c),message(m) {}

    QString message;                // 订阅的消息
    QString contents;               // 要发送的消息
    ClientIdentity receiver;        // 接收者
};

struct tagSendFailClient
{
    tagSendFailClient()
        : identity(IDENTITY_NULL),socket(NULL) {}
    tagSendFailClient(ClientIdentity ci,QString id,QWebSocket *s)
        : identity(ci),uuid(id),socket(s) {}

    ClientIdentity identity;        // 接收者类型
    QString uuid;                   // 接收者ID
    QWebSocket *socket;             // socket
};

struct tagBrotherServerItem
{
    tagBrotherServerItem()
        : port(0) {}
    tagBrotherServerItem(QString iip,int pport)
        : ip(iip),port(pport) {}

    QString ip;             // 服务器IP
    int port;               // 服务器端口
};

struct tagpluginItem
{
    tagpluginItem()
        : obj(NULL),lib(NULL) {}
    tagpluginItem(QString pname,QObject *pobj,QLibrary *plib=NULL)
        : name(pname),obj(pobj),lib(plib) {}

    inline void clear(void)
    {
        if(obj) obj->deleteLater();

        if(lib)
        {
            lib->unload();
            lib->deleteLater();
        }
    }

    QString name;
    QObject *obj;
    QLibrary *lib;
};

/* get system time */
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

/* get clock in millisecond 64 */
static inline long long iclock64(void)
{
    long s, u;
    long long value;
    itimeofday(&s, &u);
    value = ((long long)s) * 1000 + (u / 1000);
    return value;
}

static inline unsigned int iclock()
{
    return (unsigned)(iclock64() & 0xfffffffful);
}

#define MAX_FASTLZ_BUF_SIZE 10240

/// 加载本静态库的资源
void init_lib_resources(void);
/// 卸载本静态库的资源
void cleanup_lib_resources(void);
/// 生成日志文件
void init_log_file(const QString& filepath);
/// 打印日志
void print_log(LogLevel level, const QString &msg, const QString &isprintscreen="");
/// 保存源文件到目标文件(主要网络传输中使用)
bool sava_file(QString srcfile,QString decfile);
/// 初始化崩溃系统
void init_dump_system(void);
/// 初始化数据库连接池
void init_dbpool(bool paramIsDebug = false,QObject * parent = 0);
/// 卸载数据库连接池
void clean_dbpool(void);
/// 得到当前IP
QHostAddress getLocalIP(int protocolType=QAbstractSocket::IPv4Protocol);
/// 拷贝文件
bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
/// 拷贝文件夹
bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
/// 得到指定目录下所有的文件
int FindFile(const QString& _filePath,QVector<QString> &filelist,
             bool isContanDir=false,
             bool isContanSubDir=true);
/// QJsonObject转QString
QString JsonToString(const QJsonObject& json);
/// QJsonArray转QString
QString JsonArrayToString(const QJsonArray& json);
/// QString转QJsonObject
QJsonObject StringToJson(const QString& str);
/// QString转QJsonArray
QJsonArray StringToJsonArray(const QString& str);
/// QJsonObject转QByteArray
QByteArray ObjectToByteArray(const QJsonObject& json);
/// QByteArray转QJsonObject
QJsonObject ByteArrayToObject(const QByteArray& array);
/// 删除目录下所有文件和文件夹
bool removeFolderContent(const QString &folderDir,bool isDeleteDir=true);
/// 设置开机自启动
void appAutoRun(bool bAutoRun);
/// fastlz压缩数据
bool FastlzCompressData(QByteArray srcdata,QByteArray &decdata);
/// fastlz解压数据
bool FastlzDecompressData(QByteArray srcdata,QByteArray &decdata,qint32 deccSize);
/// IP地址格式化
QString convert_to_ipv4_addr(QHostAddress addr);
/// 创建指定路径的文件或者文件夹(0-文件；1-文件夹)
bool CreateTheFileOrFolder(QString path,int type=0);
/// 删除指定路径的文件或者文件夹
bool DeleteTheFileOrFolder(QString path);
/// 简单加解密
QByteArray EncData(QByteArray data,QString keyone,QString keytwo);
// 生成字符串的MD5值
QString generateMd5(const QString& str);
// 生成文件内容的MD5值
QString generateFileMd5(const QString& filePath);
/// 得到相应数据的MD5
QString getByteArrayMd5(QByteArray data);
// 转换RGB888到YUV420P
bool convertRGBToYUV420P(const QString& imagePath, int width, int height, QByteArray& yuvData);
bool convertImageToYUV420P(QImage& image, QByteArray& yuvData);
// 字符串转QSize
QSize convertStringToSize(QString str,QString separator=",");
// 字符串转QPoint
QPoint convertStringToPoint(QString str,QString separator=",");
// 字符串转QRect
QRect convertStringToRect(QString str,QString separator=",");

#endif // COMMON_H
