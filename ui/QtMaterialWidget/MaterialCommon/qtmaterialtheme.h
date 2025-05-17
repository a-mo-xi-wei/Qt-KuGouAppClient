/**
 * @file qtmaterialtheme.h
 * @brief 定义 QtMaterialTheme 类和 Material 命名空间，提供 Material Design 主题支持
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALTHEME_H
#define QTMATERIALTHEME_H

#include <QObject>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

/**
 * @brief Material Design 相关枚举和类型的命名空间
 */
namespace Material
{
    /**
     * @brief 按钮预设样式
     */
    enum ButtonPreset {
        FlatPreset,      ///< 扁平按钮样式
        CheckablePreset  ///< 可选中按钮样式
    };

    /**
     * @brief 水波纹样式
     */
    enum RippleStyle {
        CenteredRipple,   ///< 中心水波纹
        PositionedRipple, ///< 指定位置水波纹
        NoRipple          ///< 无水波纹
    };

    /**
     * @brief 覆盖层样式
     */
    enum OverlayStyle {
        NoOverlay,      ///< 无覆盖层
        TintedOverlay,  ///< 着色覆盖层
        GrayOverlay     ///< 灰色覆盖层
    };

    /**
     * @brief 控件角色
     */
    enum Role {
        Default,   ///< 默认角色
        Primary,   ///< 主角色
        Secondary  ///< 次角色
    };

    /**
     * @brief 按钮图标放置位置
     */
    enum ButtonIconPlacement {
        LeftIcon,   ///< 图标在左侧
        RightIcon   ///< 图标在右侧
    };

    /**
     * @brief 进度条类型
     */
    enum ProgressType {
        DeterminateProgress,   ///< 确定进度条
        IndeterminateProgress  ///< 不确定进度条
    };

    /**
     * @brief 头像类型
     */
    enum AvatarType {
        ImageAvatar,   ///< 图片头像
        IconAvatar,    ///< 图标头像
        LetterAvatar   ///< 字母头像
    };

    /**
     * @brief Material Design 调色板颜色枚举
     */
    enum Color {
        red50, red100, red200, red300, red400, red500, red600, red700, red800, red900,
        redA100, redA200, redA400, redA700,       ///< 红色系
        pink50, pink100, pink200, pink300, pink400, pink500, pink600, pink700, pink800, pink900,
        pinkA100, pinkA200, pinkA400, pinkA700,   ///< 粉红色系
        purple50, purple100, purple200, purple300, purple400, purple500, purple600, purple700, purple800, purple900,
        purpleA100, purpleA200, purpleA400, purpleA700, ///< 紫色系
        deepPurple50, deepPurple100, deepPurple200, deepPurple300, deepPurple400, deepPurple500,
        deepPurple600, deepPurple700, deepPurple800, deepPurple900,
        deepPurpleA100, deepPurpleA200, deepPurpleA400, deepPurpleA700, ///< 深紫色系
        indigo50, indigo100, indigo200, indigo300, indigo400, indigo500, indigo600, indigo700, indigo800, indigo900,
        indigoA100, indigoA200, indigoA400, indigoA700, ///< 靛蓝色系
        blue50, blue100, blue200, blue300, blue400, blue500, blue600, blue700, blue800, blue900,
        blueA100, blueA200, blueA400, blueA700,   ///< 蓝色系
        lightBlue50, lightBlue100, lightBlue200, lightBlue300, lightBlue400, lightBlue500,
        lightBlue600, lightBlue700, lightBlue800, lightBlue900,
        lightBlueA100, lightBlueA200, lightBlueA400, lightBlueA700, ///< 浅蓝色系
        cyan50, cyan100, cyan200, cyan300, cyan400, cyan500, cyan600, cyan700, cyan800, cyan900,
        cyanA100, cyanA200, cyanA400, cyanA700,   ///< 青色系
        teal50, teal100, teal200, teal300, teal400, teal500, teal600, teal700, teal800, teal900,
        tealA100, tealA200, tealA400, tealA700,   ///< 青绿色系
        green50, green100, green200, green300, green400, green500, green600, green700, green800, green900,
        greenA100, greenA200, greenA400, greenA700, ///< 绿色系
        lightGreen50, lightGreen100, lightGreen200, lightGreen300, lightGreen400, lightGreen500,
        lightGreen600, lightGreen700, lightGreen800, lightGreen900,
        lightGreenA100, lightGreenA200, lightGreenA400, lightGreenA700, ///< 浅绿色系
        lime50, lime100, lime200, lime300, lime400, lime500, lime600, lime700, lime800, lime900,
        limeA100, limeA200, limeA400, limeA700,   ///< 酸橙色系
        yellow50, yellow100, yellow200, yellow300, yellow400, yellow500, yellow600, yellow700, yellow800, yellow900,
        yellowA100, yellowA200, yellowA400, yellowA700, ///< 黄色系
        amber50, amber100, amber200, amber300, amber400, amber500, amber600, amber700, amber800, amber900,
        amberA100, amberA200, amberA400, amberA700, ///< 琥珀色系
        orange50, orange100, orange200, orange300, orange400, orange500, orange600, orange700, orange800, orange900,
        orangeA100, orangeA200, orangeA400, orangeA700, ///< 橙色系
        deepOrange50, deepOrange100, deepOrange200, deepOrange300, deepOrange400, deepOrange500,
        deepOrange600, deepOrange700, deepOrange800, deepOrange900,
        deepOrangeA100, deepOrangeA200, deepOrangeA400, deepOrangeA700, ///< 深橙色系
        brown50, brown100, brown200, brown300, brown400, brown500, brown600, brown700, brown800, brown900, ///< 棕色系
        blueGrey50, blueGrey100, blueGrey200, blueGrey300, blueGrey400, blueGrey500,
        blueGrey600, blueGrey700, blueGrey800, blueGrey900, ///< 蓝灰色系
        grey50, grey100, grey200, grey300, grey400, grey500, grey600, grey700, grey800, grey900, ///< 灰色系
        black, white, transparent,                        ///< 基本颜色
        fullBlack, darkBlack, lightBlack, minBlack, faintBlack, ///< 黑色变体
        fullWhite, darkWhite, lightWhite                   ///< 白色变体
    };
}

// 前向声明
class QtMaterialThemePrivate;

/**
 * @class QtMaterialTheme
 * @brief Material Design 主题类，继承自 QObject，管理颜色和图标
 * @note 内部实现
 */
class QTMATERIALWIDGET_EXPORT QtMaterialTheme : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化主题
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit QtMaterialTheme(QObject *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialTheme();

    /**
     * @brief 获取主题颜色
     * @param key 颜色键值
     * @return 对应颜色，若键值不存在返回无效颜色
     */
    QColor getColor(const QString &key) const;

    /**
     * @brief 设置主题颜色
     * @param key 颜色键值
     * @param color 颜色值
     */
    void setColor(const QString &key, const QColor &color);

    /**
     * @brief 设置主题颜色（使用枚举）
     * @param key 颜色键值
     * @param color Material 颜色枚举值
     */
    void setColor(const QString &key, Material::Color color);

    /**
     * @brief 加载 Material Design 图标
     * @param category 图标类别
     * @param icon 图标名称
     * @return 图标对象
     */
    static QIcon icon(QString category, QString icon);

protected:
    const QScopedPointer<QtMaterialThemePrivate> d_ptr; ///< 私有实现指针

private:
    Q_DISABLE_COPY(QtMaterialTheme) ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialTheme) ///< 声明私有类
};

#endif // QTMATERIALTHEME_H