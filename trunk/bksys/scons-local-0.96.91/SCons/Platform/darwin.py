__revision__ = "src/engine/SCons/Platform/darwin.py 0.96.91 2005/10/17 20:04:26 ita"
import posix
import os
def generate(env):
    posix.generate(env)
    env['SHLIBSUFFIX'] = '.dylib'
    env['ENV']['PATH'] = env['ENV']['PATH'] + ':/sw/bin'
