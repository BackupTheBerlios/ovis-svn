__revision__ = "src/engine/SCons/Tool/applelink.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
import gnulink
def generate(env):
    """Add Builders and construction variables for applelink to an
    Environment."""
    gnulink.generate(env)
    env['FRAMEWORKPATHPREFIX'] = '-F'
    env['_FRAMEWORKPATH'] = '${_concat(FRAMEWORKPATHPREFIX, FRAMEWORKPATH, "", __env__)}'
    env['_FRAMEWORKS'] = '${_concat("-framework ", FRAMEWORKS, "", __env__)}'
    env['LINKCOM'] = env['LINKCOM'] + ' $_FRAMEWORKPATH $_FRAMEWORKS'
    env['SHLINKFLAGS'] = SCons.Util.CLVar('$LINKFLAGS -dynamiclib')
    env['SHLINKCOM'] = env['SHLINKCOM'] + ' $_FRAMEWORKPATH $_FRAMEWORKS'
    env['LDMODULEPREFIX'] = '' 
    env['LDMODULESUFFIX'] = '' 
    env['LDMODULEFLAGS'] = SCons.Util.CLVar('$LINKFLAGS -bundle')
    env['LDMODULECOM'] = '$LDMODULE -o ${TARGET} $LDMODULEFLAGS $SOURCES $_LIBDIRFLAGS $_LIBFLAGS $_FRAMEWORKPATH $_FRAMEWORKS $FRAMEWORKSFLAGS'
def exists(env):
    import sys
    return sys.platform == 'darwin'
