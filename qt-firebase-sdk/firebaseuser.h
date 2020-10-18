#ifndef FIREBASEUSER_H
#define FIREBASEUSER_H

#include <QObject>
#include <memory>
#include "firebase/auth.h"
#include "firebaseauth.h"

class FirebaseUser : public QObject
{
    Q_OBJECT
public:
    FirebaseUser();
    QString email() const;
    void remove();

signals:
    void removeSuccess();
    void removeError(const QString& error);

private:
    friend class FirebaseAuth;
    FirebaseUser(firebase::auth::User *user);
    firebase::auth::User* m_user = nullptr; // The pointer will be deleted by the Firebase library.
};

#endif // FIREBASEUSER_H
