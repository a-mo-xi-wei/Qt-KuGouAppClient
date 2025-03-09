#include "resourcemanager.h"
#include "QsLog.h"

#include <QFileInfo>
//#include <qDebug>

//initialiseSingleton(CResourceManager);

CResourceManager::CResourceManager()
    : QObject()
{
    m_SystemWorkingPathTimer.setSingleShot(true);

    connect(&m_SystemWorkingPathTimer, &QTimer::timeout,this, &CResourceManager::WorkingfileChanged);
    connect(&m_pSystemWorkingPathWatcher,&QFileSystemWatcher::fileChanged,this,&CResourceManager::WorkingfileChanging);
}

CResourceManager::~CResourceManager()
{
    deleteallzipfiles();
}

void CResourceManager::deleteallzipfiles(void)
{
    QMutexLocker tmpLocker(&m_resourceMutex);

    QHash<QString,QZipReader*>::iterator iter = m_zipresources.begin();
    for(;iter != m_zipresources.end();++iter)
    {
        if(iter.value())
        {
            iter.value()->close();

            delete iter.value();
        }
    }

    m_zipresources.clear();
    m_zipsubfileresources.clear();
}

void CResourceManager::WorkingfileChanging(const QString& filepath)
{
    m_currentChangingFilePath = filepath;
    m_SystemWorkingPathTimer.start(2000);
}

void CResourceManager::WorkingfileChanged()
{
    if(m_currentChangingFilePath == "")
        return;

    loadFileResource(m_currentChangingFilePath);

    m_currentChangingFilePath="";
}

void CResourceManager::loadFileResource(const QString& filepath, const bool isAddPath)
{
    QFileInfo pFileInfo(filepath);
    if(!pFileInfo.exists())
    {
        return;
    }

    QMutexLocker tmpLocker(&m_resourceMutex);

    if(pFileInfo.suffix() == "zip")
    {
        QHash<QString,QZipReader*>::iterator iter = m_zipresources.find(filepath);
        if(iter != m_zipresources.end())
        {
            iter.value()->close();
            delete iter.value();

            m_zipsubfileresources[filepath].clear();
            m_zipresources.erase(iter);

            QZipReader *pZipReader = new QZipReader(filepath);
            if(pZipReader->exists() && pZipReader->isReadable())
            {
                m_zipresources[filepath] = pZipReader;

                QVector<QZipReader::FileInfo> pFileInfos = pZipReader->fileInfoList();
                if(!pFileInfos.isEmpty())
                {
                    for(int i=0;i<pFileInfos.size();i++)
                    {
                        m_zipsubfileresources[filepath].push_back(pFileInfos[i].filePath);
                    }
                }
            }
            else
                delete pZipReader;
        }

        return;
    }

    QFile file(filepath);

    QIODevice::OpenMode fileOpenMode = QIODevice::ReadOnly;

    if(pFileInfo.suffix() == "css" ||
       pFileInfo.suffix() == "js")
        fileOpenMode = QIODevice::ReadOnly | QIODevice::Text;

    if (file.open(fileOpenMode))
    {
        tagResource tmpResources;
        tmpResources.databytes = file.readAll();
        //tmpResources.checknum = qChecksum((const char*)tmpResources.databytes.data(),tmpResources.databytes.size());
        tmpResources.checknum = qChecksum(QByteArrayView(tmpResources.databytes));
        file.close();

        QHash<QString,tagResource>::iterator iter = m_resources.find(filepath);
        if(iter == m_resources.end())
        {
            m_resources[filepath] = tmpResources;
        }
        else
        {
            if(iter.value().checknum != tmpResources.checknum)
                m_resources[filepath] = tmpResources;
        }

        if(isAddPath) m_pSystemWorkingPathWatcher.addPath(filepath);
    }

    QLOG_INFO() << "CResourceManager::loadFileResource:"+filepath+" load successed.";
}

/**
 * @brief ResourceManager::getFile 得到指定路径的文件资源
 * @param filepath
 * @return
 */
QByteArray CResourceManager::getFile(const QString& filepath)
{
    loadFileResource(filepath,true);

    return m_resources[filepath].databytes;
}

/**
 * @brief CResourceManager::getFileFromZip 从zip文件中得到文件资源
 * @param filepath
 * @return
 */
QByteArray CResourceManager::getFileFromZip(const QString& filepath)
{
    QMutexLocker tmpLocker(&m_resourceMutex);

    QHash<QString,QList<QString>>::iterator iter = m_zipsubfileresources.begin();
    for(;iter != m_zipsubfileresources.end();++iter)
    {
        QList<QString>::iterator iter2 = std::find(iter.value().begin(),
                                               iter.value().end(),
                                               filepath);
        if(iter2 == iter.value().end())
            continue;

        return m_zipresources[iter.key()]->fileData(filepath);
    }

    return QByteArray();
}

/**
 * @brief CResourceManager::addZipFile 添加zip文件到系统中
 * @param filepath
 * @param isautoupdate
 * @return
 */
bool CResourceManager::addZipFile(const QString& filepath,bool isautoupdate)
{
    QFileInfo pfilepath(filepath);
    if(!pfilepath.exists() || pfilepath.suffix() != "zip")
        return false;

    QMutexLocker tmpLocker(&m_resourceMutex);

    QHash<QString,QZipReader*>::iterator iter = m_zipresources.find(filepath);
    if(iter != m_zipresources.end())
        return false;

    QZipReader *pZipReader = new QZipReader(filepath);
    if(!pZipReader->exists() || !pZipReader->isReadable())
    {
        delete pZipReader;
        return false;
    }

    m_zipresources[filepath] = pZipReader;
    m_zipsubfileresources[filepath].clear();

    QVector<QZipReader::FileInfo> pFileInfos = pZipReader->fileInfoList();
    if(!pFileInfos.isEmpty())
    {
        for(int i=0;i<pFileInfos.size();i++)
        {
            m_zipsubfileresources[filepath].push_back(pFileInfos[i].filePath);
        }
    }

    if(isautoupdate) m_pSystemWorkingPathWatcher.addPath(filepath);

    return true;
}
