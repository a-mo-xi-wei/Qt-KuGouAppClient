/**
 * @file ElaExitDialog.cpp
 * @brief 实现 ElaExitDialog 类，提供退出对话框的功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaExitDialog.h"
#include "ElaPushButton.h"
#include "ElaMaskWidget.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaExitDialogPrivate.h"
#include "logger.hpp"

#include <QKeyEvent>
#include <QPainter>
#include <QVBoxLayout>

/**
 * @brief 构造函数，初始化退出对话框
 * @param parent 父控件指针
 */
ElaExitDialog::ElaExitDialog(QWidget* parent)
    : QDialog{parent}, d_ptr(new ElaExitDialogPrivate())
{
    Q_D(ElaExitDialog);
    d->q_ptr = this;

    d->_maskWidget = new ElaMaskWidget(parent);
    d->_maskWidget->move(0, 0);
    d->_maskWidget->setFixedSize(parent->size());
    d->_maskWidget->setVisible(false);

    resize(400, height());
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    setWindowModality(Qt::ApplicationModal);
#ifdef Q_OS_WIN
    createWinId();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
    window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint);
#endif
    d->_leftButton = new ElaPushButton("cancel", this);
    connect(d->_leftButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT leftButtonClicked();
        onLeftButtonClicked();
        d->_maskWidget->doMaskAnimation(0);
        d->_doCloseAnimation(false);
    });
    d->_leftButton->setMinimumSize(0, 0);
    d->_leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_leftButton->setFixedHeight(38);
    d->_leftButton->setBorderRadius(6);//JetBrainsMono NF
    auto font = d->_leftButton->font();
    font.setFamily("JetBrainsMono NF");
    d->_leftButton->setFont(font);
    d->_middleButton = new ElaPushButton("minimum", this);
    connect(d->_middleButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT middleButtonClicked();
        onMiddleButtonClicked();
    });
    d->_middleButton->setMinimumSize(0, 0);
    d->_middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_middleButton->setFixedHeight(38);
    d->_middleButton->setBorderRadius(6);
    d->_middleButton->setFont(font);
    d->_rightButton = new ElaPushButton("exit", this);
    connect(d->_rightButton, &ElaPushButton::clicked, this, [=]() {
        Q_EMIT rightButtonClicked();
        onRightButtonClicked();
        d->_doCloseAnimation(true);
    });
    d->_rightButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
    d->_rightButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    d->_rightButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    d->_rightButton->setLightTextColor(Qt::white);
    d->_rightButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    d->_rightButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    d->_rightButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    d->_rightButton->setDarkTextColor(Qt::black);
    d->_rightButton->setMinimumSize(0, 0);
    d->_rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->_rightButton->setFixedHeight(38);
    d->_rightButton->setBorderRadius(6);
    d->_rightButton->setFont(font);

    d->_centralWidget = new QWidget(this);
    QVBoxLayout* centralVLayout = new QVBoxLayout(d->_centralWidget);
    centralVLayout->setContentsMargins(15, 25, 15, 10);
    ElaText* title = new ElaText("退出", this);
    title->setTextStyle(ElaTextType::Title);
    ElaText* subTitle = new ElaText("确定要退出程序吗", this);
    subTitle->setTextStyle(ElaTextType::BodyStrong);
    font = QFont("AaSongLiuKaiTi");//需要通过KuGouClient::initFontRes()打印得知真实字体名
    font.setPixelSize(15);
    subTitle->setFont(font);
    centralVLayout->addWidget(title);
    centralVLayout->addSpacing(2);
    centralVLayout->addWidget(subTitle);
    centralVLayout->addStretch();

    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_buttonWidget = new QWidget(this);
    d->_buttonWidget->setFixedHeight(60);
    QHBoxLayout* buttonLayout = new QHBoxLayout(d->_buttonWidget);
    buttonLayout->addWidget(d->_leftButton);
    buttonLayout->addWidget(d->_middleButton);
    buttonLayout->addWidget(d->_rightButton);
    d->_mainLayout->addWidget(d->_centralWidget);
    d->_mainLayout->addWidget(d->_buttonWidget);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

/**
 * @brief 析构函数，释放退出对话框资源
 */
ElaExitDialog::~ElaExitDialog() = default;

/**
 * @brief 左侧按钮点击槽函数
 */
void ElaExitDialog::onLeftButtonClicked()
{
    STREAM_INFO() << "取消退出程序";
}

/**
 * @brief 中间按钮点击槽函数
 */
void ElaExitDialog::onMiddleButtonClicked()
{
    STREAM_INFO() << "最小化程序";
}

/**
 * @brief 右侧按钮点击槽函数
 */
void ElaExitDialog::onRightButtonClicked()
{
    STREAM_INFO() << "退出程序";
}

/**
 * @brief 设置中心控件
 * @param centralWidget 中心控件指针
 */
void ElaExitDialog::setCentralWidget(QWidget* centralWidget)
{
    Q_D(ElaExitDialog);
    d->_mainLayout->takeAt(0);
    d->_mainLayout->takeAt(0);
    delete d->_centralWidget;
    d->_mainLayout->addWidget(centralWidget);
    d->_mainLayout->addWidget(d->_buttonWidget);
}

/**
 * @brief 设置左侧按钮文本
 * @param text 按钮文本
 */
void ElaExitDialog::setLeftButtonText(QString text)
{
    Q_D(ElaExitDialog);
    d->_leftButton->setText(text);
}

/**
 * @brief 设置中间按钮文本
 * @param text 按钮文本
 */
void ElaExitDialog::setMiddleButtonText(QString text)
{
    Q_D(ElaExitDialog);
    d->_middleButton->setText(text);
}

/**
 * @brief 设置右侧按钮文本
 * @param text 按钮文本
 */
void ElaExitDialog::setRightButtonText(QString text)
{
    Q_D(ElaExitDialog);
    d->_rightButton->setText(text);
}

/**
 * @brief 关闭对话框
 */
void ElaExitDialog::close()
{
    Q_D(ElaExitDialog);
    d->_doCloseAnimation(false);
}

/**
 * @brief 显示事件处理
 * @param event 显示事件
 */
void ElaExitDialog::showEvent(QShowEvent* event)
{
    Q_D(ElaExitDialog);
    d->_maskWidget->setVisible(true);
    d->_maskWidget->raise();
    d->_maskWidget->setFixedSize(parentWidget()->size());
    d->_maskWidget->doMaskAnimation(90);
#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    HWND hwnd = (HWND)d->_currentWinID;
    ElaWinShadowHelper::getInstance()->setWindowShadow(d->_currentWinID);
    DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
    bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
    if (!hasCaption)
    {
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
    }
#endif
#endif
    QDialog::showEvent(event);
}

/**
 * @brief 重绘事件，绘制对话框背景和按钮区域
 * @param event 绘图事件
 */
/*
void ElaExitDialog::paintEvent(QPaintEvent* event)
{
//background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:1, y2:0,stop:0 rgb(105, 225, 255), stop:1 rgba(255, 182, 193, 255));
    Q_D(ElaExitDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
    // 背景绘制
    painter.drawRect(rect());
    // 按钮栏背景绘制
    painter.setBrush(ElaThemeColor(d->_themeMode, DialogLayoutArea));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    painter.restore();
}*/
void ElaExitDialog::paintEvent(QPaintEvent* event)
{
    Q_D(ElaExitDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);

    // 定义渐变的起止颜色
    QColor startColor(105, 225, 255, 200);      // 淡蓝色
    QColor endColor(255, 182, 193, 200);        // 淡粉色

    // 上半部分背景 - 对角线渐变（左上到右下）
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, startColor);
    gradient.setColorAt(1, endColor);
    painter.setBrush(gradient);
    painter.drawRect(rect());

    // 计算渐变的中间色（50%位置的颜色）
    QColor midColor(
        (startColor.red() + endColor.red()) / 2,
        (startColor.green() + endColor.green()) / 2,
        (startColor.blue() + endColor.blue()) / 2,
        (startColor.alpha() + endColor.alpha()) / 2
    );

    // 按钮栏背景 - 使用渐变中间色
    painter.setBrush(midColor);
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);

    painter.restore();
}

/**
 * @brief 键盘按下事件处理
 * @param event 键盘事件
 */
void ElaExitDialog::keyPressEvent(QKeyEvent* event)
{
    event->accept();
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
/**
 * @brief 本地事件处理（Windows 平台）
 * @param eventType 事件类型
 * @param message 事件消息
 * @param result 事件结果
 * @return 是否处理事件
 */
bool ElaExitDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
/**
 * @brief 本地事件处理（Windows 平台，Qt5）
 * @param eventType 事件类型
 * @param message 事件消息
 * @param result 事件结果
 * @return 是否处理事件
 */
bool ElaExitDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_D(ElaExitDialog);
    if ((eventType != "windows_generic_MSG") || !message)
    {
        return false;
    }
    const auto msg = static_cast<const MSG*>(message);
    const HWND hwnd = msg->hwnd;
    if (!hwnd || !msg)
    {
        return false;
    }
    d->_currentWinID = (qint64)hwnd;
    const UINT uMsg = msg->message;
    const WPARAM wParam = msg->wParam;
    const LPARAM lParam = msg->lParam;
    switch (uMsg)
    {
    case WM_WINDOWPOSCHANGING:
    {
        WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
        if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
        {
            wp->flags |= SWP_NOCOPYBITS;
            *result = ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
            return true;
        }
        return false;
    }
    case WM_NCACTIVATE:
    {
        *result = TRUE;
        return true;
    }
    case WM_NCCALCSIZE:
    {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
        if (wParam == FALSE)
        {
            return false;
        }
        if (::IsZoomed(hwnd))
        {
            setContentsMargins(8, 8, 8, 8);
        }
        else
        {
            setContentsMargins(0, 0, 0, 0);
        }
        *result = 0;
        return true;
#else
        if (wParam == FALSE)
        {
            return false;
        }
        RECT* clientRect = &((NCCALCSIZE_PARAMS*)(lParam))->rgrc[0];
        if (!::IsZoomed(hwnd))
        {
            clientRect->top -= 1;
            clientRect->bottom -= 1;
        }
        *result = WVR_REDRAW;
        return true;
#endif
    }
    }
    return QDialog::nativeEvent(eventType, message, result);
}
#endif