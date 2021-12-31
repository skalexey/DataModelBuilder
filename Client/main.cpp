#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include "src/objectlistmodel.h"
#include "src/object.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    qmlRegisterType<ObjectListModel>("ObjectModel", 1, 0, "ObjectListModel");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DataModelBuilder_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    QQuickView view;
    view.rootContext()->setContextProperty("paramTypeModel", ObjectParameter::typeList);
    view.engine()->addImportPath("qrc:/imports");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    view.setSource(url);
    if (!view.errors().isEmpty())
        return -1;
    view.show();
    return app.exec();
}
