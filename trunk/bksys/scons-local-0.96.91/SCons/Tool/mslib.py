__revision__ = "src/engine/SCons/Tool/mslib.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Defaults
import SCons.Tool
import SCons.Tool.msvs
import SCons.Tool.msvc
import SCons.Util
def generate(env):
    """Add Builders and construction variables for lib to an Environment."""
    SCons.Tool.createStaticLibBuilder(env)
    try:
        version = SCons.Tool.msvs.get_default_visualstudio_version(env)
        if env.has_key('MSVS_IGNORE_IDE_PATHS') and env['MSVS_IGNORE_IDE_PATHS']:
            include_path, lib_path, exe_path = SCons.Tool.msvc.get_msvc_default_paths(version)
        else:
            include_path, lib_path, exe_path = SCons.Tool.msvc.get_msvc_paths(version)
        env.PrependENVPath('PATH', exe_path)
    except (SCons.Util.RegError, SCons.Errors.InternalError):
        pass
    env['AR']          = 'lib'
    env['ARFLAGS']     = SCons.Util.CLVar('/nologo')
    env['ARCOM']       = "${TEMPFILE('$AR $ARFLAGS /OUT:$TARGET $SOURCES')}"
    env['LIBPREFIX']   = ''
    env['LIBSUFFIX']   = '.lib'
def exists(env):
    try:
        v = SCons.Tool.msvs.get_visualstudio_versions()
    except (SCons.Util.RegError, SCons.Errors.InternalError):
        pass
    
    if not v:
        return env.Detect('lib')
    else:
        return 1
