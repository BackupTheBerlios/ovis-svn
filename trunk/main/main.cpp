#include <QApplication>
#include "mainwindow.hh"

#include "../base/log.hh"
#include "../base/stdoutstream.hh"

int main(int argc, char *argv[])
{
	ovis::base::logstream(ovis::base::Stdoutstream::stream());

	QApplication a( argc, argv );
	ovis::Mainwindow * mw = new ovis::Mainwindow(&a);
	mw->show();
	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	int ret=a.exec();
	
	delete mw;
		
	return ret;
}
