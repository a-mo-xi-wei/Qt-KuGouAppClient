//
// Created by WeiWang on 25-5-27.
//

#ifndef SPEEDDIALOG_H
#define SPEEDDIALOG_H

#include <QWidget>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYDIALOG_LIBRARY)
#define MYDIALOG_EXPORT Q_DECL_EXPORT
#else
#define MYDIALOG_EXPORT Q_DECL_IMPORT
#endif

class QGraphicsDropShadowEffect;

class MYDIALOG_EXPORT SpeedDialog : public QWidget {
    Q_OBJECT

public:
    explicit SpeedDialog(QWidget *parent = nullptr);

    ~SpeedDialog() override;

protected:
    void paintEvent(QPaintEvent *ev) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    std::unique_ptr<QGraphicsDropShadowEffect> m_effect; ///< 阴影效果
};


#endif //SPEEDDIALOG_H
