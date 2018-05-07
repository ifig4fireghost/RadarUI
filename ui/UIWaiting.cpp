#include "UIWaiting.h"
#include "ui_UIWaiting.h"

UIWaiting::UIWaiting(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UIWaiting)
{
	ui->setupUi(this);
	movie = new QMovie(":/texture/loading.gif");
	ui->lb_movie->setMovie(movie);
	movie->start();
}

void UIWaiting::SetInfo(const QString &infos)
{
	ui->lb_info->setText(infos);
}

UIWaiting::~UIWaiting()
{
	delete ui;
	delete movie;
}
