#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ChartView.h"
#include "CoreRouter.h"
#include "libs.h"
#include "LmsDataParser.h"
#include "SettingDialog.h"

#include <signal.h>

#include <QBasicTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(this, SIGNAL(showMessage(QString,int)), ui->statusBar, SLOT(showMessage(QString,int)));
	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(OnClose()));
	connect(&CoreRouter::GetInstance(), SIGNAL(onsignal(int)), this, SLOT(OnSignal(int)));

	bool tools_loadded = CoreRouter::GetInstance().ToolBarLoader(ui->toolBar, ":/config/tool_bar.dat");
	if (!tools_loadded) {
		QMessageBox::critical(this, "Error", "Load Tools failed!");
		exit(-1);
	}
	showMessage("hi, at your service ^_^");

	process = new QProcess();
	process->setProgram("Radar");

	timer_load = new QBasicTimer;
	timer_load->start(500, this);
	wb = new UIWaiting();
	wb->show();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::OnClose()
{
	Libs::Log2Console("start cleaning");
	Libs::GetInstance().Release();
	process->close();
	Libs::Log2Console("exit");
	exit(0);
}

void MainWindow::OnSignal(int sig)
{
	if (sig == SIGINT) {
		OnClose();
	} else {
		Libs::Log2Console(QString::number(sig));
	}
}

void MainWindow::timerEvent(QTimerEvent* t)
{
	if (t->timerId() == timer_load->timerId()) {
		timer_load->stop();
		this->hide();
		if (process->state() == QProcess::NotRunning) {
			process->start();
			process->waitForStarted();
			timer_load->start(1000, this);
		} else {
			if (!Libs::GetInstance().Initialize()) {
				int ret = QMessageBox::critical(wb, "Error", "Internal Wrong! Try Again?",
									  QMessageBox::Ok | QMessageBox::Default, QMessageBox::Cancel);
				switch (ret) {
				case QMessageBox::Ok:
				{
					timer_load->start(1000, this);
					break;
				}
				case QMessageBox::Cancel:
				default:
					exit(-2);
					break;
				}
			} else {
				connect(&Libs::GetInstance(), SIGNAL(dataready(QByteArray)), this, SLOT(OnReadData(QByteArray)));
				connect(this, SIGNAL(write(QString)), &Libs::GetInstance(), SLOT(OnWrite(QString)));

				SettingDialog* sd = new SettingDialog();
				connect(sd, SIGNAL(quit()), this, SLOT(OnClose()));
				connect(sd, SIGNAL(set(QString,QString)), this, SLOT(OnSet(QString,QString)));
				connect(this, SIGNAL(dataready(bool)), sd, SLOT(OnResult(bool)));
				sd->show();
				wb->close();
			}
		}
	}
	else if (t->timerId() == timer_data->timerId()) {
		emit write("&C=\x02sRN LMDscandata\x03");
	}
}

void MainWindow::OnSet(const QString &ip, const QString &port)
{
	emit write(QString("&C=CT=%1:%2").arg(ip).arg(port));
}

void MainWindow::OnReadData(const QByteArray& raw)
{
	if (raw.length() == 2) {
		QString dat(raw);
		Libs::Log2Console(dat);
		if (dat == "CF" || dat == "PF") {
			Libs::Log2Console("Connect to server failed");
			emit dataready(false);
		} else if (dat == "CD" || dat == "CA") {
			Libs::Log2Console("Connected to server");
			emit dataready(true);

			ChartView* cv = new ChartView(this);
			cv->setFixedSize(QSize(960, 640));
			ui->model_view->addWidget(cv);
			connect(this, SIGNAL(updatepoints(const QList<QPointF>*)), cv, SLOT(OnUpdatePoints(const QList<QPointF>*)));
			this->show();
			timer_data = new QBasicTimer;
			timer_data->start(1000, this);
		} else if (dat == "NC") {
			Libs::Log2Console("Not connected to server");
		} else if (dat == "NA") {
			Libs::Log2Console("No reply");
		}
	} else {
		LmsDataParser::GetInstace().Parse(raw);
		emit updatepoints(LmsDataParser::GetInstace().points);
	}
}
