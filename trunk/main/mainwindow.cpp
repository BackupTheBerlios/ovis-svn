#include "mainwindow.hh"

namespace ovis {

	Mainwindow::Mainwindow(QApplication *pApplication):m_pApplication(pApplication)
	{
		QDesktopWidget *desktop = QApplication::desktop();
		int dwidth  = desktop->width();
		int dheight = desktop->height();
		int width   = dwidth  * 7/10;
		int height  = dheight * 7/10;

		QSettings settings("OpenVisualizer", "AppExpl");
		QPoint pos = settings.value("pos",  QPoint((dwidth-width)/2,(dheight-height)/2)).toPoint();
		QSize size = settings.value("size", QSize(width, height)).toSize();
		resize(size);
		move(pos);

		statusBar();
		menuBar();	
		m_pTabwidget=new QTabWidget(this);
		setCentralWidget(m_pTabwidget);
		
	}

	Mainwindow::~Mainwindow()
	{
		delete m_pTabwidget;
	}

}
