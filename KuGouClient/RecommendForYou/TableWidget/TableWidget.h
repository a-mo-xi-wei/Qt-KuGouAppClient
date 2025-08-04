/**
 * @file TableWidget.h
 * @brief 定义 TableWidget 类，管理表格布局推荐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef TABLEWIDGET_H_
#define TABLEWIDGET_H_

#include <QGridLayout>
#include "SMaskWidget.h"

/**
 * @class QLabel
 * @brief 标签类，用于显示文本或图片
 */
class QLabel;

/**
 * @class QToolButton
 * @brief 工具按钮类，用于交互操作
 */
class QToolButton;

/**
 * @class TableWidget
 * @brief 表格布局推荐界面类，支持块状和小图标列表模式
 */
class TableWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 推荐界面类型枚举
     */
    enum KIND
    {
        BlockList, ///< 块状列表
        ItemList   ///< 小图标列表
    };

    /**
     * @brief 构造函数，初始化推荐界面
     * @param title 标题文本
     * @param kind 推荐界面类型
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit TableWidget(const QString& title, KIND kind, QWidget* parent = nullptr);

protected:
    /**
     * @brief 绘制事件
     * @param ev 绘制事件
     * @note 绘制控件背景
     */
    void paintEvent(QPaintEvent* ev) override;

    /**
     * @brief 鼠标进入事件
     * @param ev 进入事件
     * @note 处理悬停效果
     */
    void enterEvent(QEnterEvent* ev) override;

    /**
     * @brief 鼠标离开事件
     * @param ev 离开事件
     * @note 恢复默认状态
     */
    void leaveEvent(QEvent* ev) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整网格布局
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 处理点击交互
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 处理释放交互
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     * @note 处理双击交互
     */
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理特定控件事件
     */
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化标题、按钮、网格布局和容器
     */
    void initUi();

    void initSource();

    /**
     * @brief 初始化块状封面
     * @note 设置块状推荐项的封面路径
     */
    void initBlockCover();

    /**
     * @brief 打乱块状封面
     * @note 随机打乱块状封面顺序
     */
    void shuffleBlockCover();

    /**
     * @brief 初始化列表封面
     * @note 设置小图标列表推荐项的封面路径
     */
    void initListCover();

    /**
     * @brief 打乱列表封面
     * @note 随机打乱列表封面顺序
     */
    void shuffleListCover();

    /**
     * @brief 初始化块状推荐控件
     * @note 创建块状推荐项并添加到网格
     */
    void initItemBlockWidget();

    /**
     * @brief 初始化小图标列表推荐控件
     * @note 创建小图标列表推荐项并添加到网格
     */
    void initItemListWidget();

signals:
    /**
     * @brief 隐藏标题信号
     */
    void hideTitle();

    /**
     * @brief 网格列数改变信号
     * @param len 新的列数
     */
    void gridChange(const int& len);

public slots:
    /**
     * @brief 网格列数改变槽函数
     * @param len 新的列数
     * @note 调整网格布局列数
     */
    void onGridChange(int len);

    /**
     * @brief 刷新按钮点击槽函数
     * @note 触发网格内容刷新
     */
    void onRefreshBtnClicked();

    /**
     * @brief 刷新定时器超时槽函数
     * @note 执行网格内容刷新
     */
    void onRefreshTimeout();

private:
    QHBoxLayout* m_tabHLayout;                   ///< 标题栏水平布局
    QGridLayout* m_gridLayout;                   ///< 网格布局
    QWidget* m_gridContainer;                    ///< 网格容器
    QLabel* m_titleLab{};                        ///< 标题标签
    KIND m_kind;                                 ///< 推荐界面类型
    QToolButton* m_play_ToolBtn{};               ///< 播放按钮
    QToolButton* m_adjust_ToolBtn{};             ///< 调整按钮
    QLabel* m_more_Lab{};                        ///< 更多标签
    QToolButton* m_refresh_ToolBtn{};            ///< 刷新按钮
    QLabel* line1{};                             ///< 竖线
    int m_showCol = 5;                           ///< 显示列数
    QVector<QString> m_blockCoverPaths;          ///< 块状封面路径
    QVector<QString> m_listCoverPaths;           ///< 列表封面路径
    QVector<QString> m_descVector;               ///< 描述文本
    QVector<QPair<QString, QString>> m_songInfo; ///< 歌曲信息（歌曲名，歌手）
    bool m_hide_col_6 = true;                    ///< 是否隐藏第六列
    bool m_hide_col_7 = true;                    ///< 是否隐藏第七列
    QTimer* m_refreshTimer{};                    ///< 刷新定时器
};

/**
 * @class ItemBlockWidget
 * @brief 块状推荐项控件类，显示封面和描述
 */
class ItemBlockWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化块状推荐项
     * @param path 封面路径
     * @param desc 描述文本
     * @param parent 父控件指针，默认为 nullptr
     */
    ItemBlockWidget(const QString& path, const QString& desc, QWidget* parent = nullptr);

    /**
     * @brief 设置提示标签文本
     * @param text 提示文本
     */
    void setTipLabText(const QString& text) const;

    /**
     * @brief 设置流行按钮文本
     * @param text 按钮文本
     */
    void setPopularBtnText(const QString& text) const;

    /**
     * @brief 设置描述文本
     * @param desc 描述文本
     */
    void setDescribeText(QString desc) const;

private:
    /**
     * @brief 初始化界面
     * @note 初始化封面、描述、提示标签和流行按钮
     */
    void initUi();

protected:
    /**
     * @brief 绘制事件
     * @param ev 绘制事件
     * @note 绘制控件背景
     */
    void paintEvent(QPaintEvent* ev) override;

    /**
     * @brief 鼠标进入事件
     * @param ev 进入事件
     * @note 处理悬停效果
     */
    void enterEvent(QEnterEvent* ev) override;

    /**
     * @brief 鼠标离开事件
     * @param ev 离开事件
     * @note 恢复默认状态
     */
    void leaveEvent(QEvent* ev) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整控件布局
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 处理点击交互
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 处理释放交互
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     * @note 处理双击交互
     */
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QWidget* m_bacWidget;                ///< 背景控件
    std::unique_ptr<SMaskWidget> m_mask; ///< 遮罩控件
    QLabel* m_describeLab;               ///< 描述标签
    QLabel* m_tipLab;                    ///< 提示标签
    QToolButton* m_popularBtn;           ///< 流行按钮
    bool m_isHoverCoverLab = false;      ///< 是否悬停封面
    QString m_descText;                  ///< 描述文本
};

/**
 * @class ItemListWidget
 * @brief 小图标列表推荐项控件类，显示封面、歌曲名和歌手
 */
class ItemListWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化列表推荐项
     * @param coverPix 封面图片
     * @param name 歌曲名
     * @param author 歌手
     * @param parent 父控件指针，默认为 nullptr
     */
    ItemListWidget(QPixmap coverPix, const QString& name, const QString& author, QWidget* parent = nullptr);

    /**
     * @brief 设置歌曲名
     * @param name 歌曲名
     */
    void setNameText(const QString& name);

    /**
     * @brief 设置歌手
     * @param singer 歌手
     */
    void setSingerText(const QString& singer);

private:
    /**
     * @brief 初始化界面
     * @note 初始化封面、歌曲名、歌手和操作按钮
     */
    void initUi();

    /**
     * @brief 更新歌曲名
     * @note 更新歌曲名标签
     */
    void updateSongName() const;

    /**
     * @brief 更新歌手
     * @note 更新歌手标签
     */
    void updateSinger() const;

private slots:
    /**
     * @brief 播放添加按钮点击槽函数
     * @note 处理播放或添加到播放列表
     */
    void onPlayAddToolBtn();

    /**
     * @brief 喜欢按钮点击槽函数
     * @note 处理添加喜欢的歌曲
     */
    void onLikeToolBtn();

    /**
     * @brief 更多按钮点击槽函数
     * @note 处理更多操作
     */
    void onMoreToolBtn();

protected:
    /**
     * @brief 绘制事件
     * @param ev 绘制事件
     * @note 绘制控件背景
     */
    void paintEvent(QPaintEvent* ev) override;

    /**
     * @brief 鼠标进入事件
     * @param ev 进入事件
     * @note 处理悬停效果
     */
    void enterEvent(QEnterEvent* ev) override;

    /**
     * @brief 鼠标离开事件
     * @param ev 离开事件
     * @note 恢复默认状态
     */
    void leaveEvent(QEvent* ev) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整控件布局
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 处理点击交互
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 处理释放交互
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     * @note 处理双击交互
     */
    void mouseDoubleClickEvent(QMouseEvent* event) override;

public slots:
    /**
     * @brief 隐藏控件槽函数
     * @note 隐藏推荐项
     */
    void onHide();

private:
    std::unique_ptr<SMaskWidget> m_mask; ///< 遮罩控件
    QLabel* m_coverLab;                  ///< 封面标签
    QLabel* m_songNameLab;               ///< 歌曲名标签
    QLabel* m_singerLab;                 ///< 歌手标签
    QWidget* m_optionWidget;             ///< 选项控件，用于计算标签宽度
    QToolButton* m_play_add_ToolBtn;     ///< 播放添加按钮
    QToolButton* m_like_ToolBtn;         ///< 喜欢按钮
    QToolButton* m_more_ToolBtn;         ///< 更多按钮
    QString m_songName;                  ///< 歌曲名
    QString m_singer;                    ///< 歌手
    bool m_isHoverCoverLab = false;      ///< 是否悬停封面
    int m_aspectRatio      = 6;          ///< 宽高比
};

#endif // TABLEWIDGET_H_
