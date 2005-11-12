__revision__ = "src/engine/SCons/Tool/hpc++.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import string
import SCons.Util
cplusplus = __import__('c++', globals(), locals(), [])
acc = None
# search for the acc compiler and linker front end
try:
    dirs = os.listdir('/opt')
except (IOError, OSError):
    dirs = []
for dir in dirs:
    cc = '/opt/' + dir + '/bin/aCC'
    if os.path.exists(cc):
        acc = cc
        break
        
def generate(env):
    """Add Builders and construction variables for g++ to an Environment."""
    cplusplus.generate(env)
    if acc:
        env['CXX']        = acc or 'aCC'
        env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS +Z')
        line = os.popen(acc + ' -V 2>&1').readline().rstrip()
        if string.find(line, 'aCC: HP ANSI C++') == 0:
            env['CXXVERSION'] = string.split(line)[-1]
        if env['PLATFORM'] == 'cygwin':
            env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS')
        else:
            env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS +Z')
def exists(env):
    return acc
