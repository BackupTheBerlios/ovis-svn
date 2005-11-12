__revision__ = "src/engine/SCons/Platform/cygwin.py 0.96.91 2005/10/17 20:04:26 ita"
import posix
from SCons.Platform import TempFileMunge
def generate(env):
    posix.generate(env)
    env['PROGPREFIX']  = ''
    env['PROGSUFFIX']  = '.exe'
    env['SHLIBPREFIX'] = ''
    env['SHLIBSUFFIX'] = '.dll'
    env['LIBPREFIXES'] = [ '$LIBPREFIX', '$SHLIBPREFIX' ]
    env['LIBSUFFIXES'] = [ '$LIBSUFFIX', '$SHLIBSUFFIX' ]
    env['TEMPFILE']    = TempFileMunge
    env['TEMPFILEPREFIX'] = '@'
    env['MAXLINELENGTH']  = 2048
