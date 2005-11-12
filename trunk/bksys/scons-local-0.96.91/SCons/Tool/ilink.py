__revision__ = "src/engine/SCons/Tool/ilink.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Defaults
import SCons.Tool
import SCons.Util
def generate(env):
    """Add Builders and construction variables for ilink to an Environment."""
    SCons.Tool.createProgBuilder(env)
    
    env['LINK']        = 'ilink'
    env['LINKFLAGS']   = SCons.Util.CLVar('')
    env['LINKCOM']     = '$LINK $LINKFLAGS /O:$TARGET $SOURCES $( $_LIBDIRFLAGS $) $_LIBFLAGS'
    env['LIBDIRPREFIX']='/LIBPATH:'
    env['LIBDIRSUFFIX']=''
    env['LIBLINKPREFIX']=''
    env['LIBLINKSUFFIX']='$LIBSUFFIX'
def exists(env):
    return env.Detect('ilink')
