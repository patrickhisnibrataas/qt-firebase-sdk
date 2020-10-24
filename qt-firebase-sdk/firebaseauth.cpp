#include "firebaseauth.h"
#include "firebase/auth.h"
#include "firebaseuser.h"

class AuthStateListner : public QObject, public firebase::auth::AuthStateListener
{
    Q_OBJECT
public:
    void OnAuthStateChanged(firebase::auth::Auth* auth) override {
        auto user = auth->current_user();
        if (!user) {
            emit userSignedOut();
        } else {
            emit userSignedIn(user);
        }
    }

signals:
    void userSignedIn(firebase::auth::User* user);
    void userSignedOut();
};

static AuthStateListner authStateListner;

class IdTokenListner : public QObject, public firebase::auth::IdTokenListener
{
    Q_OBJECT
public:
    void OnIdTokenChanged(firebase::auth::Auth *auth) override {
        auth->current_user()->GetToken(true).OnCompletion([&](firebase::Future<std::string> result){
            if (result.error() == firebase::auth::kAuthErrorNone) {
                emit onTokenChangedSuccess(QString::fromStdString(*result.result()));
            } else {
                emit onTokenChangedError(result.error_message());
            }
        });
    }

signals:
    void onTokenChangedSuccess(const QString& token);
    void onTokenChangedError(const QString& error);
};

static IdTokenListner idTokenListner;

class PhoneListener : public QObject, public firebase::auth::PhoneAuthProvider::Listener {
    Q_OBJECT
public:
    void OnVerificationCompleted(firebase::auth::Credential credential) override {
        emit verificationSuccess(credential);
    }

    void OnVerificationFailed(const std::string& error) override {
        emit verificationError(QString::fromStdString(error));
    }

    void OnCodeSent(const std::string& verification_id, const firebase::auth::PhoneAuthProvider::ForceResendingToken& force_resending_token) override {
        Q_UNUSED(verification_id)
        Q_UNUSED(force_resending_token)
        emit codeSent(QString::fromStdString(verification_id));
    }

signals:
    void verificationSuccess(firebase::auth::Credential credential);
    void verificationError(const QString& error);
    void codeSent(const QString& verificationId);
};

static PhoneListener phoneListener;

FirebaseAuth::FirebaseAuth(FirebaseApp& app)
    : m_auth(firebase::auth::Auth::GetAuth(app.m_app))
{
    m_auth->AddAuthStateListener(&authStateListner);
    connect(&authStateListner, &AuthStateListner::userSignedOut, this, &FirebaseAuth::userSignedOut);
    connect(&authStateListner, &AuthStateListner::userSignedIn, this, [=](firebase::auth::User* user){
        emit userSignedIn(FirebaseUser(user));
    });

    m_auth->AddIdTokenListener(&idTokenListner);
    connect(&idTokenListner, &IdTokenListner::onTokenChangedError, this, &FirebaseAuth::tokenChangedError);
    connect(&idTokenListner, &IdTokenListner::onTokenChangedSuccess, this, &FirebaseAuth::tokenChangedSuccess);

    connect(&phoneListener, &PhoneListener::verificationError, this, &FirebaseAuth::phoneVerificationError);
    connect(&phoneListener, &PhoneListener::codeSent, this, &FirebaseAuth::phoneCodeSent);
}

FirebaseUser FirebaseAuth::user()
{
    return FirebaseUser(m_auth->current_user());
}

void FirebaseAuth::verifyPhoneNumber(const QString& phoneNumber)
{
    firebase::auth::PhoneAuthProvider::GetInstance(m_auth).VerifyPhoneNumber(phoneNumber.toStdString().c_str(), 2000, nullptr, &phoneListener);
}

void FirebaseAuth::providersForEmail(const QString &email)
{
    m_auth->FetchProvidersForEmail(email.toStdString().c_str()).OnCompletion([=](const firebase::Future<firebase::auth::Auth::FetchProvidersResult> result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            auto providers = result.result()->providers;

            QStringList providerList;
            for (auto provider : providers) {
                providerList.append(QString::fromStdString(provider));
            }

            emit providersForEmailSuccess(providerList);
        } else {
            emit providersForEmailError(result.error_message());
        }
    });
}

void FirebaseAuth::signInAnonymously()
{
    m_auth->SignInAnonymously().OnCompletion([=](firebase::Future<firebase::auth::User*> result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            emit signInAnonymouslySuccess(FirebaseUser(*result.result()));
        } else {
            emit signInAnonymouslyError(result.error_message());
        }
    });
}

void FirebaseAuth::sendPasswordResetEmail(const QString &email)
{
    m_auth->SendPasswordResetEmail(email.toStdString().c_str()).OnCompletion([=](firebase::Future<void> result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            emit sendPasswordResetEmailSuccess();
        } else {
            emit sendPasswordResetEmailError(result.error_message());
        }
    });
}

void FirebaseAuth::signOut()
{
    m_auth->SignOut();
}

void FirebaseAuth::createUserWithEmailAndPassword(const QString &email, const QString &password)
{
    m_auth->CreateUserWithEmailAndPassword(email.toStdString().c_str(), password.toStdString().c_str()).OnCompletion([=](firebase::Future<firebase::auth::User*> result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
            emit createUserWithEmailAndPasswordSuccess(FirebaseUser(*result.result()));
        } else {
            emit createUserWithEmailAndPasswordError(result.error_message());
        }
    });
}

void FirebaseAuth::signInWithCredential(const FirebaseCredential &credential)
{
    m_auth->SignInWithCredential(credential.m_credential).OnCompletion([=](const firebase::Future<firebase::auth::User*>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            emit signInWithCredentialSuccess(FirebaseUser(*result.result()));
        } else {
            emit signInWithCredentialError(result.error_message());
        }
    });
}

void FirebaseAuth::signInWithCustomToken(const QString &token)
{
    m_auth->SignInWithCustomToken(token.toStdString().c_str()).OnCompletion([=](firebase::Future<firebase::auth::User*> result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
            emit signInWithCustomTokenSuccess(FirebaseUser(*result.result()));
        } else {
            emit signInWithCustomTokenError(result.error_message());
        }
    });
}

void FirebaseAuth::signInWithEmailAndPassword(const QString &email, const QString &password)
{
    m_auth->SignInWithEmailAndPassword(email.toStdString().c_str(), password.toStdString().c_str()).OnCompletion([=](firebase::Future<firebase::auth::User*> result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
            emit signInWithEmailAndPasswordSuccess(FirebaseUser(*result.result()));
        } else {
            emit signInWithEmailAndPasswordError(result.error_message());
        }
    });
}

void FirebaseAuth::signInWithProvider(const QString& provider, const QStringList &scopes, const QMap<QString, QString> customParameters)
{
    std::vector<std::string> scopesVector;
    for (auto scope : scopes)
        scopesVector.push_back(scope.toStdString());

    std::map<std::string, std::string> customParametersMap;
    const auto it = customParameters.constBegin();
    while (it != customParameters.constEnd())
        customParametersMap.insert({it.key().toStdString(), it.value().toStdString()});

    const auto providerData = firebase::auth::FederatedOAuthProviderData(provider.toStdString().c_str(), scopesVector, customParametersMap);

    firebase::auth::FederatedOAuthProvider oAuthProvider;
    oAuthProvider.SetProviderData(providerData);

    m_auth->SignInWithProvider(&oAuthProvider).OnCompletion([=](const firebase::Future<firebase::auth::SignInResult>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            emit signInWithProviderSuccess(FirebaseUser(result.result()->user));
        } else {
            emit signInWithProviderError(result.error_message());
        }
    });
}

#include "firebaseauth.moc"

FirebaseCredential::FirebaseCredential()
{
    connect(&phoneListener, &PhoneListener::verificationSuccess, this, [=](firebase::auth::Credential credential){
        m_credential = credential;
    });
}

QString FirebaseCredential::provider() const {
    return QString::fromStdString(m_credential.provider());
}

bool FirebaseCredential::isValid() const {
    return m_credential.is_valid();
}

void FirebaseCredential::emailPassword(const QString &email, const QString &password)
{
    auto credential = firebase::auth::EmailAuthProvider::GetCredential(email.toStdString().c_str(), password.toStdString().c_str());
    if (!credential.is_valid()) {
        emit emailPasswordInvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::facebook(const QString &accessToken)
{
    auto credential = firebase::auth::FacebookAuthProvider::GetCredential(accessToken.toStdString().c_str());
    if (!credential.is_valid()) {
        emit facebookInvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::gameCenter()
{
    if (!firebase::auth::GameCenterAuthProvider::IsPlayerAuthenticated()) {
        emit gameCenterNotSignedIn();
        return;
    }

    firebase::auth::GameCenterAuthProvider::GetCredential().OnCompletion([=](const firebase::Future<firebase::auth::Credential>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            m_credential = *result.result();
            emit gameCenterSuccess();
        } else {
            emit gameCenterError(result.error_message());
        }
    });
}

void FirebaseCredential::gitHub(const QString &token)
{
    auto credential = firebase::auth::GitHubAuthProvider::GetCredential(token.toStdString().c_str());
    if (!credential.is_valid()) {
        emit githubInvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::google(const QString &idToken, const QString &accessToken)
{
    auto credential = firebase::auth::GoogleAuthProvider::GetCredential(idToken.toStdString().c_str(), accessToken.toStdString().c_str());
    if (!credential.is_valid()) {
        emit googleInvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::oAuth2(const QString &providerId, const QString &idToken, const QString &accessToken, const QString &rawNonce)
{
    firebase::auth::Credential credential;

    if (rawNonce.isEmpty())
        credential = firebase::auth::OAuthProvider::GetCredential(providerId.toStdString().c_str(), idToken.toStdString().c_str(), accessToken.toStdString().c_str());
    else
        credential = firebase::auth::OAuthProvider::GetCredential(providerId.toStdString().c_str(), idToken.toStdString().c_str(), rawNonce.toStdString().c_str(), accessToken.toStdString().c_str());

    if (!credential.is_valid()) {
        emit oAuth2InvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::phone(const FirebaseAuth& auth, const QString& verificationId, const QString& verificationCode)
{
    auto credential = firebase::auth::PhoneAuthProvider::GetInstance(auth.m_auth).GetCredential(verificationId.toStdString().c_str(), verificationCode.toStdString().c_str());
    if (!credential.is_valid()) {
        emit phoneInvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::playGames(const QString &serverAuthCode)
{
    auto credential = firebase::auth::PlayGamesAuthProvider::GetCredential(serverAuthCode.toStdString().c_str());
    if (!credential.is_valid()) {
        emit playGamesInvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::twitter(const QString &token, const QString &secret)
{
    auto credential = firebase::auth::TwitterAuthProvider::GetCredential(token.toStdString().c_str(), secret.toStdString().c_str());
    if (!credential.is_valid()) {
        emit twitterInvalidValues();
        return;
    }

    m_credential = credential;
}

void FirebaseCredential::yahoo()
{
    //TODO:
}
