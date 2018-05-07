#ifndef COREROUTER_H
#define COREROUTER_H

#include <QObject>
#include <QToolBar>

#include "MainWindow.h"
#include "ChangeSkin.h"

class CoreRouter : public QObject
{
	Q_OBJECT
public:
	static CoreRouter &GetInstance(MainWindow* main = NULL);
	~CoreRouter();
	static void MoveTo(QSize s, QWidget *w);

	void RegistMetaObject(const QString&, const QMetaObject&);

	bool ConfigFileLoader(const QString&, QVariantMap *&);
	bool ToolBarLoader(QToolBar *toolbar, const QString& fname = "");

	bool EncodeFile(const QString& out, const QString& in);
	bool DecodeFile(const QString& fname, QByteArray *&content);

	void OnSignal(int sig);

signals:
	void onsignal(int sig);

public slots:
	void onTriggerd(QAction*);
	void onUIChange(const QString&);

private:
	explicit CoreRouter(QObject *parent = nullptr);
};

#endif // COREROUTER_H
