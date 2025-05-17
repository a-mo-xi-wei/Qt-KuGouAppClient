/**
 * @file MVWidget.h
 * @brief 定义 MVWidget 类，提供音乐视频分类界面
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef MVWIDGET_H
#define MVWIDGET_H

#include <QWidget>

class QScrollBar;
class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MVWidget; }
QT_END_NAMESPACE

/**
 * @class MVWidget
 * @brief 音乐视频分类界面类，支持推荐、华语、欧美等分类
 */
class MVWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐视频界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MVWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MVWidget() override;

private:
    /**
     * @brief 初始化按钮组
     */
    void initButtonGroup();

    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 初始化直播场景分类
     */
    void initLiveScene();

    /**
     * @brief 初始化王者荣耀分类
     */
    void initHonorOfKings();

    /**
     * @brief 初始化颁奖典礼分类
     */
    void initAwardCeremony();

    /**
     * @brief 初始化热门 MV 分类
     */
    void initHotMV();

    /**
     * @brief 初始化数据容器
     */
    void initVector();

    /**
     * @brief 解析标题
     * @param title 原始标题
     * @return 格式化后的标题
     */
    const QString parseTitle(const QString &title);

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件对象
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 显示事件
     * @param event 显示事件对象
     * @note 重写基类方法
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件对象
     * @note 重写基类方法
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param watched 目标对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    /**
     * @brief 推荐按钮点击槽
     */
    void on_recommend_pushButton_clicked();

    /**
     * @brief 华语按钮点击槽
     */
    void on_chinese_pushButton_clicked();

    /**
     * @brief 欧美按钮点击槽
     */
    void on_west_pushButton_clicked();

    /**
     * @brief 日韩按钮点击槽
     */
    void on_koreaAndJapan_pushButton_clicked();

    /**
     * @brief 更多按钮 1 点击槽
     */
    void on_more_pushButton1_clicked();

    /**
     * @brief 更多按钮 2 点击槽
     */
    void on_more_pushButton2_clicked();

    /**
     * @brief 更多按钮 3 点击槽
     */
    void on_more_pushButton3_clicked();

    /**
     * @brief 更多按钮 4 点击槽
     */
    void on_more_pushButton4_clicked();

    /**
     * @brief 更多按钮 5 点击槽
     */
    void on_more_pushButton5_clicked();

private:
    /**
     * @struct MusicInfo
     * @brief 音乐信息结构体，存储图片路径、标题和描述
     */
    struct MusicInfo
    {
        /**
         * @brief 构造函数（左值引用）
         * @param pixmapPath 图片路径
         * @param title 标题
         * @param description 描述
         */
        MusicInfo(const QString &pixmapPath, const QString &title, const QString &description)
            : pixPath(pixmapPath), title(title), description(description) {}

        /**
         * @brief 构造函数（右值引用）
         * @param pixmapPath 图片路径
         * @param title 标题
         * @param description 描述
         */
        MusicInfo(QString &&pixmapPath, QString &&title, QString &&description)
            : pixPath(std::move(pixmapPath)), title(std::move(title)), description(std::move(description)) {}

        QString pixPath;     ///< 图片路径
        QString title;       ///< 标题
        QString description; ///< 描述
    };

    Ui::MVWidget                            *ui;                    ///< UI 指针
    std::unique_ptr<QButtonGroup>            m_buttonGroup;         ///< 按钮组
    QAction                                 *m_searchAction{};      ///< 搜索动作
    std::vector<std::pair<QString, QString>> m_titleAndDesc;        ///< 标题和描述对
    std::vector<MusicInfo>                   m_total;               ///< 所有音乐信息
    std::vector<MusicInfo>                   m_recommendVector;     ///< 推荐分类
    std::vector<MusicInfo>                   m_chineseVector;       ///< 华语分类
    std::vector<MusicInfo>                   m_westVector;          ///< 欧美分类
    std::vector<MusicInfo>                   m_koreaAndJapanVector; ///< 日韩分类
    std::vector<MusicInfo>                   m_liveSceneVector;     ///< 直播场景分类
    std::vector<MusicInfo>                   m_honorOfKingsVector;  ///< 王者荣耀分类
    std::vector<MusicInfo>                   m_awardCeremonyVector; ///< 颁奖典礼分类
    std::vector<MusicInfo>                   m_hotMVVector;         ///< 热门 MV 分类
};

#endif // MVWIDGET_H