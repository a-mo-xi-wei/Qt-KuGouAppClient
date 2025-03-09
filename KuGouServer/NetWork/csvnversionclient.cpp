#include "csvnversionclient.h"
#include "QsLog.h"
#include "fastlz.h"

#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QJsonArray>
#include <QCryptographicHash>

tagUserData m_UserData;

/**
 * @brief CSVNVersionClient::CSVNVersionClient 构造函数
 * @param parent
 */
CSVNVersionClient::CSVNVersionClient(QObject *parent)
    : QObject(parent)
{
    m_WeSocketClient.SetNetworkFrameManager(this);
}

/**
 * @brief CSVNVersionClient::~CSVNVersionClient 析构函数
 */
CSVNVersionClient::~CSVNVersionClient()
{
    stopClient();
}

/// 关闭与服务器的连接
void CSVNVersionClient::stopClient(void)
{
    m_WeSocketClient.Close();
    m_fileChangedlist.clear();
}

/**
 * @brief getUserData 得到当前玩家数据
 * @return
 */
tagUserData* CSVNVersionClient::getUserData(void)
{
    return &m_UserData;
}

/**
 * @brief CSVNVersionClient::isConnected 检测连接是否建立成功
 * @return
 */
bool CSVNVersionClient::isConnected(void)
{
    return m_WeSocketClient.isConnected();
}

/**
 * @brief CSVNVersionClient::startClient 开始客户端
 * @param serverip 要连接的服务器的IP
 * @param port 要连接的服务器的端口
 */
void CSVNVersionClient::startClient(QString serverip,int port)
{
    if(serverip.isEmpty() || port <= 0)
        return;

    QString path = QString("ws://%1:%2").arg(serverip).arg(port);
    m_WeSocketClient.Open(QUrl(path));
}

/**
 * @brief CSVNVersionClient::loginUser 登陆用户
 * @param userName 用户名
 * @param userPwd 用户密码
 */
void CSVNVersionClient::loginUser(QString userName,QString userPwd)
{
    if(userName.isEmpty() || userPwd.isEmpty())
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERION_USER;
    mesObj["subid"] = IDD_SVNVERION_LOGIN_USER;
    mesObj["username"] = userName;
    mesObj["userpwd"] = userPwd;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::registerUser 注册用户
 * @param userName 要注册的用户名
 * @param userpwd 要注册的用户密码
 * @param type 用户类型：0-普通用户；1-超级用户
 * @param projects 用户所在项目，如果是普通用户，项目例子：12,2,3,5 32,6,7 （主项目ID,子项目ID列）,超级用户不需要项目列
 */
void CSVNVersionClient::registerUser(QString userName,QString userpwd,int type,QString projects,QString devices)
{
    if(userName.isEmpty() || userpwd.isEmpty() || type < 0)
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERION_USER;
    mesObj["subid"] = IDD_SVNVERION_REGISTER_USER;
    mesObj["username"] = userName;
    mesObj["userpwd"] = userpwd;
    mesObj["type"] = type;
    mesObj["projects"] = projects;
    mesObj["devices"] = devices;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::getUserINfo 得到用户的信息
 * @param userId 用户ID
 */
void CSVNVersionClient::getUserINfo(int userId)
{
    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERION_USER;
    mesObj["subid"] = IDD_SVNVERION_GET_USERS;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;
    mesObj["userid"] = userId;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::updateUser 更改用户数据
 * @param userid 要更改的用户ID
 * @param userName 要注册的用户名
 * @param userpwd 要注册的用户密码
 * @param type 用户类型：0-普通用户；1-超级用户
 * @param projects 用户所在项目，如果是普通用户，项目例子：12|2,3,5_32|6,7 （主项目ID|子项目ID列）,超级用户不需要项目列
 */
void CSVNVersionClient::updateUser(int userid,QString userName,QString userpwd,int type,QString projects,QString devices)
{
    if(userid <= 0 || type < 0)
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERION_USER;
    mesObj["subid"] = IDD_SVNVERION_UPDATE_USER;
    mesObj["userid"] = userid;
    mesObj["username"] = userName;
    mesObj["userpwd"] = userpwd;
    mesObj["type"] = type;
    mesObj["projects"] = projects;
    mesObj["devices"] = devices;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::deleteUser 删除用户
 * @param userid 要删除的用户ID
 */
void CSVNVersionClient::deleteUser(int userid)
{
    if(userid <= 0)
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERION_USER;
    mesObj["subid"] = IDD_SVNVERION_DELETE_USER;
    mesObj["userid"] = userid;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::addMainProject 添加一个主项目
 * @param proName 项目名称
 * @param proDescribe 项目描述
 */
void CSVNVersionClient::addMainProject(QString proName,QString proDescribe)
{
    if(proName.isEmpty())
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_PROJECT_ADD;
    mesObj["type"] = 1;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;
    mesObj["name"] = proName;
    mesObj["describe"] = proDescribe;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::addSubProject 添加一个子项目
 * @param mainId 主项目的ID
 * @param proName 项目名称
 * @param proDescribe 项目描述
 */
void CSVNVersionClient::addSubProject(int mainId,QString proName,QString proDescribe)
{
    if(mainId <= 0 || proName.isEmpty())
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_PROJECT_ADD;
    mesObj["type"] = 2;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;
    mesObj["mainid"] = mainId;
    mesObj["name"] = proName;
    mesObj["describe"] = proDescribe;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::addVersion 添加一个版本
 * @param mainId 版本主项目ID
 * @param subId 版本子项目ID
 * @param type 版本类型 0-初始版本；1-修改版本；2-稳定版本；3-发布版本
 * @param log 版本日志
 * @param pFileChangedData 改变的文件
 */
void CSVNVersionClient::addVersion(int mainId,int subId,int type,QString log,QVector<tagFileChangedData> &pFileChangedData)
{
    if(mainId <= 0 || subId <= 0)
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_VERSION_ADD;
    mesObj["mainid"] = mainId;
    mesObj["subproid"] = subId;
    mesObj["type"] = type;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;
    mesObj["log"] = log;

    QJsonArray FileChangedDataArray;

    for(int i=0;i<pFileChangedData.size();i++)
    {
        //QJsonObject md5Obj;
        //md5Obj["filename"] = pFileChangedData[i].fileName;
       // md5Obj["md5"] = pFileChangedData[i].md5;

        FileChangedDataArray.push_back(pFileChangedData[i].fileName);
    }

    mesObj["filechangedata"] = FileChangedDataArray;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::addVersionData 添加一个版本数据
 * @param mainId 主项目ID
 * @param subId 子项目ID
 * @param verid 版本数据ID
 * @param data 版本数据
 */
void CSVNVersionClient::addVersionData(int mainId,int subId,int verid,QByteArray data)
{
    if(mainId <= 0 || subId <= 0 || verid <= 0 || data.isEmpty())
        return;

    tagVersionOper pVersionOper;
    memset(&pVersionOper,0,sizeof(tagVersionOper));

    pVersionOper.tagIndex = IDD_SVNVERSION_VERSION_DAT_ADD;
    pVersionOper.mainid = mainId;
    pVersionOper.subid = subId;
    pVersionOper.versionid = verid;
    pVersionOper.userid = m_UserData.userid;
    strncpy(pVersionOper.userpwd,m_UserData.pwd.toStdString().c_str(),64);

    QByteArray sendData;
    sendData.append((const char*)&pVersionOper,sizeof(tagVersionOper));
    sendData.append(data);

    m_WeSocketClient.sendBinaryMessage(sendData,true);
}

/**
 * @brief CSVNVersionClient::delProject 删除指定ID的项目信息
 * @param mainId 主项目ID
 * @param usbId 子项目ID
 * @param type 0 - 主项目；1 - 子项目；2 - 版本
 * @param id 项目ID
 */
void CSVNVersionClient::delProject(int type,int mainId,int usbId,int id)
{
    if(type < 0)
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_PROJECT_DEL;
    mesObj["type"] = type;
    mesObj["mainproid"] = mainId;
    mesObj["subproid"] = usbId;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;
    mesObj["id"] = id;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::getVersionData 得到版本数据
 * @param id 目标ID,如果目标ID为-1的话，就读取最新的数据
 */
void CSVNVersionClient::getVersionData(int id)
{
    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_VERSION_DAT_GET;
    mesObj["id"] = id;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::getLastVersionData 得到指定项目最新版本的数据
 * @param mainId 主项目ID
 * @param usbId 子项目ID
 */
void CSVNVersionClient::getLastVersionData(int mainId,int subId)
{
    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_VERSION_DAT_GET;
    mesObj["mainId"] = mainId;
    mesObj["subId"] = subId;
    mesObj["id"] = -1;
    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::getProjectVersionsInfo 得到指定ID的项目版本信息
 * @param mainId 主项目ID
 * @param usbId 子项目ID
 */
void CSVNVersionClient::getProjectVersionsInfo(int mainId,int usbId)
{
    if(mainId <= 0 || usbId <= 0)
        return;

    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_PROJECT_VERSIONS_GET;
    mesObj["mainproid"] = mainId;
    mesObj["subproid"] = usbId;

    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::getProjectInfo 得到指定ID的项目的信息
 * @param mainId 项目的主ID
 * @param subIds 项目的子ID,等于-1的话，就得到主项目的所有子项目
 */
void CSVNVersionClient::getProjectInfo(int mainId,QVector<int> subIds)
{
    QJsonObject mesObj;
    mesObj["mesid"] = IDD_SVNVERSION_PROJECT;
    mesObj["subid"] = IDD_SVNVERSION_PROJECT_INFO_GET;
    mesObj["mainid"] = mainId;

    QJsonArray subidArrays;

    for(int i=0;i<subIds.size();i++)
    {
        QJsonObject subIdObj;
        subIdObj["subid"] = subIds[i];

        subidArrays.push_back(subIdObj);
    }

    mesObj["subids"] = subidArrays;

    mesObj["myid"] = m_UserData.userid;
    mesObj["mypwd"] = m_UserData.pwd;

    m_WeSocketClient.Send(JsonToString(mesObj));
}

/**
 * @brief CSVNVersionClient::getFileMd5 得到相应文件的MD5
 * @param path 要得到MD5码的文件夹
 * @return 返回文件夹的MD5ma码
 */
QString CSVNVersionClient::getFileMd5(const QString &path)
{
    if(path.isEmpty())
        return QString();

    QCryptographicHash md5Hash(QCryptographicHash::Md5);
    QFile file(path);
    if(file.open(QIODevice::ReadWrite))
    {
        while (!file.atEnd())
        {
            QString data = file.read(1024*1024);
            QByteArray curData=data.toLatin1();
            md5Hash.addData(curData,curData.size());

            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

            //qDebug()<< md5Hash.result() << " " << md5Hash.result().toHex();
        }
    }

    file.close();

    return md5Hash.result().toHex();
}

/**
 * @brief CSVNVersionClient::Convertpackagetofolder 将指定的包数据转换到指定的文件夹
 * @param packageData 要转换的包数据
 * @param filePath 目标目录
 * @return 如果转换成功返回真，否则返回假
 */
bool CSVNVersionClient::Convertpackagetofolder(QByteArray packageData,QString filePath)
{
    if(packageData.isEmpty() || filePath.isEmpty())
        return false;

    QByteArray tempData = packageData;

    tagSVNVersionPathStruct ptagSVNVersionPathStruct;
    memset(&ptagSVNVersionPathStruct,0,sizeof(tagSVNVersionPathStruct));

    memcpy(&ptagSVNVersionPathStruct,tempData.data(),sizeof(tagSVNVersionPathStruct));
    tempData.remove(0,sizeof(tagSVNVersionPathStruct));

    if(ptagSVNVersionPathStruct.tag[0] != 'S' ||
       ptagSVNVersionPathStruct.tag[1] != 'V' ||
       ptagSVNVersionPathStruct.tag[2] != 'N')
        return false;

    qDebug()<<"Convertpackagetofolder1:"<<ptagSVNVersionPathStruct.srcDataSize;

    unsigned char *uncompressed_buffer = (unsigned char*)malloc(ptagSVNVersionPathStruct.srcDataSize);
    memset(uncompressed_buffer,0,ptagSVNVersionPathStruct.srcDataSize);
    fastlz_decompress(tempData.data(),tempData.size(),uncompressed_buffer,ptagSVNVersionPathStruct.srcDataSize);

    tempData = QByteArray((const char*)uncompressed_buffer,ptagSVNVersionPathStruct.srcDataSize);

    qDebug()<<"Convertpackagetofolder2:"<<tempData.size();

    free(uncompressed_buffer);
    uncompressed_buffer=NULL;

    m_fileChangedlist.clear();

    for(int i=0;i<ptagSVNVersionPathStruct.fileCount;i++)
    {
        tagSVNVersionFileStruct ptagSVNVersionFileStruct;
        memset(&ptagSVNVersionFileStruct,0,sizeof(ptagSVNVersionFileStruct));

        memcpy(&ptagSVNVersionFileStruct,tempData.data(),sizeof(tagSVNVersionFileStruct));
        tempData.remove(0,sizeof(tagSVNVersionFileStruct));

        QString pfileName = QString(ptagSVNVersionFileStruct.fileName);
        QString ptmpFilePath = filePath + pfileName;

        QString tmpFileDirPath = ptmpFilePath.mid(0,ptmpFilePath.lastIndexOf(tr("/")));

        QDir dir(tmpFileDirPath);
        if(!dir.exists())
        {
            if(!dir.mkpath(tmpFileDirPath))
            {
                return false;
            }
        }

        QFile precvFile(ptmpFilePath);
        if(precvFile.open(QIODevice::WriteOnly))
        {
            QByteArray tmpFileData = tempData.mid(0,ptagSVNVersionFileStruct.fileSize);
            tempData.remove(0,ptagSVNVersionFileStruct.fileSize);

            precvFile.write(tmpFileData);
            precvFile.close();
        }

        OnProcessBinaryOperProcess(NULL,2,i+1,ptagSVNVersionPathStruct.fileCount);

        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    return true;
}

/**
 * @brief CSVNVersionClient::GetFileListMd5 得到指定文件夹下所有文件的信息和md5码
 * @return
 */
QVector<tagFileChangedData> CSVNVersionClient::GetFileListMd5(void)
{
    QVector<tagFileChangedData> returnFileList;

    if(m_currentWorkingPath.isEmpty())
        return returnFileList;

    QVector<QString> pfilelist;

    int filecount = FindFile(m_currentWorkingPath,pfilelist);
    if(filecount < 0)
        return returnFileList;

    for(int i=0;i<pfilelist.size();i++)
    {
        QString tmpRealFilePath = pfilelist[i].mid(m_currentWorkingPath.size());

        returnFileList.push_back(tagFileChangedData(tmpRealFilePath,getFileMd5(pfilelist[i])));

        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    return returnFileList;
}

/**
 * @brief CSVNVersionClient::Convertfoldertopackage 将指定文件夹转换成一个包数据
 * @param filePath 要转换的包
 * @return 返回转换后的包数据
 */
QByteArray CSVNVersionClient::Convertfoldertopackage(QString filePath)
{
    QByteArray returnData;

    if(filePath.isEmpty())
        return returnData;

    QVector<QString> pfilelist;

    int filecount = FindFile(filePath,pfilelist);
    if(filecount < 0)
        return returnData;

    QByteArray filesData;

    for(int i=0;i<pfilelist.size();i++)
    {
        QFile m_sendfile;
        m_sendfile.setFileName(pfilelist[i]);

        if(m_sendfile.open(QIODevice::ReadOnly))
        {
            QByteArray fileData = m_sendfile.readAll();

            QString tmpRealFilePath = pfilelist[i].mid(filePath.size());

            tagSVNVersionFileStruct ptagSVNVersionFileStruct;

            strncpy(ptagSVNVersionFileStruct.fileName,tmpRealFilePath.toUtf8().data(),MAX_FILENAME);
            ptagSVNVersionFileStruct.fileSize = fileData.size();

            filesData.append((const char*)&ptagSVNVersionFileStruct,sizeof(ptagSVNVersionFileStruct));
            filesData.append(fileData);

            m_sendfile.close();
        }

        OnProcessBinaryOperProcess(NULL,3,i+1,pfilelist.size());

        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    tagSVNVersionPathStruct ptagSVNVersionPathStruct;
    ptagSVNVersionPathStruct.tag[0] = 'S';
    ptagSVNVersionPathStruct.tag[1] = 'V';
    ptagSVNVersionPathStruct.tag[2] = 'N';
    ptagSVNVersionPathStruct.fileCount = pfilelist.size();
    ptagSVNVersionPathStruct.srcDataSize = filesData.size();

    returnData.append((const char*)&ptagSVNVersionPathStruct,sizeof(ptagSVNVersionPathStruct));
    //returnData.append(filesData);

    //qDebug()<<"Convertfoldertopackage1:"<<filesData.size();

    unsigned char *compressed_buffer = (unsigned char*)malloc(filesData.size());
    memset(compressed_buffer,0,filesData.size());
    int compressedsize = fastlz_compress(filesData.data(),filesData.size(),compressed_buffer);
   // int compressedsize = fastlz_compress_level(2, filesData.data(),filesData.size(), compressed_buffer);

    //qDebug()<<"Convertfoldertopackage2:"<<compressedsize;

    returnData.append(QByteArray((const char*)compressed_buffer,compressedsize));

    free(compressed_buffer);
    compressed_buffer=NULL;

    return returnData;
    //return qCompress(returnData);
}

/**
 * @brief CSVNVersionClient::OnProcessConnectedNetMes 处理一个新的连接到达
 * @param conn 到达的新的连接
 */
void CSVNVersionClient::OnProcessConnectedNetMes(QWebSocket *conn)
{
    qDebug()<<"CSVNVersionClient::OnProcessConnectedNetMes";

    emit on_signalConnectedNetMes(conn);
}

/**
 * @brief CSVNVersionClient::OnProcessDisconnectedNetMes 处理一个连接关闭
 * @param conn 要断开的连接
 */
void CSVNVersionClient::OnProcessDisconnectedNetMes(QWebSocket *conn)
{
    qDebug()<<"CSVNVersionClient::OnProcessDisconnectedNetMes";

    emit on_signalDisconnectedNetMes(conn);
}

/**
 * @brief CSVNVersionClient::OnProcessNetBinary 处理网络二进制消息
 * @param conn 要处理的客户端
 * @param data 到达的二进制消息
 */
void CSVNVersionClient::OnProcessNetBinary(QWebSocket *conn,QByteArray &data)
{
    emit on_signalNetBinary(conn,data);

    tagVersionOper pVersionOper;
    QByteArray reciverData = data;

    memset(&pVersionOper,0,sizeof(tagVersionOper));
    memcpy(&pVersionOper,reciverData.data(),sizeof(tagVersionOper));
    reciverData.remove(0,sizeof(tagVersionOper));

    switch(pVersionOper.tagIndex)
    {
    case IDD_SVNVERSION_VERSION_DAT_GET:
    {
        //QDir dir;
        //dir.setPath(m_currentWorkingPath);
        //dir.removeRecursively();
        removeFolderContent(m_currentWorkingPath);

        Convertpackagetofolder(reciverData,m_currentWorkingPath);

        QLOG_INFO()<<"get version data size:"<<reciverData.size()<<" dec path:"<<m_currentWorkingPath;
        qDebug()<<"get version data size:"<<reciverData.size()<<" dec path:"<<m_currentWorkingPath;
    }
        break;
    default:
        break;
    }
}

/**
 * @brief CSVNVersionClient::OnProcessNetText 处理网络字符串消息
 * @param conn 要处理的客户端
 * @param mes 到达的字符串消息
 */
void CSVNVersionClient::OnProcessNetText(QWebSocket *conn,QString mes)
{
    //qDebug()<<"CSVNVersionClient::OnProcessNetText:"<<mes;

    QJsonObject mesObj = StringToJson(mes);

    switch (mesObj["subid"].toInt())
    {
    case IDD_SVNVERION_LOGIN_USER:            // 用户登陆
    {
        int state = mesObj["state"].toInt();

        if(state == IDD_SVNVERION_USER_SUC)
        {
            m_UserData.userid = mesObj["userid"].toInt();
            m_UserData.name = mesObj["username"].toString();
            m_UserData.pwd = mesObj["userpwd"].toString();
            m_UserData.type = mesObj["type"].toInt();

            QString pprojects = mesObj["projects"].toString();
            QString pdevices = mesObj["devices"].toString();

            QStringList pDeviceList = pdevices.split(";");

            for(int i=0;i<pDeviceList.size();i++)
                m_UserData.devices.push_back(pDeviceList[i].toInt());

            m_UserData.projects.clear();

            if(!pprojects.isEmpty())
            {
                QStringList allprojlist = pprojects.split(";");

                for(int i=0;i<allprojlist.size();i++)
                {
                    QStringList projList = allprojlist[i].split(",");

                    if(!projList.isEmpty() && projList.size() > 1)
                    {
                        for(int i=1;i<projList.size();i++)
                        {
                            m_UserData.projects[projList[0].toInt()].push_back(projList[i].toInt());
                        }
                    }
                }
            }
        }
    }
        break;
    default:
        break;
    }

    emit on_signalNetText(conn,mes);
}

/**
 * @brief CSVNVersionClient::OnProcessSendFile 处理文件发送（客户端使用）
 * @param conn 要处理的客户端
 * @param file 要处理的文件的绝对路径
 * @param sendsize 当前发送的文件数据大小
 * @param totalsize 当前文件总的大小
 */
void CSVNVersionClient::OnProcessSendFile(QWebSocket *conn,QString file,qint64 sendsize,qint64 totalsize)
{

}

/**
 * @brief CSVNVersionClient::OnProcessRecvFile 处理文件接收
 * @param conn 要处理的客户端
 * @param srcfile 原始文件名称
 * @param decfile 保存的文件
 * @param pFileRecvError 状态码
 */
void CSVNVersionClient::OnProcessRecvFile(QWebSocket *conn,QString srcfile,QString decfile,FileRecvError pFileRecvError)
{

}

/**
 * @brief CSVNVersionClient::OnProcessBinaryOperProcess 处理二进制文件发送，接收进度，type:0发送，1接收
 * @param conn 要处理的客户端
 * @param type 0 - 上传数据； 1 - 下载数据
 * @param sendsize 实际处理的数据大小
 * @param totalsize 总的要处理的数据大小
 */
void CSVNVersionClient::OnProcessBinaryOperProcess(QWebSocket *conn,int type,qint64 sendsize,qint64 totalsize)
{
    qDebug()<<"CSVNVersionClient::OnProcessBinaryOperProcess:"<<type<<" "<<sendsize<<" "<<totalsize;

    emit on_signalOperProcess(type,sendsize,totalsize);
}
