#include "firebaseapp.h"

FirebaseApp::FirebaseApp(const QString &appId, const QString &apiKey, const QString& projectId)
{
    firebase::AppOptions appOptions;
    appOptions.set_app_id(appId.toStdString().c_str());
    appOptions.set_api_key(apiKey.toStdString().c_str());
    appOptions.set_project_id(projectId.toStdString().c_str());
    m_app = firebase::App::Create(appOptions, projectId.toStdString().c_str());
}
