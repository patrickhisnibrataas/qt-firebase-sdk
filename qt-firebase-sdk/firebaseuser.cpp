#include "firebaseuser.h"
#include <QUrl>

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

QDateTime FirebaseUser::accountCreated() const
{
    return QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(m_user->metadata().creation_timestamp));
}

void FirebaseUser::updateDisplayName(const QString &displayName)
{
    firebase::auth::User::UserProfile profile;
    profile.display_name = displayName.toStdString().c_str();

    auto result = m_user->UpdateUserProfile(profile);
    result.OnCompletion([=](const firebase::Future<void>& result){
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

    auto result = m_user->UpdateUserProfile(profile);
    result.OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit updatePhotoSuccess();
        } else {
          emit updatePhotoError(result.error_message());
        }
    });
}

void FirebaseUser::refreshUserData()
{
    auto result = m_user->Reload();
    result.OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit refreshUserDataSuccess();
        } else {
          emit refreshUserDataError(result.error_message());
        }
    });
}

void FirebaseUser::updatePassword(const QString &newPassword)
{
    auto result = m_user->UpdatePassword(newPassword.toStdString().c_str());
    result.OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit updateEmailSuccess();
        } else {
          emit updatePasswordError(result.error_message());
        }
    });
}

void FirebaseUser::sendEmailVerification()
{
    auto result = m_user->SendEmailVerification();
    result.OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit sendEmailVerificationSuccess();
        } else {
          emit sendEmailVerificationError(result.error_message());
        }
    });
}

void FirebaseUser::updateEmail(const QString &email)
{
    auto result = m_user->UpdateEmail(email.toStdString().c_str());
    result.OnCompletion([=](const firebase::Future<void>& result){
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit updateEmailSuccess();
        } else {
          emit updateEmailError(result.error_message());
        }
    });
}

void FirebaseUser::token(bool forceRefresh)
{
    auto result = m_user->GetToken(forceRefresh);
    result.OnCompletion([=](const firebase::Future<std::string> result) {
        emit tokenReturned(QString::fromStdString(*result.result()));
    });
}

QVector<FirebaseUser::ProviderData> FirebaseUser::providerData() const
{
    QVector<ProviderData> allProviderData;
    auto data = m_user->provider_data();
    for (auto entry : data) {
        ProviderData providerData;
        providerData.uid = QString::fromStdString(entry->uid());
        providerData.display_name = QString::fromStdString(entry->display_name());
        providerData.phone_number = QString::fromStdString(entry->phone_number());
        providerData.photo_url = QUrl(QString::fromStdString(entry->photo_url()));
        providerData.provider_id = QString::fromStdString(entry->provider_id());
        allProviderData.append(providerData);
    }

    return allProviderData;
}

void FirebaseUser::remove()
{
    auto result = m_user->Delete();
    result.OnCompletion([=](const firebase::Future<void>& result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
          emit removeSuccess();
        } else {
          emit removeError(result.error_message());
        }
    });
}
