__revision__ = "src/engine/SCons/Tool/mwcc.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Util
import os
import os.path
import string
def set_vars(env):
    """Set MWCW_VERSION, MWCW_VERSIONS, and some codewarrior environment vars
    MWCW_VERSIONS is set to a list of objects representing installed versions
    MWCW_VERSION  is set to the version object that will be used for building.
                  MWCW_VERSION can be set to a string during Environment
                  construction to influence which version is chosen, otherwise
                  the latest one from MWCW_VERSIONS is used.
    Returns true if at least one version is found, false otherwise
    """
    desired = env.get('MWCW_VERSION', '')
    if isinstance(desired, MWVersion):
        return 1
    elif desired is None:
        return 0
    versions = find_versions()
    version = None
    if desired:
        for v in versions:
            if str(v) == desired:
                version = v
    elif versions:
        version = versions[-1]
    env['MWCW_VERSIONS'] = versions
    env['MWCW_VERSION'] = version
    if version is None:
      return 0
    env.PrependENVPath('PATH', version.clpath)
    env.PrependENVPath('PATH', version.dllpath)
    ENV = env['ENV']
    ENV['CWFolder'] = version.path
    ENV['LM_LICENSE_FILE'] = version.license
    plus = lambda x: '+%s' % x
    ENV['MWCIncludes'] = string.join(map(plus, version.includes), os.pathsep)
    ENV['MWLibraries'] = string.join(map(plus, version.libs), os.pathsep)
    return 1
def find_versions():
    """Return a list of MWVersion objects representing installed versions"""
    versions = []
    if SCons.Util.can_read_reg:
        try:
            HLM = SCons.Util.HKEY_LOCAL_MACHINE
            product = 'SOFTWARE\\Metrowerks\\CodeWarrior\\Product Versions'
            product_key = SCons.Util.RegOpenKeyEx(HLM, product)
            i = 0
            while 1:
                name = product + '\\' + SCons.Util.RegEnumKey(product_key, i)
                name_key = SCons.Util.RegOpenKeyEx(HLM, name)
                try:
                    version = SCons.Util.RegQueryValueEx(name_key, 'VERSION')
                    path = SCons.Util.RegQueryValueEx(name_key, 'PATH')
                    mwv = MWVersion(version[0], path[0], 'Win32-X86')
                    versions.append(mwv)
                except SCons.Util.RegError:
                    pass
                i = i + 1
        except SCons.Util.RegError:
            pass
    return versions
class MWVersion:
    def __init__(self, version, path, platform):
        self.version = version
        self.path = path
        self.platform = platform
        self.clpath = os.path.join(path, 'Other Metrowerks Tools',
                                   'Command Line Tools')
        self.dllpath = os.path.join(path, 'Bin')
        msl = os.path.join(path, 'MSL')
        support = os.path.join(path, '%s Support' % platform)
        self.license = os.path.join(path, 'license.dat')
        self.includes = [msl, support]
        self.libs = [msl, support]
    def __str__(self):
        return self.version
CSuffixes = ['.c', '.C']
CXXSuffixes = ['.cc', '.cpp', '.cxx', '.c++', '.C++']
def generate(env):
    """Add Builders and construction variables for the mwcc to an Environment."""
    set_vars(env)
    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    for suffix in CSuffixes:
        static_obj.add_action(suffix, SCons.Defaults.CAction)
        shared_obj.add_action(suffix, SCons.Defaults.ShCAction)
    for suffix in CXXSuffixes:
        static_obj.add_action(suffix, SCons.Defaults.CXXAction)
        shared_obj.add_action(suffix, SCons.Defaults.ShCXXAction)
    env['CCCOMFLAGS'] = '$CPPFLAGS $_CPPDEFFLAGS $_CPPINCFLAGS -nolink -o $TARGET $SOURCES'
    env['CC']         = 'mwcc'
    env['CCCOM']      = '$CC $CCFLAGS $CCCOMFLAGS'
    env['CXX']        = 'mwcc'
    env['CXXCOM']     = '$CXX $CXXFLAGS $CCCOMFLAGS'
    env['SHCC']       = '$CC'
    env['SHCCFLAGS']  = '$CCFLAGS'
    env['SHCCCOM']    = '$SHCC $SHCCFLAGS $CCCOMFLAGS'
    env['SHCXX']       = '$CXX'
    env['SHCXXFLAGS']  = '$CXXFLAGS'
    env['SHCXXCOM']    = '$SHCXX $SHCXXFLAGS $CCCOMFLAGS'
    env['CFILESUFFIX'] = '.c'
    env['CXXFILESUFFIX'] = '.cpp'
    env['CPPDEFPREFIX']  = '-D'
    env['CPPDEFSUFFIX']  = ''
    env['INCPREFIX']  = '-I'
    env['INCSUFFIX']  = ''
def exists(env):
    return set_vars(env)
