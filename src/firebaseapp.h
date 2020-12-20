#ifndef FIREBASEAPP_H
#define FIREBASEAPP_H

#include <QObject>
#include "firebase/app.h"

class FirebaseApp : public QObject
{
    Q_OBJECT
public:
    FirebaseApp(const QString& appId, const QString& apiKey, const QString& projectId);

private:
    friend class FirebaseAuth;
    firebase::App* m_app; // The pointer will be deleted by the Firebase library.
};

#endif // FIREBASEAPP_H
