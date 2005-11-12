__revision__ = "src/engine/SCons/Tool/c++.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import SCons.Tool
import SCons.Defaults
import SCons.Util
compilers = ['CC', 'c++']
CXXSuffixes = ['.cpp', '.cc', '.cxx', '.c++', '.C++', '.mm']
if SCons.Util.case_sensitive_suffixes('.c', '.C'):
    CXXSuffixes.append('.C')
def iscplusplus(source):
    if not source:
        return 0
    for s in source:
        if s.sources:
            ext = os.path.splitext(str(s.sources[0]))[1]
            if ext in CXXSuffixes:
                return 1
    return 0
def generate(env):
    """
    Add Builders and construction variables for Visual Age C++ compilers
    to an Environment.
    """
    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    for suffix in CXXSuffixes:
        static_obj.add_action(suffix, SCons.Defaults.CXXAction)
        shared_obj.add_action(suffix, SCons.Defaults.ShCXXAction)
        static_obj.add_emitter(suffix, SCons.Defaults.StaticObjectEmitter)
        shared_obj.add_emitter(suffix, SCons.Defaults.SharedObjectEmitter)
        
    env['CXX']        = 'c++'
    env['CXXFLAGS']   = SCons.Util.CLVar('$CCFLAGS')
    env['CXXCOM']     = '$CXX -o $TARGET -c $CXXFLAGS $_CCCOMCOM $SOURCES'
    env['SHCXX']      = '$CXX'
    env['SHCXXFLAGS'] = SCons.Util.CLVar('$CXXFLAGS')
    env['SHCXXCOM']   = '$SHCXX -o $TARGET -c $SHCXXFLAGS $_CCCOMCOM $SOURCES'
    env['CPPDEFPREFIX']  = '-D'
    env['CPPDEFSUFFIX']  = ''
    env['INCPREFIX']  = '-I'
    env['INCSUFFIX']  = ''
    env['SHOBJSUFFIX'] = '.os'
    env['OBJSUFFIX'] = '.o'
    env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 0
    env['CXXFILESUFFIX'] = '.cc'
def exists(env):
    return env.Detect(compilers)
