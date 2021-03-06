##
# @file 
# platform-dependent low level configuration

def detect(lenv, dest):
	import os

	content="""
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_CTYPE_H 1 
#define HAVE_MALLOC_H 1
#define HAVE_STRING_H 1 
#define LTDL_SHLIB_EXT ".dll"
#define LTDL_OBJDIR ""
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STDINT_H 1

/* Distribution Text to append to OS */
#define KDE_DISTRIBUTION_TEXT "KDE Libraries for MS Windows"
"""

	import sys
	wver=sys.getwindowsversion()
	content += """
/* What OS used for compilation */
#define KDE_COMPILING_OS \"MS Windows %d.%d %s\"
""" % (wver[0], wver[1], wver[4])

	dest.write(content)
