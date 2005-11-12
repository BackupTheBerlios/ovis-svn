__revision__ = "src/engine/SCons/Tool/tlib.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Tool
import SCons.Tool.bcc32
import SCons.Util
def generate(env):
    SCons.Tool.bcc32.findIt('tlib', env)
    """Add Builders and construction variables for ar to an Environment."""
    SCons.Tool.createStaticLibBuilder(env)
    env['AR']          = 'tlib'
    env['ARFLAGS']     = SCons.Util.CLVar('')
    env['ARCOM']       = '$AR $TARGET $ARFLAGS /a $SOURCES'
    env['LIBPREFIX']   = ''
    env['LIBSUFFIX']   = '.lib'
def exists(env):
    return SCons.Tool.bcc32.findIt('tlib', env)
