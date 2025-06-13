#include "ElaSuggestBoxPrivate.h"

#include <io.h>

#include "ElaBaseListView.h"
#include "ElaLineEdit.h"
#include "ElaSuggestBox.h"
#include "ElaSuggestBoxSearchViewContainer.h"
#include "ElaSuggestModel.h"

#include <QLayout>
#include <QNetworkReply>
#include <QPropertyAnimation>
#include <QUuid>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

class QNetworkReply;

ElaSuggestion::ElaSuggestion(QObject *parent)
    : QObject(parent) {
    _pElaIcon = ElaIconType::None;
    _pSuggestText = "";
    _pSuggestKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _pSuggestData = QVariantMap();

}

ElaSuggestion::~ElaSuggestion() {
}

ElaSuggestBoxPrivate::ElaSuggestBoxPrivate(QObject *parent)
    : QObject{parent} {
    _animationTimer = new QTimer(this);
    _animationTimer->setSingleShot(true); // 单次触发
    connect(_animationTimer, &QTimer::timeout, this, &ElaSuggestBoxPrivate::_doStartSizeAnimation);
}

ElaSuggestBoxPrivate::~ElaSuggestBoxPrivate() = default;

void ElaSuggestBoxPrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode) {
    _themeMode = themeMode;
    _searchEdit->removeAction(_themeMode == ElaThemeType::Light ? _darkSearchAction : _lightSearchAction);
    _searchEdit->addAction(_themeMode == ElaThemeType::Light ? _lightSearchAction : _darkSearchAction,
                           QLineEdit::TrailingPosition);
    _searchEdit->update();
}

/*
void ElaSuggestBoxPrivate::onSearchEditTextEdit(const QString &searchText) {
    Q_Q(ElaSuggestBox);
    if (searchText.isEmpty()) {
        _startCloseAnimation();
        return;
    }
    QVector<ElaSuggestion *> suggestionVector;
    for (const auto &suggest: _suggestionVector) {
        qDebug()<<"suggest->getSuggestText()： "<<suggest->getSuggestText()<<" searchText: "<<searchText;
        if (suggest->getSuggestText().contains(searchText, _pCaseSensitivity)) {
            suggestionVector.append(suggest);
        }
    }
    if (!suggestionVector.isEmpty()) {
        _searchModel->setSearchSuggestion(suggestionVector);
        int rowCount = suggestionVector.count();
        if (rowCount > 4) {
            rowCount = 4;
        }
        if (!_searchViewBaseWidget->isVisible()) {
            q->raise();
            _searchViewBaseWidget->show();
            _searchViewBaseWidget->raise();
            QPoint cyclePoint = _searchViewBaseWidget->mapFromGlobal(q->mapToGlobal(QPoint(-5, q->height())));
            if (cyclePoint != QPoint(0, 0)) {
                _searchViewBaseWidget->move(cyclePoint);
            }
            _startSizeAnimation(QSize(q->width() + 10, 0), QSize(q->width() + 10, 40 * rowCount + 16));
            _searchView->move(_searchView->x(), -(40 * rowCount + 16));
        } else {
            _startSizeAnimation(_searchViewBaseWidget->size(), QSize(q->width() + 12, 40 * rowCount + 16));
        }
        _startExpandAnimation();
    } else {
        _startCloseAnimation();
    }
}
*/

void ElaSuggestBoxPrivate::onSearchEditTextEdit(const QString &searchText) {
    Q_Q(ElaSuggestBox);
    if (searchText.isEmpty()) {
        _startCloseAnimation();
        return;
    }
    q->removeAllSuggestion();
    {
        QNetworkRequest request;
        QNetworkAccessManager manger;
        request.setUrl(QUrl(
            "https://c6.y.qq.com/splcloud/fcgi-bin/smartbox_new.fcg?key=" + searchText +
            "&format=json&inCharset=utf-8&outCharset=utf-8"));
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Accept-Language", "zh-CN");
        request.setRawHeader("User-Agent",
                             "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/137.0.0.0 Safari/537.36");
        request.setRawHeader("Content-Type", "text/html");
        request.setRawHeader("Accept-Encoding", "deflate");
        QNetworkReply *reply = manger.get(request);
        QEventLoop loop;
        connect(reply,SIGNAL(finished()), &loop,SLOT(quit()));
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            auto byt = reply->readAll();
            auto doc = QJsonDocument::fromJson(byt);
            QJsonObject objTemp = doc.object();
            objTemp = objTemp.value("data").toObject();
            objTemp = objTemp.value("song").toObject();
            QJsonArray arrayTemp = objTemp.value("itemlist").toArray();
            for (int i = 0; i < arrayTemp.count(); i++) {
                objTemp = arrayTemp.at(i).toObject();
                const QString songName = objTemp.value("name").toString();
                const QString songSinger = objTemp.value("singer").toString();
                /// qDebug()<<"resp text: "<<songSinger + "-" + songName;
                q->addSuggestion(songSinger + "-" + songName);
            }
        }

        QNetworkRequest request1;
        request1.setUrl(QUrl("http://music.163.com/api/search/get/web"));
        request1.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request1.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
        request1.setRawHeader("Referer", "https://music.163.com/");
        request1.setRawHeader("Origin", "https://music.163.com");

        QByteArray postData;
        postData.append("s=" + QUrl::toPercentEncoding(searchText));
        postData.append("&type=1&offset=0&limit=10");

        QNetworkReply *reply1 = manger.post(request1, postData);
        QEventLoop loop1;
        connect(reply1, SIGNAL(finished()), &loop1, SLOT(quit()));
        loop1.exec();

        if (reply1->error() == QNetworkReply::NoError) {
            QByteArray byt = reply1->readAll();
            /// qDebug() << "Raw response:" << byt;

            QJsonDocument doc = QJsonDocument::fromJson(byt);
            if (!doc.isObject()) return;

            QJsonObject rootObj = doc.object();
            QJsonArray songsArray = rootObj["result"].toObject()["songs"].toArray();

            for (const QJsonValue &val : songsArray) {
                QJsonObject songObj = val.toObject();
                QString songName = songObj["name"].toString();
                QJsonArray artists = songObj["artists"].toArray();
                QString artist = !artists.isEmpty() ? artists[0].toObject()["name"].toString() : "";
                /// qDebug() << artist + "-" + songName;
                q->addSuggestion(artist + "-" + songName);
            }
        }


        reply->deleteLater();
        reply1->deleteLater();
    }
    if (!_suggestionVector.isEmpty()) {
        _searchModel->setSearchSuggestion(_suggestionVector);
        int rowCount = static_cast<int>(_suggestionVector.count());
        rowCount = rowCount > 6 ? 6 : rowCount;
        if (!_searchViewBaseWidget->isVisible()) {
            q->raise();
            _searchViewBaseWidget->show();
            _searchViewBaseWidget->raise();

            // 修复位置计算 - 正确计算全局位置
            QPoint globalPos = q->mapToGlobal(QPoint(-5, q->height()));
            _searchViewBaseWidget->move(globalPos - _searchViewBaseWidget->parentWidget()->mapToGlobal(QPoint(0, 0)));

            _startSizeAnimation(QSize(q->width() + 10, 0), QSize(q->width() + 10, 40 * rowCount + 16));
            _searchView->move(_searchView->x(), -(40 * rowCount + 16));
        }
        else {
            _startSizeAnimation(_searchViewBaseWidget->size(), QSize(q->width() + 10, 40 * rowCount + 16));
        }
        _startExpandAnimation();
    } else {
        _startCloseAnimation();
    }
}

void ElaSuggestBoxPrivate::onSearchEditWidthChanged() {
    Q_Q(ElaSuggestBox);
    int rowCount = static_cast<int>(_suggestionVector.count());
    rowCount = rowCount > 6 ? 6 : rowCount;
    if (_searchViewBaseWidget->isVisible()) {
        // 修复位置计算 - 正确计算全局位置
        QPoint globalPos = q->mapToGlobal(QPoint(-5, q->height()));
        _searchViewBaseWidget->move(globalPos - _searchViewBaseWidget->parentWidget()->mapToGlobal(QPoint(0, 0)));

        //qDebug()<<"oldSize : "<< _searchViewBaseWidget->size() << " newSize : "<< QSize(q->width() + 10, 40 * rowCount + 16);

        _startSizeAnimation(_searchViewBaseWidget->size(), QSize(q->width() + 10, 40 * rowCount + 16));

        _searchView->move(_searchView->x(), -(40 * rowCount + 16));
    }
    //qDebug()<<__LINE__<<" _searchViewBaseWidget->pos(): "<<_searchViewBaseWidget->pos();
}

void ElaSuggestBoxPrivate::onSearchViewClicked(const QModelIndex &index) {
    Q_Q(ElaSuggestBox);
    _searchEdit->clear();
    _searchView->clearSelection();
    if (!index.isValid()) {
        return;
    }
    ElaSuggestion *suggest = _searchModel->getSearchSuggestion(index.row());
    Q_EMIT q->suggestionClicked(suggest->getSuggestText(), suggest->getSuggestData());
    _startCloseAnimation();
}

void ElaSuggestBoxPrivate::_startSizeAnimation(const QSize oldSize, const QSize newSize) {
    _pendingSize = newSize; // 更新待处理的目标尺寸
    if (!_animationTimer->isActive()) {
        _animationTimer->start(30); // 启动定时器，延迟 30ms
    }
}

void ElaSuggestBoxPrivate::_startExpandAnimation() {
    if (!_isExpandAnimationFinished) {
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

void ElaSuggestBoxPrivate::_startCloseAnimation() {
    if (!_isCloseAnimationFinished) {
        return;
    }
    _isExpandAnimationFinished = true;
    _isCloseAnimationFinished = false;
    QPropertyAnimation *baseWidgetsAnimation = new QPropertyAnimation(_searchViewBaseWidget, "size");
    baseWidgetsAnimation->setDuration(300);
    baseWidgetsAnimation->setEasingCurve(QEasingCurve::InOutSine);
    baseWidgetsAnimation->setStartValue(_searchViewBaseWidget->size());
    baseWidgetsAnimation->setEndValue(QSize(_searchViewBaseWidget->width(), 0));
    baseWidgetsAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QPropertyAnimation *closeAnimation = new QPropertyAnimation(_searchView, "pos");
    connect(closeAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isCloseAnimationFinished = true;
        _searchModel->clearSearchNode();
        _searchViewBaseWidget->hide();
    });
    closeAnimation->setDuration(300);
    closeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closeAnimation->setStartValue(_searchView->pos());
    closeAnimation->setEndValue(QPoint(_searchView->pos().x(), -_searchView->height()));
    closeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _lastSize = baseWidgetsAnimation->endValue().toSize();
}

void ElaSuggestBoxPrivate::_doStartSizeAnimation() {
    if (_pendingSize.isValid()) {
        Q_Q(ElaSuggestBox);
        if (_lastSize.isValid() && _lastSize == _pendingSize) {
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
