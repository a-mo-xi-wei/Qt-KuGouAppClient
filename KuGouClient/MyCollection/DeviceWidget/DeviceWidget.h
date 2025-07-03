/**
* @file DeviceWidget.h
 * @brief 定义 DeviceWidget 类，管理设备界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class DeviceWidget;
}
QT_END_NAMESPACE

/**
 * @class DeviceWidget
 * @brief 设备界面类，管理设备相关操作和展示
 */
class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化设备界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit DeviceWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~DeviceWidget() override;

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
    Ui::DeviceWidget *ui; ///< UI 指针
};

#endif // DEVICEWIDGET_H