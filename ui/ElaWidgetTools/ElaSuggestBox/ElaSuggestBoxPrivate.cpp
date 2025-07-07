/**
 * @file ElaSuggestBoxPrivate.cpp
 * @brief 实现 ElaSuggestBoxPrivate 类，搜索建议框的私有实现
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#include "ElaSuggestBoxPrivate.h"
#include "ElaBaseListView.h"
#include "ElaLineEdit.h"
#include "ElaSuggestBox.h"
#include "ElaSuggestBoxSearchViewContainer.h"
#include "ElaSuggestModel.h"

#include <QLayout>
#include <QPropertyAnimation>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include "Async.h"

class QNetworkReply;

/**
 * @brief 构造函数
 * @param parent 父对象指针，默认为 nullptr
 */
ElaSuggestion::ElaSuggestion(QObject *parent)
    : QObject(parent)
{
    _pElaIcon = ElaIconType::None;                            ///< 初始化图标
    _pSuggestText = "";                                       ///< 初始化文本
    _pSuggestKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-"); ///< 生成唯一键
    _pSuggestData = QVariantMap();                            ///< 初始化数据
}

/**
 * @brief 构造函数
 * @param parent 父对象指针，默认为 nullptr
 */
ElaSuggestBoxPrivate::ElaSuggestBoxPrivate(QObject *parent)
    : QObject{parent}
{
    _animationTimer = new QTimer(this);                       ///< 创建动画定时器
    _animationTimer->setSingleShot(true);                     ///< 设置单次触发
    connect(_animationTimer, &QTimer::timeout, this, &ElaSuggestBoxPrivate::_doStartSizeAnimation); ///< 连接超时信号
}

/**
 * @brief 析构函数
 */
ElaSuggestBoxPrivate::~ElaSuggestBoxPrivate() = default;

/**
 * @brief 主题模式变化槽函数
 * @param themeMode 主题模式
 * @note 更新搜索图标
 */
void ElaSuggestBoxPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;                                   ///< 更新主题模式
    _searchEdit->removeAction(_themeMode == ElaThemeType::Light ? _darkSearchAction : _lightSearchAction); ///< 移除旧图标
    _searchEdit->addAction(_themeMode == ElaThemeType::Light ? _lightSearchAction : _darkSearchAction, QLineEdit::TrailingPosition); ///< 添加新图标
    _searchEdit->update();                                    ///< 刷新编辑框
}

/**
 * @brief 搜索编辑框文本编辑槽函数
 * @param searchText 搜索文本
 * @note 根据文本获取建议项并显示，包括网络请求（QQ音乐和网易云音乐API）
 */
void ElaSuggestBoxPrivate::onSearchEditTextEdit(const QString &searchText)
{
    Q_Q(ElaSuggestBox);
    if (searchText.isEmpty() || m_ignoreTextChanges)
    {
        _startCloseAnimation();                               ///< 关闭建议框
        return;
    }
    if (sender()->property("searchWay").toString() == "search_net_song")
    {
        q->removeAllSuggestion(); // 清除现有建议项

        // Send asynchronous POST request to the local server
        auto future = Async::runAsync(QThreadPool::globalInstance(), [this, searchText] {
            return m_libHttp.UrlRequestGet(
                QString("http://127.0.0.1:8080/api/searchSuggestion"),
                "key=" + QUrl::toPercentEncoding(searchText),
                ///< 注意：此处不能直接传递纯文本 searchText（如 "123"）而非完整的查询字符串 "key=123"。
                ///< 否则：浏览器访问：http://127.0.0.1:8080/api/searchSuggestion?key=123（正确格式）
                ///< 代码生成：http://127.0.0.1:8080/api/searchSuggestion?123（缺少参数名）
                10000 // Timeout in milliseconds
            );
        });

        // Handle the response asynchronously
        Async::onResultReady(future, this, [this, q](const QString &responseData) {
            QJsonParseError err;
            const QJsonDocument doc = QJsonDocument::fromJson(responseData.toUtf8(), &err);
            if (err.error != QJsonParseError::NoError || !doc.isObject())
            {
                qWarning() << "Failed to parse response from server:" << err.errorString();
                return;
            }

            const QJsonObject obj = doc.object();
            if (obj.value("status").toString() == "success")
            {
                const QJsonArray suggestions = obj.value("data").toArray();
                for (const auto &suggestion : suggestions)
                {
                    q->addSuggestion(suggestion.toString());
                    ///< @note qDebug()<<"添加："<< suggestion.toString();
                }

                if (m_ignoreTextChanges) return;
                if (!_suggestionVector.isEmpty())
                {
                    _searchModel->setSearchSuggestion(_suggestionVector); // 设置模型建议项
                    int rowCount = static_cast<int>(_suggestionVector.count());
                    rowCount = rowCount > 6 ? 6 : rowCount;
                    if (!_searchViewBaseWidget->isVisible())
                    {
                        q->raise();
                        _searchViewBaseWidget->show();
                        _searchViewBaseWidget->raise();

                        // 修复位置计算 - 正确计算全局位置
                        const QPoint globalPos = q->mapToGlobal(QPoint(-5, q->height()));
                        _searchViewBaseWidget->move(globalPos - _searchViewBaseWidget->parentWidget()->mapToGlobal(QPoint(0, 0)));

                        _startSizeAnimation(QSize(q->width() + 10, 0), QSize(q->width() + 10, 40 * rowCount + 16));
                        _searchView->move(_searchView->x(), -(40 * rowCount + 16));
                    }
                    else
                    {
                        _startSizeAnimation(_searchViewBaseWidget->size(), QSize(q->width() + 10, 40 * rowCount + 16));
                    }
                    _startExpandAnimation(); // 启动展开动画
                }
                else
                {
                    _startCloseAnimation(); // 启动关闭动画
                }
            }
            else
            {
                qWarning() << "Server returned error:" << obj.value("message").toString();
            }
        });
    }
    else
    {
        QVector<QSharedPointer<ElaSuggestion>> suggestionVector;
        for (const auto &suggest : _suggestionVector)
        {
            if (suggest->getSuggestText().contains(searchText, _pCaseSensitivity))
            {
                suggestionVector.append(suggest);             ///< 筛选匹配建议项
            }
        }
        if (m_ignoreTextChanges) return;
        if (!suggestionVector.isEmpty())
        {
            _searchModel->setSearchSuggestion(suggestionVector); ///< 设置模型建议项
            int rowCount = static_cast<int>(suggestionVector.count());
            rowCount = rowCount > 6 ? 6 : rowCount;
            if (!_searchViewBaseWidget->isVisible())
            {
                q->raise();
                _searchViewBaseWidget->show();
                _searchViewBaseWidget->raise();

                // @note 修复位置计算 - 正确计算全局位置
                const QPoint globalPos = q->mapToGlobal(QPoint(-5, q->height()));
                _searchViewBaseWidget->move(globalPos - _searchViewBaseWidget->parentWidget()->mapToGlobal(QPoint(0, 0)));

                _startSizeAnimation(QSize(q->width() + 10, 0), QSize(q->width() + 10, 40 * rowCount + 16));
                _searchView->move(_searchView->x(), -(40 * rowCount + 16));
            }
            else
            {
                _startSizeAnimation(_searchViewBaseWidget->size(), QSize(q->width() + 10, 40 * rowCount + 16));
            }
            _startExpandAnimation();                          ///< 启动展开动画
        }
        else
        {
            _startCloseAnimation();                           ///< 启动关闭动画
        }
    }
}

/**
 * @brief 搜索编辑框宽度变化槽函数
 * @note 更新建议框位置和尺寸
 */
void ElaSuggestBoxPrivate::onSearchEditWidthChanged()
{
    Q_Q(ElaSuggestBox);
    int rowCount = 0;
    if (sender()->property("searchWay").toString() == "search_net_song")
    {
        rowCount = static_cast<int>(_suggestionVector.count());
    }
    else
    {
        rowCount = _searchModel->rowCount(QModelIndex());
    }
    rowCount = rowCount > 6 ? 6 : rowCount;
    if (_searchViewBaseWidget->isVisible())
    {
        // @note 修复位置计算 - 正确计算全局位置
        QPoint globalPos = q->mapToGlobal(QPoint(-5, q->height()));
        _searchViewBaseWidget->move(globalPos - _searchViewBaseWidget->parentWidget()->mapToGlobal(QPoint(0, 0)));

        _startSizeAnimation(_searchViewBaseWidget->size(), QSize(q->width() + 10, 40 * rowCount + 16));
        _searchView->move(_searchView->x(), -(40 * rowCount + 16));
    }
}

/**
 * @brief 搜索视图点击槽函数
 * @param index 模型索引
 * @note 处理建议项点击
 */
void ElaSuggestBoxPrivate::onSearchViewClicked(const QModelIndex &index)
{
    Q_Q(ElaSuggestBox);
    _searchView->clearSelection();                            ///< 清除选择
    if (!index.isValid()) {
        return;
    }
    // 立即禁用视图交互
    _searchView->setEnabled(false);

    ElaSuggestion *suggest = _searchModel->getSearchSuggestion(index.row());
    if (!suggest) {
        qDebug()<<"无效的索引 "<<suggest;
        return;
    }
    _searchEdit->setText(suggest->getSuggestText());          ///< 设置编辑框文本
    _searchEdit->clearFocus();
    Q_EMIT q->suggestionClicked(suggest->getSuggestText(), suggest->getSuggestData()); ///< 发射点击信号
    _startCloseAnimation();                                   ///< 关闭建议框
}

/**
 * @brief 启动尺寸动画
 * @param oldSize 旧尺寸
 * @param newSize 新尺寸
 * @note 使用定时器延迟启动动画
 */
void ElaSuggestBoxPrivate::_startSizeAnimation(const QSize oldSize, const QSize newSize)
{
    _pendingSize = newSize;                                   ///< 更新待处理尺寸
    if (!_animationTimer->isActive())
    {
        _animationTimer->start(30);                           ///< 启动定时器，延迟 30ms
    }
}

/**
 * @brief 启动展开动画
 */
void ElaSuggestBoxPrivate::_startExpandAnimation()
{
    if (!_isExpandAnimationFinished)
    {
        return;
    }
    _isCloseAnimationFinished = true;
    _isExpandAnimationFinished = false;
    QPropertyAnimation *expandAnimation = new QPropertyAnimation(_searchView, "pos");
    connect(expandAnimation, &QPropertyAnimation::finished, this, [=] {
        _isExpandAnimationFinished = true;
        _searchView->clearSelection();
    });
    expandAnimation->setDuration(300);
    expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
    expandAnimation->setStartValue(_searchView->pos());
    expandAnimation->setEndValue(QPoint(8, 8));
    expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @brief 启动关闭动画
 */
void ElaSuggestBoxPrivate::_startCloseAnimation()
{
    // qDebug() << "_isCloseAnimationFinished:" << _isCloseAnimationFinished;
    if (!_isCloseAnimationFinished){
        return;
    }
    // 确保视图已禁用（双重保障）
    _searchView->setEnabled(false);

    _isExpandAnimationFinished = true;
    _isCloseAnimationFinished = false;
    QPropertyAnimation *baseWidgetsAnimation = new QPropertyAnimation(_searchViewBaseWidget, "size");
    baseWidgetsAnimation->setDuration(300);
    baseWidgetsAnimation->setEasingCurve(QEasingCurve::InOutSine);
    baseWidgetsAnimation->setStartValue(_searchViewBaseWidget->size());
    baseWidgetsAnimation->setEndValue(QSize(_searchViewBaseWidget->width(), 0));
    baseWidgetsAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation *closeAnimation = new QPropertyAnimation(_searchView, "pos");
    connect(closeAnimation, &QPropertyAnimation::finished, this, [=] {
        _isCloseAnimationFinished = true;
        _searchModel->clearSearchNode();
        _searchViewBaseWidget->hide();
        // 重新启用视图（为下次显示准备）
        _searchView->setEnabled(true);
        // qDebug() << "===========隐藏OK";
    });
    closeAnimation->setDuration(300);
    closeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closeAnimation->setStartValue(_searchView->pos());
    closeAnimation->setEndValue(QPoint(_searchView->pos().x(), -_searchView->height()));
    closeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _lastSize = baseWidgetsAnimation->endValue().toSize();
}

/**
 * @brief 执行尺寸动画
 * @note 实际启动尺寸变化动画
 */
void ElaSuggestBoxPrivate::_doStartSizeAnimation()
{
    if (_pendingSize.isValid())
    {
        Q_Q(ElaSuggestBox);
        if (_lastSize.isValid() && _lastSize == _pendingSize)
        {
            return; // 如果目标尺寸未变，则无需动画
        }
        _shadowLayout->removeWidget(_searchView);
        auto expandAnimation = new QPropertyAnimation(_searchViewBaseWidget, "size");
        connect(expandAnimation, &QPropertyAnimation::valueChanged, this, [=] {
            _searchView->resize(_searchViewBaseWidget->size());
            QPoint globalPos = q->mapToGlobal(QPoint(-5, q->height()));
            _searchViewBaseWidget->move(globalPos - _searchViewBaseWidget->parentWidget()->mapToGlobal(QPoint(0, 0)));
        });
        connect(expandAnimation, &QPropertyAnimation::finished, this, [=] {
            _shadowLayout->addWidget(_searchView);
        });
        expandAnimation->setDuration(300);
        expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
        expandAnimation->setStartValue(_searchViewBaseWidget->size()); // 从当前尺寸开始
        expandAnimation->setEndValue(_pendingSize); // 使用最新的目标尺寸
        expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _lastSize = _pendingSize;
        _pendingSize = QSize(); // 清空待处理尺寸
    }
}