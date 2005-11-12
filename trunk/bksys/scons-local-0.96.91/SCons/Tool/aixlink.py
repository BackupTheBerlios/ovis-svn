__revision__ = "src/engine/SCons/Tool/aixlink.py 0.96.91 2005/10/17 20:04:26 ita"
import os
import os.path
import SCons.Util
import aixcc
import link
cplusplus = __import__('c++', globals(), locals(), [])
def smart_linkflags(source, target, env, for_signature):
    if cplusplus.iscplusplus(source):
        build_dir = env.subst('$BUILDDIR')
        if build_dir:
            return '-qtempinc=' + os.path.join(build_dir, 'tempinc')
    return ''
def generate(env):
    """
    Add Builders and construction variables for Visual Age linker to
    an Environment.
    """
    link.generate(env)
    env['SMARTLINKFLAGS'] = smart_linkflags
    env['LINKFLAGS']      = SCons.Util.CLVar('$SMARTLINKFLAGS')
    env['SHLINKFLAGS']    = SCons.Util.CLVar('$LINKFLAGS -qmkshrobj -qsuppress=1501-218')
    env['SHLIBSUFFIX']    = '.a'
def exists(env):
    path, _cc, _shcc, version = aixcc.get_xlc(env)
    if path and _cc:
        xlc = os.path.join(path, _cc)
        if os.path.exists(xlc):
            return xlc
    return None
