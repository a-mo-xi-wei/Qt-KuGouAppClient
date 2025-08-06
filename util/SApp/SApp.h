#ifndef SAPP_H_
#define SAPP_H_

#include <QApplication>

#include "SConfigFile.h"

#define sApp static_cast<SApp*>(qApp)

#define URL(route) QUrl("http://" + sApp->globalConfig()->value("host/host").toString() +route)

#if defined(SAPP_LIBRARY)
#define SAPP_EXPORT Q_DECL_EXPORT
#else
#define SAPP_EXPORT Q_DECL_IMPORT
#endif

class SAPP_EXPORT SApp : public QApplication
{
public:
    SApp(int argc, char* argv[]);

    inline SConfigFile* globalConfig()
    {
        if (!m_globalConfig)
            qWarning() << "global Config is nullptr";
        return m_globalConfig;
    }

    void setUserData(const QString& key, const QVariant& data);

    QVariant userData(const QString& key);

private:
    SConfigFile* m_globalConfig{};

    void initGlobalConfig();

    QVariantMap m_userData;
};

#endif // !SAPP_H_
