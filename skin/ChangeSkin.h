#ifndef CHANGESKIN_H
#define CHANGESKIN_H

#include "CommonDialog.h"
#include "ChangeStyle.h"

namespace Ui {
class ChangeSkin;
}

class ChangeSkin : public CommonDialog
{
	Q_OBJECT

public:
	explicit ChangeSkin();
	~ChangeSkin();

private slots:
	void on_buttonBox_accepted();

	void on_cb_d_clicked();

	void on_cb_b_clicked();

	void on_cb_w_clicked();

	void on_buttonBox_rejected();

private:
	Ui::ChangeSkin *ui;
	QString cur;
	QString tmp;
};

#endif // CHANGESKIN_H
