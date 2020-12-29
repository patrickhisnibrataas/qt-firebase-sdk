#ifndef FIREBASEMESSAGING_H
#define FIREBASEMESSAGING_H

#include <QObject>
#include "firebase/auth.h"
#include "firebaseapp.h"

class FirebaseMessaging : public QObject
{
    Q_OBJECT
public:
    FirebaseMessaging(FirebaseApp& app);

private:
    firebase::App* m_app;   // The pointer will be deleted by the Firebase library.
};

#endif // FIREBASEMESSAGING_H
