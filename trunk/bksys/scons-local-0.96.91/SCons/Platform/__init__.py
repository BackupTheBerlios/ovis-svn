__revision__ = "src/engine/SCons/Platform/__init__.py 0.96.91 2005/10/17 20:04:26 ita"
import imp
import os
import string
import sys
import tempfile
import SCons.Errors
import SCons.Tool
def platform_default():
    """Return the platform string for our execution environment.
    The returned value should map to one of the SCons/Platform/*.py
    files.  Since we're architecture independent, though, we don't
    care about the machine architecture.
    """
    osname = os.name
    if osname == 'java':
        osname = os._osType
    if osname == 'posix':
        if sys.platform == 'cygwin':
            return 'cygwin'
        elif string.find(sys.platform, 'irix') != -1:
            return 'irix'
        elif string.find(sys.platform, 'sunos') != -1:
            return 'sunos'
        elif string.find(sys.platform, 'hp-ux') != -1:
            return 'hpux'
        elif string.find(sys.platform, 'aix') != -1:
            return 'aix'
        elif string.find(sys.platform, 'darwin') != -1:
            return 'darwin'
        else:
            return 'posix'
    elif os.name == 'os2':
        return 'os2'
    else:
        return sys.platform
def platform_module(name = platform_default()):
    """Return the imported module for the platform.
    This looks for a module name that matches the specified argument.
    If the name is unspecified, we fetch the appropriate default for
    our execution environment.
    """
    full_name = 'SCons.Platform.' + name
    if not sys.modules.has_key(full_name):
        if os.name == 'java':
            eval(full_name)
        else:
            try:
                file, path, desc = imp.find_module(name,
                                        sys.modules['SCons.Platform'].__path__)
                mod = imp.load_module(full_name, file, path, desc)
                setattr(SCons.Platform, name, mod)
            except ImportError:
                raise SCons.Errors.UserError, "No platform named '%s'" % name
            if file:
                file.close()
    return sys.modules[full_name]
def DefaultToolList(platform, env):
    """Select a default tool list for the specified platform.
    """
    return SCons.Tool.tool_list(platform, env)
class PlatformSpec:
    def __init__(self, name):
        self.name = name
    def __str__(self):
        return self.name
        
class TempFileMunge:
    """A callable class.  You can set an Environment variable to this,
    then call it with a string argument, then it will perform temporary
    file substitution on it.  This is used to circumvent the long command
    line limitation.
    Example usage:
    env["TEMPFILE"] = TempFileMunge
    env["LINKCOM"] = "${TEMPFILE('$LINK $TARGET $SOURCES')}"
    By default, the name of the temporary file used begins with a
    prefix of '@'.  This may be configred for other tool chains by
    setting '$TEMPFILEPREFIX'.
    env["TEMPFILEPREFIX"] = '-@'        # diab compiler
    env["TEMPFILEPREFIX"] = '-via'      # arm tool chain
    """
    def __init__(self, cmd):
        self.cmd = cmd
    def __call__(self, target, source, env, for_signature):
        if for_signature:
            return self.cmd
        cmd = env.subst_list(self.cmd, 0, target, source)[0]
        try:
            maxline = int(env.subst('$MAXLINELENGTH'))
        except ValueError:
            maxline = 2048
        if (reduce(lambda x, y: x + len(y), cmd, 0) + len(cmd)) <= maxline:
            return self.cmd
        tmp = os.path.normpath(tempfile.mktemp('.lnk'))
        native_tmp = SCons.Util.get_native_path(tmp)
        if env['SHELL'] and env['SHELL'] == 'sh':
            native_tmp = string.replace(native_tmp, '\\', r'\\\\')
            rm = env.Detect('rm') or 'del'
        else:
            rm = 'del'
        prefix = env.subst('$TEMPFILEPREFIX')
        if not prefix:
            prefix = '@'
        args = map(SCons.Subst.quote_spaces, cmd[1:])
        open(tmp, 'w').write(string.join(args, " ") + "\n")
        if SCons.Action.print_actions:
            print("Using tempfile "+native_tmp+" for command line:\n"+
                  str(cmd[0]) + " " + string.join(args," "))
        return [ cmd[0], prefix + native_tmp + '\n' + rm, native_tmp ]
    
def Platform(name = platform_default()):
    """Select a canned Platform specification.
    """
    module = platform_module(name)
    spec = PlatformSpec(name)
    spec.__call__ = module.generate
    return spec
