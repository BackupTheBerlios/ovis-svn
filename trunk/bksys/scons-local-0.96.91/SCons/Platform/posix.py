__revision__ = "src/engine/SCons/Platform/posix.py 0.96.91 2005/10/17 20:04:26 ita"
import os
import os.path
import popen2
import string
import sys
import select
import SCons.Util
from SCons.Platform import TempFileMunge
exitvalmap = {
    2 : 127,
    13 : 126,
}
def escape(arg):
    "escape shell special characters"
    slash = '\\'
    special = '"$'
    arg = string.replace(arg, slash, slash+slash)
    for c in special:
        arg = string.replace(arg, c, slash+c)
    return '"' + arg + '"'
def exec_system(l, env):
    stat = os.system(string.join(l))
    if stat & 0xff:
        return stat | 0x80
    return stat >> 8
def exec_spawnvpe(l, env):
    stat = os.spawnvpe(os.P_WAIT, l[0], l, env)
    return stat
def exec_fork(l, env):
    pid = os.fork()
    if not pid:
        exitval = 127
        try:
            os.execvpe(l[0], l, env)
        except OSError, e:
            exitval = exitvalmap[e[0]]
            sys.stderr.write("scons: %s: %s\n" % (l[0], e[1]))
        os._exit(exitval)
    else:
        pid, stat = os.waitpid(pid, 0)
        if stat & 0xff:
            return stat | 0x80
        return stat >> 8
def _get_env_command(sh, escape, cmd, args, env):
    s = string.join(args)
    if env:
        l = ['env', '-'] + \
            map(lambda t, e=escape: t[0]+'='+e(t[1]), env.items()) + \
            [sh, '-c', escape(s)]
        s = string.join(l)
    return s
def env_spawn(sh, escape, cmd, args, env):
    return exec_system([_get_env_command( sh, escape, cmd, args, env)], env)
def spawnvpe_spawn(sh, escape, cmd, args, env):
    return exec_spawnvpe([sh, '-c', string.join(args)], env)
def fork_spawn(sh, escape, cmd, args, env):
    return exec_fork([sh, '-c', string.join(args)], env)
def process_cmd_output(cmd_stdout, cmd_stderr, stdout, stderr):
    stdout_eof = stderr_eof = 0
    while not (stdout_eof and stderr_eof):
        (i,o,e) = select.select([cmd_stdout, cmd_stderr], [], [])
        if cmd_stdout in i:
            str = cmd_stdout.read()
            if len(str) == 0:
                stdout_eof = 1
            elif stdout != None:
                stdout.write(str)
        if cmd_stderr in i:
            str = cmd_stderr.read()
            if len(str) == 0:
                stderr_eof = 1
            else:
                stderr.write(str)
def exec_popen3(l, env, stdout, stderr):
    proc = popen2.Popen3(string.join(l), 1)
    process_cmd_output(proc.fromchild, proc.childerr, stdout, stderr)
    stat = proc.wait()
    if stat & 0xff:
        return stat | 0x80
    return stat >> 8
def exec_piped_fork(l, env, stdout, stderr):
    if stdout != stderr:
        (rFdOut, wFdOut) = os.pipe()
        (rFdErr, wFdErr) = os.pipe()
    else:
        (rFdOut, wFdOut) = os.pipe()
        rFdErr = rFdOut
        wFdErr = wFdOut
    pid = os.fork()
    if not pid:
        os.close( rFdOut )
        if rFdOut != rFdErr:
            os.close( rFdErr )
        os.dup2( wFdOut, 1 ) # is there some symbolic way to do that ?
        os.dup2( wFdErr, 2 )
        os.close( wFdOut )
        if stdout != stderr:
            os.close( wFdErr )
        exitval = 127
        try:
            os.execvpe(l[0], l, env)
        except OSError, e:
            exitval = exitvalmap[e[0]]
            stderr.write("scons: %s: %s\n" % (l[0], e[1]))
        os._exit(exitval)
    else:
        pid, stat = os.waitpid(pid, 0)
        os.close( wFdOut )
        if stdout != stderr:
            os.close( wFdErr )
        childOut = os.fdopen( rFdOut )
        if stdout != stderr:
            childErr = os.fdopen( rFdErr )
        else:
            childErr = childOut
        process_cmd_output(childOut, childErr, stdout, stderr)
        os.close( rFdOut )
        if stdout != stderr:
            os.close( rFdErr )
        if stat & 0xff:
            return stat | 0x80
        return stat >> 8
def piped_env_spawn(sh, escape, cmd, args, env, stdout, stderr):
    return exec_popen3([_get_env_command(sh, escape, cmd, args, env)],
                       env, stdout, stderr)
def piped_fork_spawn(sh, escape, cmd, args, env, stdout, stderr):
    return exec_piped_fork([sh, '-c', string.join(args)],
                           env, stdout, stderr)
def generate(env):
    if os.__dict__.has_key('spawnvpe'):
        spawn = spawnvpe_spawn
    elif env.Detect('env'):
        spawn = env_spawn
    else:
        spawn = fork_spawn
    if env.Detect('env'):
        pspawn = piped_env_spawn
    else:
        pspawn = piped_fork_spawn
    if not env.has_key('ENV'):
        env['ENV']        = {}
    env['ENV']['PATH']    = '/usr/local/bin:/opt/bin:/bin:/usr/bin'
    env['OBJPREFIX']      = ''
    env['OBJSUFFIX']      = '.o'
    env['SHOBJPREFIX']    = '$OBJPREFIX'
    env['SHOBJSUFFIX']    = '$OBJSUFFIX'
    env['PROGPREFIX']     = ''
    env['PROGSUFFIX']     = ''
    env['LIBPREFIX']      = 'lib'
    env['LIBSUFFIX']      = '.a'
    env['SHLIBPREFIX']    = '$LIBPREFIX'
    env['SHLIBSUFFIX']    = '.so'
    env['LIBPREFIXES']    = '$LIBPREFIX'
    env['LIBSUFFIXES']    = [ '$LIBSUFFIX', '$SHLIBSUFFIX' ]
    env['PSPAWN']         = pspawn
    env['SPAWN']          = spawn
    env['SHELL']          = 'sh'
    env['ESCAPE']         = escape
    env['TEMPFILE']       = TempFileMunge
    env['TEMPFILEPREFIX'] = '@'
    env['MAXLINELENGTH']  = 128072
    env['__RPATH'] = '$_RPATH'
