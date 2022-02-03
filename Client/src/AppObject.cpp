#include <QFileInfo>
#include <QUrl>
#include <QDir>
#include "AppObject.h"

void AppObject::refresh()
{
	qDebug() << "AppObject.refresh called";
	mApp.processEvents();
}

QString AppObject::nameFromUrl(const QString &s) const
{
	QUrl u(s);
	return u.fileName();
}

QString AppObject::relPathFromUrl(const QString &s) const
{
	QDir currentDir = QDir::currentPath();
	QUrl u(s);
	auto fPath = u.isRelative() ? s.toStdString() : u.toLocalFile().toStdString();
	auto result = currentDir.relativeFilePath(fPath.c_str());
	return result;
}
