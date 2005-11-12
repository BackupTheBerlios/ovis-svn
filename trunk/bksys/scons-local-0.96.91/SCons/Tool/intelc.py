__revision__ = "src/engine/SCons/Tool/intelc.py 0.96.91 2005/10/17 20:04:26 ita"
import math, sys, os.path, glob, string, re
is_win32 = sys.platform == 'win32'
is_linux = sys.platform == 'linux2'
if is_win32:
    import SCons.Tool.msvc
elif is_linux:
    import SCons.Tool.gcc
import SCons.Util
import SCons.Warnings
# Exceptions for this tool
class IntelCError(SCons.Errors.InternalError):
    pass
class MissingRegistryError(IntelCError): # missing registry entry
    pass
class MissingDirError(IntelCError):     # dir not found
    pass
class NoRegistryModuleError(IntelCError): # can't read registry at all
    pass
def uniquify(s):
    """Return a sequence containing only one copy of each unique element from input sequence s.
    Does not preserve order.
    Input sequence must be hashable (i.e. must be usable as a dictionary key)."""
    u = {}
    for x in s:
        u[x] = 1
    return u.keys()
def linux_ver_normalize(vstr):
    """Normalize a Linux compiler version number.
    Intel changed from "80" to "9.0" in 2005, so we assume if the number
    is greater than 60 it's an old-style number and otherwise new-style.
    Always returns an old-style float like 80 or 90 for compatibility with Windows.
    Shades of Y2K!"""
    f = float(vstr)
    if is_win32:
        return f
    else:
        if f < 60: return f * 10.0
        else: return f
def check_abi(abi):
    """Check for valid ABI (application binary interface) name,
    and map into canonical one"""
    if not abi:
        return None
    abi = abi.lower()
    if is_win32:
        valid_abis = {'ia32'  : 'ia32',
                      'x86'   : 'ia32',
                      'ia64'  : 'ia64',
                      'em64t' : 'ia32e',
                      'amd64' : 'ia32e'}
    if is_linux:
        valid_abis = {'ia32'   : 'ia32',
                      'x86'    : 'ia32',
                      'x86_64' : 'x86_64',
                      'em64t'  : 'x86_64',
                      'amd64'  : 'x86_64'}
    try:
        abi = valid_abis[abi]
    except KeyError:
        raise SCons.Errors.UserError, \
              "Intel compiler: Invalid ABI %s, valid values are %s"% \
              (abi, valid_abis.keys())
    return abi
def vercmp(a, b):
    """Compare strings as floats,
    but Intel changed Linux naming convention at 9.0"""
    return cmp(linux_ver_normalize(b), linux_ver_normalize(a))
def get_version_from_list(v, vlist):
    """See if we can match v (string) in vlist (list of strings)
    Linux has to match in a fuzzy way."""
    if is_win32:
        if v in vlist: return v
        else: return None
    else:
        fuzz = 0.001
        for vi in vlist:
            if math.fabs(linux_ver_normalize(vi) - linux_ver_normalize(v)) < fuzz:
                return vi
        return None
def get_intel_registry_value(valuename, version=None, abi=None):
    """
    Return a value from the Intel compiler registry tree. (Win32 only)
    """
    K = 'Software\\Intel\\Compilers\\C++\\' + version + '\\'+abi.upper()
    try:
        k = SCons.Util.RegOpenKeyEx(SCons.Util.HKEY_LOCAL_MACHINE, K)
    except SCons.Util.RegError:
        raise MissingRegistryError, \
              "%s was not found in the registry, for Intel compiler version %s, abi='%s'"%(K, version,abi)
    try:
        v = SCons.Util.RegQueryValueEx(k, valuename)[0]
        return v  # or v.encode('iso-8859-1', 'replace') to remove unicode?
    except SCons.Util.RegError:
        raise MissingRegistryError, \
              "%s\\%s was not found in the registry."%(K, value)
def get_all_compiler_versions():
    """Returns a sorted list of strings, like "70" or "80" or "9.0"
    with most recent compiler version first.
    """
    versions=[]
    if is_win32:
        keyname = 'Software\\Intel\\Compilers\\C++'
        try:
            k = SCons.Util.RegOpenKeyEx(SCons.Util.HKEY_LOCAL_MACHINE,
                                        keyname)
        except WindowsError:
            return []
        i = 0
        versions = []
        try:
            while i < 100:
                subkey = SCons.Util.RegEnumKey(k, i) # raises EnvironmentError
                ok = False
                for try_abi in ('IA32', 'IA32e',  'IA64'):
                    try:
                        d = get_intel_registry_value('ProductDir', subkey, try_abi)
                    except MissingRegistryError:
                        continue  # not found in reg, keep going
                    if os.path.exists(d): ok = True
                if ok:
                    versions.append(subkey)
                else:
                    print "Ignoring "+str(get_intel_registry_value('ProductDir', subkey, 'IA32'))
                i = i + 1
        except EnvironmentError:
            pass
    elif is_linux:
        for d in glob.glob('/opt/intel_cc_*'):
            versions.append(re.search(r'cc_(.*)$', d).group(1))
        for d in glob.glob('/opt/intel/cc*/*'):
            versions.append(re.search(r'([0-9.]+)$', d).group(1))
    versions = uniquify(versions)       # remove dups
    versions.sort(vercmp)
    return versions
def get_intel_compiler_top(version, abi):
    """
    Return the main path to the top-level dir of the Intel compiler,
    using the given version.
    The compiler will be in <top>/bin/icl.exe (icc on linux),
    the include dir is <top>/include, etc.
    """
    if is_win32:
        if not SCons.Util.can_read_reg:
            raise NoRegistryModuleError, "No Windows registry module was found"
        top = get_intel_registry_value('ProductDir', version, abi)
        if not os.path.exists(os.path.join(top, "Bin", "icl.exe")):
            raise MissingDirError, \
                  "Can't find Intel compiler in %s"%(top)
    elif is_linux:
        dirs=('/opt/intel/cc/%s', '/opt/intel_cc_%s')
        if abi == 'x86_64':
            dirs=('/opt/intel/cce/%s',)  # 'e' stands for 'em64t', aka x86_64 aka amd64
        top=None
        for d in dirs:
            if os.path.exists(os.path.join(d%version, "bin", "icc")):
                top = d%version
                break
        if not top:
            raise MissingDirError, \
                  "Can't find version %s Intel compiler in %s (abi='%s')"%(version,top, abi)
    return top
def generate(env, version=None, abi=None, topdir=None, verbose=0):
    """Add Builders and construction variables for Intel C/C++ compiler
    to an Environment.
    args:
      version: (string) compiler version to use, like "80"
      abi:     (string) 'win32' or whatever Itanium version wants
      topdir:  (string) compiler top dir, like
                         "c:\Program Files\Intel\Compiler70"
                        If topdir is used, version and abi are ignored.
      verbose: (int)    if >0, prints compiler version used.
    """
    if not (is_linux or is_win32):
        return
    if is_win32:
        SCons.Tool.msvc.generate(env)
    elif is_linux:
        SCons.Tool.gcc.generate(env)
    vlist = get_all_compiler_versions()
    if not version:
        if vlist:
            version = vlist[0]
    else:
        v = get_version_from_list(version, vlist)
        if not v:
            raise SCons.Errors.UserError, \
                  "Invalid Intel compiler version %s: "%version + \
                  "installed versions are %s"%(', '.join(vlist))
        version = v
    abi = check_abi(abi)
    if abi is None:
        if is_linux:
            uname_m = os.uname()[4]
            if uname_m == 'x86_64':
                abi = 'x86_64'
            else:
                abi = 'ia32'
        else:
            abi = "ia32"
    if version and not topdir:
        try:
            topdir = get_intel_compiler_top(version, abi)
        except (SCons.Util.RegError, IntelCError):
            topdir = None
    if not topdir:
        class ICLTopDirWarning(SCons.Warnings.Warning):
            pass
        if is_linux and not env.Detect('icc') or \
           is_win32 and not env.Detect('icl'):
            SCons.Warnings.enableWarningClass(ICLTopDirWarning)
            SCons.Warnings.warn(ICLTopDirWarning,
                                "Failed to find Intel compiler for version='%s', abi='%s'"%
                                (str(version), str(abi)))
        else:
            SCons.Warnings.enableWarningClass(ICLTopDirWarning)
            SCons.Warnings.warn(ICLTopDirWarning,
                                "Can't find Intel compiler top dir for version='%s', abi='%s'"%
                                    (str(version), str(abi)))
    if topdir:
        if verbose:
            print "Intel C compiler: using version '%s' (%g), abi %s, in '%s'"%\
                  (version, linux_ver_normalize(version),abi,topdir)
            if is_linux:
                os.system('%s/bin/icc --version'%topdir)
        env['INTEL_C_COMPILER_TOP'] = topdir
        if is_linux:
            paths={'INCLUDE'         : 'include',
                   'LIB'             : 'lib',
                   'PATH'            : 'bin',
                   'LD_LIBRARY_PATH' : 'lib'}
            for p in paths:
                env.PrependENVPath(p, os.path.join(topdir, paths[p]))
        if is_win32:
            paths=(('INCLUDE', 'IncludeDir', 'Include'),
                   ('LIB'    , 'LibDir',     'Lib'),
                   ('PATH'   , 'BinDir',     'Bin'))
            for p in paths:
                try:
                    path=get_intel_registry_value(p[1], version, abi)
                    path=path.replace('$(ICInstallDir)', topdir + os.sep)
                except IntelCError:
                    env.PrependENVPath(p[0], os.path.join(topdir, p[2]))
                else:
                    env.PrependENVPath(p[0], string.split(path, os.pathsep))
    if is_win32:
        env['CC']        = 'icl'
        env['CXX']       = 'icl'
        env['LINK']      = 'xilink'
    else:
        env['CC']        = 'icc'
        env['CXX']       = 'icpc'
        env['LINK']      = '$CC'
        env['AR']        = 'xiar'
        env['LD']        = 'xild' # not used by default
    if version:
        env['INTEL_C_COMPILER_VERSION']=linux_ver_normalize(version)
    if is_win32:
        envlicdir = os.environ.get("INTEL_LICENSE_FILE", '')
        K = ('SOFTWARE\Intel\Licenses')
        try:
            k = SCons.Util.RegOpenKeyEx(SCons.Util.HKEY_LOCAL_MACHINE, K)
            reglicdir = SCons.Util.RegQueryValueEx(k, "w_cpp")[0]
        except (AttributeError, SCons.Util.RegError):
            reglicdir = ""
        defaultlicdir = r'C:\Program Files\Common Files\Intel\Licenses'
        licdir = None
        for ld in [envlicdir, reglicdir]:
            if ld and os.path.exists(ld):
                licdir = ld
                break
        if not licdir:
            licdir = defaultlicdir
            if not os.path.exists(licdir):
                class ICLLicenseDirWarning(SCons.Warnings.Warning):
                    pass
                SCons.Warnings.enableWarningClass(ICLLicenseDirWarning)
                SCons.Warnings.warn(ICLLicenseDirWarning,
                                    "Intel license dir was not found."
                                    "  Tried using the INTEL_LICENSE_FILE environment variable (%s), the registry (%s) and the default path (%s)."
                                    "  Using the default path as a last resort."
                                        % (envlicdir, reglicdir, defaultlicdir))
        env['ENV']['INTEL_LICENSE_FILE'] = licdir
def exists(env):
    if not (is_linux or is_win32):
        return 0
    try:
        versions = get_all_compiler_versions()
    except (SCons.Util.RegError, IntelCError):
        versions = None
    detected = versions is not None and len(versions) > 0
    if not detected:
        if is_win32:
            return env.Detect('icl')
        elif is_linux:
            return env.Detect('icc')
    return detected
# end of file
