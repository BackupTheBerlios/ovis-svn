__revision__ = "src/engine/SCons/Tool/sgilink.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
import link
linkers = ['CC', 'cc']
def generate(env):
    """Add Builders and construction variables for MIPSPro to an Environment."""
    link.generate(env)
    
    env['LINK'] = env.Detect(linkers) or 'cc'
    env['SHLINKFLAGS'] = SCons.Util.CLVar('$LINKFLAGS -shared')
    env.Append(LINKFLAGS=['$__RPATH'])
    env['RPATHPREFIX'] = '-rpath '
    env['RPATHSUFFIX'] = ''
    env['_RPATH'] = '${_concat(RPATHPREFIX, RPATH, RPATHSUFFIX, __env__)}'
def exists(env):
    return env.Detect(linkers)
