/*
MyConfigFile - 配置文件管理类实现
*/

#include "MyConfigFile.h"

/** @brief 构造函数
 *
 *  初始化配置文件对象，指定配置文件路径和格式为 INI。
 *
 *  @param filename 配置文件路径
 */
MyConfigFile::MyConfigFile(const QString& filename)
	: m_settings(filename, QSettings::IniFormat) {
}

/** @brief 设置键值对
 *
 *  将指定的键和值写入配置文件。
 *
 *  @param key 键名
 *  @param value 键值
 */
void MyConfigFile::setValue(QAnyStringView key, const QVariant& value) {
	m_settings.setValue(key, value);
}

/** @brief 获取键值
 *
 *  获取指定键的值，如果键不存在则返回无效 QVariant。
 *
 *  @param key 键名
 *  @return 键对应的值
 */
QVariant MyConfigFile::value(QAnyStringView key) const {
	return value(key, QVariant());
}

/** @brief 获取键值（带默认值）
 *
 *  获取指定键的值，如果键不存在则返回指定的默认值。在调试模式下，如果键不存在，会记录警告。
 *
 *  @param key 键名
 *  @param def_value 默认值
 *  @return 键对应的值或默认值
 */
QVariant MyConfigFile::value(QAnyStringView key, const QVariant& def_value) const {
#ifdef _DEBUG
	auto v = m_settings.value(key);
	if (!v.isValid()) {
		qWarning() << key.toString() << "Not Found";
		return def_value;
	}
#endif // _DEBUG
	return m_settings.value(key, def_value);
}

/** @brief 删除键值对
 *
 *  从配置文件中删除指定的键。
 *
 *  @param key 键名
 */
void MyConfigFile::removeValue(QAnyStringView key) {
	m_settings.remove(key);
}