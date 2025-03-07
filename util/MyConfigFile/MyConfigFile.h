#ifndef MYCONFIGFILE_H_
#define MYCONFIGFILE_H_

#include <QSettings>
#include <QAnyStringView>

#if defined(MYCONFIG_LIBRARY)
#define MYCONFIG_EXPORT Q_DECL_EXPORT
#else
#define MYCONFIG_EXPORT Q_DECL_IMPORT
#endif

class MYCONFIG_EXPORT MyConfigFile
{
public:
	MyConfigFile(const QString& filename);
	bool isOpen()const { return m_settings.status() == QSettings::NoError; }

	void setValue(QAnyStringView key, const QVariant& value);
	QVariant value(QAnyStringView key)const;
	QVariant value(QAnyStringView key,const QVariant& def_value)const;
	void removeValue(QAnyStringView key);
private:
	QSettings m_settings;
};

#endif // !MYCONFIGFILE_H_
