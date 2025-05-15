/**
 * @file BaseMenu.cpp
 * @brief 实现 BaseMenu 类，提供菜单基类功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#include "BaseMenu.h"
#include "logger.hpp"

#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QCoreApplication>
#include <QWidgetAction>
#include <QGraphicsOpacityEffect>
#include <QTimeLine>

/**
 * @brief 获取当前文件所在目录路径
 */
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化菜单基类
 * @param parent 父控件指针，默认为 nullptr
 */
BaseMenu::BaseMenu(QWidget *parent)
    : QMenu(parent)
{
    // 设置透明背景和窗口属性
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_Hover, true);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setMouseTracking(true);

    // 加载样式表
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/menu.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            STREAM_INFO() << "样式表打开失败QAQ"; ///< 记录样式表加载失败
            return;
        }
    }

    // 初始化透明度效果
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(0.0);
    this->setGraphicsEffect(m_opacityEffect);

    // 初始化动画时间线
    m_timeLine = new QTimeLine(300, this);
    connect(m_timeLine, &QTimeLine::valueChanged, this, [=](const qreal &value) {
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0, QColor(255, 255, 255, 255));
        gradient.setColorAt(value, QColor(255, 255, 255, 255));
        gradient.setColorAt(1, QColor(255, 255, 255, 0));
        m_opacityEffect->setOpacityMask(gradient); ///< 设置渐变遮罩
        m_opacityEffect->setOpacity(value); ///< 更新透明度
    });
    connect(m_timeLine, &QTimeLine::finished, this, [=]() {
        m_opacityEffect->setOpacity(1); ///< 确保完全显示
        m_opacityEffect->setOpacityMask(QBrush()); ///< 清除残留遮罩
    });
}

/**
 * @brief 获取当前菜单对象
 * @return 当前菜单对象指针
 */
const BaseMenu *BaseMenu::getMenu() const {
    return this;
}

/**
 * @brief 设置当前响应项的索引
 * @param idx 索引值
 */
void BaseMenu::setCurIndex(const int &idx) {
    this->m_curIndex = idx;
}

/**
 * @brief 设置动画持续时间
 * @param duration 动画持续时间（毫秒）
 */
void BaseMenu::setAniDuration(const int &duration) {
    m_timeLine->setDuration(duration);
}

/**
 * @brief 获取当前样式表内容
 * @return 样式表字符串
 */
QString BaseMenu::getStyleSheet() const {
    return this->styleSheet();
}

/**
 * @brief 检查并更新悬停状态，清理之前的悬停控件
 */
void BaseMenu::checkHover() {
    if (!this->m_lastHover.isEmpty()) {
        QEvent leaveEvent(QEvent::Leave);
        for (const auto val : m_lastHover) {
            QCoreApplication::sendEvent(val, &leaveEvent); ///< 发送离开事件
        }
        m_lastHover.clear();
        m_currentHover.clear();
    }
}

/**
 * @brief 检查并更新选中状态，清除上一次选中的图标
 */
void BaseMenu::checkSelection() const {
    if (m_lastSelect) {
        if (m_lastSelect == m_curSelect) return; ///< 当前选中未变化，直接返回
        if (!m_lastSelect->icon().isNull()) {
            m_lastSelect->setIcon(QIcon()); ///< 清空上一次选中的图标
            // STREAM_WARN() << "图标设置为空"; ///< 调试用，记录图标清空
        }
    }
}

/**
 * @brief 重写绘制事件，绘制自定义阴影效果
 * @param event 绘制事件
 */
void BaseMenu::paintEvent(QPaintEvent *event) {
    QMenu::paintEvent(event);

    // 绘制阴影
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(m_shadowWidth, m_shadowWidth, this->width() - m_shadowWidth * 2,
                        this->height() - m_shadowWidth * 2, m_shadowRadius, m_shadowRadius);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i != m_shadowWidth; ++i) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(m_shadowWidth - i, m_shadowWidth - i, this->width() - (m_shadowWidth - i) * 2,
                            this->height() - (m_shadowWidth - i) * 2, m_shadowRadius, m_shadowRadius);
        m_shadowColor.setAlpha(180 - static_cast<int>(qSqrt(i) * 80)); ///< 调整阴影透明度
        painter.setPen(m_shadowColor);
        painter.drawPath(path);
    }
}

/**
 * @brief 重写显示事件，触发显示动画
 * @param event 显示事件
 */
void BaseMenu::showEvent(QShowEvent *event) {
    QMenu::showEvent(event);
    // 重置并启动动画
    m_opacityEffect->setOpacity(0.0);
    m_timeLine->stop();
    m_timeLine->setCurrentTime(0); // 重置进度
    m_timeLine->start();
}

/**
 * @brief 重写离开事件，清理悬停状态
 * @param event 离开事件
 */
void BaseMenu::leaveEvent(QEvent *event) {
    QMenu::leaveEvent(event);
    checkHover();
}

/**
 * @brief 连接 QWidgetAction 的悬停事件到按钮
 * @param widgetAction 菜单项动作
 * @param btn 关联的按钮
 */
void BaseMenu::connectAction(const QWidgetAction *widgetAction, MenuBtn *btn) {
    connect(widgetAction, &QWidgetAction::hovered, this, [btn, this] {
        checkHover();
        this->m_currentHover.emplace_back(btn);
        this->m_lastHover = this->m_currentHover;
        QEvent enterEvent(QEvent::Enter);
        QCoreApplication::sendEvent(btn, &enterEvent); ///< 发送进入事件
        btn->setAttribute(Qt::WA_UnderMouse, true); ///< 模拟按钮进入悬停状态
    });
}