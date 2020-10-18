#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>

#include "firebaseconfig.h"
#include "firebaseapp.h"
#include "firebaseauth.h"
#include "firebaseuser.h"

int main(int argc, char *argv[])
{
    // Load config.ini file with app id and api key
    FirebaseConfig firebaseConfig;
    auto [status, error] = firebaseConfig.valid();
    if (!status) {
        qDebug() << error;
        return -1;
    }

    // Create firebase app which will be consumed by all firebase services
    FirebaseApp firebaseApp(firebaseConfig.appId(), firebaseConfig.apiKey(), firebaseConfig.projectId());

    // Create firebase authentication for user and authentication purposes.
    FirebaseAuth firebaseAuth(firebaseApp);

    std::shared_ptr<FirebaseUser> firebaseUser;

    QObject::connect(&firebaseAuth, &FirebaseAuth::createUserWithEmailAndPasswordSuccess, [&](std::shared_ptr<FirebaseUser> user){
        firebaseUser = user;
        qDebug() << "Created user with email: " << user->email();

        QObject::connect(firebaseUser.get(), &FirebaseUser::removeSuccess, [](){
            qDebug() << "User successfully removed";
        });

        QObject::connect(firebaseUser.get(), &FirebaseUser::removeError, [](const QString& error){
            qDebug() << error;
        });

        firebaseUser->remove();
    });

    QObject::connect(&firebaseAuth, &FirebaseAuth::createUserWithEmailAndPasswordError, [](const QString& error){
        qDebug() << error;
    });

    firebaseAuth.createUserWithEmailAndPassword("john@doe.dead", "missingperson");

    // Qt specific code
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
