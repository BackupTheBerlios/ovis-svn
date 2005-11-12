__revision__ = "src/engine/SCons/Tool/nasm.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Defaults
import SCons.Tool
import SCons.Util
ASSuffixes = ['.s', '.asm', '.ASM']
ASPPSuffixes = ['.spp', '.SPP']
if SCons.Util.case_sensitive_suffixes('.s', '.S'):
    ASPPSuffixes.extend(['.S'])
else:
    ASSuffixes.extend(['.S'])
def generate(env):
    """Add Builders and construction variables for nasm to an Environment."""
    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    for suffix in ASSuffixes:
        static_obj.add_action(suffix, SCons.Defaults.ASAction)
        static_obj.add_emitter(suffix, SCons.Defaults.StaticObjectEmitter)
    for suffix in ASPPSuffixes:
        static_obj.add_action(suffix, SCons.Defaults.ASPPAction)
        static_obj.add_emitter(suffix, SCons.Defaults.StaticObjectEmitter)
    env['AS']        = 'nasm'
    env['ASFLAGS']   = SCons.Util.CLVar('')
    env['ASPPFLAGS'] = '$ASFLAGS'
    env['ASCOM']     = '$AS $ASFLAGS -o $TARGET $SOURCES'
    env['ASPPCOM']   = '$CC $ASPPFLAGS $CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS -c -o $TARGET $SOURCES'
def exists(env):
    return env.Detect('nasm')
