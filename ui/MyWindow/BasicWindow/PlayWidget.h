/**
* @file PlayWidget.h
 * @brief 定义 PlayWidget 类，提供带圆角阴影和双击信号的播放控件
 * @author WeiWang
 * @date 2024-10-13
 * @version 1.0
 */

#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>

class ElaToolTip;

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

namespace Ui {
class PlayWidget;
}

/**
 * @class PlayWidget
 * @brief 自定义播放控件类，继承自 QWidget，支持圆角阴影和双击信号
 */
class MYWINDOW_EXPORT PlayWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化播放控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PlayWidget(QWidget *parent = nullptr);

    void setSongName(const QString &name);

    void setSingerName(const QString &singer);

    void setCover(const QPixmap &pix);

    void changeCircleToolButtonState(bool isSingleCircle);

    /**
     * @brief 更新进度条范围
     * @param duration 总时长（毫秒）
     * @note 设置进度条最大值和时长标签
     */
    void updateSliderRange(const qint64 &duration);

    void setNoVolume(const bool &flag);

    void setPlayPauseIcon(bool isPlay);

public slots:
    void onSliderPositionChanged(const int &position);

    void onCoverChanged(const QPixmap &pix);

    void onSongNameChanged(const QString &name);

    void onSingerNameChanged(const QString &name);

    void onAudioPlay();

    void onAudioPause();

private:
    void initUi();

    /**
     * @brief 设置控件的工具提示
     * @param widget 目标控件
     * @param tooltip 提示内容
     */
    static void setElaToolTip(QWidget *widget, const QString &tooltip);

protected:
    /**
     * @brief 重写鼠标双击事件，触发双击信号
     * @param event 鼠标事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 重写绘制事件，绘制带圆角的线性渐变阴影
     * @param ev 绘制事件
     */
    void paintEvent(QPaintEvent *ev) override;

    /**
     * @brief 事件过滤器
     * @param watched 监控对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理进度条和封面标签的事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_play_or_pause_toolButton_clicked();
    void on_love_toolButton_clicked();
    void on_download_toolButton_clicked();
    void on_comment_toolButton_clicked();
    void on_share_toolButton_clicked();
    void on_more_toolButton_clicked();
    void on_circle_toolButton_clicked();
    void on_pre_toolButton_clicked();
    void on_next_toolButton_clicked();
    void on_speed_pushButton_clicked();
    void on_stander_pushButton_clicked();
    void on_acoustics_pushButton_clicked();
    void on_erji_toolButton_clicked();
    void on_lyrics_toolButton_clicked();
    void on_song_queue_toolButton_clicked();
signals :
    /**
     * @brief 双击信号，当控件被左键双击时触发
     */
    void doubleClicked();

    void volumeChange(int value);

    void sliderReleased(const int &sliderValue, const int &maxSliderValue);

    void sliderPressed(const int &value);

    void clickedPlayPauseBtn();

    void clickedCircleBtn();

    void clickedPreBtn();

    void clickedNextBtn();

private:
    Ui::PlayWidget *ui; ///< UI 界面对象
    ElaToolTip *m_songNameToolTip;
    ElaToolTip *m_singerToolTip;
};

#endif // PLAYWIDGET_H