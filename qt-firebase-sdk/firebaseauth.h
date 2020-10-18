#ifndef FIREBASEAUTH_H
#define FIREBASEAUTH_H

#include <QObject>
#include "firebaseapp.h"
#include "firebase/auth.h"

class FirebaseUser;

class FirebaseAuth : public QObject
{
    Q_OBJECT
public:
    FirebaseAuth(FirebaseApp& app);
    void createUserWithEmailAndPassword(const QString& email, const QString& password);

signals:
    void createUserWithEmailAndPasswordSuccess(std::shared_ptr<FirebaseUser> user);
    void createUserWithEmailAndPasswordError(const QString& error);

private:
    firebase::auth::Auth* m_auth;   // The pointer will be deleted by the Firebase library.
};

#endif // FIREBASEAUTH_H
