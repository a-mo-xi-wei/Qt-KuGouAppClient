/*
  CResourceManager - 资源管理类
*/

#ifndef LIBLOCKCORE_RESOURCEMANAGER_H
#define LIBLOCKCORE_RESOURCEMANAGER_H

#include "singleton.h"

#include <QMutex>
#include <QFileSystemWatcher>
#include <QTimer>
#include <qzipreader_p.h>

/** @struct tagResource
 *  @brief 存储单个文件资源的结构
 */
struct tagResource {
    /** @brief 默认构造函数
     */
    tagResource()
        : checknum(0) {}

    /** @brief 构造函数
     *
     *  初始化资源数据和校验和。
     *
     *  @param cn 校验和
     *  @param data 资源数据
     */
    tagResource(quint16 cn, QByteArray data)
        : checknum(cn), databytes(data) {}

    /** @brief 资源校验和
     */
    quint16 checknum;

    /** @brief 资源数据
     */
    QByteArray databytes;
};

/** @class CResourceManager
 *  @brief 管理文件和 ZIP 资源的类
 *
 *  该类负责加载、存储和管理文件资源（包括普通文件和 ZIP 文件），支持文件监控和动态更新。
 */
class CResourceManager : public QObject /*, public Singleton<CResourceManager>*/ {
    Q_OBJECT

public:
    /** @brief 构造函数
     *
     *  初始化资源管理器。
     */
    explicit CResourceManager();

    /** @brief 析构函数
     *
     *  销毁资源管理器，释放所有
    */
    ~CResourceManager() override;

    /** @brief 获取指定路径的文件资源
     *
     *  加载并返回指定路径的文件资源内容。
     *
     *  @param filepath 文件路径
     *  @return 文件内容的字节数组
     */
    QByteArray getFile(const QString& filepath);

    /** @brief 添加 ZIP 文件
     *
     *  将指定 ZIP 文件添加到资源管理器。
     *
     *  @param filepath ZIP 文件路径
     *  @param isautoupdate 是否自动监控文件变化
     *  @return 添加成功返回 true，否则返回 false
     */
    bool addZipFile(const QString& filepath, bool isautoupdate = true);

    /** @brief 从 ZIP 文件中获取文件资源
     *
     *  从指定的 ZIP 文件中提取文件内容。
     *
     *  @param filepath ZIP Parasite framework
     *
     *  @return 提取的文件内容的字节数组
     */
    QByteArray getFileFromZip(const QString& filepath);

private slots:
    /** @brief 处理文件变化信号
     *
     *  记录文件变化并启动定时器以延迟处理。
     *
     *  @param filepath 变化的文件路径
     */
    void WorkingfileChanging(const QString& filepath);

    /** @brief 处理文件变化完成
     *
     *  加载变化后的文件资源。
     */
    void WorkingfileChanged();

private:
    /** @brief 加载文件资源
     *
     *  加载指定路径的文件资源，并可选择是否添加到文件监控。
     *
     *  @param filepath 文件路径
     *  @param isAddPath 是否添加到文件监控
     */
    void loadFileResource(const QString& filepath, bool isAddPath = true);

    /** @brief 删除所有 ZIP 文件资源
     *
     *  关闭并删除所有已加载的 ZIP 文件资源。
     */
    void deleteallzipfiles();

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
