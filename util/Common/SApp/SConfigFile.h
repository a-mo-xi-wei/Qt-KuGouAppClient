#ifndef SCONFIGFILE_H_
#define  SCONFIGFILE_H_

#include <QSettings>

#if defined(COMMON_LIBRARY)
#define COMMON_EXPORT Q_DECL_EXPORT
#else
#define COMMON_EXPORT Q_DECL_IMPORT
#endif

class COMMON_EXPORT SConfigFile
{
public:
    SConfigFile(const QString &filename);
    bool isOpen() const { return m_settings.status() == QSettings::NoError; }

    void setValue(QAnyStringView key, const QVariant &value);
    QVariant value(QAnyStringView key) const;
    QVariant value(QAnyStringView key, const QVariant &def_value) const;

    void removeValue(QAnyStringView key);

private:
    QSettings m_settings;
};

#endif // !SCONFIGFILE_H_
