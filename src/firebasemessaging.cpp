#include "firebasemessaging.h"
#include "firebase/messaging.h"

#include <QDebug>

class MessagingListener : public QObject, public firebase::messaging::Listener {
    Q_OBJECT
public:
    virtual ~MessagingListener();

    /// Called on the client when a message arrives.
    ///
    /// @param[in] message The data describing this message.
    virtual void OnMessage(const firebase::messaging::Message& message) {
        qDebug() << "Message received from: " << QString::fromStdString(message.from);
    }

    /// Called on the client when a registration token arrives. This function
    /// will eventually be called in response to a call to
    /// firebase::messaging::Initialize(...).
    ///
    /// @param[in] token The registration token.
    virtual void OnTokenReceived(const char* token) {
        qDebug() << "Token received: " << QString::fromLatin1(token);
    }

};

static MessagingListener messagingListener;

FirebaseMessaging::FirebaseMessaging(FirebaseApp &app)
    : m_app(app.m_app)
{
    firebase::messaging::Initialize(*m_app, &messagingListener);
}

#include "firebasemessaging.moc"
