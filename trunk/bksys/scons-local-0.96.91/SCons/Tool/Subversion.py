__revision__ = "src/engine/SCons/Tool/Subversion.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import SCons.Action
import SCons.Builder
import SCons.Util
def generate(env):
    """Add a Builder factory function and construction variables for
    Subversion to an Environment."""
    def SubversionFactory(repos, module='', env=env):
        """ """
        if module != '':
            module = os.path.join(module, '')
        act = SCons.Action.Action('$SVNCOM', '$SVNCOMSTR')
        return SCons.Builder.Builder(action = act,
                                     env = env,
                                     SVNREPOSITORY = repos,
                                     SVNMODULE = module)
    env.Subversion = SubversionFactory
    env['SVN']      = 'svn'
    env['SVNFLAGS'] = SCons.Util.CLVar('')
    env['SVNCOM']   = '$SVN $SVNFLAGS cat $SVNREPOSITORY/$SVNMODULE$TARGET > $TARGET'
def exists(env):
    return env.Detect('svn')
