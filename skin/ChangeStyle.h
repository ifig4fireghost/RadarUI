#ifndef CHANGESTYLE_H
#define CHANGESTYLE_H

#include <QApplication>
#include <QFile>

#include "libs.h"

class ChangeStyle {
public:
	static QString& CurrentStyle()
	{
		static QString Cur;
		return Cur;
	}
	static bool setStyle(const QString& qss)
	{
		QFile file(qss);
		if (!file.open(QFile::ReadOnly)) {
			return false;
		}
		qApp->setStyleSheet(file.readAll());
		file.close();
		CurrentStyle() = qss;
		Libs::Log2Console("Skin changed");
		return true;
	}
};

#endif // CHANGESTYLE_H
