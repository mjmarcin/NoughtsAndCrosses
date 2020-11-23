#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Controller/controller.h"
#include "Engine/engine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine qmlEngine;

    Engine gameEngine;
    Controller gameController(gameEngine);


    // Make Enums namespace available in QML views.
    qmlRegisterUncreatableMetaObject(
      Enums::staticMetaObject,  // static meta object
      "enums",                  // import statement
      1, 0,                     // major and minor version of the import
      "Enums",                  // name in QML
      "Error: only enums"       // error in case of attempt to create a Enums object
    );

    qmlEngine.rootContext()->setContextProperty("controller", &gameController);

    qmlEngine.load(QUrl(QStringLiteral("qrc:/View/main.qml")));
    if (qmlEngine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
