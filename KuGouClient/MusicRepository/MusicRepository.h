/**
 * @file MusicRepository.h
 * @brief 定义 MusicRepository 类，管理音乐仓库（歌曲和视频）界面
 * @author WeiWang
 * @date 2024-11-11
 * @version 1.0
 */

#ifndef MUSICREPOSITORY_H
#define MUSICREPOSITORY_H

#include <QWidget>
#include <QPointer>
#include <array>

class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MusicRepository;
}
QT_END_NAMESPACE

/**
 * @class MusicRepository
 * @brief 音乐仓库界面类，管理歌曲和视频的展示
 */
class MusicRepository : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐仓库界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MusicRepository(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MusicRepository() override;

private:
    QWidget* createRepoPage(const int& beg);

    /**
     * @brief 初始化按钮组
     * @note 设置按钮互斥
     */
    void initButtonGroup();

    /**
     * @brief 初始化界面
     * @note 设置鼠标样式、初始化按钮组、容器、新碟上架和精选视频
     */
    void initUi();

    /**
     * @brief 初始化新碟上架控件
     * @note 随机打乱并隐藏部分块
     */
    void initNewDiskWidget();

    /**
     * @brief 初始化精选视频控件
     * @note 随机打乱并隐藏部分视频
     */
    void initSelectWidget();

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

protected:
    //想要通过事件过滤器去捕获每个list的 进入、离开事件是不行的，若是单个的话还行，所以还得要提升
    //bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 动态调整控件高度/宽度和显示/隐藏块
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief 更多按钮 1 点击槽函数
     * @note 显示未实现提示
     */
    void on_more_pushButton1_clicked();

    /**
     * @brief 更多按钮 2 点击槽函数
     * @note 显示未实现提示
     */
    void on_more_pushButton2_clicked();

    /**
     * @brief 更多按钮 3 点击槽函数
     * @note 显示未实现提示
     */
    void on_more_pushButton3_clicked();

private:
    /**
     * @struct MusicInfo
     * @brief 存储音乐信息的结构体
     */
    struct MusicInfo
    {
        /**
         * @brief 构造函数
         * @param pixmapPath 封面路径
         * @param song 歌曲/视频名称
         * @param singer 歌手/作者
         */
        MusicInfo(const QString &pixmapPath, const QString &song, const QString &singer)
            : pixPath(pixmapPath), song(song), singer(singer) {}

        QString pixPath;                                 ///< 封面路径
        QString song;                                    ///< 歌曲/视频名称
        QString singer;                                  ///< 歌手/作者
    };

    Ui::MusicRepository                         *ui;                    ///< UI 指针
    std::unique_ptr<QButtonGroup>               m_buttonGroup;          ///< 按钮组
    enum class MusicRegion {
        Chinese,
        West,
        Korea,
        Japan,
        Video
    };
    QList<MusicInfo> m_musicData;
    QList<MusicInfo> m_videoVector;
    std::array<QPointer<QWidget>, 4> m_repoPages {};  // QPointer 可检测被释放的页面
    int m_currentIdx;
};

#endif // MUSICREPOSITORY_H