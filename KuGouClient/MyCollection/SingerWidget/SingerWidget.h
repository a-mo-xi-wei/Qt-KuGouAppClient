/**
* @file SingerWidget.h
 * @brief 定义 SingerWidget 类，管理歌手界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef SINGERWIDGET_H
#define SINGERWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class SingerWidget;
}
QT_END_NAMESPACE

/**
 * @class SingerWidget
 * @brief 歌手界面类，管理歌手相关操作和展示
 */
class SingerWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化歌手界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SingerWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~SingerWidget() override;

    private slots:
        /**
         * @brief 搜索按钮点击槽函数
         * @note 触发搜索更多音乐的信号
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐的信号
         */
        void find_more_music();

private:
    Ui::SingerWidget *ui; ///< UI 指针
};

#endif // SINGERWIDGET_H