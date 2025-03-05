#ifndef ANICHECKBOX_H
#define ANICHECKBOX_H
/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt控件] 带动画的 CheckBox
 *  项目地址为 ： https://github.com/iwxyi/Qt-AniCheckBox
 *  非常实用，有兴趣的可以star
 */
#include <QCheckBox>
#include <QEasingCurve>

#if defined(ANICHECKBOX_LIBRARY)
#define ANICHECKBOX_EXPORT Q_DECL_EXPORT
#else
#define ANICHECKBOX_EXPORT Q_DECL_IMPORT
#endif

class QPropertyAnimation;

class ANICHECKBOX_EXPORT AniCheckBox : public QCheckBox {
    Q_OBJECT
    Q_PROPERTY(double hover_prog READ getHoverProg WRITE setHoverProg)
    Q_PROPERTY(double part_prog READ getPartProg WRITE setPartProg)
    Q_PROPERTY(double check_prog READ getCheckProg WRITE setCheckProg)

public:
    explicit AniCheckBox(QWidget *parent = nullptr);

    void setForeColor(QColor c);

protected:
    void paintEvent(QPaintEvent *) override;

    void enterEvent(QEnterEvent *e) override;

    void leaveEvent(QEvent *e) override;

    bool hitButton(const QPoint &) const override;

    virtual void checkStateChanged(const int& state);

    virtual void drawBox(QPainter &painter, QRectF rect);

    QPropertyAnimation *startAnimation(const QByteArray &property, double begin, double end, int duration = 500,
                                       QEasingCurve curve = QEasingCurve::OutQuad);

protected:
    double getHoverProg() const;

    void setHoverProg(double prog);

    double getPartProg() const;

    void setPartProg(double prog);

    double getCheckProg() const;

    void setCheckProg(double prog);

protected:
    int boxSide = 0; // 选择框边长，0为自适应
    QColor foreColor = QColor(0x2753ff); // 前景颜色

    double hoverProg = 0; // 鼠标移上去的进度
    double partyProg = 0; // 部分选中的进度
    double checkProg = 0; // 选中的进度
};

#endif // ANICHECKBOX_H
