#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include <QObject>
#include "AppObject.h"

//Q_IMPORT_PLUGIN(QDMBPlugin)

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DataModelBuilder_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
	}
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	// Uncomment if want to use Item as a root QML object int main.qml
	//	QQuickView view;
	//	view.rootContext()->setContextProperty("vlTypeModel", dmb::ObjectPropListModel::typeList);
	//	view.engine()->addImportPath("qrc:/imports");
	//	view.setSource(url);
	//	if (!view.errors().isEmpty())
	//		return -1;
	//	view.show();

	// Load a Window from main.qml
	QQmlApplicationEngine engine;
	AppObject appObject(app);
	engine.rootContext()->setContextProperty("app", &appObject);
	engine.addImportPath("qrc:/imports");
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);
    return app.exec();
}
