#ifndef SAPP_H_
#define SAPP_H_

#include <QApplication>

#include "SConfigFile.h"

#define sApp static_cast<SApp*>(qApp)

#define URL(route) QUrl("http://" + sApp->globalConfig()->value("host/host").toString() +route)

#if defined(COMMON_LIBRARY)
#define COMMON_EXPORT Q_DECL_EXPORT
#else
#define COMMON_EXPORT Q_DECL_IMPORT
#endif

class COMMON_EXPORT SApp : public QApplication
{
public:
    SApp(int argc, char *argv[]);

    inline SConfigFile *globalConfig()
    {
        if (!m_globalConfig)
            qWarning() << "global Config is nullptr";
        return m_globalConfig;
    }

    void setUserData(const QString &key, const QVariant &data);

    QVariant userData(const QString &key);

private:
    SConfigFile *m_globalConfig{};

    void initGlobalConfig();

    QVariantMap m_userData;
};

#endif // !SAPP_H_
