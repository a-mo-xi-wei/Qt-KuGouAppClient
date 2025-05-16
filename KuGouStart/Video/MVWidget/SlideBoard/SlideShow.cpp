/**
 * @file SlideShow.cpp
 * @brief 实现 SlideShow 类，提供轮播图控件功能
 * @author iwxyi
 * @date 2025-05-16
 * @version 1.0
 * @note 参考开源项目：https://github.com/iwxyi/Qt-SlideShow
 */

#include "SlideShow.h"
#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsDropShadowEffect>

/** @brief 阴影半径常量 */
#define SHADOW_RADIUS 12

/** @brief 创建阴影效果宏 */
#define CREATE_SHADOW(x)                                                \
do {                                                                    \
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(x); \
    effect->setOffset(3, 3);                                            \
    effect->setBlurRadius(SHADOW_RADIUS);                               \
    effect->setColor(QColor(64, 64, 64, 64));                           \
    x->setGraphicsEffect(effect);                                       \
} while (0)

/**
 * @brief 构造函数，初始化轮播图控件
 * @param parent 父控件指针，默认为 nullptr
 */
SlideShow::SlideShow(QWidget *parent) : QWidget(parent)
{
    setPixmapSize(QSize(520, 150)); ///< 设置默认图片大小

    indicationLayout = new QHBoxLayout; ///< 创建指示器布局
    auto mainLayout = new QVBoxLayout(this); ///< 创建主布局
    mainLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Preferred, QSizePolicy::Expanding)); ///< 添加弹性空间
    mainLayout->addLayout(indicationLayout); ///< 添加指示器布局
    indicationLayout->setAlignment(Qt::AlignCenter); ///< 居中对齐

    autoSlideTimer = new QTimer(this); ///< 创建自动轮播定时器
    autoSlideTimer->setInterval(5000); ///< 默认间隔 5 秒
    connect(autoSlideTimer, &QTimer::timeout, this, &SlideShow::slideToRight); ///< 连接定时器到右滑槽
}

/**
 * @brief 设置图片大小
 * @param size 图片尺寸
 */
void SlideShow::setPixmapSize(const QSize &size)
{
    this->oneSize = size;
    this->setMinimumSize(static_cast<int>(oneSize.width() * (3 - backScale * 2) + SHADOW_RADIUS), ///< 设置最小尺寸
                        oneSize.height() + SHADOW_RADIUS);
    for (int i = 0; i < labels.size(); i++)
    {
        labels.at(i)->setPixmap(getScaledRoundedPixmap(pixmaps.at(i)), oneSize.width() * imgOffside); ///< 更新图片
        labels.at(i)->setMinimumSize(1, 1); ///< 设置最小尺寸
        labels.at(i)->setMaximumSize(oneSize); ///< 设置最大尺寸
        labels.at(i)->resize(oneSize); ///< 调整大小
    }
}

/**
 * @brief 设置图片缩放模式（已废弃）
 * @param scale 是否缩放
 */
void SlideShow::setPixmapScale(const bool &scale)
{
    this->scalePixmap = scale;
    for (int i = 0; i < labels.size(); i++)
        labels.at(i)->setPixmap(getScaledRoundedPixmap(pixmaps.at(i)), oneSize.width() * imgOffside); ///< 更新图片
}

/**
 * @brief 设置自动轮播
 * @param interval 轮播间隔（毫秒），0 表示停止
 */
void SlideShow::setAutoSlide(const int &interval) const
{
    if (interval)
    {
        autoSlideTimer->setInterval(interval); ///< 设置间隔
        autoSlideTimer->start();              ///< 启动定时器
    }
    else
    {
        autoSlideTimer->stop();               ///< 停止定时器
    }
}

/**
 * @brief 添加图片
 * @param pixmap 图片
 * @param text 关联文本，默认为空
 */
void SlideShow::addImage(const QPixmap &pixmap, QString text)
{
    insertImage(static_cast<int>(labels.size()), pixmap, text); ///< 插入到末尾
}

/**
 * @brief 插入图片
 * @param index 插入位置
 * @param pixmap 图片
 * @param text 关联文本，默认为空
 */
void SlideShow::insertImage(const int &index, const QPixmap &pixmap, QString text)
{
    auto label = new SideHideLabel(this); ///< 创建图片标签
    label->setScaledContents(true);      ///< 启用缩放内容
    labels.insert(index, label);         ///< 插入标签
    texts.insert(index, text);           ///< 插入文本
    pixmaps.insert(index, pixmap);       ///< 插入原始图片

    label->setPixmap(getScaledRoundedPixmap(pixmap), oneSize.width() * imgOffside); ///< 设置图片
    label->setMinimumSize(1, 1);        ///< 设置最小尺寸
    label->resize(oneSize);             ///< 调整大小
    CREATE_SHADOW(label);               ///< 添加阴影效果
    label->show();                      ///< 显示标签
    label->installEventFilter(this);    ///< 安装事件过滤器

    auto btn = new InteractiveButtonBase(this); ///< 创建指示器按钮
    btn->setFixedSize(8, 8);                    ///< 设置按钮大小
    btn->setRadius(4);                          ///< 设置圆角半径
    btn->setNormalColor(normalColor);           ///< 设置默认颜色
    btn->setHoverColor(selectColor);            ///< 设置悬停颜色
    indications.insert(index, btn);             ///< 插入指示器
    indicationLayout->insertWidget(index, btn); ///< 添加到布局

    connect(btn, &InteractiveButtonBase::signalMouseEnter, this, [=]{
        const auto idx = static_cast<int>(indications.indexOf(btn));
        setCurrentIndex(idx);                   ///< 鼠标悬停时切换索引
    });

    for (auto indication : indications)
        indication->raise();                    ///< 提升指示器层级

    if (currentIndex > index)
    {
        currentIndex++;
        setCurrentIndex(currentIndex);          ///< 更新当前索引
    }
}

/**
 * @brief 移除图片
 * @param index 移除位置
 */
void SlideShow::removeImage(const int &index)
{
    labels.takeAt(index)->deleteLater();       ///< 删除标签
    pixmaps.removeAt(index);                   ///< 删除图片
    texts.removeAt(index);                     ///< 删除文本
    indications.takeAt(index)->deleteLater();  ///< 删除指示器

    if (labels.empty())
        update();                              ///< 无图片时刷新
    else
    {
        if (currentIndex > index)
            currentIndex--;                    ///< 调整索引
        setCurrentIndex(currentIndex);         ///< 更新索引
    }
}

/**
 * @brief 设置当前图片索引
 * @param index 目标索引
 */
void SlideShow::setCurrentIndex(int index)
{
    const int count = static_cast<int>(labels.size());
    if (index >= count)
        index = count - 1;                     ///< 限制最大索引
    if (index < 0)
        index = 0;                             ///< 限制最小索引
    if (index >= count)
        return;
    if (autoSlideTimer->isActive())
        autoSlideTimer->start();               ///< 重启定时器

    bool leftToRight = currentIndex < index;   ///< 判断滑动方向
    if (currentIndex >= 0 && currentIndex < count)
        indications.at(currentIndex)->setNormalColor(normalColor); ///< 恢复指示器颜色

    SideHideLabel *leavingLabel = nullptr;
    if (currentIndex >= 0 && currentIndex < count)
    {
        leavingLabel = labels.at(currentIndex); ///< 获取即将离开的标签
    }

    currentIndex = index;                      ///< 更新当前索引
    adjustLabels(leavingLabel);                ///< 调整标签布局

    if (leftToRight)
        labels.at((index + 1) % count)->raise(); ///< 左滑时提升右侧标签
    labels.at((index + count - 1) % count)->raise(); ///< 提升左侧标签
    if (!leftToRight)
        labels.at((index + 1) % count)->raise(); ///< 右滑时提升右侧标签
    labels.at(index)->raise();                 ///< 提升中心标签
    if (hidingLabel)
        hidingLabel->raise();                  ///< 提升隐藏标签

    for (auto indication : indications)
    {
        indication->raise();                   ///< 提升指示器
        indication->setNormalColor(normalColor); ///< 恢复颜色
    }
    indications.at(index)->setNormalColor(selectColor); ///< 设置选中颜色
}

/**
 * @brief 获取当前图片索引
 * @return 当前索引
 */
int SlideShow::getCurrentIndex() const
{
    return currentIndex;
}

/**
 * @brief 向左滑动
 */
void SlideShow::slideToLeft()
{
    if (labels.empty())
        return;
    setCurrentIndex(static_cast<int>((currentIndex + labels.size() - 1) % labels.size())); ///< 切换到前一张
}

/**
 * @brief 向右滑动
 */
void SlideShow::slideToRight()
{
    if (labels.empty())
        return;
    setCurrentIndex(static_cast<int>((currentIndex + 1) % labels.size())); ///< 切换到后一张
}

/**
 * @brief 获取缩放和圆角处理的图片
 * @param pixmap 原始图片
 * @return 处理后的图片
 */
QPixmap SlideShow::getScaledRoundedPixmap(QPixmap pixmap) const
{
    const int needWidth = static_cast<int>(oneSize.width() * (1 + imgOffside * 2)); ///< 计算所需宽度
    pixmap = pixmap.scaled(needWidth, oneSize.height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); ///< 缩放图片

    if (true || !scalePixmap)
    {
        if (pixmap.width() > needWidth)
        {
            pixmap = pixmap.copy(pixmap.width() / 2 - needWidth / 2, 0, needWidth, oneSize.height()); ///< 裁剪宽度
        }
        else if (pixmap.height() > oneSize.height())
        {
            pixmap = pixmap.copy(0, pixmap.height() / 2 - oneSize.height() / 2, needWidth, oneSize.height()); ///< 裁剪高度
        }
    }

    return pixmap;

    // 圆角
    /* QPixmap dest(pixmap.size());
    dest.fill(Qt::transparent);
    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QRect rect = QRect(0, 0, pixmap.width(), pixmap.height());
    QPainterPath path;
    path.addRoundedRect(rect, 10, 10);
    painter.setClipPath(path);
    painter.drawPixmap(rect, pixmap);
    return dest; */
}

/**
 * @brief 调整标签布局
 * @param leavingLabel 即将隐藏的标签，默认为 nullptr
 */
void SlideShow::adjustLabels(SideHideLabel *leavingLabel)
{
    if (labels.empty())
        return;

    int sw = width(), sh = height(); ///< 获取控件尺寸
    int w = oneSize.width(), h = oneSize.height(); ///< 获取图片尺寸
    double scale = backScale;        ///< 两侧缩放比例
    int marginTop = sh / 2 - h / 2 - SHADOW_RADIUS / 2; ///< 计算顶部边距

    centerRect = QRect(sw / 2 - w / 2, marginTop, w, h); ///< 中心区域
    leftRect = QRect(static_cast<int>(centerRect.left() - w * scale * sideOffside), ///< 左侧区域
                     marginTop + static_cast<int>(h * (1 - scale) / 2),
                     static_cast<int>(w * scale),
                     static_cast<int>(h * scale));
    rightRect = QRect(static_cast<int>(centerRect.right() + w * scale * sideOffside - w * scale), ///< 右侧区域
                      marginTop + static_cast<int>(h * (1 - scale) / 2),
                      static_cast<int>(w * scale),
                      static_cast<int>(h * scale));
    backRect = QRect(static_cast<int>(sw / 2 - w * scale / 2), ///< 隐藏区域
                     marginTop + static_cast<int>(h * (1 - scale) / 2),
                     static_cast<int>(w * scale),
                     static_cast<int>(h * scale));

    int count = static_cast<int>(labels.size());
    for (int i = currentIndex; i < count + currentIndex; i++)
    {
        QRect rect = backRect;
        if (i == currentIndex)
            rect = centerRect;       ///< 当前图片居中
        else if (i == currentIndex + 1)
            rect = rightRect;        ///< 下一张在右侧
        else if ((i + 1) % count == currentIndex)
            rect = leftRect;         ///< 上一张在左侧
        auto label = labels.at(i % count);
        moveTo(label, rect);         ///< 移动标签

        if (leavingLabel && label == leavingLabel)
        {
            hidingLabel = SideHideLabel::copy(leavingLabel); ///< 复制隐藏标签
            auto opaLabel = hidingLabel;
            moveTo(opaLabel, rect);      ///< 移动隐藏标签
            opaLabel->show();            ///< 显示隐藏标签

            auto effect = new QGraphicsOpacityEffect(opaLabel); ///< 创建透明度效果
            effect->setOpacity(1);       ///< 设置初始透明度
            opaLabel->setGraphicsEffect(effect);

            auto ani = new QPropertyAnimation(effect, "opacity"); ///< 创建透明度动画
            ani->setStartValue(1);       ///< 起始透明度
            ani->setEndValue(0);         ///< 结束透明度
            ani->setEasingCurve(QEasingCurve::OutQuad); ///< 动画曲线
            ani->setDuration(200);       ///< 动画时长
            connect(ani, &QPropertyAnimation::finished, this, [=]{
                if (hidingLabel == opaLabel)
                    hidingLabel = nullptr; ///< 清理隐藏标签
                ani->deleteLater();      ///< 删除动画
                opaLabel->deleteLater(); ///< 删除标签
                effect->deleteLater();   ///< 删除效果
            });
            ani->start();                ///< 启动动画
        }
    }
}

/**
 * @brief 移动标签到指定位置
 * @param label 目标标签
 * @param geometry 目标几何
 */
void SlideShow::moveTo(SideHideLabel *label, QRect geometry) const
{
    if (label->geometry() == geometry)
        return;

    auto ani = new QPropertyAnimation(label, "geometry"); ///< 创建几何动画
    ani->setStartValue(label->geometry());               ///< 起始几何
    ani->setEndValue(geometry);                          ///< 结束几何
    ani->setDuration(300);                               ///< 动画时长
    ani->setEasingCurve(QEasingCurve::OutQuad);          ///< 动画曲线
    connect(ani, &QPropertyAnimation::finished, ani, &QObject::deleteLater); ///< 动画结束时删除
    ani->start();

    ani = new QPropertyAnimation(label, "sideOffset");   ///< 创建偏移动画
    ani->setStartValue(label->getSideOffset());          ///< 起始偏移
    double offset = 0;
    if (geometry.x() == leftRect.x())
        offset = -label->getMaxOffset();                 ///< 左侧偏移
    else if (geometry.x() == rightRect.x())
        offset = label->getMaxOffset();                  ///< 右侧偏移
    ani->setEndValue(offset);                            ///< 结束偏移
    ani->setDuration(500);                               ///< 动画时长
    ani->setEasingCurve(QEasingCurve::OutQuad);          ///< 动画曲线
    connect(ani, &QPropertyAnimation::finished, ani, &QObject::deleteLater); ///< 动画结束时删除
    ani->start();
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件对象
 * @note 重写基类方法
 */
void SlideShow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    const auto w = this->width() / 2 + 20; ///< 计算宽度
    const auto h = static_cast<int>(150 + 60 * (w * 1.0 / 500)); ///< 计算高度
    oneSize = QSize(w, h);                 ///< 更新图片大小
    adjustLabels();                        ///< 调整标签布局
}

/**
 * @brief 事件过滤器
 * @param obj 目标对象
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool SlideShow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        if (labels.contains(static_cast<SideHideLabel*const>(obj)))
        {
            // 图片被单击
            int index = static_cast<int>(labels.indexOf(static_cast<SideHideLabel*const>(obj)));
            if (currentIndex == index) // 正面图片被单击
            {
                emit signalImageClicked(index); ///< 中心图片点击
                emit signalTextActivated(texts.at(index)); ///< 文本激活
            }
            else // 不是当前图片，可能是动画或者两侧的
                setCurrentIndex(index);        ///< 切换到点击的图片
        }
    }

    return QWidget::eventFilter(obj, event);
}