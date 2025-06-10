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
#include <QTimer>

#include "ElaLineEditPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaSuggestBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaSuggestBox, Qt::CaseSensitivity, CaseSensitivity)

ElaSuggestBox::ElaSuggestBox(QWidget *parent)
    : QWidget{parent}, d_ptr(new ElaSuggestBoxPrivate()) {
    Q_D(ElaSuggestBox);
    setFixedSize(280, 35);
    setAttribute(Qt::WA_StyledBackground, true); // 阻止继承样式
    d->q_ptr = this;
    d->_pBorderRadius = 8;
    d->_pCaseSensitivity = Qt::CaseInsensitive;
    d->_searchEdit = new ElaLineEdit(this);
    d->_searchEdit->setFixedHeight(35);
    d->_searchEdit->setPlaceholderText("查找功能");
    d->_searchEdit->setClearButtonEnabled(true);
    d->_searchEdit->setIsClearButtonEnable(true);
    d->_lightSearchAction = new QAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::MagnifyingGlass), "Search",
                                        this);
    d->_darkSearchAction = new QAction(
        ElaIcon::getInstance()->getElaIcon(ElaIconType::MagnifyingGlass, QColor(0xFF, 0xFF, 0xFF)), "Search", this);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaSuggestBoxPrivate::onThemeModeChanged);
    if (d->_themeMode == ElaThemeType::Light) {
        d->_searchEdit->addAction(d->_lightSearchAction, QLineEdit::TrailingPosition);
    } else {
        d->_searchEdit->addAction(d->_darkSearchAction, QLineEdit::TrailingPosition);
    }

    connect(d->_lightSearchAction, &QAction::triggered, this, [=](bool checked) {
        //qDebug() << "Search";
    });

    connect(d->_darkSearchAction, &QAction::triggered, this, [=](bool checked) {
        //qDebug() << "Search";
    });
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->_searchEdit);
    d->_searchViewBaseWidget = new ElaSuggestBoxSearchViewContainer(window());
    d->_shadowLayout = new QVBoxLayout(d->_searchViewBaseWidget);
    d->_shadowLayout->setContentsMargins(0, 0, 0, 0);
    d->_searchView = new ElaBaseListView(d->_searchViewBaseWidget);
    auto floatVScrollBar = new ElaScrollBar(d->_searchView->verticalScrollBar(), d->_searchView);
    floatVScrollBar->setIsAnimation(true);
    d->_searchViewBaseWidget->resize(270, 300);
    d->_shadowLayout->addWidget(d->_searchView);
    d->_searchModel = new ElaSuggestModel(this);
    d->_searchDelegate = new ElaSuggestDelegate(this);
    d->_searchView->setModel(d->_searchModel);
    d->_searchView->setItemDelegate(d->_searchDelegate);
    d->_searchViewBaseWidget->hide();
    connect(d->_searchEdit, &ElaLineEdit::textChanged, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);
    //connect(d->_searchEdit, &ElaLineEdit::focusIn, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);

    connect(d->_searchView, &ElaBaseListView::clicked, d, &ElaSuggestBoxPrivate::onSearchViewClicked);

    // 焦点事件
    connect(d->_searchEdit, &ElaLineEdit::wmFocusOut, this, [d] {
        d->_startCloseAnimation();
    });

    QApplication::instance()->installEventFilter(this); ///< 安装应用程序级事件过滤器

}

ElaSuggestBox::~ElaSuggestBox() = default;

void ElaSuggestBox::setPlaceholderText(const QString &placeholderText) {
    Q_D(ElaSuggestBox);
    d->_searchEdit->setPlaceholderText(placeholderText);
}

QString ElaSuggestBox::addSuggestion(const QString &suggestText, const QVariantMap &suggestData) {
    Q_D(ElaSuggestBox);
    auto suggest = new ElaSuggestion(this);
    suggest->setSuggestText(suggestText);
    suggest->setSuggestData(suggestData);
    d->_suggestionVector.append(suggest);
    return suggest->getSuggestKey();
}

QString ElaSuggestBox::addSuggestion(ElaIconType::IconName icon, const QString &suggestText,
                                     const QVariantMap &suggestData) {
    Q_D(ElaSuggestBox);
    auto suggest = new ElaSuggestion(this);
    suggest->setElaIcon(icon);
    suggest->setSuggestText(suggestText);
    suggest->setSuggestData(suggestData);
    d->_suggestionVector.append(suggest);
    return suggest->getSuggestKey();
}

void ElaSuggestBox::removeSuggestion(const QString &suggestKey) {
    Q_D(ElaSuggestBox);
    foreach(auto suggest, d->_suggestionVector) {
        if (suggest->getSuggestKey() == suggestKey) {
            d->_suggestionVector.removeOne(suggest);
            suggest->deleteLater();
        }
    }
}

void ElaSuggestBox::removeSuggestion(int index) {
    Q_D(ElaSuggestBox);
    if (index >= d->_suggestionVector.count()) {
        return;
    }
    ElaSuggestion *suggest = d->_suggestionVector[index];
    d->_suggestionVector.removeOne(suggest);
    suggest->deleteLater();
}

void ElaSuggestBox::removeAllSuggestion() {
    Q_D(ElaSuggestBox);
    foreach(auto suggest, d->_suggestionVector) {
        d->_suggestionVector.removeOne(suggest);
        suggest->deleteLater();
    }
}

void ElaSuggestBox::setLineEdit(ElaLineEdit *lineEdit) {    //该函数仅供MySearchLineEdit使用
    Q_D(ElaSuggestBox);

    if (!lineEdit) {
        qWarning() << "setLineEdit: 传入了 nullptr，操作无效";
        return;
    }

    // 1. 把旧的 _searchEdit 从布局里移除，并安排删除
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        mainLayout->removeWidget(d->_searchEdit);
    }
    // 隐藏旧的、断开所有与之关联的信号槽
    d->_searchEdit->hide();
    d->_searchEdit->disconnect();
    d->_searchEdit->deleteLater();  // 延迟删除，避免立刻 delete 导致后面引用悬空

    // 2. 使用新的 lineEdit 替换
    //    先把它从原来的父对象脱离（如果它已经有父），然后把父设为 this
    if (lineEdit->parent() && lineEdit->parent() != this) {
        lineEdit->setParent(nullptr);
    }
    lineEdit->setParent(this);

    // 3. 把新控件赋值给私有成员
    d->_searchEdit = lineEdit;

    // 4. 设置新 _searchEdit 的样式/尺寸/placeholder/clear button
    d->_searchEdit->setFixedHeight(35);
    d->_searchEdit->setPlaceholderText("查找功能");
    d->_searchEdit->setClearButtonEnabled(true);
    d->_searchEdit->setIsClearButtonEnable(true);

    // 5. 把原先的图标 action 重新加到 new _searchEdit 上
    //    （假设 d->_lightSearchAction / d->_darkSearchAction 已经在私有成员初始化好）
    if (d->_themeMode == ElaThemeType::Light) {
        d->_searchEdit->addAction(d->_lightSearchAction, QLineEdit::TrailingPosition);
    } else {
        d->_searchEdit->addAction(d->_darkSearchAction, QLineEdit::TrailingPosition);
    }

    // 6. 重新绑定原来对 _searchEdit 的所有信号槽
    connect(d->_searchEdit, &ElaLineEdit::textChanged, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);
    //connect(d->_searchEdit, &ElaLineEdit::focusIn, d, &ElaSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchEdit, &ElaLineEdit::wmFocusOut, this, [d] {
        // 第一步：收缩宽度至搜索框宽度，高度保持不变
        QSize targetSize(d->_searchEdit->getOriginalWidth() + 10, d->_searchViewBaseWidget->height());
        d->_startSizeAnimation(d->_searchViewBaseWidget->size(), targetSize);

        // 延迟执行关闭动画（确保宽度收缩完成）
        QTimer::singleShot(300, d, [d] {
            d->_startCloseAnimation();
        });
    });
    connect(d->_searchEdit, &ElaLineEdit::widthChanged, d, &ElaSuggestBoxPrivate::onSearchEditWidthChanged);

    // 7. 把它插回原来的布局（假设它是放在最上层的第 0 个位置）
    if (mainLayout) {
        mainLayout->insertWidget(0, d->_searchEdit);
    }
    d->_searchEdit->show();
}

void ElaSuggestBox::suggestBoxPositionChanged() {
    Q_D(ElaSuggestBox);
    ///< 此处需要取消聚焦，动画才会自然
    d->_searchEdit->clearFocus();
}

bool ElaSuggestBox::eventFilter(QObject *watched, QEvent *event) {
   if (event->type() == QEvent::MouseButtonPress) {
       const auto ev = static_cast<QMouseEvent *>(event);

       QWidget *activePopup = QApplication::activePopupWidget();
       if (activePopup && (activePopup->inherits("QMenu") || activePopup->inherits("QWidget"))) {
           return QWidget::eventFilter(watched, event); ///< 菜单弹出时不处理
       }

       // qDebug() << "鼠标按下 : " << mapFromGlobal(me->globalPosition().toPoint()); ///< 调试用，记录鼠标位置
       if (!rect().contains(mapFromGlobal(ev->globalPosition().toPoint()))) {
           this->clearFocus(); ///< 点击外部区域时清除焦点
           // qDebug() << "清除焦点"; ///< 调试用，记录焦点清除
       }
   }
   return QWidget::eventFilter(watched, event);
}
