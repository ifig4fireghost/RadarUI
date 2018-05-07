#include "ChangeSkin.h"
#include "ui_ChangeSkin.h"

ChangeSkin::ChangeSkin() :
	ui(new Ui::ChangeSkin)
{
	ui->setupUi(this);
	cur = ChangeStyle::CurrentStyle();
}

ChangeSkin::~ChangeSkin()
{
	delete ui;
}

void ChangeSkin::on_buttonBox_accepted()
{
	cur = tmp;
	emit change("MainWindow");
}

void ChangeSkin::on_cb_d_clicked()
{
	tmp = ":qss/all.qss";
	ChangeStyle::setStyle(tmp);
}

void ChangeSkin::on_cb_b_clicked()
{
	tmp = ":qss/black.qss";
	ChangeStyle::setStyle(tmp);
}

void ChangeSkin::on_cb_w_clicked()
{
	tmp = ":qss/white.qss";
	ChangeStyle::setStyle(tmp);
}

void ChangeSkin::on_buttonBox_rejected()
{
	ChangeStyle::setStyle(cur);
	emit change("MainWindow");
}
