#include "SettingDialog.h"
#include "ui_SettingDialog.h"

#include <QRegExpValidator>

SettingDialog::SettingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingDialog)
{
	ui->setupUi(this);
	ui->lb_hint->setVisible(false);
	QRegExpValidator* ip = new QRegExpValidator;
	ip->setRegExp(QRegExp("[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}"));
	ui->le_ip->setValidator(ip);

	QRegExpValidator* port = new QRegExpValidator;
	port->setRegExp(QRegExp("[1-9]{4}"));
	ui->le_port->setValidator(port);

	pAnimation = new QPropertyAnimation(this, "geometry");
	pDesktopWidget = qApp->desktop();

	setting.beginGroup("SettingDialog");
	ui->le_ip->setText(setting.value("ip", "192.168.1.20").toString());
	ui->le_port->setText(setting.value("port", "2111").toString());
	setting.endGroup();
}

SettingDialog::~SettingDialog()
{
	delete ui;
}

void SettingDialog::on_btn_ok_clicked()
{
	QString ip = ui->le_ip->text();
	QString port = ui->le_port->text();

	ui->btn_ok->setEnabled(false);
	ui->le_ip->setEnabled(false);
	ui->le_port->setEnabled(false);
	ui->lb_hint->setVisible(false);
	setting.beginGroup("SettingDialog");
	setting.setValue("ip", ip);
	setting.setValue("port", port);
	setting.endGroup();

	emit set(ip, port);
}

void SettingDialog::OnResult(bool ok)
{
	if (ok) {
		this->close();
	} else {
		ui->lb_hint->setVisible(true);
		ui->le_ip->setEnabled(true);
		ui->le_port->setEnabled(true);
		ui->btn_ok->setEnabled(true);

		int x = (pDesktopWidget->availableGeometry().width() - width()) / 2;
		int y = (pDesktopWidget->availableGeometry().height() - height()) / 2;

		pAnimation->setDuration(1000);
		pAnimation->setStartValue(QRect(x, 0, width(), height()));
		pAnimation->setEndValue(QRect(x, y, width(), height()));
		pAnimation->setEasingCurve(QEasingCurve::OutElastic);
		pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
}

void SettingDialog::on_btn_exit_clicked()
{
	emit quit();
}
