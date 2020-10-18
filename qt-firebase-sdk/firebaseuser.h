#ifndef FIREBASEUSER_H
#define FIREBASEUSER_H

#include <QObject>
#include <QDateTime>
#include <QUrl>
#include "firebase/auth.h"
#include "firebaseauth.h"

class FirebaseUser : public QObject
{
    Q_OBJECT
public:
    struct ProviderData {
        QString uid;
        QString email;
        QString display_name;
        QUrl photo_url;
        QString provider_id;
        QString phone_number;
    };

    // User info
    QString name() const;
    QString email() const;
    QString uid() const;
    QUrl photo() const;
    QString providerId() const;
    QString phoneNumber() const;
    QVector<FirebaseUser::ProviderData> providerData() const;

    // User status
    bool isAnonymous() const;
    bool isEmailVerified() const;
    QDateTime lastSignIn() const;
    QDateTime accountCreated() const;

    // User operations
    void updateDisplayName(const QString& displayName);
    void updatePhoto(const QUrl& url);
    void refreshUserData();
    void updatePassword(const QString& newPassword);
    void sendEmailVerification();
    void updateEmail(const QString& email);
    void token(bool forceRefresh = false);
    void remove();

signals:
    void updateDisplayNameSuccess();
    void updateDisplayNameError(const QString& error);
    void updatePhotoSuccess();
    void updatePhotoError(const QString& error);
    void refreshUserDataSuccess();
    void refreshUserDataError(const QString& error);
    void updatePasswordSuccess();
    void updatePasswordError(const QString& error);
    void sendEmailVerificationSuccess();
    void sendEmailVerificationError(const QString& error);
    void updateEmailSuccess();
    void updateEmailError(const QString& error);
    void removeSuccess();
    void removeError(const QString& error);
    void tokenReturned(const QString& token);

private:
    friend class FirebaseAuth;
    FirebaseUser(firebase::auth::User *user);
    firebase::auth::User* m_user = nullptr; // The pointer will be deleted by the Firebase library.
};

#endif // FIREBASEUSER_H
