__revision__ = "src/engine/SCons/Tool/ar.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Defaults
import SCons.Tool
import SCons.Util
def generate(env):
    """Add Builders and construction variables for ar to an Environment."""
    SCons.Tool.createStaticLibBuilder(env)
    arcom = '$AR $ARFLAGS $TARGET $SOURCES'
    ranlib = 'ranlib'
    if env.Detect(ranlib):
        arcom = arcom + '\n$RANLIB $RANLIBFLAGS $TARGET'
    env['AR']          = 'ar'
    env['ARFLAGS']     = SCons.Util.CLVar('r')
    env['RANLIB']      = ranlib
    env['RANLIBFLAGS'] = SCons.Util.CLVar('')
    env['ARCOM']       = arcom
    env['LIBPREFIX']   = 'lib'
    env['LIBSUFFIX']   = '.a'
def exists(env):
    return env.Detect('ar')
