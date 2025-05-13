#ifndef ELAICON_H
#define ELAICON_H

#include <QIcon>
#include "Def.h"
#include "singleton.h"
#include "stdafx.h"

/**
 * @class   ElaIcon
 * @brief   提供图标生成功能的单例类
 *
 * 该类使用单例模式，基于 ElaAwesome 字体生成自定义图标，支持多种尺寸、颜色和固定宽高的配置。
 */
class ELA_EXPORT ElaIcon
{
    Q_SINGLETON_CREATE_H(ElaIcon)

private:
    /**
     * @brief   构造函数
     *
     * 私有构造函数，确保类只能通过单例模式实例化。
     */
    explicit ElaIcon();

    /**
     * @brief   析构函数
     *
     * 释放资源，确保单例对象的正确销毁。
     */
    ~ElaIcon();

public:
    /**
     * @brief   获取指定图标
     * @param   awesome  图标名称，来自 ElaIconType::IconName 枚举
     * @return  生成的 QIcon 对象
     *
     * 使用默认尺寸和颜色生成图标。
     */
    QIcon getElaIcon(ElaIconType::IconName awesome);

    /**
     * @brief   获取指定图标并设置颜色
     * @param   awesome   图标名称，来自 ElaIconType::IconName 枚举
     * @param   iconColor 图标的颜色
     * @return  生成的 QIcon 对象
     *
     * 使用默认尺寸生成指定颜色的图标。
     */
    QIcon getElaIcon(ElaIconType::IconName awesome, QColor iconColor);

    /**
     * @brief   获取指定图标并设置像素大小
     * @param   awesome    图标名称，来自 ElaIconType::IconName 枚举
     * @param   pixelSize  图标的像素大小
     * @return  生成的 QIcon 对象
     *
     * 使用指定像素大小生成默认颜色的图标。
     */
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize);

    /**
     * @brief   获取指定图标并设置像素大小和颜色
     * @param   awesome    图标名称，来自 ElaIconType::IconName 枚举
     * @param   pixelSize  图标的像素大小
     * @param   iconColor  图标的颜色
     * @return  生成的 QIcon 对象
     *
     * 使用指定像素大小和颜色生成图标。
     */
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize, QColor iconColor);

    /**
     * @brief   获取指定图标并设置像素大小和固定宽高
     * @param   awesome      图标名称，来自 ElaIconType::IconName 枚举
     * @param   pixelSize    图标的像素大小
     * @param   fixedWidth   图标的固定宽度
     * @param   fixedHeight  图标的固定高度
     * @return  生成的 QIcon 对象
     *
     * 使用指定像素大小和固定宽高生成默认颜色的图标。
     */
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight);

    /**
     * @brief   获取指定图标并设置像素大小、固定宽高和颜色
     * @param   awesome      图标名称，来自 ElaIconType::IconName 枚举
     * @param   pixelSize    图标的像素大小
     * @param   fixedWidth   图标的固定宽度
     * @param   fixedHeight  图标的固定高度
     * @param   iconColor    图标的颜色
     * @return  生成的 QIcon 对象
     *
     * 使用指定像素大小、固定宽高和颜色生成图标。
     */
    QIcon getElaIcon(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor);
};

#endif // ELAICON_H