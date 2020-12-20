#ifndef FIREBASEAUTH_H
#define FIREBASEAUTH_H

#include <QObject>
#include <QMap>
#include "firebaseapp.h"
#include "firebase/auth.h"
#include "firebaseuser.h"

class FirebaseUser;

class FirebaseCredential : public QObject
{
    Q_OBJECT
public:
    FirebaseCredential();
    QString provider() const;
    bool isValid() const;
    void emailPassword(const QString& email, const QString& password);
    void facebook(const QString& accessToken);
    void gameCenter();
    void gitHub(const QString& token);
    void google(const QString& idToken, const QString& accessToken);
    void oAuth2(const QString& providerId, const QString& idToken, const QString& accessToken, const QString& rawNonce = {});
    void phone(const FirebaseAuth &auth, const QString& verificationId, const QString& verificationCode);
    void playGames(const QString& serverAuthCode);
    void twitter(const QString& token, const QString& secret);
    void yahoo();

signals:
    void emailPasswordInvalidValues();
    void facebookInvalidValues();
    void gameCenterNotSignedIn();
    void gameCenterSuccess();
    void gameCenterError(const QString& error);
    void githubInvalidValues();
    void googleInvalidValues();
    void oAuth2InvalidValues();
    void phoneInvalidValues();
    void playGamesInvalidValues();
    void twitterInvalidValues();

private:
    friend class FirebaseAuth;
    friend class FirebaseUser;
    firebase::auth::Credential m_credential;
};

class FirebaseAuth : public QObject
{
    Q_OBJECT
public:
    FirebaseAuth(FirebaseApp& app);
    void createUserWithEmailAndPassword(const QString& email, const QString& password);
    void providersForEmail(const QString& email);
    void sendPasswordResetEmail(const QString& email);
    void signInAnonymously();
    void signInWithCredential(const FirebaseCredential& credential);
    void signInWithCustomToken(const QString& token);
    void signInWithEmailAndPassword(const QString& email, const QString& password);
    void signInWithProvider(const QString &provider, const QStringList& scopes, const QMap<QString,QString> customParameters);
    void signOut();
    FirebaseUser user();
    void verifyPhoneNumber(const QString &phoneNumber);

signals:    
    void createUserWithEmailAndPasswordSuccess(FirebaseUser user);
    void createUserWithEmailAndPasswordError(const QString& error);
    void phoneVerificationSuccess();
    void phoneVerificationError(const QString& error);
    void phoneCodeSent(const QString& verificationId);
    void providersForEmailSuccess(const QStringList& providers);
    void providersForEmailError(const QString& error);
    void sendPasswordResetEmailSuccess();
    void sendPasswordResetEmailError(const QString& error);
    void signInAnonymouslySuccess(FirebaseUser user);
    void signInAnonymouslyError(const QString& error);
    void signInWithCredentialSuccess(FirebaseUser user);
    void signInWithCredentialError(const QString& error);
    void signInWithCustomTokenSuccess(FirebaseUser user);
    void signInWithCustomTokenError(const QString& error);
    void signInWithEmailAndPasswordSuccess(FirebaseUser user);
    void signInWithEmailAndPasswordError(const QString& error);
    void signInWithProviderSuccess(FirebaseUser user);
    void signInWithProviderError(const QString& error);
    void tokenChangedSuccess(const QString& token);
    void tokenChangedError(const QString& error);
    void userSignedOut();
    void userSignedIn(FirebaseUser user);

private:
    friend class FirebaseCredential;
    firebase::auth::Auth* m_auth;   // The pointer will be deleted by the Firebase library.
};

#endif // FIREBASEAUTH_H
