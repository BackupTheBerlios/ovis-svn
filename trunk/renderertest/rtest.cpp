#include <QApplication>
#include "testwindow.hh"








int main(int argc, char *argv[])
{
	QApplication a( argc, argv );
	Testwindow * mw = new Testwindow(&a);
	mw->show();
	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	int ret=a.exec();
	
	delete mw;
		
	return ret;
}
