__revision__ = "src/engine/SCons/Tool/tar.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Action
import SCons.Builder
import SCons.Defaults
import SCons.Node.FS
import SCons.Util
tars = ['tar', 'gtar']
TarAction = SCons.Action.Action('$TARCOM', '$TARCOMSTR')
TarBuilder = SCons.Builder.Builder(action = TarAction,
                                   source_factory = SCons.Node.FS.Entry,
                                   source_scanner = SCons.Defaults.DirScanner,
                                   suffix = '$TARSUFFIX',
                                   multi = 1)
def generate(env):
    """Add Builders and construction variables for tar to an Environment."""
    try:
        bld = env['BUILDERS']['Tar']
    except KeyError:
        bld = TarBuilder
        env['BUILDERS']['Tar'] = bld
    env['TAR']        = env.Detect(tars) or 'gtar'
    env['TARFLAGS']   = SCons.Util.CLVar('-c')
    env['TARCOM']     = '$TAR $TARFLAGS -f $TARGET $SOURCES'
    env['TARSUFFIX']  = '.tar'
def exists(env):
    return env.Detect(tars)
