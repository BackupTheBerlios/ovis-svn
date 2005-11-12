__revision__ = "src/engine/SCons/Tool/gnulink.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
import link
linkers = ['g++', 'gcc']
def generate(env):
    """Add Builders and construction variables for gnulink to an Environment."""
    link.generate(env)
    if env['PLATFORM'] == 'hpux':
        env['SHLINKFLAGS'] = SCons.Util.CLVar('$LINKFLAGS -shared -fPIC')
    env.Append(LINKFLAGS=['$__RPATH'])
    env['RPATHPREFIX'] = '-Wl,-rpath='
    env['RPATHSUFFIX'] = ''
    env['_RPATH'] = '${_concat(RPATHPREFIX, RPATH, RPATHSUFFIX, __env__)}'
    
def exists(env):
    return env.Detect(linkers)
