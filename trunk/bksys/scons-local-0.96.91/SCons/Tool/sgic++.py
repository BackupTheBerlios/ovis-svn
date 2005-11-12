__revision__ = "src/engine/SCons/Tool/sgic++.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
cplusplus = __import__('c++', globals(), locals(), [])
def generate(env):
    """Add Builders and construction variables for SGI MIPS C++ to an Environment."""
    cplusplus.generate(env)
    env['CXX']         = 'CC'
    env['CXXFLAGS']    = SCons.Util.CLVar('$CCFLAGS -LANG:std')
    env['SHCXX']       = 'CC'
    env['SHOBJSUFFIX'] = '.o'
    env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1
    
def exists(env):
    return env.Detect('CC')
