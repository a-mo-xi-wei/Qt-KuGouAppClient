#ifndef _VALIDATIONHINT_
#define _VALIDATIONHINT_

#include <QLabel>

class ValidationHint : public QWidget
{
    Q_OBJECT

public:
    explicit ValidationHint(QWidget* parent = nullptr);

    ~ValidationHint() override;

    enum Status
    {
        Valid,
        Invalid,
        Neutral
    };

    void setHintText(const QString& text);

    void setStatus(Status status, const QString& text = "");

    Status getStatus();

    void smoothShow();

    void smoothHide();

    void setTargetPos(const QPoint& pos);

private:
    void fadeInIcon();

    void fadeOutIcon();

private:
    QLabel* iconLabel;
    QLabel* textLabel;
    QString m_text;
    Status m_status = Neutral;

private:
    QPoint m_targetPos;
};
#endif
