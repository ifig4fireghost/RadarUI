#ifndef UIWAITING_H
#define UIWAITING_H

#include <QDialog>

#include <QMovie>

namespace Ui {
class UIWaiting;
}

class UIWaiting : public QDialog
{
	Q_OBJECT

public:
	explicit UIWaiting(QWidget *parent = 0);
	~UIWaiting();
	void SetInfo(const QString& infos);

private:
	Ui::UIWaiting *ui;
	QMovie* movie;
};

#endif // UIWAITING_H
