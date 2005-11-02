#ifndef OVIS_MAINWINDOW_HH_INCLUDED
#define OVIS_MAINWINDOW_HH_INCLUDED

#include <qapplication.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qtabwidget.h>

namespace ovis {

	class Mainwindow:public QMainWindow
	{
		Q_OBJECT
		
	public:
		Mainwindow(QApplication *pApplication);
		~Mainwindow();
		
	protected:
		QApplication *m_pApplication;
		QTabWidget *m_pTabwidget;
		QMenuBar *m_pMenubar;
	};

}

#endif
