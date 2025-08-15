#ifndef MVWIDGET_H
#define MVWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QTimer>

#include <array>

class RefreshMask;
class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE

namespace Ui {
class MVWidget;
}

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
     * @brief 创建仓库页面
     * @param beg 开始索引
     * @return 创建的页面控件
     */
    QWidget *createPage(const int &beg);

    /**
     * @brief 初始化按钮组
     */
    void initButtonGroup();

    /**
     * @brief 初始化界面
     */
    void initUi();

    void initAdvertiseWidget() const;

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
     * @brief 解析标题
     * @param title 原始标题
     * @return 格式化后的标题
     */
    const QString parseTitle(const QString &title);

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件对象
     * @note 重写基类方法
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param watched 目标对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    void showEvent(QShowEvent *event) override;

private slots:
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

public slots:
    void emitInitialized() { QTimer::singleShot(0, this, [this] { emit initialized(); }); }

signals:
    void initialized();

private:
    /**
     * @struct MusicInfo
     * @brief 音乐信息结构体，存储图片路径、标题和描述
     */
    struct MusicInfo
    {
        /**
         * @brief 构造函数
         * @param pixmapPath 图片路径
         * @param title 标题
         * @param description 描述
         */
        MusicInfo(const QString &pixmapPath, const QString &title, const QString &description)
            : pixPath(pixmapPath), title(title), description(description) {}

        QString pixPath;     ///< 图片路径
        QString title;       ///< 标题
        QString description; ///< 描述
    };

    Ui::MVWidget *ui;                                  ///< UI 指针
    std::unique_ptr<QButtonGroup> m_buttonGroup;       ///< 按钮组
    QAction *m_searchAction{};                         ///< 搜索动作
    QList<std::pair<QString, QString>> m_titleAndDesc; ///< 标题和描述对
    QList<MusicInfo> m_total;                          ///< 所有音乐信息
    std::array<QPointer<QWidget>, 4> m_pages{};        // QPointer 可检测被释放的页面
    int m_currentIdx{0};
    std::unique_ptr<RefreshMask> m_refreshMask; ///< 刷新遮罩
};

#endif // MVWIDGET_H