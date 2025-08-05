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

    void setHintText(const QString& text) const;

    void setStatus(Status status);

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
    Status m_status = Neutral;

private:
    QPoint m_targetPos;
};
#endif
