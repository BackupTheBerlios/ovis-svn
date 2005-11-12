#include <QApplication>
#include "mainwindow.hh"

int main(int argc, char *argv[])
{
	QApplication a( argc, argv );
	ovis::Mainwindow * mw = new ovis::Mainwindow(&a);
	mw->show();
	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	int ret=a.exec();
	
	delete mw;
		
	return ret;
}
