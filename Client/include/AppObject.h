#ifndef APPOBJECT_H
#define APPOBJECT_H

#include <QObject>
#include <QGuiApplication>

class AppObject: public QObject {
	Q_OBJECT

public:
	AppObject(QGuiApplication& app) : mApp(app) {}

public:
	// Properties
	Q_INVOKABLE void refresh();
	Q_INVOKABLE QString nameFromUrl(const QString& s) const;
	Q_INVOKABLE QString relPathFromUrl(const QString& s) const;

protected:
	QGuiApplication& mApp;
};

#endif // APPOBJECT_H
