#include "mainwindow.hh"

namespace ovis {

	Mainwindow::Mainwindow(QApplication *pApplication):m_pApplication(pApplication)
	{
		unsigned int dw=m_pApplication->desktop()->width();
		unsigned int dh=m_pApplication->desktop()->height();

		unsigned int w=dw*7/10;
		unsigned int h=dh*7/10;

		move((dw-w)/2,(dh-h)/2);
		resize(w,h);

		// This forces the creation of a status bar
		statusBar();
		
		m_pTabwidget=new QTabWidget(this);
		setCentralWidget(m_pTabwidget);
		
		m_pMenubar=new QMenuBar(this,"Menubar");
	}

	Mainwindow::~Mainwindow()
	{
		delete m_pTabwidget;
	}

}
