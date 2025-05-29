/**
* @file SpeedDialogState.cpp
 * @brief 实现 SpeedDialogState 类，管理弹窗状态的持久化
 * @author WeiWang
 * @date 2025-05-27
 * @version 1.0
 */

#include "SpeedDialogState.h"

/**
 * @brief 组织名称
 */
const QString SpeedDialogState::SETTINGS_ORG = "WeiWang";

/**
 * @brief 应用名称
 */
const QString SpeedDialogState::SETTINGS_APP = "KuGouPlayer";

/**
 * @brief DJ 模式键
 */
const QString SpeedDialogState::KEY_DJ_MODE = "SpeedDialog/isDJMode";

/**
 * @brief DJ 按钮键
 */
const QString SpeedDialogState::KEY_DJ_BUTTON = "SpeedDialog/selectedDJButton";

/**
 * @brief 升降调键
 */
const QString SpeedDialogState::KEY_ADJUSTMENT = "SpeedDialog/adjustmentValue";

/**
 * @brief 倍速键
 */
const QString SpeedDialogState::KEY_SPEED = "SpeedDialog/speedValue";

/**
 * @brief 构造函数
 * @note 自动加载状态
 */
SpeedDialogState::SpeedDialogState()
{
    load();                                                   ///< 加载状态
}

/**
 * @brief 加载状态
 * @note 从 QSettings 读取状态值
 */
void SpeedDialogState::load()
{
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    isDJMode = settings.value(KEY_DJ_MODE, false).toBool();    ///< 加载 DJ 模式
    selectedDJButton = settings.value(KEY_DJ_BUTTON, "").toString(); ///< 加载 DJ 按钮
    adjustmentValue = settings.value(KEY_ADJUSTMENT, 50).toInt(); ///< 加载升降调值
    speedValue = settings.value(KEY_SPEED, 50).toInt();        ///< 加载倍速值
}

/**
 * @brief 保存状态
 * @note 将状态值写入 QSettings
 */
void SpeedDialogState::save()
{
    QSettings settings(SETTINGS_ORG, SETTINGS_APP);
    settings.setValue(KEY_DJ_MODE, isDJMode);                 ///< 保存 DJ 模式
    settings.setValue(KEY_DJ_BUTTON, selectedDJButton);       ///< 保存 DJ 按钮
    settings.setValue(KEY_ADJUSTMENT, adjustmentValue);       ///< 保存升降调值
    settings.setValue(KEY_SPEED, speedValue);                 ///< 保存倍速值
}

// 重置为默认状态
void SpeedDialogState::reset() {
    isDJMode = false;
    selectedDJButton.clear();
    adjustmentValue = 50;
    speedValue = 50;
}