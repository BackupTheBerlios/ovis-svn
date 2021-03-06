##
# @file 
# win32 related QT 4 detection 

import os, re, types, sys

def detect(env):
	def getpath(varname):
		if not env.has_key('ARGS'): return None
		v=env['ARGS'].get(varname, None)
		if v : v=os.path.abspath(v)
		return v

	# set qt relating library name suffix, depends on version and debug/release mode 
	# This values could be taken from QTDIR/.qmake.conf 
	#  debug -> CONFIG 
	#  library version extension -> QMAKE_QT_VERSION_OVERRIDE (=4 yet) 
	if env['BKS_DEBUG']: lib_addon = 'd4'
	else:                lib_addon = '4'

	# TODO (rh) libsuffix and lib_addon may be overlap in some areas, potential for cleanup 
	libsuffix	= ''
	if env.has_key('ARGS'): libsuffix=env['ARGS'].get('libsuffix', '')

	p=env.pprint
	# do our best to find the QTDIR
	qtdir       = os.getenv("QTDIR")
	qtincludes	= os.getenv("QTINCLUDES")
	qtlibs		= os.getenv("QTLIBS")
	if not qtdir:
		qtdir=env.find_path('include/', [ # lets find the qt include directory
				'c:/Qt/4.0.3/', # one never knows
				'c:/Qt/4.0.2/',
				'c:/Qt/4.0.1/',
				'c:/Qt/4.0.0/'])
		if qtdir: p('YELLOW', 'The qtdir was found as '+qtdir)
		else:     p('YELLOW', 'There is no QTDIR set')
	else: env['QTDIR'] = qtdir.strip()

	# if we have the QTDIR, finding the qtlibs and qtincludes is easy
	if qtdir:
		if not qtlibs:     qtlibs     = os.path.join(qtdir, 'lib' + libsuffix)
		if not qtincludes: qtincludes = os.path.join(qtdir, 'include')
		#os.putenv('PATH', os.path.join(qtdir , 'bin') + ":" + os.getenv("PATH")) # TODO ita 

	# Check for uic, uic-qt3, moc, rcc, ..
	def find_qt_bin(progs):
		# first use the qtdir
		path=''
		for prog in progs:
			path=env.find_program(prog, [env.join(qtdir, 'bin')])
			if path:
				p('GREEN',"%s was found as %s" % (prog, path))
				return path

		# everything failed
		p('RED',"%s was not found - make sure Qt4-devel is installed, or set $QTDIR or $PATH" % prog)
		env.Exit(1)
	
	print "Checking for uic               :",
	env['QT_UIC']  = find_qt_bin(['uic', 'uic-qt4'])

	print "Checking for uic-qt3           :",
	env['QT_UIC3'] = find_qt_bin(['uic3', 'uic-qt3'])

	print "Checking for moc               :",
	env['QT_MOC'] = find_qt_bin(['moc', 'moc-qt4'])

	print "Checking for rcc               :",
	env['QT_RCC'] = find_qt_bin(['rcc'])

	# TODO is this really needed now ?
	print "Checking for uic3 version      :",
	version = os.popen(env['QT_UIC'] + " -version 2>&1").read().strip()
	if version.find(" 3.") != -1:
		version = version.replace('Qt user interface compiler','')
		version = version.replace('User Interface Compiler for Qt', '')
		p('RED', version + " (too old)")
		env.Exit(1)
	p('GREEN', "fine - %s" % version)

	#if os.environ.has_key('PKG_CONFIG_PATH'):
	#	os.environ['PKG_CONFIG_PATH'] = os.environ['PKG_CONFIG_PATH'] + ':' + qtlibs
	#else:
	#	os.environ['PKG_CONFIG_PATH'] = qtlibs

	## check for the Qt4 includes
	print "Checking for the Qt4 includes  :",
	if qtincludes and os.path.isfile(qtincludes + "/QtGui/QFont"):
		# The user told where to look for and it looks valid
		p('GREEN','ok '+qtincludes)
	else:
		if os.path.isfile(qtdir+'/include/QtGui/QFont'):
			# Automatic detection
			p('GREEN','ok '+qtdir+"/include/")
			qtincludes = qtdir + "/include/"
		else:
			p('RED',"the qt headers were not found")
			env.Exit(1)


	#env['QTPLUGINS']=os.popen('kde-config --expandvars --install qtplugins').read().strip()

	## qt libs and includes
	env['QTINCLUDEPATH']=qtincludes
	if not qtlibs: qtlibs=env.join(qtdir, 'lib', libsuffix)
	env['QTLIBPATH']=qtlibs

	########## QT
	# QTLIBPATH is a special var used in the qt4 module - has to be changed (ita)

	# taken from qt4 qmake generated example 

	env['CXXFLAGS_QT']         = '-DQT_EDITION=QT_EDITION_DESKTOP -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_THREAD_SUPPORT -DQT_DLL'.split()
	if not env['ARGS'].get('debug', None):
		env['CXXFLAGS_QT']       += ['-DQT_NO_DEBUG']
	env['CPPPATH_QT']          = [ env.join(env['QTINCLUDEPATH'], 'Qt'), env['QTINCLUDEPATH'] ] # TODO QTINCLUDEPATH (ita)
	env['LIBPATH_QT']          = [env['QTLIBPATH']]
	env['LIB_QT']              = ['QtGui'+lib_addon]
	env['RPATH_QT']            = [env['QTLIBPATH']]
	env['CXXFLAGS_QT3SUPPORT'] = ['-DQT3_SUPPORT']
	env['CPPPATH_QT3SUPPORT']  = [ env.join(env['QTINCLUDEPATH'], 'Qt3Support') ]
	env['LIB_QT3SUPPORT']      = ['Qt3Support'+lib_addon]
	env['RPATH_QT3SUPPORT']    = env['RPATH_QT']
	
	env['CXXFLAGS_QTCORE']     = ['-DQT_CORE_LIB']
	env['CPPPATH_QTCORE']      = [ env.join(env['QTINCLUDEPATH'], 'QtCore') ]
	env['LIB_QTCORE']          = ['QtCore'+lib_addon]
	env['RPATH_QTCORE']        = env['RPATH_QT']
	
	env['CPPPATH_QTASSISTANT'] = [ env.join(env['QTINCLUDEPATH'], 'QtAssistant') ]
	env['LIB_QTASSISTANT']     = ['QtAssistant'+lib_addon]
	
	env['CPPPATH_QTDESIGNER']  = [ env.join(env['QTINCLUDEPATH'], 'QtDesigner') ]
	env['LIB_QTDESIGNER']      = ['QtDesigner'+lib_addon]
	
	env['CPPPATH_QTNETWORK']   = [ env.join(env['QTINCLUDEPATH'], 'QtNetwork') ]
	env['LIB_QTNETWORK']       = ['QtNetwork'+lib_addon]
	env['RPATH_QTNETWORK']     = env['RPATH_QT']
	
	env['CPPPATH_QTGUI']       = [ env.join(env['QTINCLUDEPATH'], 'QtGui') ]
	env['LIB_QTGUI']           = ['QtCore'+lib_addon, 'QtGui'+lib_addon]
	env['RPATH_QTGUI']         = env['RPATH_QT']
	
	env['CPPPATH_OPENGL']      = [ env.join(env['QTINCLUDEPATH'], 'QtOpengl') ]
	env['LIB_QTOPENGL']        = ['QtOpenGL'+lib_addon]
	env['RPATH_QTOPENGL']      = env['RPATH_QT']
	
	env['CPPPATH_QTSQL']       = [ env.join(env['QTINCLUDEPATH'], 'QtSql') ]
	env['LIB_QTSQL']           = ['QtSql'+lib_addon]
	env['RPATH_QTSQL']         = env['RPATH_QT']
	
	env['CPPPATH_QTXML']       = [ env.join(env['QTINCLUDEPATH'], 'QtXml') ]
	env['LIB_QTXML']           = ['QtXml'+lib_addon]
	env['RPATH_QTXML']         = env['RPATH_QT']
	
	env['QTLOCALE']=env.join(env['PREFIX'], 'share', 'locale')
    
