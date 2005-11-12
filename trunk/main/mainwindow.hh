#ifndef OVIS_MAINWINDOW_HH_INCLUDED
#define OVIS_MAINWINDOW_HH_INCLUDED

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QTabWidget>
#include <QSettings>
#include <QDesktopWidget>
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
