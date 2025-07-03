/**
* @file SpecialAlbum.h
 * @brief 定义 SpecialAlbum 类，管理专辑界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef SPECIALALBUM_H
#define SPECIALALBUM_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class SpecialAlbum;
}
QT_END_NAMESPACE

/**
 * @class SpecialAlbum
 * @brief 专辑界面类，管理专辑相关操作和展示
 */
class SpecialAlbum : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化专辑界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SpecialAlbum(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~SpecialAlbum() override;

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
    Ui::SpecialAlbum *ui; ///< UI 指针
};

#endif // SPECIALALBUM_H