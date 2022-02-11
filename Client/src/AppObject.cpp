#include <QFileInfo>
#include <QUrl>
#include <QDir>
#include "AppObject.h"
#include "Utils.h"

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

QString AppObject::relPath(const QString &s) const
{
	QUrl u(s);
	QString fPath;
	QDir currentDir = QDir::currentPath();
	if (u.scheme() == "file")
	{
		if (u.isRelative())
			fPath = s;
		else
			fPath = u.toLocalFile();
	}
	else
	{
		fPath = s;
	}
	auto result = currentDir.relativeFilePath(fPath);
	return result;
}

QString AppObject::fullPathUrl(const QString &s) const
{
	QFileInfo f(s);
	return Utils::FormatStr("file:///%s", f.path().toStdString().c_str()).c_str();
}
