#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include <QObject>
#include "VLObjectVarModel.h"
#include "VLListModelInterface.h"
#include "VLListModel.h"
#include "ObjectProperty.h"
#include "DMBModel.h"
#include "AppObject.h"
#include "DMBUtils.h"

static QObject *example_qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine)
	Q_UNUSED(scriptEngine)
	return &dmb::Utils::Instance();
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
//	app.processEvents();
//	for (auto w : app.topLevelWindows())
//		w->re
	dmb::emptyObjectModel->Init(&app); // Set parent to
	qmlRegisterType<dmb::DMBModel>("DMB", 1, 0, "DMBModel");
	qmlRegisterType<dmb::ObjectProperty>("DMB", 1, 0, "ObjectProperty");
	//qmlRegisterUncreatableType<dmb::Utils>("DMB", 1, 0, "DMB", "Global help functions bundle");
	qmlRegisterSingletonType<dmb::Utils>("DMB", 1, 0, "DMB", example_qobject_singletontype_provider);
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
	engine.rootContext()->setContextProperty("vlTypeModel", dmb::ObjectProperty::typeList);
	engine.rootContext()->setContextProperty("vlTypeModelDetailed", dmb::ObjectProperty::typeMap);
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
