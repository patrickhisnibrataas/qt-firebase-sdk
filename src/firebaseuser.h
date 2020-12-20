#ifndef FIREBASEUSER_H
#define FIREBASEUSER_H

#include <QObject>
#include <QDateTime>
#include <QUrl>
#include "firebase/auth.h"
#include "firebaseauth.h"

class FirebaseCredential;

class FirebaseUser : public QObject
{
    Q_OBJECT
public:
    struct ProviderData {
        QString displayName;
        QString email;
        QString phoneNumber;
        QUrl photoUrl;
        QString providerId;
        QString uid;
    };

    // Constructors
    FirebaseUser();
    FirebaseUser(const FirebaseUser& other);
    FirebaseUser& operator=(const FirebaseUser& other);

    // User info
    QString email() const;
    QString name() const;
    QString phoneNumber() const;
    QUrl photo() const;
    QVector<FirebaseUser::ProviderData> providerData() const;
    QString providerId() const;
    QString uid() const;

    // User status
    bool isAnonymous() const;
    bool isEmailVerified() const;
    QDateTime accountCreated() const;
    QDateTime lastSignIn() const;

    // User operations
    void linkWithCredentials(const FirebaseCredential& credentials);
    void linkWithProvider(const QString& provider, const QStringList &scopes, const QMap<QString, QString> customParameters);
    void reauthenticate(const FirebaseCredential &credential);
    void reauthenticateWithProvider(const QString& provider, const QStringList &scopes, const QMap<QString, QString> customParameters);
    void refreshUserData();
    void sendEmailVerification();
    void token(bool forceRefresh = false);
    void updateDisplayName(const QString& displayName);
    void updatePhoto(const QUrl& url);
    void unlink(const QString& provider);

    // User operations requiring reauthentication
    void remove();
    void updateEmail(const QString& email);
    void updatePassword(const QString& newPassword);
    void updatePhoneNumber(const FirebaseAuth &auth, const FirebaseCredential& credentials, const QString& number);

signals:
    void linkWithCredentialsSuccess();
    void linkWithCredentialsError(const QString& error);
    void linkWithProviderSuccess();
    void linkWithProviderError(const QString& error);
    void reauthenticateSuccess();
    void reauthenticateError(const QString& error);
    void reauthenticateWithProviderSuccess();
    void reauthenticateWithProviderError(const QString& error);
    void refreshUserDataSuccess();
    void refreshUserDataError(const QString& error);
    void removeSuccess();
    void removeError(const QString& error);
    void sendEmailVerificationSuccess();
    void sendEmailVerificationError(const QString& error);
    void tokenReturnedSuccess(const QString& token);
    void tokenReturnedError(const QString& error);
    void unlinkSuccess();
    void unlinkError(const QString& error);
    void updateDisplayNameSuccess();
    void updateDisplayNameError(const QString& error);
    void updateEmailSuccess();
    void updateEmailError(const QString& error);
    void updatePasswordSuccess();
    void updatePasswordError(const QString& error);
    void updatePhoneNumberSuccess();
    void updatePhoneNumberError(const QString& error);
    void updatePhotoSuccess();
    void updatePhotoError(const QString& error);

private:
    friend class FirebaseAuth;
    FirebaseUser(firebase::auth::User *user);
    firebase::auth::User* m_user = nullptr; // The pointer will be deleted by the Firebase library.
};

Q_DECLARE_METATYPE(FirebaseUser)

#endif // FIREBASEUSER_H
