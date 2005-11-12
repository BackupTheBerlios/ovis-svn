__revision__ = "src/engine/SCons/Tool/hplink.py 0.96.91 2005/10/17 20:04:26 ita"
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
for dir in dirs:
    linker = '/opt/' + dir + '/bin/aCC'
    if os.path.exists(linker):
        ccLinker = linker
        break
def generate(env):
    """
    Add Builders and construction variables for Visual Age linker to
    an Environment.
    """
    link.generate(env)
    
    env['LINKFLAGS']   = SCons.Util.CLVar('-Wl,+s -Wl,+vnocompatwarnings')
    env['SHLINKFLAGS'] = SCons.Util.CLVar('$LINKFLAGS -b')
    env['SHLIBSUFFIX'] = '.sl'
def exists(env):
    return ccLinker
