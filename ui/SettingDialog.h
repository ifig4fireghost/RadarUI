#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QSettings>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingDialog(QWidget *parent = 0);
	~SettingDialog();

signals:
	void quit();
	void set(const QString& ip, const QString& port);

private slots:
	void OnResult(bool);
	void on_btn_ok_clicked();

	void on_btn_exit_clicked();

private:
	Ui::SettingDialog *ui;
	QPropertyAnimation *pAnimation;
	QDesktopWidget *pDesktopWidget;
	QSettings setting;
};

#endif // SETTINGDIALOG_H
