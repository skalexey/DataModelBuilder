#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include "TypeListModel.h"
#include "TypeModel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    qmlRegisterType<TypeListModel>("TypeModel", 1, 0, "TypeListModel");
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
    view.rootContext()->setContextProperty("propTypeModel", TypePropListModel::typeList);
    view.engine()->addImportPath("qrc:/imports");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    view.setSource(url);
    if (!view.errors().isEmpty())
        return -1;
    view.show();
    return app.exec();
}
