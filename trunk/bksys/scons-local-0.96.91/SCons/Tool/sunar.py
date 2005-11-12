__revision__ = "src/engine/SCons/Tool/sunar.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Defaults
import SCons.Tool
import SCons.Util
def generate(env):
    """Add Builders and construction variables for ar to an Environment."""
    SCons.Tool.createStaticLibBuilder(env)
    
    if env.Detect('CC'):
        env['AR']          = 'CC'
        env['ARFLAGS']     = SCons.Util.CLVar('-xar')
        env['ARCOM']       = '$AR $ARFLAGS -o $TARGET $SOURCES'
    else:
        env['AR']          = 'ar'
        env['ARFLAGS']     = SCons.Util.CLVar('r')
        env['ARCOM']       = '$AR $ARFLAGS $TARGET $SOURCES'
    env['SHLINK']      = '$LINK'
    env['SHLINKFLAGS'] = SCons.Util.CLVar('$LINKFLAGS -G')
    env['SHLINKCOM']   = '$SHLINK $SHLINKFLAGS -o $TARGET $SOURCES $_LIBDIRFLAGS $_LIBFLAGS'
    env['LIBPREFIX']   = 'lib'
    env['LIBSUFFIX']   = '.a'
def exists(env):
    return env.Detect('CC') or env.Detect('ar')
