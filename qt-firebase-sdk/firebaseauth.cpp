#include "firebaseauth.h"
#include "firebase/auth.h"
#include "firebaseuser.h"

FirebaseAuth::FirebaseAuth(FirebaseApp& app)
    : m_auth(firebase::auth::Auth::GetAuth(app.m_app))
{

}

void FirebaseAuth::createUserWithEmailAndPassword(const QString &email, const QString &password)
{
    auto result = m_auth->CreateUserWithEmailAndPassword(email.toStdString().c_str(), password.toStdString().c_str());
    result.OnCompletion([=](const firebase::Future<firebase::auth::User*>& result) {
        if (result.error() == firebase::auth::kAuthErrorNone) {
            auto user = std::shared_ptr<FirebaseUser>(new FirebaseUser(*result.result()));
            emit createUserWithEmailAndPasswordSuccess(user);
        } else {
            emit createUserWithEmailAndPasswordError(result.error_message());
        }
    });
}
