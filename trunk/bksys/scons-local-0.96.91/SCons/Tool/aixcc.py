__revision__ = "src/engine/SCons/Tool/aixcc.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import SCons.Platform.aix
import cc
packages = ['vac.C', 'ibmcxx.cmp']
def get_xlc(env):
    xlc = env.get('CC', 'xlc')
    xlc_r = env.get('SHCC', 'xlc_r')
    return SCons.Platform.aix.get_xlc(env, xlc, xlc_r, packages)
def generate(env):
    """Add Builders and construction variables for xlc / Visual Age
    suite to an Environment."""
    path, _cc, _shcc, version = get_xlc(env)
    if path:
        _cc = os.path.join(path, _cc)
        _shcc = os.path.join(path, _shcc)
    cc.generate(env)
    env['CC'] = _cc
    env['SHCC'] = _shcc
    env['CCVERSION'] = version
def exists(env):
    path, _cc, _shcc, version = get_xlc(env)
    if path and _cc:
        xlc = os.path.join(path, _cc)
        if os.path.exists(xlc):
            return xlc
    return None
