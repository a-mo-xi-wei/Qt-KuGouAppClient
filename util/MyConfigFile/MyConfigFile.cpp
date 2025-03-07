#include "MyConfigFile.h"

MyConfigFile::MyConfigFile(const QString& filename)
	:m_settings(filename,QSettings::IniFormat)
{
}

void MyConfigFile::setValue(QAnyStringView key, const QVariant& value)
{
	m_settings.setValue(key, value);
}

QVariant MyConfigFile::value(QAnyStringView key) const
{
	return value(key,QVariant());
}

QVariant MyConfigFile::value(QAnyStringView key, const QVariant& def_value) const
{
#ifdef _DEBUG
	auto v = m_settings.value(key);
	if (!v.isValid())
	{
		qWarning() << key.toString() << "Not Found";
		return def_value;
	}
#endif // _DEBUG
	return m_settings.value(key,def_value);//key存在就返回对应的value,否则返回def_value
}  

void MyConfigFile::removeValue(QAnyStringView key)
{
	m_settings.remove(key);
}
