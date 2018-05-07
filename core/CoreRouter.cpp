#include "CoreRouter.h"
#include "libs.h"
#include "CommonWidget.h"
#include "reflecthelper.h"

#include <QFile>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QToolButton>
#include <QMessageBox>

static MainWindow* mainwindow = NULL;

static QMap<QString, CommonWidget*> __global_map;

CoreRouter::CoreRouter(QObject *parent) : QObject(parent) {}

CoreRouter::~CoreRouter() {}

CoreRouter& CoreRouter::GetInstance(MainWindow *main)
{
	static CoreRouter core;
	if (main != NULL && mainwindow == NULL) {
		mainwindow = main;
	}

	return core;
}

void CoreRouter::OnSignal(int sig)
{
	emit onsignal(sig);
}

void CoreRouter::MoveTo(QSize s, QWidget *w)
{
	w->move((s.width() - w->width()) / 2, (s.height() - w->height()) / 2);
}

bool CoreRouter::EncodeFile(const QString &out, const QString &in)
{
	QFile fin(in);
	QFile fout(out);
	if (!fin.open(QFile::ReadOnly) || !fout.open(QFile::WriteOnly)) {
		return false;
	}
	QByteArray *bo;
	bool ret = Libs::Encode(bo, fin.readAll());
	if (ret) {
		fout.write(*bo);
		fout.flush();
		delete bo;
	}
	fin.close();
	fout.close();
	return ret;
}

bool CoreRouter::DecodeFile(const QString &fname, QByteArray*& content)
{
	QFile fin(fname);
	if (!fin.open(QFile::ReadOnly)) {
		return false;
	}
	bool ret = Libs::Decode(content, fin.readAll());
	fin.close();

	return ret;
}

bool CoreRouter::ConfigFileLoader(const QString &fname, QVariantMap *&v)
{
	QByteArray *result;
	if (!DecodeFile(fname, result)) {
		return false;
	}
	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(*result, &err);
	delete result;
	if (err.error != QJsonParseError::NoError) {
		return false;
	}
	v = new QVariantMap(doc.toVariant().toMap());
	return true;
}

bool CoreRouter::ToolBarLoader(QToolBar* toolbar, const QString &fname)
{
	QVariantMap *v;
	if (!ConfigFileLoader(fname, v)) {
		return false;
	}
	if (!v->contains("tools")) {
		delete v;
		return false;
	}

	QVariantList all = (*v)["tools"].toList();

	QActionGroup *actions = new QActionGroup(this);

	foreach (QVariant value, all) {
		QVariantMap t = value.toMap();
		QAction* a = new QAction(QIcon(t["Icon"].toString()), t["Text"].toString());
		a->setObjectName(t["ObjectName"].toString());
		a->setStatusTip(t["ToolTips"].toString());
		a->setCheckable(t["Checkable"].toBool());
		a->setData(t["Action"]);

		actions->addAction(a);
	}
	connect(actions, SIGNAL(triggered(QAction*)), this, SLOT(onTriggerd(QAction*)));
	toolbar->addActions(actions->actions());

	delete v;

	return true;
}

void CoreRouter::onTriggerd(QAction *action)
{
	QStringList act = action->data().toString().split(' ');
	if (act.length() < 2)
		return;
	if (act[0] == 'C') {
		if (!__global_map.contains(act[1])) {
			CommonWidget* w = (CommonWidget*)ObjectFactory::createObject(act[1].toLatin1());
			connect(w, SIGNAL(change(QString)), this, SLOT(onUIChange(QString)));
			__global_map.insert(act[1], w);
		}
		onUIChange(act[1]);
	} else if (act[0] == 'I') {
		QByteArray *v;
		if (!DecodeFile(act[1], v))
			return;
		QMessageBox::information(NULL, "info", QString::fromLatin1(*v));
		delete v;
	} else if (act[0] == 'D') {
		CommonDialog* w = (CommonDialog*)ObjectFactory::createObject(act[1].toLatin1());
		connect(w, SIGNAL(change(QString)), this, SLOT(onUIChange(QString)));
		mainwindow->hide();
		w->show();
	}
}

void CoreRouter::onUIChange(const QString &towho)
{
	QStringList all = towho.split(' ');
	QString to;
	if (all.length() == 2) {
		to = all[0];
		if (__global_map.contains(all[1])) {
			__global_map[all[1]]->deleteLater();
			__global_map.remove(all[1]);
		}
	} else {
		to = towho;
	}
	if (to == "MainWindow") {
		mainwindow->show();
	} else {
		mainwindow->hide();
		CommonWidget* w = __global_map[to];
		w->show();
		QSize s = mainwindow->screen;
		MoveTo(s, w);
	}
}
