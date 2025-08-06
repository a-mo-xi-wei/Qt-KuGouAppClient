#include "SConfigFile.h"

SConfigFile::SConfigFile(const QString& filename)
    : m_settings(filename, QSettings::IniFormat)
{
}

void SConfigFile::setValue(QAnyStringView key, const QVariant& value)
{
    m_settings.setValue(key, value);
}

QVariant SConfigFile::value(QAnyStringView key) const
{
    return value(key, QVariant());
}

QVariant SConfigFile::value(QAnyStringView key, const QVariant& def_value) const
{
#ifdef _DEBUG
    auto v = m_settings.value(key);
    if (!v.isValid())
    {
        qWarning() << key.toString() << "Not Found";
        return def_value;
    }
#endif // _DEBUG
    return m_settings.value(key, def_value); //key存在就返回对应的value,否则返回def_value
}

void SConfigFile::removeValue(QAnyStringView key)
{
    m_settings.remove(key);
}
