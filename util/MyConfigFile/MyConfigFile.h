/*
MyConfigFile - 配置文件管理类
*/

#ifndef MYCONFIGFILE_H_
#define MYCONFIGFILE_H_

#include <QSettings>
#include <QAnyStringView>

/** @def MYCONFIG_LIBRARY
 *  @brief 定义是否编译为动态库
 *
 *  如果定义了 MYCONFIG_LIBRARY，则导出符号；否则导入符号。
 */

/** @def MYCONFIG_EXPORT
 *  @brief 定义符号导出或导入
 *
 *  根据 MYCONFIG_LIBRARY 的定义，设置为 Q_DECL_EXPORT 或 Q_DECL_IMPORT。
 */
#if defined(MYCONFIG_LIBRARY)
#define MYCONFIG_EXPORT Q_DECL_EXPORT
#else
#define MYCONFIG_EXPORT Q_DECL_IMPORT
#endif

/** @class MyConfigFile
 *  @brief 管理 INI 格式配置文件的类
 *
 *  该类基于 QSettings 提供对 INI 格式配置文件的读写操作，支持设置、读取和删除键值对。
 */
class MYCONFIG_EXPORT MyConfigFile {
public:
	/** @brief 构造函数
	 *
	 *  初始化配置文件对象，指定配置文件路径和格式为 INI。
	 *
	 *  @param filename 配置文件路径
	 */
	MyConfigFile(const QString& filename);

	/** @brief 检查配置文件是否成功打开
	 *
	 *  @return 如果配置文件成功打开则返回 true，否则返回 false
	 */
	bool isOpen() const { return m_settings.status() == QSettings::NoError; }

	/** @brief 设置键值对
	 *
	 *  将指定的键和值写入配置文件。
	 *
	 *  @param key 键名
	 *  @param value 键值
	 */
	void setValue(QAnyStringView key, const QVariant& value);

	/** @brief 获取键值
	 *
	 *  获取指定键的值，如果键不存在则返回无效 QVariant。
	 *
	 *  @param key 键名
	 *  @return 键对应的值
	 */
	QVariant value(QAnyStringView key) const;

	/** @brief 获取键值（带默认值）
	 *
	 *  获取指定键的值，如果键不存在则返回指定的默认值。
	 *
	 *  @param key 键名
	 *  @param def_value 默认值
	 *  @return 键对应的值或默认值
	 */
	QVariant value(QAnyStringView key, const QVariant& def_value) const;

	/** @brief 删除键值对
	 *
	 *  从配置文件中删除指定的键。
	 *
	 *  @param key 键名
	 */
	void removeValue(QAnyStringView key);

private:
	/** @brief 配置文件操作对象
	 */
	QSettings m_settings;
};

#endif // MYCONFIGFILE_H_