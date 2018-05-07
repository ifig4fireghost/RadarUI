#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <QCloseEvent>
#include <QProcess>

#include "UIWaiting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	QSize screen;

signals:
	void showMessage(const QString&, int timeout = 0);
	void dataready(bool);
	void write(const QString& dat);
	void updatepoints(const QList<QPointF>* points);

private slots:
	void timerEvent(QTimerEvent *t);
	void OnSet(const QString& ip, const QString& port);
	void OnReadData(const QByteArray& raw);
	void OnClose();
	void OnSignal(int sig);

private:
	Ui::MainWindow *ui;
	QBasicTimer* timer_load;
	QBasicTimer* timer_data;
	UIWaiting* wb;
	QProcess* process;
	void load_tool_bar();
};

#endif // MAINWINDOW_H
