#include "firebaseuser.h"

FirebaseUser::FirebaseUser()
{

}

FirebaseUser::FirebaseUser(firebase::auth::User* user)
    : m_user(user)
{

}

QString FirebaseUser::email() const
{
    return QString::fromStdString(m_user->email());
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
