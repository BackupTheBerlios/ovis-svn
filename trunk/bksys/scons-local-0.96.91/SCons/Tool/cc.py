__revision__ = "src/engine/SCons/Tool/cc.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Tool
import SCons.Defaults
import SCons.Util
CSuffixes = ['.c', '.m']
if not SCons.Util.case_sensitive_suffixes('.c', '.C'):
    CSuffixes.append('.C')
def generate(env):
    """
    Add Builders and construction variables for C compilers to an Environment.
    """
    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    for suffix in CSuffixes:
        static_obj.add_action(suffix, SCons.Defaults.CAction)
        shared_obj.add_action(suffix, SCons.Defaults.ShCAction)
        static_obj.add_emitter(suffix, SCons.Defaults.StaticObjectEmitter)
        shared_obj.add_emitter(suffix, SCons.Defaults.SharedObjectEmitter)
        
    env['_CCCOMCOM'] = '$CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS'
    env['FRAMEWORKS'] = SCons.Util.CLVar('')
    env['FRAMEWORKPATH'] = SCons.Util.CLVar('')
    if env['PLATFORM'] == 'darwin':
        env['_CCCOMCOM'] = env['_CCCOMCOM'] + ' $_FRAMEWORKPATH'
    env['CC']        = 'cc'
    env['CCFLAGS']   = SCons.Util.CLVar('')
    env['CCCOM']     = '$CC -o $TARGET -c $CCFLAGS $_CCCOMCOM $SOURCES'
    env['SHCC']      = '$CC'
    env['SHCCFLAGS'] = SCons.Util.CLVar('$CCFLAGS')
    env['SHCCCOM']   = '$SHCC -o $TARGET -c $SHCCFLAGS $_CCCOMCOM $SOURCES'
    env['CPPDEFPREFIX']  = '-D'
    env['CPPDEFSUFFIX']  = ''
    env['INCPREFIX']  = '-I'
    env['INCSUFFIX']  = ''
    env['SHOBJSUFFIX'] = '.os'
    env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 0
    env['CFILESUFFIX'] = '.c'
def exists(env):
    return env.Detect('cc')
