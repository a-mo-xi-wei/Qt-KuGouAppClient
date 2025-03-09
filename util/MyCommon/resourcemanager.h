#ifndef LIBLOCKCORE_RESOURCEMANAGER_H
#define LIBLOCKCORE_RESOURCEMANAGER_H

#include "singleton.h"

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QMutex>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QtCore/private/qzipreader_p.h>

struct tagResource
{
    tagResource()
        : checknum(0) {}
    tagResource(quint16 cn,QByteArray data)
        : checknum(cn),databytes(data) {}

    quint16 checknum;
    QByteArray databytes;
};

class CResourceManager : public QObject /*, public Singleton<CResourceManager>*/
{
    Q_OBJECT

public:
    explicit CResourceManager();
    ~CResourceManager();

    /// 得到指定路径的文件资源
    QByteArray getFile(QString filepath);

    /// 添加zip文件到系统中
    bool addZipFile(QString filepath,bool isautoupdate=true);
    /// 从zip文件中得到文件资源
    QByteArray getFileFromZip(QString filepath);

private slots:
    void WorkingfileChanging(const QString filepath);
    void WorkingfileChanged();

private:
    void loadFileResource(QString filepath,bool isAddPath=true);
    void deleteallzipfiles(void);

private:
    QHash<QString,tagResource> m_resources;              /**< 管理所有的文件资源 */
    QMutex m_resourceMutex;                              /**< 保护文件资源 */
    QFileSystemWatcher m_pSystemWorkingPathWatcher;      /**< 监控资源文件是否改变，如果改变，重新加载 */
    QTimer m_SystemWorkingPathTimer;                     /**< 用于过滤工作目录不停改变的情况,我们只需要最后的改变 */
    QString m_currentChangingFilePath;                   /**< 当前变化的文件路径 */
    QHash<QString,QZipReader*> m_zipresources;           /**< 管理所有的zip文件 */
    QHash<QString,QList<QString>> m_zipsubfileresources; /**< zip文件中所有的子文件 */
};

#endif // LIBLOCKCORE_RESOURCEMANAGER_H
