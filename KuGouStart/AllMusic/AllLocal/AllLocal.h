/**
* @file AllLocal.h
 * @brief 定义 AllLocal 类，提供本地音乐界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef ALLLOCAL_H
#define ALLLOCAL_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class AllLocal;
}
QT_END_NAMESPACE

/**
 * @class AllLocal
 * @brief 本地音乐界面类，支持音乐列表显示和搜索
 */
class AllLocal : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化本地音乐界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AllLocal(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~AllLocal() override;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    private slots:
        /**
         * @brief 处理搜索按钮点击
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐信号
         */
        void find_more_music();

private:
    Ui::AllLocal *ui; ///< UI 指针
};

#endif // ALLLOCAL_H