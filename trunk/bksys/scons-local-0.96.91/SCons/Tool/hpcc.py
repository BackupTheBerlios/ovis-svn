__revision__ = "src/engine/SCons/Tool/hpcc.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
import cc
def generate(env):
    """Add Builders and construction variables for aCC & cc to an Environment."""
    cc.generate(env)
    env['CXX']        = 'aCC'
    env['SHCCFLAGS']  = SCons.Util.CLVar('$CCFLAGS +Z')
def exists(env):
    return env.Detect('aCC')
