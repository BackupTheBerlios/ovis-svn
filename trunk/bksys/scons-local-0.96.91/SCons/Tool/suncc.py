__revision__ = "src/engine/SCons/Tool/suncc.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
import cc
def generate(env):
    """
    Add Builders and construction variables for Forte C and C++ compilers
    to an Environment.
    """
    cc.generate(env)
    env['CXX']          = 'CC'
    env['SHCCFLAGS']    = SCons.Util.CLVar('$CCFLAGS -KPIC')
    env['SHOBJPREFIX']  = 'so_'
    env['SHOBJSUFFIX']  = '.o'
def exists(env):
    return env.Detect('CC')
