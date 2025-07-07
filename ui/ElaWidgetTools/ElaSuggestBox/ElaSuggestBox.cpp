/**
 * @file ElaSuggestBox.cpp
 * @brief 实现 ElaSuggestBox 类，搜索建议框控件
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#include "ElaSuggestBox.h"
#include "ElaBaseListView.h"
#include "ElaIcon.h"
#include "ElaLineEdit.h"
#include "ElaScrollBar.h"
#include "ElaSuggestBoxSearchViewContainer.h"
#include "ElaSuggestDelegate.h"
#include "ElaSuggestModel.h"
#include "ElaTheme.h"
#include "ElaSuggestBoxPrivate.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QToolButton>
#include <QTimer>

Q_PROPERTY_CREATE_Q_CPP(ElaSuggestBox, int, BorderRadius)    ///< 实现 BorderRadius 属性
Q_PROPERTY_CREATE_Q_CPP(ElaSuggestBox, Qt::CaseSensitivity, CaseSensitivity) ///< 实现 CaseSensitivity 属性

/**
 * @brief 构造函数
 * @param parent 父控件指针，默认为 nullptr
 */
ElaSuggestBox::ElaSuggestBox(QWidget *parent)
    : QWidget{parent}, d_ptr(new ElaSuggestBoxPrivate())
{
    Q_D(ElaSuggestBox);
    setFixedSize(280, 35);                                    ///< 设置固定尺寸
    setAttribute(Qt::WA_StyledBackground, true);              ///< 启用样式背景
    d->q_ptr = this;
    d->_pBorderRadius = 8;                                    ///< 初始化圆角半径
    d->_pCaseSensitivity = Qt::CaseInsensitive;               ///< 初始化大小写敏感性
    d->_searchEdit = new ElaLineEdit(this);                   ///< 创建搜索编辑框
    d->_searchEdit->setFixedHeight(35);                       ///< 设置编辑框高度
    d->_searchEdit->setPlaceholderText("查找功能");            ///< 设置占位文本
    d->_searchEdit->setClearButtonEnabled(true);              ///< 启用清除按钮
    d->_searchEdit->setIsClearButtonEnable(true);             ///< 启用清除按钮
    d->_lightSearchAction = new QAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::MagnifyingGlass), "Search", this); ///< 亮色搜索图标
    d->_darkSearchAction = new QAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::MagnifyingGlass, QColor(0xFF, 0xFF, 0xFF)), "Search", this); ///< 暗色搜索图标

    d->_themeMode = eTheme->getThemeMode();                   ///< 获取当前主题模式
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaSuggestBoxPrivate::onThemeModeChanged); ///< 连接主题变化信号
    // 获取关联的QToolButton
    QToolButton* searchButton = nullptr;
    if (d->_themeMode == ElaThemeType::Light)
    {
        d->_searchEdit->addAction(d->_lightSearchAction, QLineEdit::TrailingPosition); ///< 添加亮色图标
        foreach (QToolButton* btn, d->_searchEdit->findChildren<QToolButton*>()) {
            if (btn->defaultAction() == d->_lightSearchAction) {
                searchButton = btn;
                break;
            }
        }
    }
    else
    {
        d->_searchEdit->addAction(d->_darkSearchAction, QLineEdit::TrailingPosition); ///< 添加暗色图标
        foreach (QToolButton* btn, d->_searchEdit->findChildren<QToolButton*>()) {
            if (btn->defaultAction() == d->_darkSearchAction) {
                searchButton = btn;
                break;
            }
        }
    }
    if (searchButton) {
        searchButton->setCursor(Qt::PointingHandCursor);
    }

    connect(d->_lightSearchAction, &QAction::triggered, this, [=](bool checked) {
        // @note 调试用
        // qDebug() << "Search";
    });                                                       ///< 连接亮色搜索动作

    connect(d->_darkSearchAction, &QAction::triggered, this, [=](bool checked) {
        // @note 调试用
        // qDebug() << "Search";
    });                                                       ///< 连接暗色搜索动作

    auto mainLayout = new QVBoxLayout(this);                  ///< 创建主布局
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(1, 0, 1, 0);
    mainLayout->addWidget(d->_searchEdit);                    ///< 添加编辑框
    d->_searchViewBaseWidget = new ElaSuggestBoxSearchViewContainer(window()); ///< 创建建议框容器
    d->_shadowLayout = new QVBoxLayout(d->_searchViewBaseWidget); ///< 创建阴影布局
    d->_shadowLayout->setContentsMargins(0, 0, 0, 0);
    d->_searchView = new ElaBaseListView(d->_searchViewBaseWidget); ///< 创建建议项视图
    auto floatVScrollBar = new ElaScrollBar(d->_searchView->verticalScrollBar(), d->_searchView); ///< 创建浮动滚动条
    floatVScrollBar->setIsAnimation(true);                    ///< 启用滚动条动画
    d->_searchViewBaseWidget->resize(270, 300);               ///< 设置建议框尺寸
    d->_shadowLayout->addWidget(d->_searchView);              ///< 添加视图到布局
    d->_searchModel = new ElaSuggestModel(this);              ///< 创建建议项模型
    d->_searchDelegate = new ElaSuggestDelegate(this);        ///< 创建建议项委托
    d->_searchView->setModel(d->_searchModel);                ///< 设置视图模型
    d->_searchView->setItemDelegate(d->_searchDelegate);      ///< 设置视图委托
    d->_searchViewBaseWidget->hide();                         ///< 初始隐藏建议框
    connect(d->_searchView, &ElaBaseListView::clicked, d, &ElaSuggestBoxPrivate::onSearchViewClicked); ///< 连接点击信号
    connect(d->_searchEdit, &ElaLineEdit::textChanged, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit); ///< 连接文本变化信号
    //connect(d->_searchEdit, &ElaLineEdit::focusIn, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);

    // @note 焦点事件
    connect(d->_searchEdit, &ElaLineEdit::wmFocusOut, this, [d] {
        d->_startCloseAnimation();                            ///< 失去焦点时关闭建议框
    });
    QApplication::instance()->installEventFilter(this);       ///< 安装应用程序级事件过滤器
}

/**
 * @brief 析构函数
 */
ElaSuggestBox::~ElaSuggestBox() = default;

/**
 * @brief 设置占位文本
 * @param placeholderText 占位文本
 */
void ElaSuggestBox::setPlaceholderText(const QString &placeholderText)
{
    Q_D(ElaSuggestBox);
    d->_searchEdit->setPlaceholderText(placeholderText);      ///< 设置编辑框占位文本
}

/**
 * @brief 添加建议项
 * @param suggestText 建议文本
 * @param suggestData 建议数据
 * @return 建议项唯一键
 */
QString ElaSuggestBox::addSuggestion(const QString &suggestText, const QVariantMap &suggestData)
{
    Q_D(ElaSuggestBox);
    auto suggest = QSharedPointer<ElaSuggestion>::create(this);     // 创建智能指针
    suggest->setSuggestText(suggestText);                       ///< 设置文本
    suggest->setSuggestData(suggestData);                       ///< 设置数据
    d->_suggestionVector.append(suggest);                           ///< 添加到列表
    return suggest->getSuggestKey();                                ///< 返回唯一键
}

/**
 * @brief 添加带图标的建议项
 * @param icon 图标
 * @param suggestText 建议文本
 * @param suggestData 建议数据
 * @return 建议项唯一键
 */
QString ElaSuggestBox::addSuggestion(ElaIconType::IconName icon, const QString &suggestText, const QVariantMap &suggestData)
{
    Q_D(ElaSuggestBox);
    auto suggest = QSharedPointer<ElaSuggestion>::create(this); // 创建智能指针
    suggest->setElaIcon(icon);                                ///< 设置图标
    suggest->setSuggestText(suggestText);                     ///< 设置文本
    suggest->setSuggestData(suggestData);                     ///< 设置数据
    d->_suggestionVector.append(suggest);                     ///< 添加到列表
    return suggest->getSuggestKey();                          ///< 返回唯一键
}

/**
 * @brief 移除建议项
 * @param suggestKey 建议项唯一键
 */
void ElaSuggestBox::removeSuggestion(const QString &suggestKey)
{
    Q_D(ElaSuggestBox);
    for (auto suggest : d->_suggestionVector)
    {
        if (suggest->getSuggestKey() == suggestKey)
        {
            d->_suggestionVector.removeOne(suggest);          ///< 移除建议项
            suggest->deleteLater();                           ///< 延迟删除
        }
    }
}

/**
 * @brief 移除指定索引的建议项
 * @param index 索引
 */
void ElaSuggestBox::removeSuggestion(int index)
{
    Q_D(ElaSuggestBox);
    if (index >= d->_suggestionVector.count())
    {
        return;
    }
    ElaSuggestion *suggest = d->_suggestionVector[index].get();
    d->_suggestionVector.removeOne(suggest);                  ///< 移除建议项
    suggest->deleteLater();                                   ///< 延迟删除
}

/**
 * @brief 移除所有建议项
 */
void ElaSuggestBox::removeAllSuggestion()
{
    Q_D(ElaSuggestBox);
    for (auto suggest : d->_suggestionVector)
    {
        d->_suggestionVector.removeOne(suggest);              ///< 移除建议项
        suggest->deleteLater();                               ///< 延迟删除
    }
}

/**
 * @brief 设置搜索编辑框
 * @param lineEdit 编辑框指针
 * @note 替换默认编辑框，包括移除旧控件、设置新控件、重新连接信号槽
 */
void ElaSuggestBox::setLineEdit(ElaLineEdit *lineEdit)
{
    Q_D(ElaSuggestBox);

    if (!lineEdit)
    {
        qWarning() << "setLineEdit: 传入了 nullptr，操作无效"; ///< 警告
        return;
    }

    // @note 移除旧编辑框
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout)
    {
        mainLayout->removeWidget(d->_searchEdit);             ///< 移除旧编辑框
    }
    d->_searchEdit->hide();                                   ///< 隐藏旧编辑框
    d->_searchEdit->disconnect();                             ///< 断开信号槽
    d->_searchEdit->deleteLater();                            ///< 延迟删除

    // @note 设置新编辑框
    if (lineEdit->parent() && lineEdit->parent() != this)
    {
        lineEdit->setParent(nullptr);                         ///< 脱离旧父对象
    }
    lineEdit->setParent(this);                                ///< 设置新父对象
    d->_searchEdit = lineEdit;                                ///< 更新私有成员
    d->_searchEdit->setFixedHeight(35);                       ///< 设置高度
    d->_searchEdit->setPlaceholderText("查找功能");            ///< 设置占位文本
    d->_searchEdit->setClearButtonEnabled(true);              ///< 启用清除按钮
    d->_searchEdit->setIsClearButtonEnable(true);             ///< 启用清除按钮

    // 获取关联的QToolButton
    QToolButton* searchButton = nullptr;
    // @note 添加图标动作
    if (d->_themeMode == ElaThemeType::Light)
    {
        d->_searchEdit->addAction(d->_lightSearchAction, QLineEdit::TrailingPosition);///< 添加亮色图标
        connect(d->_lightSearchAction, &QAction::triggered, d, [d, this] {
            d->_searchEdit->clearFocus();
            d->_startCloseAnimation();
            emit searchTextReturnPressed(d->_searchEdit->text());
        });
        foreach (QToolButton* btn, d->_searchEdit->findChildren<QToolButton*>()) {
            if (btn->defaultAction() == d->_lightSearchAction) {
                searchButton = btn;
                break;
            }
        }
    }
    else
    {
        d->_searchEdit->addAction(d->_darkSearchAction, QLineEdit::TrailingPosition);///< 添加暗色图标
        connect(d->_darkSearchAction, &QAction::triggered, d, [d, this] {
            d->_searchEdit->clearFocus();
            d->_startCloseAnimation();
            emit searchTextReturnPressed(d->_searchEdit->text());
        });
        foreach (QToolButton* btn, d->_searchEdit->findChildren<QToolButton*>()) {
            if (btn->defaultAction() == d->_darkSearchAction) {
                searchButton = btn;
                break;
            }
        }
    }
    if (searchButton) {
        searchButton->setCursor(Qt::PointingHandCursor);
    }

    // @note 重新连接信号槽
    connect(d->_searchEdit, &ElaLineEdit::textChanged, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchEdit, &ElaLineEdit::wmFocusOut, this, [d] {
        // @note 收缩宽度至搜索框宽度，高度保持不变
        QSize targetSize(d->_searchEdit->getOriginalWidth() + 10, d->_searchViewBaseWidget->height());
        d->_startSizeAnimation(d->_searchViewBaseWidget->size(), targetSize);

        // @note 延迟执行关闭动画
        QTimer::singleShot(300, d, [d] {
            d->_startCloseAnimation();
        });
    });
    connect(d->_searchEdit, &ElaLineEdit::widthChanged, d, &ElaSuggestBoxPrivate::onSearchEditWidthChanged);
    connect(d->_searchEdit,&ElaLineEdit::returnPressed, d, [d, this] {    // 设置忽略标志
        d->m_ignoreTextChanges = true;
        d->_searchEdit->clearFocus();
        d->_startCloseAnimation();
        emit searchTextReturnPressed(d->_searchEdit->text());
        QTimer::singleShot(1000, d, [d] {
            d->m_ignoreTextChanges = false;
        });
    });
    // @note 添加到布局
    if (mainLayout)
    {
        mainLayout->insertWidget(0, d->_searchEdit);
    }
    d->_searchEdit->show();                                   ///< 显示新编辑框
}

/**
 * @brief 移除默认尾部动作
 */
void ElaSuggestBox::removeDefaultTrailAction()
{
    Q_D(ElaSuggestBox);
    if (d->_themeMode == ElaThemeType::Light)
    {
        d->_searchEdit->removeAction(d->_lightSearchAction);  ///< 移除亮色动作
    }
    else
    {
        d->_searchEdit->removeAction(d->_darkSearchAction);   ///< 移除暗色动作
    }
}

/**
 * @brief 建议框位置变化
 * @note 取消编辑框焦点
 */
void ElaSuggestBox::suggestBoxPositionChanged()
{
    Q_D(ElaSuggestBox);
    d->_searchEdit->clearFocus();                             ///< 取消焦点
}

/**
 * @brief 事件过滤
 * @param watched 监控对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理点击外部关闭建议框，特殊处理弹出菜单
 */
bool ElaSuggestBox::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        const auto ev = static_cast<QMouseEvent *>(event);

        QWidget *activePopup = QApplication::activePopupWidget();
        if (activePopup && (activePopup->inherits("QMenu") || activePopup->inherits("QWidget")))
        {
            return QWidget::eventFilter(watched, event);      ///< 弹出菜单时不处理
        }

        if (!rect().contains(mapFromGlobal(ev->globalPosition().toPoint())))
        {
            this->clearFocus();                               ///< 点击外部清除焦点
        }
    }
    return QWidget::eventFilter(watched, event);              ///< 调用父类处理
}