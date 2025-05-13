#include "ElaIcon.h"
#include <QPainter>
#include <QPixmap>

/**
 * @brief   定义单例类实现
 *
 * 为 ElaIcon 类生成线程安全的单例实现。
 */
Q_SINGLETON_CREATE_CPP(ElaIcon)

/**
 * @brief   构造函数
 *
 * 初始化 ElaIcon 单例对象。
 */
ElaIcon::ElaIcon()
{
}

/**
 * @brief   析构函数
 *
 * 清理 ElaIcon 单例对象的资源。
 */
ElaIcon::~ElaIcon()
{
}

/**
 * @brief   获取指定图标
 * @param   awesome  图标名称，来自 ElaIconType::IconName 枚举
 * @return  生成的 QIcon 对象
 *
 * 使用默认尺寸（30x30）和默认颜色生成图标。
 */
QIcon ElaIcon::getElaIcon(ElaIconType::IconName awesome)
{
    QFont iconFont = QFont("ElaAwesome");
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    // painter.setPen(QColor("#1570A5"));
    // painter.setBrush(QColor("#1570A5"));
    iconFont.setPixelSize(25);
    painter.setFont(iconFont);
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

/**
 * @brief   获取指定图标并设置颜色
 * @param   awesome   图标名称，来自 ElaIconType::IconName 枚举
 * @param   iconColor 图标的颜色
 * @return  生成的 QIcon 对象
 *
 * 使用默认尺寸（30x30）和指定颜色生成图标。
 */
QIcon ElaIcon::getElaIcon(ElaIconType::IconName awesome, QColor iconColor)
{
    QFont iconFont = QFont("ElaAwesome");
    QPixmap pix(30, 30);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    // painter.setBrush(QColor("#1570A5"));
    iconFont.setPixelSize(25);
    painter.setFont(iconFont);
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

/**
 * @brief   获取指定图标并设置像素大小
 * @param   awesome    图标名称，来自 ElaIconType::IconName 枚举
 * @param   pixelSize  图标的像素大小
 * @return  生成的 QIcon 对象
 *
 * 使用指定像素大小和默认颜色生成图标。
 */
QIcon ElaIcon::getElaIcon(ElaIconType::IconName awesome, int pixelSize)
{
    QFont iconFont = QFont("ElaAwesome");
    QPixmap pix(pixelSize, pixelSize);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

/**
 * @brief   获取指定图标并设置像素大小和颜色
 * @param   awesome    图标名称，来自 ElaIconType::IconName 枚举
 * @param   pixelSize  图标的像素大小
 * @param   iconColor  图标的颜色
 * @return  生成的 QIcon 对象
 *
 * 使用指定像素大小和颜色生成图标。
 */
QIcon ElaIcon::getElaIcon(ElaIconType::IconName awesome, int pixelSize, QColor iconColor)
{
    QFont iconFont = QFont("ElaAwesome");
    QPixmap pix(pixelSize, pixelSize);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

/**
 * @brief   获取指定图标并设置像素大小和固定宽高
 * @param   awesome      图标名称，来自 ElaIconType::IconName 枚举
 * @param   pixelSize    图标的像素大小
 * @param   fixedWidth   图标的固定宽度
 * @param   fixedHeight  图标的固定高度
 * @return  生成的 QIcon 对象
 *
 * 使用指定像素大小、固定宽高和默认颜色生成图标。
 */
QIcon ElaIcon::getElaIcon(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight)
{
    QFont iconFont = QFont("ElaAwesome");
    QPixmap pix(fixedWidth, fixedHeight);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}

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
QIcon ElaIcon::getElaIcon(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor)
{
    QFont iconFont = QFont("ElaAwesome");
    QPixmap pix(fixedWidth, fixedHeight);
    pix.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(iconColor);
    iconFont.setPixelSize(pixelSize);
    painter.setFont(iconFont);
    // 画图形字体
    painter.drawText(pix.rect(), Qt::AlignCenter, QChar((unsigned short)awesome));
    painter.end();
    return QIcon(pix);
}