__revision__ = "src/engine/SCons/Tool/mslink.py 0.96.91 2005/10/17 20:04:26 ita"
import os.path
import SCons.Action
import SCons.Defaults
import SCons.Errors
import SCons.Platform.win32
import SCons.Tool
import SCons.Tool.msvc
import SCons.Tool.msvs
import SCons.Util
def pdbGenerator(env, target, source, for_signature):
    if target and env.has_key('PDB') and env['PDB']:
        return ['/PDB:%s'%target[0].File(env['PDB']).get_string(for_signature),
                '/DEBUG']
    return None
def win32ShlinkTargets(target, source, env, for_signature):
    listCmd = []
    dll = env.FindIxes(target, 'SHLIBPREFIX', 'SHLIBSUFFIX')
    if dll: listCmd.append("/out:%s"%dll.get_string(for_signature))
    implib = env.FindIxes(target, 'LIBPREFIX', 'LIBSUFFIX')
    if implib: listCmd.append("/implib:%s"%implib.get_string(for_signature))
    return listCmd
def win32ShlinkSources(target, source, env, for_signature):
    listCmd = []
    deffile = env.FindIxes(source, "WIN32DEFPREFIX", "WIN32DEFSUFFIX")
    for src in source:
        if src == deffile:
            listCmd.append("/def:%s" % src.get_string(for_signature))
        else:
            listCmd.append(src)
    return listCmd
    
def win32LibEmitter(target, source, env):
    SCons.Tool.msvc.validate_vars(env)
    
    dll = env.FindIxes(target, "SHLIBPREFIX", "SHLIBSUFFIX")
    no_import_lib = env.get('no_import_lib', 0)
    
    if not dll:
        raise SCons.Errors.UserError, "A shared library should have exactly one target with the suffix: %s" % env.subst("$SHLIBSUFFIX")
    if env.get("WIN32_INSERT_DEF", 0) and \
       not env.FindIxes(source, "WIN32DEFPREFIX", "WIN32DEFSUFFIX"):
        source.append(env.ReplaceIxes(dll, 
                                      "SHLIBPREFIX", "SHLIBSUFFIX",
                                      "WIN32DEFPREFIX", "WIN32DEFSUFFIX"))
    if env.has_key('PDB') and env['PDB']:
        target.append(env['PDB'])
    if not no_import_lib and \
       not env.FindIxes(target, "LIBPREFIX", "LIBSUFFIX"):
        target.append(env.ReplaceIxes(dll, 
                                      "SHLIBPREFIX", "SHLIBSUFFIX",
                                      "LIBPREFIX", "LIBSUFFIX"))
        target.append(env.ReplaceIxes(dll, 
                                      "SHLIBPREFIX", "SHLIBSUFFIX",
                                      "WIN32EXPPREFIX", "WIN32EXPSUFFIX"))
    return (target, source)
def prog_emitter(target, source, env):
    SCons.Tool.msvc.validate_vars(env)
    
    if env.has_key('PDB') and env['PDB']:
        target.append(env['PDB'])
        
    return (target,source)
def RegServerFunc(target, source, env):
    if env.has_key('register') and env['register']:
        ret = regServerAction([target[0]], [source[0]], env)
        if ret:
            raise SCons.Errors.UserError, "Unable to register %s" % target[0]
        else:
            print "Registered %s sucessfully" % target[0]
        return ret
    return 0
regServerAction = SCons.Action.Action("$REGSVRCOM", "$REGSVRCOMSTR")
regServerCheck = SCons.Action.Action(RegServerFunc, None)
shlibLinkAction = SCons.Action.Action('${TEMPFILE("$SHLINK $SHLINKFLAGS $_SHLINK_TARGETS $( $_LIBDIRFLAGS $) $_LIBFLAGS $_PDB $_SHLINK_SOURCES")}')
compositeLinkAction = shlibLinkAction + regServerCheck
def generate(env):
    """Add Builders and construction variables for ar to an Environment."""
    SCons.Tool.createSharedLibBuilder(env)
    SCons.Tool.createProgBuilder(env)
    
    env['SHLINK']      = '$LINK'
    env['SHLINKFLAGS'] = SCons.Util.CLVar('$LINKFLAGS /dll')
    env['_SHLINK_TARGETS'] = win32ShlinkTargets
    env['_SHLINK_SOURCES'] = win32ShlinkSources
    env['SHLINKCOM']   =  compositeLinkAction
    env.Append(SHLIBEMITTER = [win32LibEmitter])
    env['LINK']        = 'link'
    env['LINKFLAGS']   = SCons.Util.CLVar('/nologo')
    env['_PDB'] = pdbGenerator
    env['LINKCOM'] = '${TEMPFILE("$LINK $LINKFLAGS /OUT:$TARGET.win32 $( $_LIBDIRFLAGS $) $_LIBFLAGS $_PDB $SOURCES.win32")}'
    env.Append(PROGEMITTER = [prog_emitter])
    env['LIBDIRPREFIX']='/LIBPATH:'
    env['LIBDIRSUFFIX']=''
    env['LIBLINKPREFIX']=''
    env['LIBLINKSUFFIX']='$LIBSUFFIX'
    env['WIN32DEFPREFIX']        = ''
    env['WIN32DEFSUFFIX']        = '.def'
    env['WIN32_INSERT_DEF']      = 0
    env['WIN32EXPPREFIX']        = ''
    env['WIN32EXPSUFFIX']        = '.exp'
    env['REGSVRACTION'] = regServerCheck
    env['REGSVR'] = os.path.join(SCons.Platform.win32.get_system_root(),'System32','regsvr32')
    env['REGSVRFLAGS'] = '/s '
    env['REGSVRCOM'] = '$REGSVR $REGSVRFLAGS ${TARGET.win32}'
    try:
        version = SCons.Tool.msvs.get_default_visualstudio_version(env)
        if env.has_key('MSVS_IGNORE_IDE_PATHS') and env['MSVS_IGNORE_IDE_PATHS']:
            include_path, lib_path, exe_path = SCons.Tool.msvc.get_msvc_default_paths(version)
        else:
            include_path, lib_path, exe_path = SCons.Tool.msvc.get_msvc_paths(version)
        env.PrependENVPath('INCLUDE', include_path)
        env.PrependENVPath('LIB', lib_path)
        env.PrependENVPath('PATH', exe_path)
    except (SCons.Util.RegError, SCons.Errors.InternalError):
        pass
    SCons.Tool.createLoadableModuleBuilder(env)
    env['LDMODULE'] = '$SHLINK'
    env['LDMODULEPREFIX'] = '$SHLIBPREFIX' 
    env['LDMODULESUFFIX'] = '$SHLIBSUFFIX' 
    env['LDMODULEFLAGS'] = '$SHLINKFLAGS'
    env['LDMODULECOM'] = compositeLinkAction
def exists(env):
    if SCons.Tool.msvs.is_msvs_installed():
        return 1
    else:
        return env.Detect('link')