__revision__ = "src/engine/SCons/Warnings.py 0.96.91 2005/10/17 20:04:26 ita"
import SCons.Errors
class Warning(SCons.Errors.UserError):
    pass
class CacheWriteErrorWarning(Warning):
    pass
class CorruptSConsignWarning(Warning):
    pass
class DependencyWarning(Warning):
    pass
class DeprecatedWarning(Warning):
    pass
class DuplicateEnvironmentWarning(Warning):
    pass
class MissingSConscriptWarning(Warning):
    pass
class NoParallelSupportWarning(Warning):
    pass
class ReservedVariableWarning(Warning):
    pass
class MisleadingKeywordsWarning(Warning):
    pass
_warningAsException = 0
# The below is a list of 2-tuples.  The first element is a class object.
# The second element is true if that class is enabled, false if it is disabled.
_enabled = []
_warningOut = None
def suppressWarningClass(clazz):
    """Suppresses all warnings that are of type clazz or
    derived from clazz."""
    _enabled.insert(0, (clazz, 0))
    
def enableWarningClass(clazz):
    """Suppresses all warnings that are of type clazz or
    derived from clazz."""
    _enabled.insert(0, (clazz, 1))
def warningAsException(flag=1):
    """Turn warnings into exceptions.  Returns the old value of the flag."""
    global _warningAsException
    old = _warningAsException
    _warningAsException = flag
    return old
def warn(clazz, *args):
    global _enabled, _warningAsException, _warningOut
    warning = clazz(args)
    for clazz, flag in _enabled:
        if isinstance(warning, clazz):
            if flag:
                if _warningAsException:
                    raise warning
            
                if _warningOut:
                    _warningOut(warning)
            break
