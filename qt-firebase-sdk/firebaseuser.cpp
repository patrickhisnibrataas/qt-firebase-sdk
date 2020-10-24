#include "firebaseuser.h"
#include <QUrl>

FirebaseUser::FirebaseUser()
{

}

FirebaseUser::FirebaseUser(const FirebaseUser &other)
{
    operator=(other);
}

FirebaseUser &FirebaseUser::operator=(const FirebaseUser &other)
{
    if (this->m_user != other.m_user) {
        this->m_user = other.m_user;
    }

    return *this;
}

QString FirebaseUser::name() const
{
    return QString::fromStdString(m_user->display_name());
}

FirebaseUser::FirebaseUser(firebase::auth::User* user)
    : m_user(user)
{

}

QString FirebaseUser::email() const
{
    return QString::fromStdString(m_user->email());
}

QString FirebaseUser::uid() const
{
    return QString::fromStdString(m_user->uid());
}

QUrl FirebaseUser::photo() const
{
    return QUrl(QString::fromStdString(m_user->photo_url()));
}

QString FirebaseUser::providerId() const
{
    return QString::fromStdString(m_user->provider_id());
}

QString FirebaseUser::phoneNumber() const
{
    return QString::fromStdString(m_user->phone_number());
}

bool FirebaseUser::isAnonymous() const
{
    return m_user->is_anonymous();
}

bool FirebaseUser::isEmailVerified() const
{
    return m_user->is_email_verified();
}

QDateTime FirebaseUser::lastSignIn() const
{
    return QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(m_user->metadata().last_sign_in_timestamp));
}

void FirebaseUser::linkWithCredentials(const FirebaseCredential &credentials)
{
    m_user->LinkWithCredential(credentials.m_credential).OnCompletion([=](const firebase::Future<firebase::auth::User*>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            m_user = *result.result();
            emit linkWithCredentialsSuccess();
        } else {
            emit linkWithCredentialsError(result.error_message());
        }
    });
}

void FirebaseUser::linkWithProvider(const QString& provider, const QStringList &scopes, const QMap<QString, QString> customParameters)
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

    m_user->LinkWithProvider(&oAuthProvider).OnCompletion([=](const firebase::Future<firebase::auth::SignInResult>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            m_user = result.result()->user;
            emit linkWithProviderSuccess();
        } else {
            emit linkWithProviderError(result.error_message());
        }
    });
}

QDateTime FirebaseUser::accountCreated() const
{
    return QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(m_user->metadata().creation_timestamp));
}

void FirebaseUser::updateDisplayName(const QString &displayName)
{
    firebase::auth::User::UserProfile profile;
    profile.display_name = displayName.toStdString().c_str();

    m_user->UpdateUserProfile(profile).OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit updateDisplayNameSuccess();
        } else {
          emit updateDisplayNameError(result.error_message());
        }
    });
}

void FirebaseUser::updatePhoto(const QUrl &url)
{
    firebase::auth::User::UserProfile profile;
    profile.photo_url = url.toString().toStdString().c_str();

    m_user->UpdateUserProfile(profile).OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit updatePhotoSuccess();
        } else {
          emit updatePhotoError(result.error_message());
        }
    });
}

void FirebaseUser::unlink(const QString &provider)
{
    m_user->Unlink(provider.toStdString().c_str()).OnCompletion([=](firebase::Future<firebase::auth::User*> result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
            m_user = *result.result();
            emit unlinkSuccess();
        } else {
            emit unlinkError(result.error_message());
        }
    });
}

void FirebaseUser::refreshUserData()
{
    m_user->Reload().OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit refreshUserDataSuccess();
        } else {
          emit refreshUserDataError(result.error_message());
        }
    });
}

void FirebaseUser::updatePassword(const QString &newPassword)
{
    m_user->UpdatePassword(newPassword.toStdString().c_str()).OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit updatePasswordSuccess();
        } else {
          emit updatePasswordError(result.error_message());
        }
    });
}

void FirebaseUser::updatePhoneNumber(const FirebaseAuth& auth, const FirebaseCredential &credentials, const QString &number)
{
    m_user->UpdatePhoneNumberCredential(credentials.m_credential).OnCompletion([=](firebase::Future<firebase::auth::User*> result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
            m_user = *result.result();
            emit updatePhoneNumberSuccess();
        } else {
            emit updatePhoneNumberError(result.error_message());
        }
    });
}

void FirebaseUser::sendEmailVerification()
{
    m_user->SendEmailVerification().OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit sendEmailVerificationSuccess();
        } else {
          emit sendEmailVerificationError(result.error_message());
        }
    });
}

void FirebaseUser::updateEmail(const QString &email)
{
    m_user->UpdateEmail(email.toStdString().c_str()).OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit updateEmailSuccess();
        } else {
          emit updateEmailError(result.error_message());
        }
    });
}

void FirebaseUser::token(bool forceRefresh)
{
    m_user->GetToken(forceRefresh).OnCompletion([=](const firebase::Future<std::string> result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit tokenReturnedSuccess(QString::fromStdString(*result.result()));
        } else {
          emit tokenReturnedError(result.error_message());
        }
    });
}

void FirebaseUser::reauthenticate(const FirebaseCredential& credential)
{
    m_user->Reauthenticate(credential.m_credential).OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit reauthenticateSuccess();
        } else {
          emit reauthenticateError(result.error_message());
        }
    });
}

void FirebaseUser::reauthenticateWithProvider(const QString &provider, const QStringList &scopes, const QMap<QString, QString> customParameters)
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

    m_user->ReauthenticateWithProvider(&oAuthProvider).OnCompletion([=](const firebase::Future<firebase::auth::SignInResult>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
            m_user = result.result()->user;
            emit reauthenticateWithProviderSuccess();
        } else {
            emit reauthenticateWithProviderError(result.error_message());
        }
    });
}

QVector<FirebaseUser::ProviderData> FirebaseUser::providerData() const
{
    QVector<ProviderData> allProviderData;
    auto data = m_user->provider_data();
    for (auto entry : data) {
        ProviderData providerData;
        providerData.uid = QString::fromStdString(entry->uid());
        providerData.displayName = QString::fromStdString(entry->display_name());
        providerData.phoneNumber = QString::fromStdString(entry->phone_number());
        providerData.photoUrl = QUrl(QString::fromStdString(entry->photo_url()));
        providerData.providerId = QString::fromStdString(entry->provider_id());
        allProviderData.append(providerData);
    }

    return allProviderData;
}

void FirebaseUser::remove()
{
    m_user->Delete().OnCompletion([=](const firebase::Future<void>& result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit removeSuccess();
        } else {
          emit removeError(result.error_message());
        }
    });
}
