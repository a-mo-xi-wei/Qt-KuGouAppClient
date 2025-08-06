#include "SApp.h"
#include <QFile>
#include<QDir>
SApp::SApp(int argc, char* argv[])
    : QApplication(argc, argv)
{
    initGlobalConfig();
}


void SApp::initGlobalConfig()
{
    auto configFileName = QDir::currentPath() + "config.ini";
    //判断一下配置文件是否存在，如果存在则打开即可；如果不存在则添加默认配置
    bool isExists = false;
    if (QFile::exists(configFileName))
    {
        isExists = true;
    }
    //打开配置文件
    m_globalConfig = new SConfigFile(configFileName);
    Q_ASSERT_X(m_globalConfig != nullptr, __FUNCTION__, "Global Config File Create Failed~");
    if (!m_globalConfig->isOpen())
    {
        qWarning() << "global config init failed";
        return;
    }

    //如果原本是不存在的，则添加默认配置
    if (!isExists)
    {
        globalConfig()->setValue("host/host", "127.0.0.1:8888");
    }
}


void SApp::setUserData(const QString& key, const QVariant& data)
{
    auto it = m_userData.find(key);
    if (it != m_userData.end())
    {
        qWarning() << key << "of value exists";
    }
    m_userData.insert(key, data);
}

QVariant SApp::userData(const QString& key)
{
    auto it = m_userData.find(key);
    if (it == m_userData.end())
    {
        qWarning() << key << "of value not exists";
        return QVariant();
    }
    return m_userData.value(key);
}

