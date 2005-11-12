__revision__ = "src/engine/SCons/Tool/icc.py 0.96.91 2005/10/17 20:04:26 ita"
import cc
def generate(env):
    """Add Builders and construction variables for the OS/2 to an Environment."""
    cc.generate(env)
    env['CC']         = 'icc'
    env['CCCOM']      = '$CC $CCFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS /c $SOURCES /Fo$TARGET'
    env['CXXCOM']     = '$CXX $CXXFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS /c $SOURCES /Fo$TARGET'
    env['CPPDEFPREFIX']  = '/D'
    env['CPPDEFSUFFIX']  = ''
    env['INCPREFIX']  = '/I'
    env['INCSUFFIX']  = ''
    env['CFILESUFFIX'] = '.c'
    env['CXXFILESUFFIX'] = '.cc'
def exists(env):
    return env.Detect('icc')
