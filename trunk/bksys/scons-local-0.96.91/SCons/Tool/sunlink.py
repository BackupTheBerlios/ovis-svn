__revision__ = "src/engine/SCons/Tool/sunlink.py 0.96.91 2005/10/17 20:04:26 ita"
import os
import os.path
import SCons.Util
import link
ccLinker = None
# search for the acc compiler and linker front end
try:
    dirs = os.listdir('/opt')
except (IOError, OSError):
    dirs = []
for d in dirs:
    linker = '/opt/' + d + '/bin/CC'
    if os.path.exists(linker):
        ccLinker = linker
        break
def generate(env):
    """Add Builders and construction variables for Forte to an Environment."""
    link.generate(env)
    
    env['SHLINKFLAGS'] = SCons.Util.CLVar('$LINKFLAGS -G')
    env.Append(LINKFLAGS=['$__RPATH'])
    env['RPATHPREFIX'] = '-R'
    env['RPATHSUFFIX'] = ''
    env['_RPATH'] = '${_concat(RPATHPREFIX, RPATH, RPATHSUFFIX, __env__)}'
def exists(env):
    return ccLinker
