#ifndef FIREBASECONFIG_H
#define FIREBASECONFIG_H

#include <QSettings>

class FirebaseConfig
{
public:
    FirebaseConfig();
    std::tuple<bool, QString> valid() const;
    QString appId() const;
    QString apiKey() const;
    QString projectId() const;

private:
    QSettings m_settings;
};

#endif // FIREBASECONFIG_H
