__revision__ = "src/engine/SCons/Tool/swig.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Action
import SCons.Defaults
import SCons.Tool
import SCons.Util
SwigAction = SCons.Action.Action('$SWIGCOM', '$SWIGCOMSTR')
def swigSuffixEmitter(env, source):
    if '-c++' in SCons.Util.CLVar(env.subst("$SWIGFLAGS")):
        return '$SWIGCXXFILESUFFIX'
    else:
        return '$SWIGCFILESUFFIX'
def generate(env):
    """Add Builders and construction variables for swig to an Environment."""
    c_file, cxx_file = SCons.Tool.createCFileBuilders(env)
    c_file.suffix['.i'] = swigSuffixEmitter
    cxx_file.suffix['.i'] = swigSuffixEmitter
    c_file.add_action('.i', SwigAction)
    cxx_file.add_action('.i', SwigAction)
    env['SWIG']              = 'swig'
    env['SWIGFLAGS']         = SCons.Util.CLVar('')
    env['SWIGCFILESUFFIX']   = '_wrap$CFILESUFFIX'
    env['SWIGCXXFILESUFFIX'] = '_wrap$CXXFILESUFFIX'
    env['SWIGCOM']           = '$SWIG $SWIGFLAGS -o $TARGET $SOURCES'
def exists(env):
    return env.Detect(['swig'])
