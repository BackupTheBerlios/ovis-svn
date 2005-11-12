__revision__ = "src/engine/SCons/Platform/aix.py 0.96.91 2005/10/17 20:04:26 ita"
import os
import string
import posix
def get_xlc(env, xlc, xlc_r, packages):
    xlcPath = None
    xlcVersion = None
    xlc = env.get('CC', 'xlc')
    for package in packages:
        cmd = "lslpp -fc " + package + " 2>/dev/null | egrep '" + xlc + "([^-_a-zA-Z0-9].*)?$'"
        line = os.popen(cmd).readline()
        if line:
            v, p = string.split(line, ':')[1:3]
            xlcVersion = string.split(v)[1]
            xlcPath = string.split(p)[0]
            xlcPath = xlcPath[:xlcPath.rindex('/')]
            break
    return (xlcPath, xlc, xlc_r, xlcVersion)
def generate(env):
    posix.generate(env)
    env['MAXLINELENGTH']  = 21576
