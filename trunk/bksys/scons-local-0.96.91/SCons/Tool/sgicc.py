__revision__ = "src/engine/SCons/Tool/sgicc.py 0.96.91 2005/10/17 20:04:26 ita"
import cc
def generate(env):
    """Add Builders and construction variables for gcc to an Environment."""
    cc.generate(env)
    env['CXX']        = 'CC'
    env['SHOBJSUFFIX'] = '.o'
    env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1
def exists(env):
    return env.Detect('cc')
