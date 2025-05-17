/**
 * @file MusicRepoList.h
 * @brief 定义 MusicRepoList 类，显示音乐仓库列表项
 * @author WeiWang
 * @date 2024-11-24
 * @version 1.0
 */

#ifndef MUSICREPOLIST_H
#define MUSICREPOLIST_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MusicRepoList;
}
QT_END_NAMESPACE

/**
 * @class MusicRepoList
 * @brief 音乐仓库列表项类，显示歌曲封面、名称和歌手信息
 */
class MusicRepoList : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐仓库列表项
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MusicRepoList(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MusicRepoList() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置歌曲名称
     * @param song 歌曲名称
     */
    void setSongName(const QString &song);

    /**
     * @brief 设置歌手名称
     * @param singer 歌手名称
     */
    void setSinger(const QString &singer);

private:
    /**
     * @brief 初始化界面
     * @note 隐藏工具按钮、设置遮罩、扩展响应范围和工具提示
     */
    void initUi();

    /**
     * @brief 更新歌曲名称文本
     * @note 根据控件宽度截断文本
     */
    void updateSongText() const;

    /**
     * @brief 更新歌手名称文本
     * @note 根据控件宽度截断文本
     */
    void updateSingerText() const;

signals:
    /**
     * @brief 进入列表信号
     */
    void enterList();

    /**
     * @brief 离开列表信号
     */
    void leaveList();

    /**
     * @brief 当前歌曲名称变化信号
     * @param songName 歌曲名称
     */
    void curPlaySongNameChange(const QString &songName);

    /**
     * @brief 当前歌手名称变化信号
     * @param singer 歌手名称
     */
    void curPlaySingerChange(const QString &singer);

private slots:
    /**
     * @brief 播放添加按钮点击槽函数
     * @note 显示未实现提示
     */
    void onPlayAddToolBtn();

    /**
     * @brief 收藏按钮点击槽函数
     * @note 显示未实现提示
     */
    void onLikeToolBtn();

    /**
     * @brief 评论按钮点击槽函数
     * @note 显示未实现提示
     */
    void onCommentToolBtn();

protected:
    /**
     * @brief 进入事件
     * @param event 进入事件
     * @note 显示按钮和遮罩
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 离开事件
     * @param event 事件
     * @note 隐藏按钮和遮罩
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制圆角背景
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 更新文本
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MusicRepoList                   *ui;                    ///< UI 指针
    bool                                 m_isEnter = false;     ///< 是否进入状态
    QString                              m_songName;            ///< 歌曲名称
    QString                              m_singer;              ///< 歌手名称
};

#endif // MUSICREPOLIST_H