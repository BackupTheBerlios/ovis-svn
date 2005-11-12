__revision__ = "src/engine/SCons/Tool/ilink32.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Tool
import SCons.Tool.bcc32
import SCons.Util
def generate(env):
    """Add Builders and construction variables for ilink to an
    Environment."""
    SCons.Tool.createProgBuilder(env)
    env['LINK']        = '$CC'
    env['LINKFLAGS']   = SCons.Util.CLVar('')
    env['LINKCOM']     = '$LINK -q $LINKFLAGS $SOURCES $LIBS'
    env['LIBDIRPREFIX']=''
    env['LIBDIRSUFFIX']=''
    env['LIBLINKPREFIX']=''
    env['LIBLINKSUFFIX']='$LIBSUFFIX'
def exists(env):
    return SCons.Tool.bcc32.findIt('bcc32', env)
