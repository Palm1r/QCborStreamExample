#include "deviceManager.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "rootController.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    qmlRegisterSingletonType<DeviceManager>("Palm1r.networkProject.DeviceManager",
                                            1,
                                            0,
                                            "DeviceManager",
                                            [](QQmlEngine *, QJSEngine *) {
                                                DeviceManager *deviceManager = new DeviceManager();
                                                return deviceManager;
                                            });
    qmlRegisterSingletonType<RootController>("Palm1r.networkProject.RootController",
                                             1,
                                             0,
                                             "RootController",
                                             [](QQmlEngine *, QJSEngine *) {
                                                 auto rootController = new RootController();
                                                 return rootController;
                                             });
    engine.load(url);

    return app.exec();
}
