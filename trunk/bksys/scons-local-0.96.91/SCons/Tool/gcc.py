__revision__ = "src/engine/SCons/Tool/gcc.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
import cc
compilers = ['gcc', 'cc']
def generate(env):
    """Add Builders and construction variables for gcc to an Environment."""
    cc.generate(env)
    env['CC'] = env.Detect(compilers) or 'gcc'
    if env['PLATFORM'] == 'cygwin':
        env['SHCCFLAGS'] = SCons.Util.CLVar('$CCFLAGS')
    else:
        env['SHCCFLAGS'] = SCons.Util.CLVar('$CCFLAGS -fPIC')
def exists(env):
    return env.Detect(compilers)
