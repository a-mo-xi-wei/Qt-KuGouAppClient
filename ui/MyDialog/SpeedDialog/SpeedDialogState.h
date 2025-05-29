/**
* @file SpeedDialogState.h
 * @brief 定义 SpeedDialogState 类，用于管理弹窗状态
 * @author WeiWang
 * @date 2025-05-27
 * @version 1.0
 */

#ifndef SPEEDDIALOGSTATE_H
#define SPEEDDIALOGSTATE_H

#include <QSettings>

/**
 * @def MYDIALOG_EXPORT
 * @brief 动态库导出宏，定义库的导出/导入行为
 */
#if defined(MYDIALOG_LIBRARY)
#define MYDIALOG_EXPORT Q_DECL_EXPORT
#else
#define MYDIALOG_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class SpeedDialogState
 * @brief 管理 SpeedDialog 的状态，包含 DJ 模式和滑块值
 */
class MYDIALOG_EXPORT SpeedDialogState
{
public:
    /**
     * @brief 构造函数
     * @note 自动加载状态
     */
    SpeedDialogState();

    /**
     * @brief 加载状态
     * @note 从 QSettings 读取
     */
    void load();

    /**
     * @brief 保存状态
     * @note 写入 QSettings
     */
    void save();

    /**
     * @brief 重置状态
     * @note 重置 QSettings
     */
    void reset();

    bool isDJMode = false;                                    ///< 是否启用 DJ 模式
    QString selectedDJButton;                                 ///< 选中的 DJ 按钮
    int adjustmentValue = 50;                                 ///< 升降调值（0-100）
    int speedValue = 50;                                      ///< 倍速值（0-100）

private:
    static const QString SETTINGS_ORG;                        ///< 组织名称
    static const QString SETTINGS_APP;                        ///< 应用名称
    static const QString KEY_DJ_MODE;                         ///< DJ 模式键
    static const QString KEY_DJ_BUTTON;                       ///< DJ 按钮键
    static const QString KEY_ADJUSTMENT;                      ///< 升降调键
    static const QString KEY_SPEED;                           ///< 倍速键
};

#endif //SPEEDDIALOGSTATE_H