__revision__ = "src/engine/SCons/Platform/hpux.py 0.96.91 2005/10/17 20:04:26 ita"
import posix
def generate(env):
    posix.generate(env)
    env['MAXLINELENGTH']  = 2045000