/**
 * @file MusicRepository.cpp
 * @brief 实现 MusicRepository 类，管理音乐仓库（歌曲和视频）界面
 * @author WeiWang
 * @date 2024-11-11
 * @version 1.0
 */

#include "MusicRepository.h"
#include "ui_MusicRepository.h"
#include "MusicRepoList.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "Async.h"

#include <QFile>
#include <QButtonGroup>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMouseEvent>
#include <QTimer>
#include <random>


/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐仓库界面
 * @param parent 父控件指针，默认为 nullptr
 */
MusicRepository::MusicRepository(QWidget *parent)
    : QWidget(parent)
  , ui(new Ui::MusicRepository)
  , m_buttonGroup(std::make_unique<QButtonGroup>(this))
  , m_currentIdx(0)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/musicrepo.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll()); ///< 应用样式表
    }
    else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
        return;
    }
    initUi(); ///< 初始化界面
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] {
        enableButton(true); ///< 动画结束时启用按钮
    });
    enableButton(true); ///< 初始启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
MusicRepository::~MusicRepository() {
    delete ui; ///< 删除 UI
}

QWidget * MusicRepository::createRepoPage(const int& beg) {
    auto pageWidget = new QWidget;
    auto mainLayout = new QVBoxLayout(pageWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 0, 10, 0);

    for (int row = 0; row < 3; ++row) {
        auto rowLayout = new QHBoxLayout;
        rowLayout->setSpacing(10);
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col + beg;
            if (index >= 9 + beg) break;
            auto item = new MusicRepoList;
            item->setCoverPix(m_musicData[index].pixPath);
            item->setSongName(m_musicData[index].song);
            item->setSinger(m_musicData[index].singer);
            rowLayout->addWidget(item);
            rowLayout->setStretch(col, 1);
        }
        mainLayout->addLayout(rowLayout);
    }

    return pageWidget;
}

/**
 * @brief 初始化按钮组
 * @note 设置按钮互斥
 */
void MusicRepository::initButtonGroup() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(this->m_musicData.begin(), this->m_musicData.end(), std::default_random_engine(seed)); ///< 随机打乱

    // 设置按钮组（互斥）
    this->m_buttonGroup->addButton(ui->chinese_pushButton, 0);
    this->m_buttonGroup->addButton(ui->west_pushButton, 1);
    this->m_buttonGroup->addButton(ui->korea_pushButton, 2);
    this->m_buttonGroup->addButton(ui->japan_pushButton, 3);
    this->m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 4; ++i) {
        auto *placeholder = new QWidget;
        auto *layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_repoPages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    m_repoPages[m_currentIdx]->layout()->addWidget(createRepoPage(1));
    ui->stackedWidget->slideInIdx(0);

    // 响应按钮点击事件
    connect(m_buttonGroup.get(), &QButtonGroup::idClicked, this, [this](const int& id) {
        // qDebug() << "[DEBUG] Current stack index:" << m_currentIdx;
        // qDebug() << "[DEBUG] Clicked page ID:" << id;

        if (m_currentIdx == id) {
            // qDebug() << "[DEBUG] Already on the selected page. No action taken.";
            // qDebug() << "[DEBUG] -------------------------------------------------------- ";
            return;
        }

        enableButton(false);

        QWidget *placeholder = m_repoPages[m_currentIdx];
        if (!placeholder) {
            qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
            enableButton(true);
            return;
        }

        // 清理目标 placeholder 内旧的控件
        QLayout *layout = placeholder->layout();
        if (!layout) {
            layout = new QVBoxLayout(placeholder);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
        } else {
            while (QLayoutItem* item = layout->takeAt(0)) {
                if (QWidget* widget = item->widget()) {
                    // qDebug()<<"**********删除控件*********";
                    widget->deleteLater();
                }
                delete item;
            }
        }
        placeholder = m_repoPages[id];
        layout = placeholder->layout();
        // 创建新页面
        int beginIndex = id * 10 + 1;
        // qDebug() << "[DEBUG] Creating new repo page. Begin index:" << beginIndex;
        QWidget *realPage = createRepoPage(beginIndex);
        if (!realPage) {
            qWarning() << "[WARNING] Failed to create repo page at index:" << id;
        } else {
            layout->addWidget(realPage);
            // qDebug() << "[DEBUG] Repo page added to placeholder layout";
        }

        ui->stackedWidget->slideInIdx(id);
        m_currentIdx = id;

        enableButton(true);
        // qDebug() << "[DEBUG] -------------------------------------------------------- ";
        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString();
    });
}

/**
 * @brief 初始化界面
 * @note 设置鼠标样式、初始化按钮组、容器、新碟上架和精选视频
 */
void MusicRepository::initUi() {
    ui->ranking_list_widget->setCursor(Qt::PointingHandCursor); ///< 设置排行榜鼠标样式
    ui->singer_widget->setCursor(Qt::PointingHandCursor);       ///< 设置歌手鼠标样式
    ui->classify_widget->setCursor(Qt::PointingHandCursor);     ///< 设置分类鼠标样式

    const auto future = Async::runAsync(QThreadPool::globalInstance(), [this]() {
        QList<QJsonObject> data;                                         ///< 数据列表
        QFile file(GET_CURRENT_DIR + QStringLiteral("/musicrepo.json")); ///< 加载 JSON 文件
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading musicrepo.json";
            STREAM_WARN() << "Could not open file for reading musicrepo.json"; ///< 记录警告日志
            return data;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        auto arr       = obj.array();
        for (const auto &item: arr) {
            data.append(item.toObject()); ///< 添加数据项
        }
        file.close();
        return data;
    });

    Async::onResultReady(future, this, [this](const QList<QJsonObject> &datas) {
        if (datas.isEmpty()) {
            qWarning() << "musicrepo.json is empty or failed to parse";
            STREAM_WARN() << "musicrepo.json is empty or failed to parse"; ///< 记录警告日志
            return;
        }

        for (int i = 1; i <= std::min(60, static_cast<int>(datas.size())); i++) {
            this->m_musicData.emplace_back(
                QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i),
                datas[i].value("song").toString(),
                datas[i].value("singer").toString()); ///< 插入全部音乐信息
        }
        for (int i = 1; i <= 40; ++i) {
            this->m_videoVector.emplace_back(
                QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i),
                m_musicData[i + 10].song,
                m_musicData[i + 10].singer); ///< 插入视频信息
        }

        initNewDiskWidget(); ///< 初始化新碟上架
        initSelectWidget();  ///< 初始化精选视频
        initButtonGroup();   ///< 初始化按钮组
        ui->chinese_pushButton->click(); ///< 默认点击华语按钮
    });
}

/**
 * @brief 初始化新碟上架控件
 * @note 随机打乱并隐藏部分块
 */
void MusicRepository::initNewDiskWidget() {
    ui->block_widget6->hide();                                                                  ///< 隐藏块 6
    ui->block_widget7->hide();                                                                  ///< 隐藏块 7
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();                ///< 获取时间种子
    std::shuffle(this->m_musicData.begin(), this->m_musicData.end(), std::default_random_engine(seed)); ///< 随机打乱
    // 控件数组，便于循环处理
    MusicRepoBlock *blockWidgets[] = {
        ui->block_widget1,
        ui->block_widget2,
        ui->block_widget3,
        ui->block_widget4,
        ui->block_widget5,
        ui->block_widget6,
        ui->block_widget7
    };

    // 使用 lambda 设置控件属性
    auto setupWidget = [](MusicRepoBlock *widget, const auto &block) {
        if (!widget) {
            qWarning() << "Widget is null";
            return;
        }
        widget->setCoverPix(block.pixPath);
        widget->setSongName(block.song);
        widget->setSinger(block.singer);
    };

    // 循环设置每个控件
    for (int i = 0; i < 7; ++i) {
        setupWidget(blockWidgets[i], m_musicData[i + 1]);
    }
}

/**
 * @brief 初始化精选视频控件
 * @note 随机打乱并隐藏部分视频
 */
void MusicRepository::initSelectWidget() {
    ui->video_widget4->hide(); ///< 隐藏视频 4
    ui->video_widget5->hide(); ///< 隐藏视频 5
    ui->video_widget9->hide(); ///< 隐藏视频 9
    ui->video_widget10->hide(); ///< 隐藏视频 10
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取时间种子
    std::shuffle(this->m_videoVector.begin(), this->m_videoVector.end(), std::default_random_engine(seed)); ///< 随机打乱
    // 控件数组，便于循环处理
    MusicRepoVideo *videoWidgets[] = {
        ui->video_widget1,
        ui->video_widget2,
        ui->video_widget3,
        ui->video_widget4,
        ui->video_widget5,
        ui->video_widget6,
        ui->video_widget7,
        ui->video_widget8,
        ui->video_widget9,
        ui->video_widget10
    };

    // 使用 lambda 设置控件属性
    auto setupWidget = [](MusicRepoVideo *widget, const auto &video) {
        if (!widget) {
            qWarning() << "Widget is null";
            return;
        }
        widget->setCoverPix(video.pixPath);
        widget->setVideoName(video.song);
        widget->setIconPix(video.pixPath);
        widget->setAuthor(video.singer);
    };

    // 循环设置每个控件
    for (int i = 0; i < 10; ++i) {
        setupWidget(videoWidgets[i], m_videoVector[i + 1]);
    }
}

void MusicRepository::enableButton(const bool &flag) const {
    ui->chinese_pushButton->setEnabled(flag);
    ui->west_pushButton->setEnabled(flag);
    ui->korea_pushButton->setEnabled(flag);
    ui->japan_pushButton->setEnabled(flag);
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 动态调整控件高度/宽度和显示/隐藏块
 */
void MusicRepository::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    int average = static_cast<int>(160 + (event->size().width() - 900) * 0.15); ///< 计算控件高度
    ui->ranking_list_widget->setFixedHeight(average);                           ///< 设置排行榜高度
    ui->singer_widget->setFixedHeight(average);                                 ///< 设置歌手高度
    ui->classify_widget->setFixedHeight(average);                               ///< 设置分类高度

    static int lastVisibleState = -1;            ///< 记录上一次可见状态
    const int currentWidth      = this->width(); ///< 获取当前宽度
    int newVisibleState;
    if (currentWidth < 1045) {
        newVisibleState = 0; ///< 状态 0：隐藏较多块
    }
    else if (currentWidth < 1250) {
        newVisibleState = 1; ///< 状态 1：显示部分块
    }
    else {
        newVisibleState = 2; ///< 状态 2：显示全部块
    }
    if (newVisibleState != lastVisibleState) {
        switch (newVisibleState) {
            case 0:
                ui->block_widget6->hide();  ///< 隐藏块 6
                ui->block_widget7->hide();  ///< 隐藏块 7
                ui->video_widget4->hide();  ///< 隐藏视频 4
                ui->video_widget5->hide();  ///< 隐藏视频 5
                ui->video_widget9->hide();  ///< 隐藏视频 9
                ui->video_widget10->hide(); ///< 隐藏视频 10
                break;
            case 1:
                ui->block_widget6->show(); ///< 显示块 6
                ui->block_widget7->hide();
                ui->video_widget4->show(); ///< 显示视频 4
                ui->video_widget5->hide();
                ui->video_widget9->show(); ///< 显示视频 9
                ui->video_widget10->hide();
                break;
            case 2:
                ui->block_widget6->show();  ///< 显示块 6
                ui->block_widget7->show();  ///< 显示块 7
                ui->video_widget4->show();  ///< 显示视频 4
                ui->video_widget5->show();  ///< 显示视频 5
                ui->video_widget9->show();  ///< 显示视频 9
                ui->video_widget10->show(); ///< 显示视频 10
                break;
            default:
                break;
        }
        lastVisibleState = newVisibleState; ///< 更新可见状态
    }
}

/**
 * @brief 更多按钮 1 点击槽函数
 * @note 显示未实现提示
 */
void MusicRepository::on_more_pushButton1_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(
                                   ui->more_pushButton1->text().left(ui->more_pushButton1->text().size() - 2)),
                               1000, this->window()); ///< 显示提示
}

/**
 * @brief 更多按钮 2 点击槽函数
 * @note 显示未实现提示
 */
void MusicRepository::on_more_pushButton2_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(
                                   ui->more_pushButton2->text().left(ui->more_pushButton2->text().size() - 2)),
                               1000, this->window()); ///< 显示提示
}

/**
 * @brief 更多按钮 3 点击槽函数
 * @note 显示未实现提示
 */
void MusicRepository::on_more_pushButton3_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(
                                   ui->more_pushButton3->text().left(ui->more_pushButton3->text().size() - 2)),
                               1000, this->window()); ///< 显示提示
}
