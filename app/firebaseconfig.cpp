#include "firebaseconfig.h"
#include <exception>

FirebaseConfig::FirebaseConfig()
    :m_settings(":/config.ini", QSettings::Format::IniFormat)
{
}

std::tuple<bool, QString> FirebaseConfig::valid() const
{
    QStringList expectedKeys = {
        "firebase/app-id",
        "firebase/api-key",
        "firebase/project-id"
    };

    for (auto key : expectedKeys) {
        if (!m_settings.contains(key))
            return {false, "Could not find key '" + key + "'"};
    }

    return {true, ""};
}

QString FirebaseConfig::appId() const
{
    return m_settings.value("firebase/app-id", "").toString();
}

QString FirebaseConfig::apiKey() const
{
    return m_settings.value("firebase/api-key", "").toString();
}

QString FirebaseConfig::projectId() const
{
    return m_settings.value("firebase/project-id", "").toString();
}
