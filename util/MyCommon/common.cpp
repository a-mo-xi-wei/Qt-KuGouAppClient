#include "common.h"
#include "fastlz.h"
#include "NedAllocatorImpl.h"
#include "../MyDBPool/ndbpool.h"
#include "../MyQsLog/QsLog.h"

#include "../MyBreakPad/exception_handler.h"

#include <iostream>
#include <QDir>
#include <QImage>
#include <QHostInfo>
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>

#if defined (WIN32)
#include <sapi.h>
#include <sphelper.h>
#endif

#pragma comment(lib, "sapi.lib")

using namespace QsLogging;
static NDBPool *mNDBPool = nullptr;

#if defined (WIN32)
bool dump_callback(const wchar_t *dump_path, const wchar_t *id, void *content, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded)
{
    if(succeeded)
    {
        QLOG_ERROR()<<"系统已经崩溃,dmp已经生成成功,请联系开发人员!";
    }
    else
    {
        QLOG_ERROR()<<"系统已经崩溃,dmp生成失败,请联系开发人员!";
    }

    return succeeded;
}
#else
static bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
                            void* context, bool succeeded)
{
  QLOG_ERROR()<< "Dump path:" << descriptor.path();
  qDebug()<< "Dump path:" << descriptor.path();
  //printf("Dump path: %s\n", descriptor.path());

  return succeeded;
}
#endif

/**
 * @brief init_lib_resources 加载本静态库的资源
 */
void init_lib_resources()
{
    Q_INIT_RESOURCE(libqtcore);
}

/**
 * @brief clean_lib_resources 卸载本静态库的资源
 */
void cleanup_lib_resources()
{
    Q_CLEANUP_RESOURCE(libqtcore);
}

/**
 * @brief init_log_file 生成日志文件
 * @param filepath 要生成的日志文件路径
 */
void init_log_file(const QString& filepath)
{
    if(filepath.isEmpty())
        return;

    // 初始化日志机制
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);

    // 添加文件为目的地
    const QString sLogPath(QDir(QApplication::applicationDirPath()).filePath(filepath));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
      sLogPath, EnableLogRotation, MaxSizeBytes(512*1024), MaxOldLogCount(5)));
    logger.addDestination(fileDestination);

    QLOG_INFO()<<"log system init success.";
}

/**
 * @brief print_log 打印日志
 * @param level 日志等级
 * @param msg 要打印的日志
 * @param isprintscreen 是否输出到屏幕
 */
void print_log(const LogLevel level, const QString &msg, const QString &isprintscreen)
{
    QStringList printsreens = isprintscreen.split("|");
    bool isPrint = (printsreens.size() > 0 && printsreens[0].toInt() == 1 ? true : false);
    bool isColor = (printsreens.size() > 1 && printsreens[1].toInt() == 1 ? true : false);
    bool isTime = (printsreens.size() > 2 && printsreens[2].toInt() == 1 ? true : false);

    const QString timeMsg = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");

    switch(level)
    {
    case LOG_DEBUG:
        QLOG_DEBUG()<<msg;
        if(isPrint) qDebug().noquote()<<(isColor ? "\033[32m" : "")<<(isTime ? timeMsg : "")<<msg;
        break;
    case LOG_INFO:
        QLOG_INFO()<<msg;
        if(isPrint) qDebug().noquote()<<(isColor ? "\033[37m" : "")<<(isTime ? timeMsg : "")<<msg;
        break;
    case LOG_WARN:
        QLOG_WARN()<<msg;
        if(isPrint) qDebug().noquote()<<(isColor ? "\033[43m" : "")<<(isTime ? timeMsg : "")<<msg;
        break;
    case LOG_ERROR:
        QLOG_ERROR()<<msg;
        if(isPrint) qDebug().noquote()<<(isColor ? "\033[41m" : "")<<(isTime ? timeMsg : "")<<msg;
        break;
    case LOG_FATAL:
        QLOG_FATAL()<<msg;
        if(isPrint) qDebug().noquote()<<(isColor ? "\033[45m" : "")<<(isTime ? timeMsg : "")<<msg;
        break;
    default:
        break;
    }
}

/**
 * @brief init_dbpool 初始化数据库连接池
 * @param paramIsDebug
 * @param parent
 */
void init_dbpool(const bool paramIsDebug,QObject * parent)
{
    if(mNDBPool != nullptr)
        return;

    mNDBPool = new NDBPool(paramIsDebug,parent);
}

/**
 * @brief clean_dbpool 卸载数据库连接池
 */
void clean_dbpool()
{
    if(mNDBPool == nullptr)
        return;

    mNDBPool->deleteLater();
    mNDBPool = nullptr;
}

/**
 * @brief sava_file 保存源文件到目标文件(主要网络传输中使用)
 * @param srcfile 要保存的源文件的路径(相对路径)
 * @param decfile 目标文件路径(绝对路径)
 *
 * @return 如果文件保存成功返回真，否则返回假
 */
bool sava_file(QString srcfile,QString decfile)
{
    bool preturnState = true;

    QString appfiledir = decfile;
    QString tmpFileDirPath = appfiledir.mid(0,appfiledir.lastIndexOf("/"));

    QDir dir(tmpFileDirPath);
    if(!dir.exists())
    {
        if(!dir.mkpath(tmpFileDirPath))
        {
            QLOG_ERROR()<<"sava_file:"<<tmpFileDirPath<<" create fail.";
            preturnState=false;
        }
    }

    QFileInfo pFileInfo(appfiledir);
    if(pFileInfo.exists())
    {
        if(!QFile::remove(appfiledir))
        {
            QLOG_ERROR()<<"sava_file:"<<appfiledir<<" remove fail.";
            preturnState=false;
        }
    }

    if(!QFile::copy(srcfile,appfiledir))
    {
       QLOG_ERROR()<<"sava_file:"<<appfiledir<<" copy fail.";
       preturnState=false;
    }

    return preturnState;
}

/**
 * @brief init_dump_system 初始化崩溃系统
 */
void init_dump_system(void)
{
#if defined (WIN32)
    google_breakpad::ExceptionHandler eh(
        L".", nullptr, dump_callback, nullptr, google_breakpad::ExceptionHandler::HANDLER_ALL);
#else
    google_breakpad::MinidumpDescriptor minidump(".");
    google_breakpad::ExceptionHandler breakpad(minidump, nullptr, DumpCallback, nullptr,
        true, -1);
#endif
}

//拷贝文件：
bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
	toDir.replace("\\","/");
	if (sourceDir == toDir){
		return true;
	}
	if (!QFile::exists(sourceDir)){
		return false;
	}
	QDir *createfile     = new QDir;
	bool exist = createfile->exists(toDir);
	if (exist){
		if(coverFileIfExist){
			createfile->remove(toDir);
		}
	}//end if

	if(!QFile::copy(sourceDir, toDir))
	{
		return false;
	}
	return true;
}

//拷贝文件夹：
bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
		if(!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}

	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList){
		if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
			if(!copyDirectoryFiles(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()),
				coverFileIfExist))
				return false;
		}
		else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
			if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
				targetDir.remove(fileInfo.fileName());
			}

			/// 进行文件copy
			if(!QFile::copy(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()))){
					return false;
			}
		}
	}
	return true;
}

/// 得到当前IP
QHostAddress getLocalIP(int protocolType)
{
    QHostAddress returnAddress;

    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());

    foreach(QHostAddress address, info.addresses())
    {
        if (address.protocol() == protocolType)
            returnAddress = address;
    }

    return returnAddress;
}

/**
 * 得到指定目录下所有的文件
 *
 * @param _filePath 要得到文件的目录
 * @param filelist 用于存放读取到的文件
 * @param isContanDir 是否包含目录
 * @param isContanSubDir 是否包含子目录
 * @return 如果文件读取成功返回0，否则返回-1
 */
int FindFile(const QString& _filePath,QVector<QString> &filelist,
             bool isContanDir,
             bool isContanSubDir)
{
    QDir dir(_filePath);

    if (!dir.exists()) {
        return -1;
    }

    //取到所有的文件和文件名，但是去掉.和..的文件夹（这是QT默认有的）
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //文件夹优先
    dir.setSorting(QDir::DirsFirst);

    //转化成一个list
    QFileInfoList list = dir.entryInfoList();
    //QStringList infolist = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    if(list.size()< 1 ) {
        return -1;
    }
    int i=0;

    //递归算法的核心部分
    do{
        QFileInfo fileInfo = list.at(i);
        //如果是文件夹，递归
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            if(isContanDir)
                filelist.push_back(fileInfo.absoluteFilePath());

            if(isContanSubDir)
                FindFile(fileInfo.filePath(),filelist);
        }
        else{
            filelist.push_back(list[i].absoluteFilePath());
        }//end else
        i++;
    } while(i < list.size());

    return 0;
}

/**
 * @brief JsonToString QJsonObject转QString
 * @param json 要转换的QJsonObject
 * @return 返回转换后的字符串
 */
QString JsonToString(const QJsonObject& json)
{
    if(json.isEmpty()) return "";

    return QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

/**
 * @brief JsonArrayToString QJsonArray转QString
 * @param json
 * @return
 */
QString JsonArrayToString(const QJsonArray& json)
{
    if(json.isEmpty()) return "";

    return QString(QJsonDocument(json).toJson(QJsonDocument::Compact));
}

/**
 * @brief ObjectToByteArray QJsonObject转QByteArray
 * @param json
 * @return
 */
QByteArray ObjectToByteArray(const QJsonObject& json)
{
    QJsonDocument document;
    document.setObject(json);

    return document.toJson(QJsonDocument::Compact);
}

/**
 * @brief ByteArrayToObject QByteArray转QJsonObject
 * @param array
 * @return
 */
QJsonObject ByteArrayToObject(const QByteArray& array)
{
    QJsonDocument doc = QJsonDocument::fromJson(array);
    return doc.object();
}

/**
 * @brief StringToJson QString转QJsonObject
 * @param str 要转换的字符串
 * @return 返回转换后的QJsonObject
 */
QJsonObject StringToJson(const QString& str)
{
    QJsonObject l_ret;

    QJsonParseError l_err;
    QJsonDocument l_doc = QJsonDocument::fromJson(str.toUtf8(), &l_err);
    if (l_err.error == QJsonParseError::NoError)
    {
        if (l_doc.isObject())
        {
            l_ret = l_doc.object();
        }
    }
    return l_ret;
}

/// QString转QJsonArray
QJsonArray StringToJsonArray(const QString& str)
{
    QJsonArray l_ret;

    QJsonParseError l_err;
    QJsonDocument l_doc = QJsonDocument::fromJson(str.toUtf8(), &l_err);
    if (l_err.error == QJsonParseError::NoError)
    {
        if (l_doc.isArray())
        {
            l_ret = l_doc.array();
        }
    }

    return l_ret;
}

/// 删除目录下所有文件和文件夹
bool removeFolderContent(const QString &folderDir,bool isDeleteDir)
{
    QDir dir(folderDir);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists())  {return false;}//文件不存，则返回false
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(fileList.size()>0)
    {
        int infoNum=fileList.size();
        for(int i=infoNum-1;i>=0;i--)
        {
            curFile=fileList[i];
            if(curFile.isFile())//如果是文件，删除文件
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }
            if(curFile.isDir())//如果是文件夹
            {
                QDir dirTemp(curFile.filePath());
                QFileInfoList fileList1=dirTemp.entryInfoList(QDir::Dirs|QDir::Files
                                                              |QDir::Readable|QDir::Writable
                                                              |QDir::Hidden|QDir::NoDotAndDotDot
                                                              ,QDir::Name);
                if(fileList1.size()==0)//下层没有文件或文件夹
                {
                    dirTemp.rmdir(".");
                    fileList.removeAt(i);
                }
                else//下层有文件夹或文件
                {
                    for(int j=0;j<fileList1.size();j++)
                    {
                        if(!(fileList.contains(fileList1[j])))
                            fileList.append(fileList1[j]);
                    }
                }
            }
        }
    }

    if(isDeleteDir) dir.removeRecursively();

    return true;
}

/**
 * @brief FastlzCompressData fastlz压缩数据
 * @param srcdata
 * @return
 */
bool FastlzCompressData(QByteArray srcdata,QByteArray &decdata)
{
    if(srcdata.isEmpty())
        return false;

    unsigned char *fastlzBuffer = (unsigned char*)allocBytes(srcdata.size()*2);
    memset(fastlzBuffer,0,srcdata.size()*2);

    int decSize = fastlz_compress(srcdata.data(),srcdata.size(),fastlzBuffer);
    if(decSize <= 0)
    {
        deallocBytes(fastlzBuffer);
        fastlzBuffer = nullptr;
        return false;
    }

    decdata.append((const char *)fastlzBuffer,decSize);

    deallocBytes(fastlzBuffer);
    fastlzBuffer = nullptr;

    return true;
}

/**
 * @brief FastlzDecompressData fastlz解压数据
 * @param srcdata
 * @return
 */
bool FastlzDecompressData(QByteArray srcdata,QByteArray &decdata,qint32 deccSize)
{
    if(srcdata.isEmpty())
        return false;

    char *fastlzBuffer = (char*)allocBytes(deccSize);
    if(fastlzBuffer == nullptr)
        return false;

    int decSize = fastlz_decompress(srcdata.data(),srcdata.size(),
                                    fastlzBuffer,deccSize);
    if(decSize <= 0)
    {
        deallocBytes(fastlzBuffer);
        fastlzBuffer=nullptr;
        return false;
    }

    decdata.append(fastlzBuffer,decSize);

    deallocBytes(fastlzBuffer);
    fastlzBuffer=nullptr;

    return true;
}

/**
 * 设置开机自启动
 *
 * @param bAutoRun 是否可以开机自启动
 */
void appAutoRun(bool bAutoRun)
{
#if defined (WIN32)
    //注册表路径需要使用双反斜杠，如果是32位系统，要使用QSettings::Registry32Format
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::Registry64Format);

    QString appPath = QCoreApplication::applicationFilePath();

    //以程序名称作为注册表中的键
    //根据键获取对应的值（程序路径）
    QFileInfo fInfo(appPath);
    QString name = fInfo.baseName();
    QString path = settings.value(name).toString();

    //如果注册表中的路径和当前程序路径不一样，
    //则表示没有设置自启动或自启动程序已经更换了路径
    //toNativeSeparators的意思是将"/"替换为"\"

    if(bAutoRun)
    {
        QString newPath = QDir::toNativeSeparators(appPath);
        if (path != newPath)
        {
            settings.setValue(name, newPath);
        }
    }
    else
    {
        settings.setValue(name, "");
    }
#endif
}

QString convert_to_ipv4_addr(QHostAddress addr)
{
    quint32  addr_origin = addr.toIPv4Address();
    QHostAddress addr_host = QHostAddress(addr_origin);
    QString  addr_str = addr_host.toString();
    return addr_str;
}

/// 删除指定路径的文件或者文件夹
bool DeleteTheFileOrFolder(QString path)
{
    if(path.isEmpty())
        return false;

    QFileInfo pfileinfo(path);
    if(!pfileinfo.exists())
        return false;

    if(pfileinfo.isDir())
        return removeFolderContent(path);

    return QFile::remove(path);
}

/// 创建指定路径的文件或者文件夹
bool CreateTheFileOrFolder(QString path,int type)
{
    if(path.isEmpty())
        return false;

    bool isOk = true;

    QString decPath = path;

    if(type == 0)
    {
        decPath = path.mid(0,path.lastIndexOf("/"));
    }

    QDir dir(decPath);
    if(!dir.exists())
    {
        isOk = dir.mkpath(decPath);
        if(!isOk)
            return false;
    }

    if(type == 1)
        return isOk;

    QFile file(path);
    isOk = file.open(QIODevice::WriteOnly);
    file.close();

    return isOk;
}

/**
 * @brief EncData 简单加解密
 * @param data
 * @param key1
 * @param key2
 * @return
 */
QByteArray EncData(QByteArray data,QString keyone,QString keytwo)
{
    if(data.isEmpty() ||
       keyone.isEmpty() ||
       keytwo.isEmpty())
        return data;

    QByteArray key1 = keyone.toUtf8();
    QByteArray key2 = keytwo.toUtf8();

    for(int i = 0; i < data.size(); ++i)
    {
        int keyIndex1 = i % key1.size();
        int keyIndex2 = i % key2.size();
        data[i] = data[i] ^ key1[keyIndex1];
        data[i] = data[i] ^ key2[keyIndex2];
    }

    return data;
}

/**
 * @brief getByteArrayMd5 得到相应数据的MD5
 * @param data
 * @return
 */
QString getByteArrayMd5(QByteArray data)
{
    if(data.isEmpty())
        return QString();

    QCryptographicHash md5Hash(QCryptographicHash::Md5);
    md5Hash.addData(data,data.size());

    return md5Hash.result().toHex();
}

/**
 * @brief generateMd5 生成字符串的MD5值
 * @param str
 * @return
 */
QString generateMd5(const QString& str)
{
    QByteArray bytes = str.toUtf8();
    QByteArray hash = QCryptographicHash::hash(bytes, QCryptographicHash::Md5);
    return QString(hash.toHex());
}

/**
 * @brief generateFileMd5 生成文件内容的MD5值
 * @param filePath
 * @return
 */
QString generateFileMd5(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return "";

    QCryptographicHash hash(QCryptographicHash::Md5);
    if (hash.addData(&file))
        return QString(hash.result().toHex());

    return "";
}

bool convertImageToYUV420P(QImage& image, QByteArray& yuvData)
{
    if(image.isNull())
        return false;

    // 提取RGB数据
    QByteArray rgbData;
    const int bytesPerLine = image.width() * 3;
    for (int y = 0; y < image.height(); ++y) {
        const uchar* scanline = image.constScanLine(y);
        rgbData.append(reinterpret_cast<const char*>(scanline), bytesPerLine);
    }

    // 将RGB数据转换为YUV420P
    yuvData.resize(image.width() * image.height() * 3 / 2);
    uchar* yuvPtr = reinterpret_cast<uchar*>(yuvData.data());
    const uchar* rgbPtr = reinterpret_cast<const uchar*>(rgbData.constData());
    const int uvOffset = image.width() * image.height();

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            const int yIndex = y * image.width() + x;
            const int uvIndex = uvOffset + (y / 2) * (image.width() / 2) + (x / 2) * 2;

            const int r = *rgbPtr++;
            const int g = *rgbPtr++;
            const int b = *rgbPtr++;

            // 计算Y分量
            yuvPtr[yIndex] = static_cast<uchar>((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;

            // 计算U和V分量
            if ((y % 2 == 0) && (x % 2 == 0)) {
                yuvPtr[uvIndex] = static_cast<uchar>((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
                yuvPtr[uvIndex + 1] = static_cast<uchar>((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
            }
        }
    }

    return true;
}

bool convertRGBToYUV420P(const QString& imagePath, int width, int height, QByteArray& yuvData)
{
    QImage image(imagePath);
    if (image.isNull()) {
        // 加载图片失败
        return false;
    }

    // 将图片转换为指定大小
    image = image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    return convertImageToYUV420P(image,yuvData);
}

// 字符串转QSize
QSize convertStringToSize(QString str,QString separator)
{
    if(str == "" || separator == "")
        return QSize();

    QStringList strParams = str.split(separator);
    if(strParams.size() != 2)
        return QSize();

    return QSize(strParams[0].toInt(),strParams[1].toInt());
}

// 字符串转QPoint
QPoint convertStringToPoint(QString str,QString separator)
{
    if(str == "" || separator == "")
        return QPoint();

    QStringList strParams = str.split(separator);
    if(strParams.size() != 2)
        return QPoint();

    return QPoint(strParams[0].toInt(),strParams[1].toInt());
}

// 字符串转QRect
QRect convertStringToRect(QString str,QString separator)
{
    if(str == "" || separator == "")
        return QRect();

    QStringList strParams = str.split(separator);
    if(strParams.size() != 4)
        return QRect();

    return QRect(QPoint(strParams[0].toInt(),strParams[1].toInt()),
                 QSize(strParams[2].toInt(),strParams[3].toInt()));
}
