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

FirebaseAuth::FirebaseAuth(FirebaseApp& app)
    : m_auth(firebase::auth::Auth::GetAuth(app.m_app))
{
    m_auth->AddAuthStateListener(&authStateListner);
    connect(&authStateListner, &AuthStateListner::userSignedOut, this, &FirebaseAuth::userSignedOut);
    connect(&authStateListner, &AuthStateListner::userSignedIn, this, [=](firebase::auth::User* user){
        emit userSignedIn(std::shared_ptr<FirebaseUser>(new FirebaseUser(user)));
    });
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

#include "firebaseauth.moc"
