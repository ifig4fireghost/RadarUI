#include "MainWindow.h"
#include "CoreRouter.h"
#include "reflecthelper.h"
#include "ChangeStyle.h"

#include <QApplication>
#include <QDesktopWidget>

#include <signal.h>

void onsignal(int sig)
{
	CoreRouter::GetInstance().OnSignal(sig);
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ChangeStyle::setStyle(":/qss/white.qss");
	MainWindow w;
	CoreRouter::GetInstance(&w);
	w.show();
	w.screen = a.desktop()->size();
	CoreRouter::MoveTo(w.screen, &w);

	ObjectFactory::registerClass<ChangeSkin>();

	struct sigaction act;
	act.sa_handler = onsignal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);

	return a.exec();
}
