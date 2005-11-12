__revision__ = "src/engine/SCons/Scanner/Prog.py 0.96.91 2005/10/17 20:04:26 ita"
import string
import SCons.Node
import SCons.Node.FS
import SCons.Scanner
import SCons.Util
# global, set by --debug=findlibs
print_find_libs = None
def ProgramScanner(**kw):
    """Return a prototype Scanner instance for scanning executable
    files for static-lib dependencies"""
    kw['path_function'] = SCons.Scanner.FindPathDirs('LIBPATH')
    ps = apply(SCons.Scanner.Base, [scan, "ProgramScanner"], kw)
    return ps
def scan(node, env, libpath = ()):
    """
    This scanner scans program files for static-library
    dependencies.  It will search the LIBPATH environment variable
    for libraries specified in the LIBS variable, returning any
    files it finds as dependencies.
    """
    try:
        libs = env['LIBS']
    except KeyError:
        return []
    if SCons.Util.is_String(libs):
        libs = string.split(libs)
    elif SCons.Util.is_List(libs):
        libs = SCons.Util.flatten(libs)
    else:
        libs = [libs]
    try:
        prefix = env['LIBPREFIXES']
        if not SCons.Util.is_List(prefix):
            prefix = [ prefix ]
    except KeyError:
        prefix = [ '' ]
    try:
        suffix = env['LIBSUFFIXES']
        if not SCons.Util.is_List(suffix):
            suffix = [ suffix ]
    except KeyError:
        suffix = [ '' ]
    pairs = []
    for suf in map(env.subst, suffix):
        for pref in map(env.subst, prefix):
            pairs.append((pref, suf))
    result = []
    if callable(libpath): libpath = libpath()
    
    find_file = SCons.Node.FS.find_file
    adjustixes = SCons.Util.adjustixes
    for lib in libs:
        if SCons.Util.is_String(lib):
            lib = env.subst(lib)
            for pref, suf in pairs:
                l = adjustixes(lib, pref, suf)
                l = find_file(l, libpath, verbose=print_find_libs)
                if l:
                    result.append(l)
        else:
            result.append(lib)
    return result
