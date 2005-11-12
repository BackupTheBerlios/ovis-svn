__revision__ = "src/engine/SCons/Tool/g++.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import re
import SCons.Defaults
import SCons.Tool
import SCons.Util
cplusplus = __import__('c++', globals(), locals(), [])
compilers = ['g++']
def generate(env):
    """Add Builders and construction variables for g++ to an Environment."""
    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    cplusplus.generate(env)
    env['CXX']        = env.Detect(compilers)
    if env['PLATFORM'] == 'cygwin':
        env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS')
    elif env['PLATFORM'] == 'aix':
        env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS -mminimal-toc')
        env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1
        env['SHOBJSUFFIX'] = '$OBJSUFFIX'
    elif env['PLATFORM'] == 'hpux':
        env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS -fPIC')
        env['SHOBJSUFFIX'] = '.pic.o'
    elif env['PLATFORM'] == 'sunos':
        env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS -fPIC')
        env['SHOBJSUFFIX'] = '.pic.o'
    else:
        env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS -fPIC')
    if env['CXX']:
        line = os.popen(env['CXX'] + ' --version').readline()
        match = re.search(r'[0-9]+(\.[0-9]+)+', line)
        if match:
            env['CXXVERSION'] = match.group(0)
def exists(env):
    return env.Detect(compilers)
