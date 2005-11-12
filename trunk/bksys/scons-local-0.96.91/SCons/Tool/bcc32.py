__revision__ = "src/engine/SCons/Tool/bcc32.py 0.96.91 2005/10/17 20:04:26 ita"
import os
import os.path
import string
import SCons.Defaults
import SCons.Tool
import SCons.Util
def findIt(program, env):
    borwin = env.WhereIs(program) or SCons.Util.WhereIs(program)
    if borwin:
        dir = os.path.dirname(borwin)
        path = env['ENV'].get('PATH', [])
        if not path:
            path = []
        if SCons.Util.is_String(path):
            path = string.split(path, os.pathsep)
        env['ENV']['PATH'] = string.join([dir]+path, os.pathsep)
    return borwin
def generate(env):
    findIt('bcc32', env)
    """Add Builders and construction variables for bcc to an
    Environment."""
    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    for suffix in ['.c', '.cpp']:
        static_obj.add_action(suffix, SCons.Defaults.CAction)
        shared_obj.add_action(suffix, SCons.Defaults.ShCAction)
        static_obj.add_emitter(suffix, SCons.Defaults.StaticObjectEmitter)
        shared_obj.add_emitter(suffix, SCons.Defaults.SharedObjectEmitter)
    env['CC']        = 'bcc32'
    env['CCFLAGS']   = SCons.Util.CLVar('')
    env['CCCOM']     = '$CC -q $CCFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS -c -o$TARGET $SOURCES'
    env['SHCC']      = '$CC'
    env['SHCCFLAGS'] = SCons.Util.CLVar('$CCFLAGS')
    env['SHCCCOM']   = '$SHCC -WD $SHCCFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS -c -o$TARGET $SOURCES'
    env['CPPDEFPREFIX']  = '-D'
    env['CPPDEFSUFFIX']  = ''
    env['INCPREFIX']  = '-I'
    env['INCSUFFIX']  = ''
    env['SHOBJSUFFIX'] = '.dll'
    env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 0
    env['CFILESUFFIX'] = '.cpp'
def exists(env):
    return findIt('bcc32', env)
