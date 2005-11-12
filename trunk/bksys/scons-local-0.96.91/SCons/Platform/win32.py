__revision__ = "src/engine/SCons/Platform/win32.py 0.96.91 2005/10/17 20:04:26 ita"
import os
import os.path
import string
import sys
import tempfile
from SCons.Platform.posix import exitvalmap
from SCons.Platform import TempFileMunge
# XXX See note below about why importing SCons.Action should be
# eventually refactored.
import SCons.Action
import SCons.Util
# The upshot of all this is that, if you are using Python 1.5.2,
# you had better have cmd or command.com in your PATH when you run
# scons.
def piped_spawn(sh, escape, cmd, args, env, stdout, stderr):
    if not sh:
        sys.stderr.write("scons: Could not find command interpreter, is it in your PATH?\n")
        return 127
    else:
        tmpFileStdout = os.path.normpath(tempfile.mktemp())
        tmpFileStderr = os.path.normpath(tempfile.mktemp())
        stdoutRedirected = 0
        stderrRedirected = 0
        for arg in args:
            if (string.find( arg, ">", 0, 1 ) != -1 or
                string.find( arg, "1>", 0, 2 ) != -1):
                stdoutRedirected = 1
            if string.find( arg, "2>", 0, 2 ) != -1:
                stderrRedirected = 1
        if stdoutRedirected == 0:
            args.append(">" + str(tmpFileStdout))
        if stderrRedirected == 0:
            args.append("2>" + str(tmpFileStderr))
        try:
            args = [sh, '/C', escape(string.join(args)) ]
            ret = os.spawnve(os.P_WAIT, sh, args, env)
        except OSError, e:
            ret = exitvalmap[e[0]]
            if stderr != None:
                stderr.write("scons: %s: %s\n" % (cmd, e[1]))
        if stdout != None and stdoutRedirected == 0:
            try:
                stdout.write(open( tmpFileStdout, "r" ).read())
                os.remove( tmpFileStdout )
            except (IOError, OSError):
                pass
        if stderr != None and stderrRedirected == 0:
            try:
                stderr.write(open( tmpFileStderr, "r" ).read())
                os.remove( tmpFileStderr )
            except (IOError, OSError):
                pass
        return ret
def exec_spawn(l, env):
    try:
        result = os.spawnve(os.P_WAIT, l[0], l, env)
    except OSError, e:
        result = exitvalmap[e[0]]
        sys.stderr.write("scons: %s: %s\n" % (l[0], e[1]))
    return result
def spawn(sh, escape, cmd, args, env):
    if not sh:
        sys.stderr.write("scons: Could not find command interpreter, is it in your PATH?\n")
        return 127
    return exec_spawn([sh, '/C', escape(string.join(args))], env)
# Windows does not allow special characters in file names anyway, so
# no need for a complex escape function, we will just quote the arg.
escape = lambda x: '"' + x + '"'
# Get the windows system directory name
def get_system_root():
    try:
        val = os.environ['SYSTEMROOT']
    except KeyError:
        val = "C:/WINDOWS"
        pass
    if SCons.Util.can_read_reg:
        try:
            k=SCons.Util.RegOpenKeyEx(SCons.Util.hkey_mod.HKEY_LOCAL_MACHINE,
                                      'Software\\Microsoft\\Windows NT\\CurrentVersion')
            val, tok = SCons.Util.RegQueryValueEx(k, 'SystemRoot')
        except SCons.Util.RegError:
            try:
                k=SCons.Util.RegOpenKeyEx(SCons.Util.hkey_mod.HKEY_LOCAL_MACHINE,
                                          'Software\\Microsoft\\Windows\\CurrentVersion')
                val, tok = SCons.Util.RegQueryValueEx(k, 'SystemRoot')
            except KeyboardInterrupt:
                raise
            except:
                pass
    return val
# Get the location of the program files directory
def get_program_files_dir():
    val = ''
    if SCons.Util.can_read_reg:
        try:
            k=SCons.Util.RegOpenKeyEx(SCons.Util.hkey_mod.HKEY_LOCAL_MACHINE,
                                      'Software\\Microsoft\\Windows\\CurrentVersion')
            val, tok = SCons.Util.RegQueryValueEx(k, 'ProgramFilesDir')
        except SCons.Util.RegError:
            val = ''
            pass
    if val == '':
        val = os.path.join(os.path.dirname(get_system_root()),"Program Files")
        
    return val
def generate(env):
    cmd_interp = ''
    if SCons.Util.can_read_reg:
        try:
            k=SCons.Util.RegOpenKeyEx(SCons.Util.hkey_mod.HKEY_LOCAL_MACHINE,
                                          'Software\\Microsoft\\Windows NT\\CurrentVersion')
            val, tok = SCons.Util.RegQueryValueEx(k, 'SystemRoot')
            cmd_interp = os.path.join(val, 'System32\\cmd.exe')
        except SCons.Util.RegError:
            try:
                k=SCons.Util.RegOpenKeyEx(SCons.Util.hkey_mod.HKEY_LOCAL_MACHINE,
                                              'Software\\Microsoft\\Windows\\CurrentVersion')
                val, tok = SCons.Util.RegQueryValueEx(k, 'SystemRoot')
                cmd_interp = os.path.join(val, 'command.com')
            except KeyboardInterrupt:
                raise
            except:
                pass
    if not cmd_interp:
        systemroot = r'C:\Windows'
        if os.environ.has_key('SYSTEMROOT'):
            systemroot = os.environ['SYSTEMROOT']
        tmp_path = systemroot + os.pathsep + \
                   os.path.join(systemroot,'System32')
        tmp_pathext = '.com;.exe;.bat;.cmd'
        if os.environ.has_key('PATHEXT'):
            tmp_pathext = os.environ['PATHEXT'] 
        cmd_interp = SCons.Util.WhereIs('cmd', tmp_path, tmp_pathext)
        if not cmd_interp:
            cmd_interp = SCons.Util.WhereIs('command', tmp_path, tmp_pathext)
    if not cmd_interp:
        cmd_interp = env.Detect('cmd')
        if not cmd_interp:
            cmd_interp = env.Detect('command')
    
    if not env.has_key('ENV'):
        env['ENV']        = {}
    import_env = [ 'SYSTEMDRIVE', 'SYSTEMROOT', 'TEMP', 'TMP' ]
    for var in import_env:
        v = os.environ.get(var)
        if v:
            env['ENV'][var] = v
    env['ENV']['PATHEXT'] = '.COM;.EXE;.BAT;.CMD'
    env['OBJPREFIX']      = ''
    env['OBJSUFFIX']      = '.obj'
    env['SHOBJPREFIX']    = '$OBJPREFIX'
    env['SHOBJSUFFIX']    = '$OBJSUFFIX'
    env['PROGPREFIX']     = ''
    env['PROGSUFFIX']     = '.exe'
    env['LIBPREFIX']      = ''
    env['LIBSUFFIX']      = '.lib'
    env['SHLIBPREFIX']    = ''
    env['SHLIBSUFFIX']    = '.dll'
    env['LIBPREFIXES']    = [ '$LIBPREFIX' ]
    env['LIBSUFFIXES']    = [ '$LIBSUFFIX' ]
    env['PSPAWN']         = piped_spawn
    env['SPAWN']          = spawn
    env['SHELL']          = cmd_interp
    env['TEMPFILE']       = TempFileMunge
    env['TEMPFILEPREFIX'] = '@'
    env['MAXLINELENGTH']  = 2048
    env['ESCAPE']         = escape
