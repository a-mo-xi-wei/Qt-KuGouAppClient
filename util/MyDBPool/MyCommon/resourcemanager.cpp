#include "resourcemanager.h"
#include "QsLog.h"

#include <QFileInfo>

//initialiseSingleton(CResourceManager);

/** @brief 构造函数
 *
 *  初始化资源管理器，设置文件监控和定时器。
 */
CResourceManager::CResourceManager()
    : QObject()
{
    m_SystemWorkingPathTimer.setSingleShot(true);

    connect(&m_SystemWorkingPathTimer, &QTimer::timeout,this, &CResourceManager::WorkingfileChanged);
    connect(&m_pSystemWorkingPathWatcher,&QFileSystemWatcher::fileChanged,this,&CResourceManager::WorkingfileChanging);
}

/** @brief 析构函数
 *
 *  销毁资源管理器，释放所有 ZIP 文件资源。
 */
CResourceManager::~CResourceManager() {
    deleteallzipfiles();
}

/** @brief 删除所有 ZIP 文件资源
 *
 *  关闭并删除所有已加载的 ZIP 文件资源。
 */
void CResourceManager::deleteallzipfiles(void) {
    QMutexLocker tmpLocker(&m_resourceMutex);

    QHash<QString, QZipReader*>::iterator iter = m_zipresources.begin();
    for (; iter != m_zipresources.end(); ++iter) {
        if (iter.value()) {
            iter.value()->close();
            delete iter.value();
        }
    }

    m_zipresources.clear();
    m_zipsubfileresources.clear();
}

/** @brief 处理文件变化信号
 *
 *  记录文件变化并启动定时器以延迟处理。
 *
 *  @param filepath 变化的文件路径
 */
void CResourceManager::WorkingfileChanging(const QString& filepath) {
    m_currentChangingFilePath = filepath;
    m_SystemWorkingPathTimer.start(2000);
}

/** @brief 处理文件变化完成
 *
 *  加载变化后的文件资源。
 */
void CResourceManager::WorkingfileChanged() {
    if (m_currentChangingFilePath == "")
        return;

    loadFileResource(m_currentChangingFilePath);

    m_currentChangingFilePath = "";
}

/** @brief 加载文件资源
 *
 *  加载指定路径的文件资源，并可选择是否添加到文件监控。
 *
 *  @param filepath 文件路径
 *  @param isAddPath 是否添加到文件监控
 */
void CResourceManager::loadFileResource(const QString& filepath, const bool isAddPath) {
    QFileInfo pFileInfo(filepath);
    if (!pFileInfo.exists()) {
        return;
    }

    QMutexLocker tmpLocker(&m_resourceMutex);

    if (pFileInfo.suffix() == "zip") {
        QHash<QString, QZipReader*>::iterator iter = m_zipresources.find(filepath);
        if (iter != m_zipresources.end()) {
            iter.value()->close();
            delete iter.value();

            m_zipsubfileresources[filepath].clear();
            m_zipresources.erase(iter);
        }

        QZipReader *pZipReader = new QZipReader(filepath);
        if (pZipReader->exists() && pZipReader->isReadable()) {
            m_zipresources[filepath] = pZipReader;

            QVector<QZipReader::FileInfo> pFileInfos = pZipReader->fileInfoList();
            if (!pFileInfos.isEmpty()) {
                for (int i = 0; i < pFileInfos.size(); i++) {
                    m_zipsubfileresources[filepath].push_back(pFileInfos[i].filePath);
                }
            }
        } else {
            delete pZipReader;
        }

        return;
    }

    QFile file(filepath);

    QIODevice::OpenMode fileOpenMode = QIODevice::ReadOnly;

    if (pFileInfo.suffix() == "css" ||
        pFileInfo.suffix() == "js")
        fileOpenMode = QIODevice::ReadOnly | QIODevice::Text;

    if (file.open(fileOpenMode)) {
        tagResource tmpResources;
        tmpResources.databytes = file.readAll();
        tmpResources.checknum = qChecksum(QByteArrayView(tmpResources.databytes));
        file.close();

        QHash<QString, tagResource>::iterator iter = m_resources.find(filepath);
        if (iter == m_resources.end()) {
            m_resources[filepath] = tmpResources;
        } else {
            if (iter.value().checknum != tmpResources.checknum)
                m_resources[filepath] = tmpResources;
        }

        if (isAddPath) m_pSystemWorkingPathWatcher.addPath(filepath);
    }

    QLOG_INFO() << "CResourceManager::loadFileResource: " + filepath + " load successed.";
}

/** @brief 获取指定路径的文件资源
 *
 *  加载并返回指定路径的文件资源内容。
 *
 *  @param filepath 文件路径
 *  @return 文件内容的字节数组
 */
QByteArray CResourceManager::getFile(const QString& filepath) {
    loadFileResource(filepath, true);

    return m_resources[filepath].databytes;
}

/** @brief 从 ZIP 文件中获取文件资源
 *
 *  从指定的 ZIP 文件中提取文件内容。
 *
 *  @param filepath 文件路径
 *  @return 提取的文件内容的字节数组
 */
QByteArray CResourceManager::getFileFromZip(const QString& filepath) {
    QMutexLocker tmpLocker(&m_resourceMutex);

    QHash<QString, QList<QString>>::iterator iter = m_zipsubfileresources.begin();
    for (; iter != m_zipsubfileresources.end(); ++iter) {
        QList<QString>::iterator iter2 = std::find(iter.value().begin(),
                                                  iter.value().end(),
                                                  filepath);
        if (iter2 == iter.value().end())
            continue;

        return m_zipresources[iter.key()]->fileData(filepath);
    }

    return QByteArray();
}

/** @brief 添加 ZIP 文件
 *
 *  将指定 ZIP 文件添加到资源管理器。
 *
 *  @param filepath ZIP 文件路径
 *  @param isautoupdate 是否自动监控文件变化
 *  @return 添加成功返回 true，否则返回 false
 */
bool CResourceManager::addZipFile(const QString& filepath, bool isautoupdate) {
    QFileInfo pfilepath(filepath);
    if (!pfilepath.exists() || pfilepath.suffix() != "zip")
        return false;

    QMutexLocker tmpLocker(&m_resourceMutex);

    QHash<QString, QZipReader*>::iterator iter = m_zipresources.find(filepath);
    if (iter != m_zipresources.end())
        return false;

    QZipReader *pZipReader = new QZipReader(filepath);
    if (!pZipReader->exists() || !pZipReader->isReadable()) {
        delete pZipReader;
        return false;
    }

    m_zipresources[filepath] = pZipReader;
    m_zipsubfileresources[filepath].clear();

    QVector<QZipReader::FileInfo> pFileInfos = pZipReader->fileInfoList();
    if (!pFileInfos.isEmpty()) {
        for (int i = 0; i < pFileInfos.size(); i++) {
            m_zipsubfileresources[filepath].push_back(pFileInfos[i].filePath);
        }
    }

    if (isautoupdate) m_pSystemWorkingPathWatcher.addPath(filepath);

    return true;
}