__revision__ = "src/engine/SCons/Tool/aixc++.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import SCons.Platform.aix
cplusplus = __import__('c++', globals(), locals(), [])
packages = ['vacpp.cmp.core', 'vacpp.cmp.batch', 'vacpp.cmp.C', 'ibmcxx.cmp']
def get_xlc(env):
    xlc = env.get('CXX', 'xlC')
    xlc_r = env.get('SHCXX', 'xlC_r')
    return SCons.Platform.aix.get_xlc(env, xlc, xlc_r, packages)
def smart_cxxflags(source, target, env, for_signature):
    build_dir = env.GetBuildPath()
    if build_dir:
        return '-qtempinc=' + os.path.join(build_dir, 'tempinc')
    return ''
def generate(env):
    """Add Builders and construction variables for xlC / Visual Age
    suite to an Environment."""
    path, _cxx, _shcxx, version = get_xlc(env)
    if path:
        _cxx = os.path.join(path, _cxx)
        _shcxx = os.path.join(path, _shcxx)
    cplusplus.generate(env)
    env['CXX'] = _cxx
    env['SHCXX'] = _shcxx
    env['CXXVERSION'] = version
    env['SHOBJSUFFIX'] = '.pic.o'
    
def exists(env):
    path, _cxx, _shcxx, version = get_xlc(env)
    if path and _cxx:
        xlc = os.path.join(path, _cxx)
        if os.path.exists(xlc):
            return xlc
    return None
